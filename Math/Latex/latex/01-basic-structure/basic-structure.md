# LaTeX 基本结构

## 最小文档

```latex
\documentclass{article}

\begin{document}

Hello, World!

\end{document}
```

## 完整文档模板

```latex
\documentclass[12pt, a4paper]{article}

% ---- 导言区（Preamble） ----
\usepackage[UTF8]{ctex}          % 中文支持
\usepackage{amsmath}             % 数学公式
\usepackage{graphicx}            % 图片
\usepackage{hyperref}            % 超链接
\usepackage[margin=2.5cm]{geometry} % 页边距

\title{论文标题}
\author{作者姓名}
\date{\today}

% ---- 正文 ----
\begin{document}

\maketitle
\tableofcontents

\section{引言}
正文内容。

\section{方法}
正文内容。

\end{document}
```

## 文档类（Document Class）

| 文档类       | 用途           |
| ------------ | -------------- |
| `article`    | 短文、报告     |
| `report`     | 长报告、学位论文 |
| `book`       | 书籍           |
| `beamer`     | 演示文稿       |
| `letter`     | 信件           |

```latex
\documentclass[选项]{文档类}
```

常用选项：`10pt` / `11pt` / `12pt`、`a4paper` / `letterpaper`、`twocolumn`。

## 章节层次

```latex
\part{部分}             % book/report 中使用
\chapter{章}            % book/report 中使用
\section{节}
\subsection{小节}
\subsubsection{小小节}
\paragraph{段落}
\subparagraph{子段落}
```

> `article` 没有 `\chapter`，从 `\section` 开始。

## 注释

```latex
% 这是单行注释

%% 多行注释可以用 iffalse ... fi
\iffalse
这段内容不会被编译
\fi
```

## 特殊字符

| 字符 | 输入方式   |
| ---- | ---------- |
| `#`  | `\#`       |
| `$`  | `\$`       |
| `%`  | `\%`       |
| `&`  | `\&`       |
| `_`  | `\_`       |
| `{`  | `\{`       |
| `}`  | `\}`       |
| `~`  | `\textasciitilde{}` |
| `^`  | `\textasciicircum{}` |
| `\`  | `\textbackslash{}` |

## 换行与分段

```latex
段落之间空一行即可分段。

强制换行用 \\（不推荐频繁使用）。
这里会换到下一行。
```
