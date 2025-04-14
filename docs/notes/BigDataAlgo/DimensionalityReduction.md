# 数据降维

对于高维数据集，数据经常是非常稀疏的，一个合理的操作是将这些高维数据降为低维数据。本章介绍两种数据降维的方法，分别为**主成分分析**和 **Johnson-Lindenstraus 变换**。

## 主成分分析

主成分分析的基本思想是找到数据空间中信息量最大的方向，并将数据投影到这些方向张成的子空间中。因此我们需要解决的问题就是：

1. 什么样的方向是信息量最大的方向；
2. 如何找到这样的方向。

### 问题转化

通常认为，数据在一个方向上**越分散**，该方向越能对数据进行区分，也就是说该方向蕴含的数据的信息量较大。因此我们可以用方差来衡量信息量，换言之，**数据在某个方向上的投影的方差越大，该方向的信息量就越大**。

考虑数据集 $X = ( x_1, x_2, \cdots, x_m ) \in \mathcal{R}^{n \times m}$ 有 $m$ 个 $n$ 维的数据点。为了方便计算，我们可以对这 $m$ 个数据点做去中心化处理即令

$$
X = X - \frac{1}{m} \sum_{i = 1}^m x_i
$$

设数据集在 $\omega$ 方向上信息量最大，则数据点在该方向上的投影为 $\omega^\top x_i$，投影的方差为

$$
Cov(\omega^\top X) = \frac{1}{m - 1} \sum_{i = 1}^m \omega^\top x_i x_i^\top \omega = \omega^\top \left( \frac{1}{m - 1}\sum_{i = 1}^m x_i x_i^\top \right) \omega = \omega^\top C \omega
$$

其中 $C$ 为数据集的**协方差矩阵**。于是，原本的问题转化为最优化问题：

$$
\arg\max_{\omega} \{\omega^\top C \omega\}, \quad \text{s.t.} \Vert \omega \Vert_2^2 = 1
$$

该问题的 Lagrange 函数为

$$
\mathcal{L}(\omega, \lambda) = \omega^\top C \omega + \lambda (1 - \Vert \omega \Vert_2^2)
$$

求导

$$
\begin{aligned}
    \begin{cases}
        \frac{\partial \mathcal{L}}{\partial \omega} = 2 C\omega - 2\lambda \omega = 0 \\
        \frac{\partial \mathcal{L}}{\partial \lambda} = 1 - \omega^\top \omega = 0
    \end{cases}
\end{aligned}
$$

解得

$$
\begin{aligned}
    \begin{cases}
        C \omega = \lambda \omega \\
        \omega^\top \omega = 1
    \end{cases}
\end{aligned}
$$

显然这个 $\omega$ 则是协方差矩阵 $C$ 的**最大的特征值对应的特征向量的方向**。

### 奇艺值分解

通过问题转化，我们知道数据集信息量最大的方向是其协方差矩阵的最大特征值对应的特征向量的方向，因此求解任意数据集协方差矩阵的特征值和特征向量就很关键。

!!! note "奇艺值分解"
    设一个 $n \times m$ 的矩阵 $A$，可以被分解为

    $$
        A = U \Sigma V^\top
    $$

    其中 $U \in \mathcal{R}^{n \times n}$ 是 $AA^\top$ 的特征向量；$\Sigma \in \mathcal{R}^{n \times m}$ 是对角阵，其对角元 $\sigma_i = \sqrt{\lambda_i}$ 被称为奇艺值，$\lambda_i$ 为 $A^\top A$ 或 $AA^\top$ 的特征值；$V \in \mathcal{R}^{m \times m}$ 是 $A^\top A$ 的特征向量，$U$ 和 $V$ 都是正交阵。

显然，如果对数据集 $X$ 做奇艺值分解，得到的 $U$ 就是协方差矩阵 $\dfrac{1}{m-1}XX^\top$ 的特征向量。

??? tip "主成分分析的缺点"
    主成分分析作为常用的最简单的数据降维算法，其缺点包括：
    
    1. 复杂度高，基于 QR 分解的奇艺值分解复杂度 $\mathcal{O}(nd^2)$；
    2. 作用于矩阵，数值稳定性在维度高时难以保障；
    3. 对数据多次读取，不适合流式数据和分布式计算；
    4. 有隐私泄漏的问题。

## Johnson-Lindenstrauss 变换

Johnson-Lindenstrauss 变换是一种用于高维数据的降维技术。其基本思想是将高维数据投影到一个低维的欧氏空间，**同时保持数据点之间的距离**。相比于主成分分析，JL 变换是一种随机算法，不需要计算奇艺值分解，从而大大节省了计算开销。

### 引理

