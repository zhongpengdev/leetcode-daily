# LaTeX in Markdown 语法参考

## 两种渲染模式

| 语法 | 效果 | 用途 |
|------|------|------|
| `$...$` | 行内（inline） | 公式嵌入在文字中间 |
| `$$...$$` | 行间（display） | 公式独立成行，居中展示 |

**注意：** 不是所有平台都支持 LaTeX 渲染。常见支持的有：GitHub、Typora、Obsidian、VS Code（需装插件）、Jupyter Notebook。

---

## 基础符号

### 上下标

```latex
$x^2$          # 上标
$x_i$          # 下标
$x_i^2$        # 同时上下标
$x^{2y}$       # 多字符上标必须用 {}
$x_{i,j}$      # 多字符下标必须用 {}
```

渲染效果：$x^2$, $x_i$, $x_i^2$, $x^{2y}$, $x_{i,j}$

### 分数

```latex
$\frac{a}{b}$          # 分数 \frac{分子}{分母}
$\dfrac{a}{b}$         # 强制 display 风格（更大）
$\tfrac{a}{b}$         # 强制 text 风格（更小）
$\cfrac{a}{1+\cfrac{b}{c}}$   # 连分数
```

### 根号

```latex
$\sqrt{x}$             # 平方根
$\sqrt[3]{x}$          # 立方根
$\sqrt[n]{x+y}$        # n 次根
```

---

## 希腊字母

### 小写

| 符号 | 代码 | 符号 | 代码 | 符号 | 代码 | 符号 | 代码 |
|------|------|------|------|------|------|------|------|
| $\alpha$ | `\alpha` | $\beta$ | `\beta` | $\gamma$ | `\gamma` | $\delta$ | `\delta` |
| $\epsilon$ | `\epsilon` | $\zeta$ | `\zeta` | $\eta$ | `\eta` | $\theta$ | `\theta` |
| $\iota$ | `\iota` | $\kappa$ | `\kappa` | $\lambda$ | `\lambda` | $\mu$ | `\mu` |
| $\nu$ | `\nu` | $\xi$ | `\xi` | $\pi$ | `\pi` | $\rho$ | `\rho` |
| $\sigma$ | `\sigma` | $\tau$ | `\tau` | $\upsilon$ | `\upsilon` | $\phi$ | `\phi` |
| $\chi$ | `\chi` | $\psi$ | `\psi` | $\omega$ | `\omega` | | |

### 大写（首字母大写）

$\Gamma$, $\Delta$, $\Theta$, $\Lambda$, $\Xi$, $\Pi$, $\Sigma$, $\Phi$, $\Psi$, $\Omega$

### 变体

| 符号 | 代码 | 说明 |
|------|------|------|
| $\varepsilon$ | `\varepsilon` | 手写体 ε |
| $\varphi$ | `\varphi` | 手写体 φ |
| $\vartheta$ | `\vartheta` | 变体 θ |
| $\varsigma$ | `\varsigma` | 变体 σ |

---

## 运算符

### 算术

| 符号 | 代码 | 符号 | 代码 |
|------|------|------|------|
| $\times$ | `\times` | $\div$ | `\div` |
| $\cdot$ | `\cdot` | $\pm$ | `\pm` |
| $\mp$ | `\mp` | $\ast$ | `\ast` |

### 比较

| 符号 | 代码 | 符号 | 代码 |
|------|------|------|------|
| $\neq$ | `\neq` | $\leq$ | `\leq` |
| $\geq$ | `\geq` | $\approx$ | `\approx` |
| $\equiv$ | `\equiv` | $\sim$ | `\sim` |
| $\propto$ | `\propto` | $\ll$ / $\gg$ | `\ll` / `\gg` |

### 集合

| 符号 | 代码 | 符号 | 代码 |
|------|------|------|------|
| $\in$ | `\in` | $\notin$ | `\notin` |
| $\subset$ | `\subset` | $\supset$ | `\supset` |
| $\subseteq$ | `\subseteq` | $\supseteq$ | `\supseteq` |
| $\cup$ | `\cup` | $\cap$ | `\cap` |
| $\emptyset$ | `\emptyset` | $\varnothing$ | `\varnothing` |

