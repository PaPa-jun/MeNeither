---
katex: true
pseudocode: true
---

# 信赖域算法

在线搜索类算法中，我们先利用近似模型求出下降方向，然后给定步长；而在信赖域算法中，我们**直接在一个有界区域内求解这个近似模型**，然后到下一个点。因此信赖域算法实际上是**同时选择了步长和方向**。

## 信赖域

根据带**拉格朗日余项**的泰勒展开

$$
f(x^k + d) = f(x^k) + \nabla f(x^k)^\top d + \frac{1}{2} d^\top \nabla^2 f(x^k + td) d
$$

其中 $t \in (0, 1)$ 为和 $d$ 有关的正数。参考[牛顿法](./NewtonMethod.md)，我们利用 $f(x)$ 的一个二阶近似来刻画 $f(x)$ 在 $x = x^k$ 处的性质

$$
m_k(d) = f(x^k) + \nabla f(x^k)^\top d + \frac{1}{2} d^\top B^k d
$$

其中 $B^k$ 是**对称矩阵**，为海瑟矩阵的近似矩阵(1)。
{ .annotate }

1.  如果 $B^k$ 恰好是函数 $f(x)$ 在点 $x^k$ 处的海瑟矩阵，那么当 $f(x)$ 充分光滑时，$m_k(d)$ 的逼近误差为 $\mathcal{O}(\Vert d \Vert^3)$.

考虑到泰勒展开是函数的**局部性质**，它仅仅对模长较小的 $d$ 有意义。为此我们需要对近似添加约束，对于信赖域算法，我们只在如下**球**内考虑 $f(x)$ 的近似

$$
\Omega_k = \{ x^k + d \mid \Vert d \Vert \leqslant \Delta_k \}
$$

其中 $\Delta_k > 0$ 是一个和迭代有关的参数。我们称 $\Omega_k$ 为**信赖域**，$\Delta_k$ 为**信赖域半径**。

## 算法框架

信赖域就是我们相信 $m_k(d)$ 能够很好地近似 $f(x)$ 的区域，而 $\Delta_k$ 表示了这个区域的大小。因此信赖域算法每一步都需要求解如下**子问题**

$$
\min_{d \in \mathbb{R}^n} m_k(d), \quad \text{s.t. } \Vert d \Vert \leqslant \Delta_k
$$

上述子问题解得的 $d^*$ 显然**包含了线搜索类方法中的步长和下降方向**。

在子问题的求解中非常关键的部分是信赖域半径的选取，它决定了算法的收敛性。考虑到信赖域半径反映的是对模型 $m_k(d)$ 的**相信程度**，因此如果模型近似较好，就应该扩大信赖域半径，反之应该缩小信赖域半径。引入以下定义来衡量 $m_k(d)$ 对 $f(x)$ 近似程度的好坏

$$
\rho_k = \frac{f(x^k) - f(x^k + d^k)}{m_k(0) - m_k(d^k)}
$$

其中 $d^k$ 为子问题得到的迭代方向。根据定义 $\rho_k$ 是**函数值实际下降量与预估下降量的比值**，如果 $\rho_k \to 1$ 说明用 $m_k(d)$ 来近似 $f(x)$ 是比较成功的。我们可以根据这个量来动态调整信赖域半径，得到算法的基本框架。

<div class="pseudocode">
    \begin{algorithm}
    \caption{信赖域算法}
    \begin{algorithmic}
    \STATE 给定最大半径 $\Delta_{\max}$，初始半径 $\Delta_0$，初始点 $x^0$，$k \gets 0$；
    \STATE 给定参数 $0 \leqslant \eta  < \bar{\rho}_1 < \bar{\rho}_2 < 1$，$\gamma_1 < 1 < \gamma_2$；
    \WHILE{未达到收敛准则}
        \STATE 计算子问题 $\min_{d \in \mathbb{R}^n} m_k(d), \text{ s.t. } \Vert d \Vert \leqslant \Delta_k$ 得到迭代方向 $d^k$；
        \STATE 计算下降率 $\rho_k$；
        \IF{$\rho_k < \bar{\rho}_1$}
            \STATE 缩小信赖域半径：$\Delta_{k + 1} = \gamma_1 \Delta_k$
        \ELSE
            \IF{$\rho_k > \bar{\rho}_2$ \and $\Vert d^k \Vert = \Delta_k$}
                \STATE 扩大信赖域半径：$\Delta_{k + 1} = \min \{ \gamma_2 \Delta_{k}, \Delta_{\max} \}$
            \ELSE
                \STATE 信赖域半径不变：$\Delta_{k + 1} = \Delta_k$
            \ENDIF
        \ENDIF
        \IF{$\rho_k > \eta$}
            \STATE 更新 $x^{k + 1} = x^k + d$；
        \ELSE
            \STATE $x^{k + 1} = x^k$；
        \ENDIF
        \STATE $ k \gets k + 1 $；
    \ENDWHILE
    \end{algorithmic}
    \end{algorithm}
