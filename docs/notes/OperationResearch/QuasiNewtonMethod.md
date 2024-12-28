# 拟牛顿类算法

对于大规模问题，函数的海瑟矩阵计算代价特别大或难以得到，即使得到海瑟矩阵我们还需要求解一个大规模线性方程组。因此考虑用海瑟矩阵或其你矩阵的**近似**来进行牛顿迭代，这就是拟牛顿法的基本思想。

## 割线方程

回顾牛顿法的推导过程，设 $f(x)$ 是二阶连续可微函数，根据泰勒展开，向量值函数 $\nabla f(x)$ 在点 $x^{k + 1}$ 处的近似为

$$
\nabla f(x) = \nabla f(x^{k + 1}) + \nabla^2 f(x^{k + 1})(x - x^{k + 1}) + \mathcal{O}(\Vert x - x^{k + 1} \Vert^2)
$$

令 $x = x^k, s^k = x^{k + 1} - x^k$ 以及 $y^k = \nabla f(x^{k + 1}) - \nabla f(x^k)$，得到

$$
\nabla f(x^{k + 1})s^k + \mathcal{O}(\Vert s \Vert^2) = y^k
$$

忽略高阶项，我们希望海瑟矩阵或其逆矩阵的近似 $B^{k + 1}$ 和 $H^{k + 1}$ 满足

$$
\begin{aligned}
\begin{cases}
y^k = B^{k + 1}s^k\\
s^k = H^{k + 1}y^k
\end{cases}
\end{aligned}
$$

上式被称为**割线方程**。为了满足海瑟矩阵的近似矩阵**正定**，我们需要满足**曲率条件**即

$$
(s^k)^\top B^{k + 1}s^k = (s^k)^\top y^k \implies (s^k)^\top y^k > 0
$$

