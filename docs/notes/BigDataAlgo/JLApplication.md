---
katex: true
---

# JL 变换的应用

本章介绍一些 JL 变换的实际应用。

## 线性回归

当给定矩阵 $A \in \mathcal{R}^{n \times d}$ 和向量 $y \in \mathcal{R}^{n \times 1}$，且假设 $n \gg d$，则线性规划的目标是求解优化问题

$$
\min_{\beta \in \mathcal{R}^d} \Vert A \cdot \beta - y \Vert_2^2
$$

同时，我们希望 $\beta$ 是一个 **$k$-Sparse** 的向量，即** $\beta$ 中只有 $k$ 个值非零**。由于优化目标是一个**凸函数**，因此存在解析解

$$
\beta_{\text{opt}} = \left( A^\top A \right)^{-1}A^\top y
$$

且求解析解的复杂度为 $\Theta(nd^2)$。

### 几何角度

若将矩阵 $A$ 看作一个线性变换，将 $n$ 维的数据空间投影到 $d$ 维的特征空间。则对应的

$$
A \cdot \beta_{\text{opt}} = A \left( A^\top A \right)^{-1}A^\top y = Hy
$$

其中 $H = A \left( A^\top A \right)^{-1}A^\top$ 是一个**投影矩阵**。

### 降维

上述从几何角度对线性变换做出的解释都是在一个 $n$ 维所做的操作，如果可以将问题降维，则计算开销则会大大减小。

一个很自然的想法是利用 JL 变换对数据进行降维，但是 JL 变换适用于有限个点的情况，而线性规划问题的子空间 $A \cdot \beta$ 中有无穷多个点，因此要应用 JL 变换，需要利用**空间离散化**思想。
