# 次梯度法

使用[梯度下降法](./GradientDescent.md)的前提为目标函数 $f(x)$ 是一阶可微的，这在实际应用中常常得不到满足。为了能处理这一类问题，本文介绍次梯度算法。

## 次梯度

!!! note "次梯度和次微分"
    设 $f$ 为适当凸函数，$x$ 为定义域 $\mathbf{dom} f$ 中的一点。若向量 $g \in \mathbb{R}^n$ 满足

    $$
    \begin{aligned}
    f(y) \geqslant f(x) + g^\top (y - x), \forall y \in \mathbf{dom}f
    \end{aligned}
    $$

    则称 $g$ 为函数 $f$ 在点 $x$ 处的一个**次梯度**。进一步，称集合

    $$
    \partial f(x) = \{ g \vert g \in \mathbb{R}^n, f(y) \geqslant f(x) + g^\top (y - x), \forall y \in \mathbf{dom}f \}
    $$

    为 $f$ 在点 $x$ 处的**次微分**。

有一个非常重要的性质可以帮助我们计算函数的次梯度即

!!! note annotate "可微点的次梯度"
    设 $f(x)$ 在 $x_0 \in \mathbf{intdom}f$ 处可微，则

    $$
    \partial f(x_0) = \{ \nabla f(x_0) \}
    $$

    其中 $\mathbf{intdom} f$ 表示 $\mathbf{dom}f$ 的所有**内点**。(1)

1.  内点指去除集合边界的点。

!!! example "次梯度的计算"
    设 $f(x) = \Vert x \Vert_2$ 为凸函数，且 $f(x)$ 在 $x \neq 0$ 处可微，因此
    
    $$
    \partial f(x) = \nabla f(x) = \frac{x}{\Vert x \Vert_2}, x \neq 0
    $$

    若 $x = 0$，由定义有

    $$
    \Vert y^2 \Vert_2 \geqslant g^\top y
    $$

    再由 Cauchy-Schwarz 不等式有

    $$
    g^\top y \leqslant \Vert g \Vert_2 \Vert y \Vert_2
    $$

    因此若 $\Vert g \Vert_2 \leqslant 1$ 则 $g$ 满足次梯度定义，反之也成立。因此

    $$
    \begin{aligned}
    \partial f(x) = 
    \begin{cases}
        \frac{x}{\Vert x \Vert_2} & x \neq 0 \\
        \{ g \vert \Vert g \Vert_2 \leqslant 1 \} & x = 0
    \end{cases}
    \end{aligned}
    $$



## 算法结构

现在假设优化问题中 $f(x)$ 为**凸函数**，但不一定可微。对凸函数可以在定义域内点处定义次梯度 $g \in \partial f(x)$，有如下次梯度算法迭代格式：

$$
\begin{aligned}
x^{k + 1} = x^k - \alpha_k g^k, g^k \in \partial f(x^k)
\end{aligned}
$$

其中 $\alpha_k > 0$ 为步长。同之前一样，步长可以有多种选取方式。(1)
{ .annotate }

1.  - 固定步长 $\alpha_k = \alpha$；
    - 固定 $\Vert x^{k + 1} - x^k \Vert$，即 $\alpha_k g^k$ 为常数；
    - 消失的步长 $\alpha_k \to 0$ 且 $\sum_{k = 1}^{\infty} \alpha_k = \infty$；
    - 选取 $\alpha_k$ 使其满足某种线搜索准则。

## 收敛性分析

首先我们列出 $f(x)$ 所需要满足的基本假设：

- $f(x)$ 为**凸函数**；
- $f(x)$ 至少存在一个有限的极小值点 $x^*$，且 $f(x^*) > -\infty$；
- $f(x)$ 本身利普希茨连续，即 $\vert f(x) - f(y) \Vert \leqslant G\Vert x - y \Vert, \forall x, y \in \mathbb{R}^n$。

