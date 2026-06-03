# Context in AI / LLMs

## What is "Context"?

In Large Language Models (LLMs), **context** (or **context window**) refers to the number of tokens the model can process in a single forward pass — both input (prompt) and output (response combined). It is the model's "working memory": everything in the context window is visible to the model when it predicts the next token.

Key points:

- **Not memory** — context is ephemeral. Once a session ends, the context is gone. This is distinct from fine-tuning or persistent memory.
- **Fixed upper bound** — each model architecture defines a maximum context length (e.g., 128K, 200K, 1M tokens).
- **Attention scope** — in the Transformer architecture, each token can attend to all other tokens in the context. The longer the context, the more relationships the model must compute.

## What Manufacturers Mean by "1M Context" (or "10M Context")

Marketing claims like "1M tokens" or "10M tokens" refer to the maximum input length the model can theoretically accept. Notable examples:

| Model | Claimed Context |
|---|---|
| GPT-4 / GPT-4 Turbo | 128K tokens |
| Claude 3 / 4 family | 200K tokens |
| Gemini 1.5 / 2.0 | 1M ~ 10M tokens |
| GLM-4 (Zhipu) | 1M tokens |
| Yi-34B (01.AI) | 200K tokens |
| DeepSeek V2/V3 | 128K ~ 1M tokens |

The manufacturer's claim means: *"You can feed this many tokens into the model and it will produce a response."* But it does **not** guarantee that the model uses all of them equally well.

## Underlying Principles: How Long Context Is Achieved

The core difficulty: standard Transformer self-attention has **O(n²) time and memory complexity** with sequence length n. Processing 1M tokens naively would require ~10¹² operations per layer — infeasible.

### 1. Flash Attention

An IO-aware exact attention algorithm that avoids materializing the full n×n attention matrix in HBM (high-bandwidth memory). Instead, it tiles the computation and processes it in SRAM. This gives 2–4× speedup and **linear memory growth** relative to sequence length (vs quadratic), making longer contexts practical on existing hardware.

### 2. Ring Attention / Sequence Parallelism

Distributes the attention computation across multiple devices (GPUs). Each device holds a chunk of the sequence, and they communicate in a ring to compute the full attention collectively. This is how Gemini achieves its multi-million context — by scaling across TPU pods.

### 3. RoPE (Rotary Position Embedding) with Position Interpolation

- **RoPE** encodes position by rotating query/key vectors, allowing relative position encoding.
- To extend context beyond training length, **position interpolation** scales the position indices (e.g., compress [0, N] → [0, N/k]) so the model sees positions within its trained range. This enables fine-tuned extension from 4K → 32K or beyond without full retraining.

### 4. KV Cache Optimization

During autoregressive decoding, the model caches Key (K) and Value (V) tensors for each token to avoid recomputation. For 1M tokens, this cache alone could be **hundreds of GB**. Techniques include:

- **Multi-Query Attention (MQA) / Grouped-Query Attention (GQA)** — share KV heads across multiple query heads to reduce cache size.
- **KV Cache quantization** — store KV cache in int4 or int8 instead of fp16.
- **Eviction / compression** — selectively drop or merge less important KV entries.

### 5. Sparse / Sliding Window Attention

Instead of attending to all tokens, attend only to a local window (e.g., 4K tokens) for most layers, with a few global layers that attend to everything. Used by Mistral, Mixtral, and Longformer.

### 6. Contextual Compression / Retrieval-Augmented

Some "long context" systems are actually hybrid: they embed chunks, store them in a vector DB, and retrieve the most relevant ones into the prompt. This gives the *appearance* of infinite context without changing the model architecture.

## The Gap Between Claim and Reality

Manufacturers rarely disclose *effective* context length — how far back the model can recall information accurately.

Known phenomena:

- **Lost in the Middle** (Liu et al., 2023): Positional bias — models perform significantly worse when relevant information is in the middle of long contexts.
- **Needle in a Haystack** (NIAH) benchmarks: A simple test (insert a fact in a long document and ask about it). Most models pass NIAH but fail at more complex long-context reasoning (multi-hop retrieval over long docs).
- **Compute cost**: Running 1M-token inference is expensive (high latency, high GPU memory). It's often not practical for real-time applications even if the model technically supports it.

## Summary

| Aspect | Detail |
|---|---|
| Context window | Maximum tokens a model can process at once |
| Why it's hard | O(n²) attention complexity |
| Key techniques | Flash Attention, Ring Attention, RoPE interpolation, KV cache optimization |
| Marketing vs reality | Claim ≠ effective context; "supports 1M" ≠ "performs well at 1M" |
