# 数学公式

> 推荐 `\usepackage{amsmath}`、`amssymb`。

## 行内公式

```latex
爱因斯坦公式 $E = mc^2$ 嵌在文字中。

或者用 \( E = mc^2 \) 也行。
```

## 行间公式

```latex
$$ E = mc^2 $$           % 居中，无编号

\begin{equation}         % 居中，有编号
E = mc^2
\end{equation}

\begin{equation*}        % 居中，无编号（需要 amsmath）
E = mc^2
\end{equation*}
```

## 常用数学符号

### 希腊字母

| 小写        | 大写          |
| ----------- | ------------- |
| `\alpha` α  | `\Alpha` A    |
| `\beta` β   | `\Beta` B     |
| `\gamma` γ  | `\Gamma` Γ    |
| `\delta` δ  | `\Delta` Δ    |
| `\epsilon` ε | `\Epsilon` E |
| `\theta` θ  | `\Theta` Θ    |
| `\lambda` λ | `\Lambda` Λ   |
| `\mu` μ     | `\Mu` M       |
| `\pi` π     | `\Pi` Π       |
| `\sigma` σ  | `\Sigma` Σ    |
| `\phi` φ    | `\Phi` Φ      |
| `\omega` ω  | `\Omega` Ω    |

### 上下标

```latex
x^2              % 上标：x²
x_{i}            % 下标：xᵢ
x_{i}^{2}        % 同时有上下标
a_{ij}^{k}       % 多字符需要用花括号
```

### 分数与根号

```latex
\frac{a}{b}      % 分数：a/b
\sqrt{x}         % 平方根
\sqrt[3]{x}      % 立方根
```

### 求和、积分、极限

```latex
\sum_{i=1}^{n} i         % 求和
\prod_{i=1}^{n} x_i      % 连乘
\int_{0}^{1} f(x) dx     % 积分
\iint f(x,y) dxdy        % 二重积分
\lim_{x \to 0} \frac{\sin x}{x}   % 极限
```

### 关系与运算符

```latex
\leq, \geq       % ≤, ≥
\neq             % ≠
\approx          % ≈
\equiv           % ≡
\pm              % ±
\times           % ×
\div             % ÷
\cdot            % ·
\in              % ∈
\subset          % ⊂
\cup, \cap       % ∪, ∩
\forall          % ∀
\exists          % ∃
\infty           % ∞
```

## 多行公式

### 对齐（align）

```latex
\begin{align}
  x &= a + b \label{eq:1} \\
  y &= c + d \label{eq:2}
\end{align}

% 无编号版本
\begin{align*}
  x &= a + b \\
  y &= c + d
\end{align*}
```

`&` 是对齐点，`\\` 是换行。

### 分段函数（cases）

```latex
f(x) = \begin{cases}
  x^2       & \text{if } x \geq 0 \\
  -x        & \text{if } x < 0
\end{cases}
```

### 矩阵

```latex
\begin{bmatrix}
  a & b \\
  c & d
\end{bmatrix}

% 其他括号类型
\begin{pmatrix} ... \end{pmatrix}   % 圆括号
\begin{vmatrix} ... \end{vmatrix}   % 竖线
\begin{Bmatrix} ... \end{Bmatrix}   % 花括号
```

## 数学字体

```latex
\mathbb{R}       % 黑板粗体：ℝ（需要 amssymb）
\mathcal{L}      % 花体：ℒ
\mathbf{v}       % 粗体：v
\hat{x}          % 帽子：x̂
\bar{x}          % 横线：x̄
\vec{v}          % 箭头：v⃗
\dot{x}          % 点：ẋ
\tilde{x}        % 波浪：x̃
```

## 括号自适应大小

```latex
\left( \frac{a}{b} \right)         % 自动调整括号大小
\left[ \frac{a}{b} \right]         % 方括号
\left\{ \frac{a}{b} \right\}       % 花括号（需转义）
\left| \frac{a}{b} \right|         % 绝对值
```
