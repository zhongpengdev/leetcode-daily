# 表格

## 基本表格

```latex
\begin{tabular}{|l|c|r|}
  \hline
  左对齐 & 居中 & 右对齐 \\
  \hline
  A      & B    & C      \\
  D      & E    & F      \\
  \hline
\end{tabular}
```

列对齐参数：`l`（左）、`c`（居中）、`r`（右）。

## 常用列格式符号

| 符号  | 含义              |
| ----- | ----------------- |
| `l`   | 左对齐            |
| `c`   | 居中              |
| `r`   | 右对齐            |
| `\|`  | 竖线              |
| `||`  | 双竖线            |
| `p{3cm}` | 固定宽度，自动换行 |

## 三线表（学术论文常用）

```latex
\begin{tabular}{lccr}
  \toprule
  姓名 & 数学 & 英语 & 总分 \\
  \midrule
  张三 & 90   & 85   & 175  \\
  李四 & 88   & 92   & 180  \\
  王五 & 95   & 78   & 173  \\
  \bottomrule
\end{tabular}
```

> 需要 `\usepackage{booktabs}`，三线表用 `\toprule`、`\midrule`、`\bottomrule`，不要用 `\hline`。

## 带标题的表格（table 环境）

```latex
\begin{table}[htbp]
  \centering
  \caption{学生成绩表}
  \label{tab:score}
  \begin{tabular}{lccr}
    \toprule
    姓名 & 数学 & 英语 & 总分 \\
    \midrule
    张三 & 90   & 85   & 175  \\
    李四 & 88   & 92   & 180  \\
    \bottomrule
  \end{tabular}
\end{table}
```

浮动体位置参数：`h`（此处）、`t`（顶部）、`b`（底部）、`p`（单独一页），`!` 表示强制。

## 合并单元格

```latex
\usepackage{multirow}

\begin{tabular}{|c|c|c|c|}
  \hline
  \multicolumn{2}{|c|}{合并两列} & C & D \\
  \hline
  A & B & \multirow{2}{*}{合并两行} & D \\
  \hline
  A & B & & D \\
  \hline
\end{tabular}
```

- `\multicolumn{列数}{对齐}{内容}`：横向合并
- `\multirow{行数}{宽度}{内容}`：纵向合并（需要 `multirow` 宏包）

## 固定宽度列

```latex
\begin{tabular}{|p{4cm}|p{6cm}|}
  \hline
  短标题 & 长内容会自动换行，不需要手动指定换行位置。 \\
  \hline
  另一行 & 另一段长文字。 \\
  \hline
\end{tabular}
```

## 表格中的数学公式

```latex
\begin{tabular}{l>{$}c<{$} r}
  \toprule
  名称 & \text{公式} & 值 \\
  \midrule
  圆周率 & \pi & 3.14159 \\
  自然对数底 & e & 2.71828 \\
  \bottomrule
\end{tabular}
```

> `>{$}c<{$}` 让该列自动进入/退出数学模式。需要 `\usepackage{array}`。

## 长表格（跨页）

需要 `\usepackage{longtable}`：

```latex
\begin{longtable}{lcc}
  \caption{长表格标题} \label{tab:long} \\
  \toprule
  项目 & 值A & 值B \\
  \midrule
  \endfirsthead

  \toprule
  项目 & 值A & 值B \\
  \midrule
  \endhead

  \bottomrule
  \endfoot

  数据1 & 10 & 20 \\
  数据2 & 30 & 40 \\
  % ... 很多行 ...
\end{longtable}
```