### 逻辑

| 符号 | 代码 | 符号 | 代码 |
|------|------|------|------|
| $\forall$ | `\forall` | $\exists$ | `\exists` |
| $\neg$ | `\neg` | $\land$ | `\land` |
| $\lor$ | `\lor$ | $\implies$ | `\implies` |
| $\iff$ | `\iff` | | |

---

## 求和、积分、极限

### 求和

```latex
$\sum_{i=1}^{n} x_i$        # 行内
$$\sum_{i=1}^{n} x_i$$       # 行间（上下标显示不同）
```

渲染：$\sum_{i=1}^{n} x_i$

### 积分

```latex
$$\int_{a}^{b} f(x)\,dx$$
$$\iint_{D} f(x,y)\,dxdy$$      # 二重积分
$$\iiint_{V} f\,dV$$             # 三重积分
$$\oint_{C} \vec{F}\cdot d\vec{r}$$  # 曲线积分
```

### 极限

```latex
$$\lim_{x \to 0} \frac{\sin x}{x} = 1$$
$$\lim_{n \to \infty} \left(1 + \frac{1}{n}\right)^n = e$$
```

### 其他大型运算符

```latex
$$\prod_{i=1}^{n} a_i$$      # 连乘
$$\bigcup_{i=1}^{n} A_i$$    # 并集
$$\bigcap_{i=1}^{n} A_i$$    # 交集
```

---

## 括号与定界符

### 自动缩放（推荐）

```latex
$\left( \frac{a}{b} \right)$          # 自动匹配括号大小
$\left[ \frac{a}{b} \right]$
$\left\{ \frac{a}{b} \right\}$        # 花括号需要转义
$\left| x \right|$
$\left\langle x \right\rangle$        # 尖括号
```

### 手动指定大小

```latex
$\big( \Big( \bigg( \Bigg($
$\big[ \Big[ \bigg[ \Bigg[$
$\big\{ \Big\{ \bigg\{ \Bigg\{$
```

---

## 特殊函数与文字

LaTeX 会把连续字母当作变量名（斜体），所以函数名要用 `\` 前缀：

```latex
$\sin, \cos, \tan, \arcsin, \arccos, \arctan$
$\log, \ln, \lg, \exp$
$\max, \min, \sup, \inf$
$\det, \dim, \ker, \deg$
$\arg, \gcd, \hom$
$\lim, \limsup, \liminf$
```

**常用 text 模式：**

```latex
$\text{hello world}$        # 在公式中插入普通文字
$\textrm{text}$              # roman 字体
$\textbf{bold}$              # 粗体
$\textit{italic}$            # 斜体
```

---

## 箭头

| 符号 | 代码 | 符号 | 代码 |
|------|------|------|------|
| $\rightarrow$ | `\rightarrow` / `\to` | $\leftarrow$ | `\leftarrow` |
| $\Rightarrow$ | `\Rightarrow` | $\Leftarrow$ | `\Leftarrow` |
| $\leftrightarrow$ | `\leftrightarrow$ | $\Leftrightarrow$ | `\Leftrightarrow` |
| $\mapsto$ | `\mapsto$ | $\longrightarrow$ | `\longrightarrow` |
| $\uparrow$ | `\uparrow` | $\downarrow$ | `\downarrow` |

---

## 矩阵

```latex
$$
\begin{pmatrix}
a & b \\
c & d
\end{pmatrix}
$$          # 圆括号矩阵

$$
\begin{bmatrix}
a & b \\
c & d
\end{bmatrix}
$$          # 方括号矩阵

$$
\begin{vmatrix}
a & b \\
c & d
\end{vmatrix}
$$          # 行列式（竖线）

$$
\begin{Bmatrix}
a & b \\
c & d
\end{Bmatrix}
$$          # 花括号矩阵
```

**对齐方式：**

```latex
$$
\begin{matrix}
a & b \\
c & d
\end{matrix}
$$          # 无括号，居中对齐
```

- `&` 分隔同一行的元素
- `\\` 换行

---

## 分段函数与对齐

### 分段函数

```latex
$$
f(x) = \begin{cases}
x^2 & \text{if } x \geq 0 \\
-x  & \text{if } x < 0
\end{cases}
$$
```

### 多行对齐

```latex
$$
\begin{align}
a &= b + c \\
  &= d + e
\end{align}
$$
```

- `&` 指定对齐位置（通常放在 `=` 前面）
- `\\` 换行
- `align` 会自动编号，`align*` 不编号

---

## 空格控制

| 效果 | 代码 | 说明 |
|------|------|------|
| $a b$ | `a b` | 默认小空格 |
| $a\,b$ | `a\,` | thin space |
| $a\;b$ | `a\;` | medium space |
| $a\quad b$ | `a\quad` | 一个字符宽度 |
| $a\qquad b$ | `a\qquad` | 两个字符宽度 |
| $a\!b$ | `a\!` | 负空格（缩小间距） |

---

## 标注符号

| 符号 | 代码 | 用途 |
|------|------|------|
| $\hat{x}$ | `\hat{x}` | 帽 |
| $\bar{x}$ | `\bar{x}` | 上划线 |
| $\vec{x}$ | `\vec{x}` | 向量 |
| $\dot{x}$ | `\dot{x}` | 一阶导数点 |
| $\ddot{x}$ | `\ddot{x}` | 二阶导数点 |
| $\tilde{x}$ | `\tilde{x}` | 波浪号 |
| $\overline{AB}$ | `\overline{AB}` | 长上划线 |
| $\overrightarrow{AB}$ | `\overrightarrow{AB}` | 长箭头 |
| $\underline{x}$ | `\underline{x}` | 下划线 |

---

## 常见坑与注意事项

### 1. 花括号必须转义

`{` 和 `}` 在 LaTeX 中是分组符号，显示花括号要写 `\{` `\}`。

```latex
$\{1, 2, 3\}$     # 正确
${1, 2, 3}$       # 错误：花括号消失
```

### 2. 多字符上下标必须用 `{}`

```latex
$x^{10}$          # 正确 → $x^{10}$
$x^10$            # 错误 → $x^10$（只有 1 是上标）
```

### 3. 反斜杠转义

`\` 和 `$` 等特殊字符需要转义：`\\`, `\$`

### 4. 中文在公式中

公式中插入中文要用 `\text{}` 或 `\mbox{}`：

```latex
$\text{当 } x > 0 \text{ 时}$
```

### 5. 行内 vs 行间渲染差异

`$` 公式中的 `\sum`, `\int` 等大型运算符会压缩显示，`$$` 公式会完整展开。

### 6. 平台兼容性

- **GitHub**：支持 `$...$` 和 `$$...$$`，但渲染偶尔有延迟
- **Typora**：原生支持，实时渲染
- **Obsidian**：原生支持
- **VS Code Markdown**：需安装插件（如 Markdown+Math）
- **Jekyll/Hugo 博客**：需配置 MathJax 或 KaTeX

### 7. 美元符冲突

如果 `$` 出现在非公式场景（如金额 `$100`），可能会误触发渲染。可以用 `\$` 转义，或在 `$` 前加空格让渲染器不识别。

---

## 快速对照表

```
基础：  ^ _ \frac{}{} \sqrt{} \cdot \times
希腊：  \alpha \beta \gamma \delta \theta \lambda \pi \sigma \omega
运算：  \sum \prod \int \lim \partial \nabla
比较：  \leq \geq \neq \approx \equiv
集合：  \in \notin \subset \cup \cap \emptyset
逻辑：  \forall \exists \implies \iff
箭头：  \to \Rightarrow \Leftarrow \mapsto
括号：  \left( \right) \left[ \right] \left\{ \right\}
标注：  \hat{} \bar{} \vec{} \dot{} \overline{} \underline{}
空格：  \, \; \quad \qquad \!
函数：  \sin \cos \log \ln \max \min \lim
文字：  \text{} \mathrm{} \mathbf{}
```