</div>

上述算法有一些参数，但是算法对参数并不敏感。在实际应用中可取 $\bar{\rho}_1 = 0.25, \bar{\rho}_2 = 0.75$ 以及 $\gamma_1 = 0.25, \gamma_2 = 2$，参数 $\eta$ 保证了当模型近似较差时算法不会对 $x$ 进行迭代。

!!! tip
    算法第 9 行的判断保证信赖域半径仅在模型近似较好且**信赖域约束起作用**时扩大，接下来的第 10 行保证其不会无限制的增大。

## 子问题求解

在多数实际应用中，信赖域子问题的解是无法显示写出的，为了求出迭代方向 $d^k$，需要设计算法快速或近似求解子问题。

!!! note "信赖域子问题解的最优性条件"
    $d^*$ 是信赖域子问题

    $$
    \min_{d \in \mathbb{R}^n} m(d) = f + g^\top d + \frac{1}{2} d^\top B d, \quad \text{s.t. } \Vert d \Vert \leqslant \Delta
    $$

    的全局极小解当且仅当 $d^*$ 是可行的且存在 $\lambda > 0$ 使得

    $$
    \begin{aligned}
    &(B + \lambda I) d^* = -g \\
    &\lambda(\Delta - \Vert d^* \Vert) = 0 \\
    &(B + \lambda I) \succeq 0
    \end{aligned}
    $$

    其中 $g$ 表示 $f$ 的梯度。

??? quote "证明"
    先证明必要性。子问题的 $Lagrange$ 函数为

    $$
    L(d, \lambda) = f + g^\top d + \frac{1}{2} d^\top B d - \frac{\lambda}{2} (\Delta^2 - \Vert d \Vert^2), \quad \lambda \geqslant 0
    $$

    根据 KKT 条件，$d^*$ 是可行解，且

    $$
    \nabla_d L(d^*, \lambda) = (B + \lambda I )d^* + g = 0 \implies (B + \lambda I) d^* = -g
    $$

    此外，还有互补条件

    $$
    \frac{\lambda}{2} (\Delta^2 - \Vert d \Vert^2) = 0 \implies \lambda(\Delta - \Vert d^* \Vert) = 0
    $$

    任取 $d$ 满足 $\Vert d \Vert = \Delta$，根据最优性有

    $$
    m(d) \geqslant m(d^*) = m(d^*) + \frac{\lambda}{2}(\Vert d^* \Vert^2 - \Vert d \Vert^2) \implies (d - d^*)^\top (B + \lambda I)(d - d^* \geqslant 0)
    $$

    由任意性可知 $B + \lambda I$ 半正定。
    再证明充分性。定义辅助函

    $$
    \hat{m}(d) = f + g^\top d + \frac{1}{2} d^\top (B + \lambda I)d = m(d) + \frac{\lambda}{2} d^\top d
    $$

    由条件 $(B + \lambda I) \succeq 0$ 可知 $\hat{m}(d)$ 关于 $d$ 是凸函数。根据条件 $(B + \lambda I) d^* = -g$，$d^*$ 满足凸函数一阶最优性条件，可推出 $d^*$ 是 $\hat{m}(d)$ 的全局极小值点，进而对任意可行解 $d$，有

    $$
    m(d) \geqslant m(d^*) + \frac{\lambda}{2} (\Vert d^* \Vert^2 - \Vert d \Vert^2)
    $$

    由互补条件 $\lambda(\Delta - \Vert d^* \Vert) = 0$ 知 $\lambda(\Delta^2 - \Vert d^* \Vert^2) = 0$，带入上式消去 $\Vert d^* \Vert^2$ 得

    $$
    m(d) \geqslant m(d^*) + \frac{\lambda}{2}(\Delta^2 - \Vert d^* \Vert^2) \geqslant m(d^*)
    $$