!!! note "Johnson-Lindenstraus 引理"
    对于任意包含 $n$ 个 $d$ 维数据点的集合 $S$，存在一个矩阵 $A \in \mathcal{R}^{k \times d}$ 使得对 $\forall u, v \in S$ 有

    $$
    (1 - \varepsilon)\Vert u - v \Vert_2^2 \leqslant \Vert Au - Av \Vert_2^2 \leqslant (1 + \varepsilon)\Vert u - v \Vert_2^2
    $$

    对 $\forall \varepsilon \in (0, 1)$ 成立，且 $k = \mathcal{O}\left(\dfrac{\log n}{\varepsilon^2}\right)$。

这个引理的含义是，对于任意的两个向量 $x, y$，在映射后的空间中，它们的距离与原空间中的距离保持在 $(1 − \varepsilon, 1 + \varepsilon)$ 的范围内。换句话说，**该映射尽量模仿了刚体变换**，因为刚体变换就是保距变换, 即引理在 $\varepsilon = 0$ 的情形。

### 构造变换

一个很自然的问题是：如何找到这样的矩阵 $A$ 使得 JL 性质成立。假设集合 $X$ 是 $S$ 中的任意点对构成的集合，那么只需要找到一个满足

$$
(1 - \varepsilon)\Vert x \Vert_2^2 \leqslant \Vert Ax \Vert_2^2 \leqslant (1 + \varepsilon)\Vert x \Vert_2^2, \quad \forall x \in X
$$

的矩阵即可。一个简单的构造方法是另 $A$ 为任意元素 $a_{ij} \sim \mathcal{N}(0, \frac{1}{k})$ 且元素间**相互独立**的矩阵。

### 证明

由于任意 $A$ 中元素 $a_{ij} \sim \mathcal{N}(0, \frac{1}{k})$ 且相互独立，取 $X$ 中某一特定的 $x$ 有

$$
\left( (Ax)_1 \right)^2 = \left( \sum_{j = 1}^d A_{1j}x_j \right)^2
$$

考虑变换后的向量的模长的期望有

$$
\mathbb{E}(\Vert Ax \Vert_2^2) = k\mathbb{E}\left( (Ax)_1^2 \right) = k \frac{\Vert x \Vert_2^2}{k} = \Vert x \Vert_2^2
$$

这说明变换对原始向量的模长是**无偏**的。但是仅仅知道这个是不够的，我们关心的是变换后的向量模长偏离这个均值的概率。

设 $z_i = \dfrac{\sqrt{k} \sum\limits_{j = 1}^dA_{ij}x_j}{\Vert x \Vert_2} \sim \mathcal{N}(0, 1)$ 则变换后向量模长的右尾概率可以表示为

$$
\begin{aligned}
    P\left( \Vert Ax \Vert_2^2 \geqslant (1 + \varepsilon)\Vert x \Vert_2^2 \right) &= P\left( \sum_{i = 1}^k z_i^2 \geqslant (1 + \varepsilon)k \right) \\
    &= P\left( \exp{\left( \lambda \sum_{i = 1}^k z_i^2 \right)} \geqslant \exp{\left( \lambda (1 + \varepsilon) k \right)} \right) \\
    & \leqslant \exp{\left(- \lambda (1 + \varepsilon) k \right)}\mathbb{E}\left(\exp{\left( \lambda \sum_{i = 1}^k z_i^2 \right)}\right) \\
    &= \exp{\left(- \lambda (1 + \varepsilon) k \right)}\prod_{i = 1}^k\mathbb{E}\left(e^{\left( \lambda z_i^2 \right)}\right) \\
    &= \frac{1}{\exp{\left(\lambda (1 + \varepsilon) k \right)}}\frac{1}{(1 - 2\lambda)^{k/2}}  \quad \left(\text{令 } \lambda = \frac{\varepsilon}{2(1 + \varepsilon)}\right)\\
    &= ((1 + \varepsilon)e^{-\varepsilon})^{k/2} \\
    &\leqslant e^{-\frac{k}{4}(\varepsilon^2 - \varepsilon^3)}
\end{aligned}
$$

同理可得

$$
P\left( \Vert Ax \Vert_2^2 \leqslant (1 - \varepsilon)\Vert x \Vert_2^2 \right) \leqslant e^{-\frac{k}{4}(\varepsilon^2 - \varepsilon^3)}
$$

从而得出

$$
P\left( (1 - \varepsilon)\Vert x \Vert_2^2 \leqslant \Vert Ax \Vert_2^2 \leqslant (1 + \varepsilon)\Vert x \Vert_2^2 \right) = 1 - 2e^{-\frac{k}{4}(\varepsilon^2 - \varepsilon^3)}
$$

由于集合 $X$ 中一共有 $\binom{n}{2} = n^2$ 个点对，利用 Union Bound 技术，想要得到常数概率，令 $k = \mathcal{O}(\frac{1}{\varepsilon^2}\log n)$ 即可。
