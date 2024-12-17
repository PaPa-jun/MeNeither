# 约束优化：增广拉格朗日函数法

## 问题形式

考虑一般的约束优化问题，可以写成

$$
\begin{aligned}
&\min_{x \in \mathbb{R}^n} && f(x) \\
&\text{s.t.} && c_i(x) = 0, & i \in \mathcal{E}\\
&&& c_i(x) \leqslant 0, & i \in \mathcal{I}
\end{aligned}
$$

目标函数 $f(x)$ 和约束函数 $c_i(x)$ 在定义域内都是可微的。