!!! note "凸函数 $G$-利普希茨连续则次梯度有界"
    设 $f(x)$ 为凸函数，则 $f(x)$ 是 $G$-利普希茨连续的当且仅当 $f(x)$ 的次梯度是有界的，即

    $$
    \Vert g \Vert \leqslant G, \forall g \in \partial f(x), x \in \mathbb{R}^n
    $$

    证明. 先证充分性：假设对任意次梯度 $g$ 都有 $g \leqslant G$，并选取 $g_y \in \partial f(y), g_x \in \partial f(x)$，由次梯度的定义有

    $$
    \begin{aligned}
    g_x^\top (x - y) \geqslant f(x) - f(y) \geqslant g_y^\top (x - y)
    \end{aligned}
    $$

    再由 Cauchy-Schwarz 不等式

    $$
    \begin{aligned}
    &g_x^\top (x - y) \leqslant \Vert g_x^\top \Vert \Vert x - y \Vert \leqslant G \Vert x - y \Vert \\
    &g_y^\top (x - y) \geqslant - \Vert g_y^\top \Vert \Vert x - y \Vert \geqslant - G \Vert x - y \Vert
    \end{aligned}
    $$

    综合有

    $$
    \vert f(x) - f(y) \vert G \Vert x - y \Vert
    $$

    再证必要性：设 $f(x)$ 是 $G$-利普希茨连续的，反设存在 $x$ 和 $g \in \partial f(x)$ 使得 $\Vert g \Vert > G$，取 $y = x +\dfrac{g}{\Vert g \Vert}$，根据次梯度的定义

    $$
    \begin{aligned}
    f(y) &\geqslant f(x) + g^\top (y - x) \\
    &= f(x) + \Vert g \Vert \\
    &= f(x) + G
    \end{aligned}
    $$

    与 $f(x)$ 是 $G$-利普希茨连续的矛盾。

对于次梯度法，一个重要的观察就是它并不是一个下降方法，即无法保证 $f(x^{k + 1}) < f(x^k)$，这给收敛性的证明带来了困难。不过可以分析 $f(x)$ 历史迭代最优点满足的性质。

!!! note "次梯度算法的收敛性"
    在本文假设的条件下，设 $\{ \alpha_k > 0 \}$ 为任意步长序列，$\{ x^k \}$ 是由次梯度算法产生的迭代序列，则对任意的 $k \geqslant 0$，有

    $$
    2\left( \sum_{i = 0}^k \alpha_i \right)(\hat{f}^k - f^*) \leqslant \Vert x^0 - x^* \Vert^2 + \sum_{i = 0}^k \alpha_i^2 G^2
    $$

    其中 $x^*$ 是 $f(x)$ 的一个全局极小值点，$f^* = f(x^*)$，$\hat{f}^k$ 为前 $k$ 次迭代 $f(x)$ 的最小值，即

    $$
    \hat{f}^k = \min_{0 \leqslant i \leqslant k} f(x^i)
    $$

上述定理揭示了次梯度法的一些关键性质：

- 次梯度法的收敛性非常依赖步长的选取；
- 次梯度法是非单调算法，可配套非单调线搜索准则使用。

同时，可直接得到不同步长取法下次梯度法的收敛性。

!!! note "不同步长取法下次梯度算法的收敛性"
    在本文假设条件下，次梯度法的收敛性满足：
    
    - 取 $\alpha_i = t$ 为固定步长，则

    $$
    \begin{aligned}
    \hat{f}^k - f^* \leqslant \frac{\Vert x^0 - x^* \Vert^2}{2kt} + \frac{G^2t}{2}
    \end{aligned}
    $$

    - 取 $\alpha_i$ 使得 $\Vert x^{i + 1} - x^i \Vert$ 固定，即 $\alpha_i \Vert g^i \Vert = s$ 为常数，则

    $$
    \begin{aligned}
    \hat{f}^k - f^* \leqslant \frac{G \Vert x^0 - x^* \Vert^2}{2ks} + \frac{Gs}{2}
    \end{aligned}
    $$

    - 取 $\alpha_i$ 为消失步长，即 $\alpha_i \to 0$ 且 $\sum_{k = 1}^{\infty} \alpha_k = \infty$，则

    $$
    \begin{aligned}
    \hat{f}^k - f^* \leqslant \frac{\Vert x^0 - x^* \Vert^2 + G^2 \sum_{i = 0}^k \alpha_i^2}{2\sum_{i = 0}^k \alpha_i}
    \end{aligned}
    $$

    进一步可得 $\hat{f}^k$ 收敛到 $f^*$。

从上述结论可知，无论是固定补偿还是固定 $\Vert x^{i + 1} - x^i \Vert$，次梯度法均没有收敛性，只能得到一个次优解；只有当 $\alpha_k$ 取消失步长时 $\hat{f}^k$ 才具有收敛性。一个常用的取法是 $\alpha_k = \dfrac{1}{k}$，这样不但可以保证其为消失步长，还能保证 $\sum_{i = 0}^{\infty}\alpha_i^2$ 有界。