上述定理提供了维数 $n$ 较小时寻找 $d^*$ 的一个方法。

!!! note annotate "求解信赖域子问题的迭代法"
    根据互补条件 $\lambda(\Delta - \Vert d^* \Vert) = 0$，有

    - 若 $\lambda = 0$ 并且 $B \succeq 0$，此时 $m(d)$ 是凸函数。求出 $d$ 使其满足 $Bd=−g$ 且 $\Vert d \Vert \leqslant \Delta$ 满足；
    - 选择充分大的 $\lambda > 0$ 使得 $B + \lambda I \succeq 0$ 且 $\Vert d(λ) \Vert = \Delta$，并且满足

        $$
        (B + \lambda I)d(\lambda) = -g
        $$

        此时问题等价于：求解关于 $\lambda$ 的方程 $\Vert d(λ) \Vert = \Delta$ 或者 $1/\Vert d(λ) \Vert = 1/\Delta$。

    可以证明 $\Vert d(λ) \Vert = \Delta$ 的解必存在且唯一(1)，所以寻找 $\lambda^*$ 已经转化为一个一元方程求根问题，可使用**牛顿法**求解。

1.  这里不是很严谨，实际上存在一种**困难情形**需要用奇艺矩阵的特征根分解，分析太复杂遂略去。

## 收敛性分析

### 柯西点

为了估计求解每个信赖域子问题得到的函数值改善情况，我们引入柯西点。

!!! note "柯西点"
    设 $m_k(d)$ 是 $f(x)$ 在点 $x = x^k$ 处的二阶近似，$\tau_k$ 为如下优化问题的解：

    $$
    \begin{aligned}
    \min_{\tau} \quad & m_k(-\tau \nabla f(x^k)) \\
    \text{s.t.} \quad & \Vert \tau \nabla f(x^k) \Vert \leqslant \Delta_k, \\
    & \tau \geqslant 0
    \end{aligned}
    $$

    则称 $x^k_C \triangleq x^k + d^k_C$ 为**柯西点**，其中 $d^k_C = - \tau_k \nabla f(x^k)$.

根据柯西点的定义，它实际上是在信赖域约束下对 $m_k(d)$ 进行了一次精确线搜索的梯度法。给定 $m_k(d)$，柯西点可以显示计算出来，设 $g^k = \nabla f(x^k)$，有

$$
\begin{aligned}
\tau_k = 
\begin{cases}
\frac{\Delta_k}{\Vert g^k \Vert}, & (g^k)^\top B^k g^k \leqslant 0,\\
\min \left\{ \frac{\Vert g^k \Vert^2}{(g^k)^\top B^k g^k}, \frac{\Delta_k}{\Vert g^k \Vert} \right\}, & \text{others.}
\end{cases}
\end{aligned}
$$

以上分析表明，**柯西点是信赖域子问题的一个可行点**，但它并没有充分利用海瑟矩阵 $B^k$ 的信息，因此并不实际使用。人们将柯西点作为信赖域子问题算法的一个**评判标准**，即要求**子问题算法产生的迭代点至少要比柯西点好**。

??? note "柯西点的下降量"
    设 $d^k_C$ 为求解柯西点产生的下降方向，则

    $$
    m_k(0) - m_k(d^k_C) \geqslant \frac{1}{2} \Vert g^k \Vert \min \left\{ \Delta_k, \frac{\Vert g^k \Vert}{\Vert B^k \Vert_2} \right\}
    $$

    之前介绍的**迭代法**满足 $m_k(0) - m_k(d^k) \geqslant c_2 (m_k(0) - m_k(d^k_C))$。

### 全局收敛性

回顾信赖域算法框架，参数 $\eta$ 来确定是否应该更新迭代点。这分为两种情况：

