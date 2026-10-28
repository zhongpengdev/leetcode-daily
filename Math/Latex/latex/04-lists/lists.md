# 列表

## 无序列表（itemize）

```latex
\begin{itemize}
  \item 第一项
  \item 第二项
  \item 第三项
\end{itemize}
```

## 有序列表（enumerate）

```latex
\begin{enumerate}
  \item 第一项
  \item 第二项
  \item 第三项
\end{enumerate}
```

## 描述列表（description）

```latex
\begin{description}
  \item[术语A] 解释说明
  \item[术语B] 解释说明
  \item[术语C] 解释说明
\end{description}
```

## 嵌套列表

```latex
\begin{enumerate}
  \item 第一项
    \begin{itemize}
      \item 子项 a
      \item 子项 b
    \end{itemize}
  \item 第二项
    \begin{enumerate}
      \item 子项 1
      \item 子项 2
    \end{enumerate}
\end{enumerate}
```

LaTeX 支持最多 4 层嵌套，各层符号自动变化。

## 自定义列表样式

需要 `\usepackage{enumitem}`：

```latex
% 修改标签
\begin{itemize}[label=$\bullet$]
  \item 用实心圆点
\end{itemize}

\begin{itemize}[label=$\circ$]
  \item 用空心圆圈
\end{itemize}

\begin{itemize}[label=--]
  \item 用短横线
\end{itemize}

% 修改有序列表起始编号
\begin{enumerate}[start=5]
  \item 从 5 开始
\end{enumerate}

% 修改编号格式
\begin{enumerate}[label=(\alph*)]
  \item (a)
  \item (b)
\end{enumerate}

\begin{enumerate}[label=\Roman*.]
  \item I.
  \item II.
\end{enumerate}

% 去掉缩进
\begin{itemize}[leftmargin=*]
  \item 无缩进的列表
\end{itemize}
```

## 常用 label 格式

| 格式               | 效果        |
| ------------------ | ----------- |
| `label=$\bullet$`  | ●           |
| `label=$\circ$`    | ○           |
| `label=$\diamond$` | ◇           |
| `label=$\star$`    | ★           |
| `label=--`         | –           |
| `label={\color{red}$\bullet$}` | 红色 ● |

| enumerate 格式      | 效果     |
| -------------------- | -------- |
| `label=\arabic*.`   | 1. 2. 3. |
| `label=\alph*.)`    | a.) b.)  |
| `label=\Alph*.`     | A. B. C. |
| `label=\roman*.`    | i. ii.   |
| `label=\Roman*.`    | I. II.   |