回顾 [Wolfe 准则](./LinearSearch.md/#arimjo-wolfe)有 $\nabla f(x^{k + 1})s^k \geqslant c_2 \nabla f(x^k)^\top s^k$，两边同时减去 $\nabla f(x^k)^\top s^k$ 得

$$
(y^k)^\top s^k \geqslant (c_2 - 1)\nabla f(x^k)^\top s^k > 0
$$

这是因为 $c_2 < 1$ 以及 $s^k = \alpha_k d^k$ 是下降方向。所以**利用 Wolfe 准则选择迭代步长可以保证拟牛顿法的曲率调条件成立**。

## 算法框架

参考经典牛顿法的迭代格式和上述近似分析，可以写出拟牛顿法的算法框架。

<div class="pseudocode">
    \begin{algorithm}
    \caption{拟牛顿法算法框架}
    \begin{algorithmic}
    \STATE 给定 $x^0 \in \mathbb{R}^{n \times n}$（或 $H^0$），令 $k = 0$；
    \WHILE{未达到停机准则}
        \STATE 计算方向 $d^k = -(B^k)^{-1}\nabla f(x^k)$ 或 $d^k = -H^{k}\nabla f(x^k)$；
        \STATE 通过线搜索找到合适的步长 $\alpha_k > 0$，令 $x^{k + 1} = x^k + \alpha_k d^k$；
        \STATE 更新海瑟矩阵的近似矩阵 $B^{k + 1}$ 或其逆的近似矩阵 $H^{k + 1}$；
        \STATE $k \gets k + 1$；
    \ENDWHILE
    \end{algorithmic}
    \end{algorithm}
</div>

下面将讨论一些具体的更新近似矩阵的方法。

??? tip "实际应用中的拟牛顿法"
    在实际应用中基于 $H^k$ 的拟牛顿法更加实用，因为不用矩阵求逆，但是基于 $B^{k}$ 的拟牛顿法有更好的理论性质，产生的迭代点更稳定。

## 拟牛顿矩阵更新方式

### 秩一更新

秩一更新（SR1）公式是结构最简单的拟牛顿矩阵更新公式。设 $B^k$ 是第 $k$ 步的近似海瑟矩阵，我们通过对 $B^k$进行**秩一修正**得到 $B^{k + 1}$，使其满足割线方程。设

$$
B^{k + 1} = B^k + auu^\top
$$

其中 $u \in \mathbb{R}^n, a \in \mathbb{R}$ 为待定系数。根据割线方程

$$
B^{k + 1}s^k = (B^{k} + auu^\top)s^k = y^k \implies (au^\top s^k)u = y^k - B^k s^k
$$

注意到 $au^\top s^k$ 是一个标量，因此 $u$ 和 $y^k - B^k s^k$ 的方向一致。不妨令 $u = y^k - B^k s^k$，带入原方程

$$
a((y^k - B^k s^k)^\top s^k)(y^k - B^k s^k) = y^k - B^k s^k
$$

如果假设 $(y^k - B^k s^k)^\top s^k \neq 0$，可以得到 $a = \dfrac{1}{(y^k - B^k s^k)^\top s^k}$。最终得到更新公式为

$$
\begin{aligned}
\begin{cases}
B^{k + 1} = B^k + \dfrac{(y^k - B^k s^k)(y^k - B^k s^k)^\top}{(y^k - B^k s^k)^\top s^k} \\
H^{k + 1} = H^k + \dfrac{(s^k - H^ky^k)(s^k - H^ky^k)^\top}{(s^k - H^ky^k)^\top s^k}
\end{cases}
\end{aligned}
$$

其中 $H^{k}$ 的更新公式由完全一样的过程得到。

!!! tip "SR1 的缺陷"
    SR1 公式虽然结构简单，但是有一个重大缺陷：它**不能**保证矩阵在迭代过程中保持正定。容易验证 $(y^k - B^k s^k)^\top s^k > 0$ 是 $B^{k + 1}$ 正定的一个充分条件，但这个条件在迭代过程中未必满足。因此在实际应用中较少使用 SR1 公式。

### BFGS 公式

为了克服 SR1 的缺陷，现在考虑对 $B^k$ 的秩二更新。同样采用待定系数法，设

$$
B^{k + 1} = B^k + auu^\top + bvv^\top
$$

其中 $u, v \in \mathbb{R}^n, a, b \in \mathbb{R}$ 待定。根据割线方程有

$$
B^{k + 1}s^k = (B^k + auu^\top + bvv^\top)s^k = y^k \implies (au^\top s^k)u + (bv^\top s^k)v = y^k - B^k s^k
$$

通过选取 $u$ 和 $v$ 可以使得上式成立。最直接地，让上面等式左右两边**两项分别对应相等**，即

$$
\begin{aligned}
\begin{cases}
u = y^k \\
au^\top s^k = 1
\end{cases}
\qquad
\begin{cases}
v = B^k s^k \\
bv^\top s^k = -1
\end{cases}
\end{aligned}
$$

因此得到更新方式

$$
B^{k + 1} = B^k + \frac{y^k (y^k)^\top}{(s^k)^\top y^k} - \frac{B^k s^k (B^k s^k)^\top}{(s^k)^\top B^k s^k}
$$

上式被称为基于 $B^k$ 的 BFGS 公式。根据 SMW 公式并假设 $H^k = (B^k)^{-1}$，可推出基于 $H^k$ 的 BFGS 公式

$$
H^{k + 1} = (I - \rho_k s^k (y^k)^\top)^\top H^k (I - \rho_k s^k (y^k)^\top) + \rho_k s^k (s^k)^\top
$$

其中 $\rho_k = \dfrac{1}{(s^k)^\top y^k}$。若要 BFGS 公式更新产生的矩阵 $H^{k + 1}$ 正定，一个充分条件是不等式 $(s^k)^\top y^k > 0$ 成立且上一步更新矩阵 $H^k$ 正定，实际求解中应使用 Wolfe 准则保证正定。

!!! note "BFGS 的另一个角度"
    BFGS 格式的另一个更深刻的含义是其满足了某种逼近的最优性。具体来说，基于 $H^k$ 的 BFGS 公式得到的 $H^{k + 1}$ 恰好是如下优化问题的最优解：

    $$
    \begin{aligned}
    &\min_{H} && \Vert H - H^k \Vert_W \\
    &\text{s.t.} && H = H^\top\\
    &&& Hy^k = s^k
    \end{aligned}
    $$

    这个优化问题的含义是在满足割线方程的对称矩阵中找到离 $H^k$ 最近的矩阵 $H$。这里 $\Vert \cdot \Vert_W$ 的含义是加权范数，定义为

    $$
    \Vert H - H^k \Vert_W = \Vert W^{1 / 2} H W^{1 / 2} \Vert_F
    $$

    其中 $W$ 可以是任意满足割线方程 $Ws^k = y^k$ 的矩阵。

??? tip "实际应用中的 BFGS"
    BFGS 公式是目前最有效的拟牛顿更新格式之一，它有较好的理论性质，实现起来也并不复杂。

### DFP 公式

如果利用割线方程对 $H^k$ 推导秩二修正的拟牛顿修正，我们将得到基于 $H^k$ 的拟牛顿矩阵更新

$$
H^{k + 1} = H^k - \dfrac{H^k y^k (H^k y^k)^\top}{(y^k)^\top H^k y^k} + \frac{s^k (s^k)^\top}{(y^k)^\top s^k}
$$

这种迭代格式被称为 DFP 公式。根据 SMW 公式可得起关于 $B^k$ 的更新格式

$$
B^{k + 1} = (I - \rho_k y^k (s^k)^\top)^\top B^k (I - \rho_k y^k (s^k)^\top) + \rho_k y^k (y^k)^\top
$$

其中 $\rho_k = \dfrac{1}{(s^k)^\top y^k}$。

可以看到，DFP 公式和 BFGS 公式呈对偶关系。

!!! note "BFGS 的另一个角度"
    具体来说，基于 $B^k$ 的 DFP 公式得到的 $B^{k + 1}$ 恰好是如下优化问题的最优解：

    $$
    \begin{aligned}
    &\min_{B} && \Vert B - B^k \Vert_W \\
    &\text{s.t.} && B = B^\top\\
    &&& Bs^k = y^k
    \end{aligned}
    $$

    这个优化问题的含义是在满足割线方程的对称矩阵中找到离 $B^k$ 最近的矩阵 $B$。这里 $\Vert \cdot \Vert_W$ 的含义是加权范数，定义为

    $$
    \Vert H - H^k \Vert_W = \Vert W^{1 / 2} H W^{1 / 2} \Vert_F
    $$

    其中 $W$ 可以是任意满足割线方程 $Wy^k = s^k$ 的矩阵。

??? tip "实际应用中的 DFP"
    虽然 DFP 和 BFGS 呈对偶关系，但是从实际效果看，DFP 格式整体不如 BFGS，因此实际应用中还是 BFGS 用的多。

## 收敛性分析

本节主要讨论 BFGS 公式的收敛性。

!!! note "BFGS 全局收敛性"
    假设初始矩阵 $B^0$ 是对称正定矩阵，目标函数 $f(x)$ 是二阶连续可微函数，下水平集

    $$
    \mathcal{L} = \{ x \in \mathcal{R}^n \vert f(x) \leqslant f(x^0) \}
    $$

    是凸的，并且存在正数 $m$ 以及 $M$ 使得对于任意 $z \in \mathbb{R}^n$ 以及任意的 $x \in \mathcal{L}$ 有

    $$
    m \Vert z \Vert^2 \leqslant z^\top \nabla^2 f(x)z \leqslant M \Vert z \Vert^2
    $$

    则采用 BFGS 公式并结合 Wolfe 线搜索的拟牛顿法全局收敛到 $f(x)$ 的极小值点 $x^*$。

上述定理说明了 BFGS 公式的全局收敛性，下面的定理将给出收敛速度。

!!! note "BFGS 收敛速度"
    设 $f(x)$ 二阶连续可微，在最优点 $x^*$ 的一个领域内海瑟矩阵利普希茨连续，且使用 BFGS 迭代格式收敛到 $f$ 的最优值点 $x^*$，若迭代点列 $\{ x^k \}$ 满足

    $$
    \sum_{k = 1}^\infty \Vert x^k - x^* \Vert < + \infty
    $$

    则 $\{ x^k \}$ 为 $\mathcal{Q}$-超线性收敛的。
