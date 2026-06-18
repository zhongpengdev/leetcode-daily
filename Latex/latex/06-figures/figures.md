# 图片

> 需要 `\usepackage{graphicx}`。

## 基本插入

```latex
\includegraphics{图片路径/文件名.png}
```

## 带标题和标签

```latex
\begin{figure}[htbp]
  \centering
  \includegraphics[width=0.8\textwidth]{图片.png}
  \caption{图片标题}
  \label{fig:example}
\end{figure}
```

引用：`如图~\ref{fig:example} 所示`。

## 常用选项

```latex
% 宽度
\includegraphics[width=0.5\textwidth]{图.png}

% 高度
\includegraphics[height=5cm]{图.png}

% 缩放
\includegraphics[scale=0.5]{图.png}

% 旋转
\includegraphics[angle=45]{图.png}

% 裁剪（左 下 右 上）
\includegraphics[trim=1cm 2cm 1cm 2cm, clip]{图.png}

% 组合使用
\includegraphics[width=\textwidth, angle=0]{图.png}
```

## 并排图片

```latex
\begin{figure}[htbp]
  \centering
  \begin{minipage}[t]{0.48\textwidth}
    \centering
    \includegraphics[width=\textwidth]{图1.png}
    \caption{图1标题}
    \label{fig:1}
  \end{minipage}
  \hfill
  \begin{minipage}[t]{0.48\textwidth}
    \centering
    \includegraphics[width=\textwidth]{图2.png}
    \caption{图2标题}
    \label{fig:2}
  \end{minipage}
\end{figure}
```

> 两个 `minipage` 的总宽度加上 `\hfill` 间隔应不超过 `\textwidth`。

## 支持的图片格式

| 格式  | pdfLaTeX | XeLaTeX |
| ----- | -------- | ------- |
| PDF   | ✅       | ✅      |
| PNG   | ✅       | ✅      |
| JPG   | ✅       | ✅      |
| EPS   | ✅       | 需转换  |
| SVG   | 需转换   | 需转换  |

> 优先使用 PDF（矢量图）和 PNG（位图），照片用 JPG。

## 图片搜索路径

在导言区设置，避免每次写完整路径：

```latex
\graphicspath{{images/}{figures/}}
```

之后 `\includegraphics{photo}` 会自动在 `images/` 和 `figures/` 中查找。

## 用 TikZ 画简单图形

```latex
\usepackage{tikz}

\begin{figure}[htbp]
  \centering
  \begin{tikzpicture}
    \draw (0,0) -- (2,0) -- (1,1.73) -- cycle;  % 三角形
    \draw[fill=blue!20] (3,0) circle (1);        % 填充圆
    \node at (3,0) {中心};                        % 文字
  \end{tikzpicture}
  \caption{TikZ 示例}
\end{figure}
```

## 浮动体位置参数

| 参数 | 含义                     |
| ---- | ------------------------ |
| `h`  | 尽量放在当前位置         |
| `t`  | 放在页面顶部             |
| `b`  | 放在页面底部             |
| `p`  | 放在单独的浮动页         |
| `!`  | 放宽限制（如 `!htbp`）   |
| `H`  | 强制放在此处（需 `float` 宏包） |
