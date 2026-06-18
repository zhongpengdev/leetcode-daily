# 交叉引用与超链接

> 需要 `\usepackage{hyperref}`。

## 标签与引用

```latex
\section{引言} \label{sec:intro}

如第~\ref{sec:intro}~节所述……

% 带页码
详见第~\pageref{sec:intro}~页。

% nameref 引用标题名
在"\nameref{sec:intro}"一节中……
```

## 可打标签的对象

| 对象       | 示例                              |
| ---------- | --------------------------------- |
| 章节       | `\section{...} \label{sec:x}`     |
| 公式       | `\begin{equation} \label{eq:y}`   |
| 图片       | `\begin{figure} \label{fig:z}`    |
| 表格       | `\begin{table} \label{tab:w}`     |
| 列表项     | `\item \label{item:a}`            |

## 标签命名约定

```latex
\label{sec:方法}      % 章节 sec: 前缀
\label{eq:能量}       % 公式 eq: 前缀
\label{fig:结果}      % 图片 fig: 前缀
\label{tab:数据}      % 表格 tab: 前缀
\label{ch:引言}       % 章 ch: 前缀
```

## 超链接设置

```latex
\usepackage[colorlinks=true,
            linkcolor=blue,       % 内部链接颜色
            citecolor=green,      % 引用链接颜色
            urlcolor=cyan]{hyperref}  % URL 颜色
```

## URL 与外部链接

```latex
% 普通 URL
\url{https://www.example.com}

% 带文字的链接
\href{https://www.example.com}{点击这里}

% 裸露的邮箱
\href{mailto:user@example.com}{发邮件}
```

## 脚注

```latex
正文内容带脚注\footnote{这是脚注内容。}继续正文。

% 手动标记
\footnotemark[1]    % 在正文中标记
\footnotetext[1]{脚注文字}  % 在页面底部放置
```

## 尾注

需要 `\usepackage{endnotes}`：

```latex
正文带尾注\endnote{这是尾注。}

% 在文档末尾输出所有尾注
\theendnotes
```

## cleveref 宏包（智能引用）

```latex
\usepackage{cleveref}

\cref{eq:x}        % 自动输出 "式(1)"
\Cref{sec:y}       % 句首大写，"第2节"
\cref{fig:a,fig:b} % 自动合并："图1和图2"
\cref{eq:1,eq:2,eq:3}  % "式(1)到式(3)"
```

> `cleveref` 会自动判断引用类型，无需手动写"图"/"表"/"式"。需要在 `hyperref` **之后**加载。