- 当 $\eta = 0$ 时，只要原目标函数有下降量就接受信赖域迭代步的更新；
- 当 $\eta > 0$ 时，只有当改善量 $\rho_k$ 到达一定程度时再进行更新。

这两种情况的收敛性结果时不同的。

!!! note "全局收敛性 1"
    设近似海瑟矩阵 $B_k$ 有界，即 $\Vert B_k \Vert_2 \leqslant M, \forall k$，$f(x)$ 在下水平集 $\mathcal{L} = \{x \mid f(x) \leqslant f(x^0)\}$ 上有下界，且 $\nabla f(x)$ 在 $\mathcal{L}$ 的一个开邻域 $S(R_0)$ 内利普希茨连续。若 $d_k$ 为信赖域子问题的近似解且满足
    
    $$
    m_k(0) - m_k(d^k) \geqslant \frac{1}{2}c_2 \Vert g \Vert^k \min \left\{ \Delta_k, \frac{\Vert g^k \Vert}{\Vert B^k \Vert_2} \right\} 
    $$

    信赖域算法选取参数 $\eta = 0$，则

    $$
    \lim_{k\to \infty} \inf \Vert \nabla f(x^k) \Vert = 0
    $$

    即 $x^k$ 的聚点中**包含**稳定点。

上述定理表明若无条件接受信赖域子问题的更新，则算法仅仅有**子序列的收敛性**，迭代点序列本身**不一定**收敛。

!!! note "全局收敛性 2"
    在全局收敛性 1 的条件下，若算法选取参数 $\eta > 0$，且信赖域子问题近似解 $d^k$ 满足

    $$
    m_k(0) - m_k(d^k) \geqslant \frac{1}{2}c_2 \Vert g \Vert^k \min \left\{ \Delta_k, \frac{\Vert g^k \Vert}{\Vert B^k \Vert_2} \right\} 
    $$

    则 $\lim\limits_{k \to \infty} \Vert \nabla f(x^k) \Vert = 0$.

上述分析说明信赖域算法和牛顿法不同，具有全局收敛性，因此它对迭代的初值选取的要求比较弱。

### 局部收敛性

在构造信赖域子问题时利用了 $f(x)$ 的二阶信息，它在最优点附近应该具有牛顿法的性质。特别地，当近似矩阵 $B_k$ 取为海瑟矩阵 $\nabla^2f(x^k)$ 时，根据信赖域子问题的更新方式，二次模型 $m_k(d)$ 将会越来越逼近原函数 $f(x)$，最终信赖域约束将会失效。此时信赖域方法将会和牛顿法十分接近。

!!! note "信赖域算法与牛顿方向"
    设 $f(x)$ 在最优点 $x = x^∗$ 的一个邻域内二阶连续可微，且 $\nabla f(x)$ 利普希茨连续，在最优点 $x^∗$ 处二阶充分条件成立，即 $\nabla^2f (x) \succ 0$。若迭代点列 $\{x_k\}收敛到 $x^∗$，且在迭代中选取 $B_k$ 为海瑟矩阵 $\nabla^2 f(x^k)$，则对充分大的 $k$，任意满足

    $$
    m_k(0) - m_k(d^k) \geqslant \frac{1}{2}c_2 \Vert g \Vert^k \min \left\{ \Delta_k, \frac{\Vert g^k \Vert}{\Vert B^k \Vert_2} \right\} 
    $$

    的信赖域子问题算法产生的迭代方向 $d^k$ 均满足

    $$
    \Vert d^k - d^k_N \Vert = \mathcal{o}(\Vert d^k_N \Vert)
    $$

    其中 $d^k_N$ 为第 $k$ 步迭代的牛顿方向且满足假设 $\Vert d^k_N \Vert \leqslant \Delta_k / 2$。

该定理说明若信赖域算法收敛，则当 $k$ 充分大时，信赖域半径的约束终将失效，且算法产生的迭代方向将会越来越接近牛顿方向。

!!! note "推论：信赖域算法的局部收敛速度"
    在前面的定理的条件下，信赖域算法产生的迭代序列 $\{x^k\}$ 具有 $\mathcal{Q}$-超线性收敛速度。
