# 数据降维

对于高维数据集，数据经常是非常稀疏的，一个合理的操作是将这些高维数据降为低维数据。本章介绍两种数据降维的方法，分别为**主成分分析**和 **JL 变换**。

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

显然，如果对数据集 $X$ 做奇艺值分解，得到的 $U$ 就是协方差矩阵 $\dfrac{1}{m}XX^\top$ 的特征向量。

??? tip "主成分分析的缺点"
    主成分分析作为常用的最简单的数据降维算法，其缺点包括：
    
    1. 复杂度高，基于 QR 分解的奇艺值分解复杂度 $\mathcal{O}(nd^2)$；
    2. 作用于矩阵，数值稳定性在维度高时难以保障；
    3. 对数据多次读取，不适合流式数据和分布式计算；
    4. 有隐私泄漏的问题。

## JL 变换



