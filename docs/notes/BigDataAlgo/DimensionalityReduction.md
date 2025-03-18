# 数据降维

## 主成分分析

主成分分析（或称 PCA）是一个基础且常用的数据降维算法。其几何意义非常直观：设在高纬空间中存在点集 $P = \{ P_1, P_2, \dots, P_n \} \in \mathcal{R}^d$，考虑将点集**投影**到 $k < d$ 维空间，PCA 的优化目标为

$$
\min \sum_{i = 1}^n \Vert P_i - \pi(P_i) \Vert^2
$$

其中 $\pi(P_i)$ 为点 $P_i$ 在 $k$ 维空间的投影。

??? tip "优化目标"
    不一定非要用距离的平方和作为优化的目标，也可以选用其他的，只是平方和易于计算。

从代数角度来看，PCA 的输入为矩阵 $\mathbf{A} = \mathcal{R}^{n \times d}$，目标是找到一个**秩**为 $k$ 的矩阵 $\mathbf{A}_k$ 使得

$$
\Vert \mathbf{A} - \mathbf{A}_k \Vert_F
$$

最小化。其中 $\Vert \cdot \Vert_F$ 为 Frobenius 范数。

??? note "Frobenius 范数"
    Frobenius 范数展开为

    $$
    \Vert \mathbf{X} \Vert_F = \sqrt{\sum_i \sum_j \mathbf{X}_{ij}}
    $$

### 计算

首先考虑 $k = 0$ 时的情况，低维空间为一个**点**，因此有

$$
\pi(P_1) = \pi(P_2) = \cdots = \pi(P_n) = q
$$

优化目标

$$
\min \sum_{i = 1}^n \Vert P_i - q \Vert^2 \implies q = \frac{1}{n} \sum_{i = 1}^n {P_i}
$$

求出的 $q$ 即为 $P$ 的重心。

再考虑 $k = 1$ 的情况，低维空间为一条**直线**，可以证明最优解得到的直线 $F$ 一定经过重心 $q$。

!!! note "奇艺值分解"
    考虑矩阵 $\mathbf{A} \in \mathcal{R}^{n \times d}, n > d$ 则通过奇艺值分解，可以得到

    $$
    \mathbf{A} = \mathbf{U} \cdot \mathbf{S} \cdot \mathbf{V}^\top
    $$
    其中 $\mathbf{U}, \mathbf{V}$ 均为单位正交矩阵，$\mathbf{S}$ 为奇艺值矩阵。

现在假设重心 $q$ 恰好为原点，否则可以将坐标系进行一些平移变换。此时，优化目标为

$$
\Vert P_i \Vert^2 = \Vert P_i - \pi(P_i) \Vert^2 + \Vert <p_i, \boldsymbol{t}_i> \Vert^2
$$

其中，$\boldsymbol{t}$ 为直线 $F$ 的方向向量，进一步有

$$
\sum_{i= 1}^n \Vert P_i - \pi(P_i) \Vert^2 = \sum_{i = 1}^n \Vert P_i \Vert^2 - \sum_{i = 1}^n \Vert <P_i, \boldsymbol{t}_i> \Vert^2
$$

从而 PCA 的优化目标转化为

$$
\max \sum_{i = 1}^n \Vert <P_i, \boldsymbol{t_i}> \Vert^2 = \Vert A \cdot t_i \Vert_F^2
$$

??? info "算法点评"
    主成分分析算法有很多缺点：

    - 复杂度高（$\mathcal{O}(nd^2)$）
    - 稳定性差
    - 对数据多次读取
        - 不适合 Streaming Data
        - 不适合分布式计算
        - 泄漏隐私
