# 文本格式

## 字体样式

```latex
\textbf{粗体}          % Bold
\textit{斜体}          % Italic
\underline{下划线}     % Underline
\textsc{小型大写字母}  % Small Caps
\emph{强调}            % 强调（通常是斜体，嵌套时自动切换）
```

## 字体大小

从小到大：

```latex
{\tiny 极小}
{\scriptsize 脚本大小}
{\footnotesize 脚注大小}
{\small 小}
\normalsize % 正常（命令形式，不加花括号）
{\large 大}
{\Large 更大}
{\LARGE 再大}
{\huge 巨大}
{\Huge 最大}
```

## 颜色

需要 `\usepackage{xcolor}`：

```latex
\textcolor{red}{红色文字}
\textcolor{blue}{蓝色文字}
\textcolor[RGB]{255, 128, 0}{自定义颜色}

{\color{green} 整段绿色文字}
```

## 对齐方式

需要 `\usepackage{ragged2e}`：

```latex
% 左对齐
\begin{flushleft}
左对齐文字
\end{flushleft}

% 居中
\begin{center}
居中文字
\end{center}

% 右对齐
\begin{flushright}
右对齐文字
\end{flushright}
```

## 行距

需要 `\usepackage{setspace}`：

```latex
\onehalfspacing      % 1.5 倍行距
\doublespacing       % 双倍行距
\setstretch{1.25}    % 自定义行距

% 或在 \begin{document} 之前
\linespread{1.5}
```

## 首行缩进

```latex
\usepackage{indentfirst}  % 让 section 后也首行缩进
\setlength{\parindent}{2em}  % 缩进 2 个字符宽度

% 取消某段缩进
\noindent 不缩进的段落。
```

## 引用与破折号

```latex
`单引号'          % 或者直接输入 '单引号'
``双引号''        % 或者直接输入 "双引号"

-              % 连字符：短横线（如 well-known）
--             % 破折号：范围（如 pages 1--10）
---            % 长破折号：句子中的破折号
```

## 水平线与空格

```latex
\hrule            % 水平线

\quad             % 大空格（约一个字符宽度）
\qquad            % 更大的空格
\,                % 小空格
\hspace{1cm}      % 指定宽度的空格
\hfill            % 填满剩余空间
```

## 无格式文本

```latex
\verb|代码中的 $ & % 不会被解释|    % 逐字输出
\begin{verbatim}
多行
逐字输出
\end{verbatim}
```
