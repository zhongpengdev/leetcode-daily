# 参考文献

## 方法一：手动 thebibliography

```latex
\begin{thebibliography}{99}
  \bibitem{lamport94}
    Leslie Lamport.
    \textit{\LaTeX: A Document Preparation System}.
    Addison Wesley, 1994.

  \bibitem{knuth84}
    Donald E. Knuth.
    \textit{The \TeX book}.
    Addison Wesley, 1984.
\end{thebibliography}
```

引用：`如文献~\cite{lamport94} 所述`。

`{99}` 是最大编号宽度占位，9 篇以内用 `{9}`，超过 9 篇用 `{99}`。

## 方法二：BibTeX（推荐）

### 1. 创建 `.bib` 文件（refs.bib）

```bibtex
@article{einstein05,
  author  = {Albert Einstein},
  title   = {Zur Elektrodynamik bewegter K{\"o}rper},
  journal = {Annalen der Physik},
  year    = {1905},
  volume  = {322},
  number  = {10},
  pages   = {891--921},
}

@book{knuth84,
  author    = {Donald E. Knuth},
  title     = {The {\TeX} Book},
  publisher = {Addison-Wesley},
  year      = {1984},
}

@inproceedings{smith20,
  author    = {John Smith and Jane Doe},
  title     = {A Great Paper},
  booktitle = {Proceedings of Conference},
  year      = {2020},
  pages     = {100--110},
}
```

### 2. 在 LaTeX 文档中使用

```latex
\documentclass{article}
\usepackage[UTF8]{ctex}

\begin{document}

Einstein 提出了相对论~\cite{einstein05}。

\bibliographystyle{plain}    % 样式：plain, unsrt, alpha, abbrv
\bibliography{refs}          % refs.bib（不写扩展名）

\end{document}
```

### 3. 编译命令

```bash
xelatex 文档
bibtex 文档
xelatex 文档
xelatex 文档
```

需要编译 4 次才能正确解析所有引用。

## 常用 bibliography style

| 样式     | 效果                        |
| -------- | --------------------------- |
| `plain`  | 按引用顺序编号，完整作者名  |
| `unsrt`  | 按引用顺序编号，原文顺序    |
| `alpha`  | 用标签如 [Ein05]            |
| `abbrv`  | 缩写作者名和月份            |
| `ieeetr` | IEEE 风格                   |
| `acm`    | ACM 风格                    |

## 方法三：BibLaTeX + Biber（现代推荐）

```latex
\usepackage[backend=biber, style=numeric]{biblatex}
\addbibresource{refs.bib}

\begin{document}
引用~\cite{einstein05}。

\printbibliography
\end{document}
```

编译：

```bash
xelatex 文档
biber 文档
xelatex 文档
```

### BibLaTeX 常用样式

| style        | 效果           |
| ------------ | -------------- |
| `numeric`    | [1], [2], [3]  |
| `alphabetic` | [Ein05]        |
| `authoryear` | (Einstein, 1905) |
| `authortitle`| (Einstein, Zur...) |

### 中文参考文献

```bibtex
@book{someCN20,
  author    = {张三 and 李四},
  title     = {LaTeX 中文排版},
  publisher = {清华大学出版社},
  year      = {2020},
}
```

> BibLaTeX + Biber 原生支持 UTF-8，中文作者名直接写即可。

## 引用命令

```latex
\cite{key}              % 普通引用 [1]
\cite{key1, key2}       % 多个引用 [1, 2]
\parencite{key}         % 带括号 (Author, 2020)   [BibLaTeX]
\textcite{key}          % 文中引用 Author (2020)  [BibLaTeX]
\nocite{key}            % 不引用但列入参考文献
\nocite{*}              % 列出 .bib 中所有条目
```
