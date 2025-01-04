# 罚函数法

考虑约束优化问题

$$
\begin{aligned}
\min& & &f(x) \\
\text{s.t.}& & &x \in \mathcal{X}\\
\end{aligned}
$$

这里 $\mathcal{X} \in \mathbb{R}^n$ 是问题的可行域。

## 等式约束的二次罚函数法

假设约束优化问题的约束为等式约束，即

$$
\begin{aligned}
\min_x& & &f(x) \\
\text{s.t.}& & &c_i(x) = 0, \quad i \in \mathcal{E}
\end{aligned}
$$

其中 $x \in \mathbb{R}^n$，$\mathcal{E}$ 为等式约束的指标集，$c_i(x)$ 为连续函数。罚函数的基本思想是将上述约束优化问题转化为**无约束**优化问题来进行求解。

为了保证逼近质量，无约束优化问题的目标函数为原约束问题的目标函数加上与约束函数有关的惩罚项。对于可行域外的点，惩罚项为正；对可行域内的点，惩罚项为 0。

!!! note "等式约束的二次罚函数法"
    对等式约束的优化问题，定义**二次罚函数**为

    $$
    P_E(x, \sigma) = f(x) + \frac{1}{2}\sigma \sum_{i \in \mathcal{E}} c_i^2(x)
    $$

    其中等式右端第二项称为惩罚项，$\sigma > 0$ 称为罚因子。

由于上述二次惩罚函数对不满足约束的点进行惩罚，在迭代过程中点列**一般处于可行域之外**，因此它也被称为**外点罚函数**。

<div class="pseudocode">
    \begin{algorithm}
    \caption{二次罚函数法}
    \begin{algorithmic}
    \STATE 给定 $\sigma_1 > 0$，$x^0$，$k \gets 1$。罚因子增长系数 $\rho > 1$；
    \WHILE{未达到收敛准则}
        \STATE 以 $x^k$ 为初始点，求解 $x^{k + 1} = \mathop{\arg\min}_x P_E(x, \sigma_k)$；
        \STATE 选取 $\sigma_{k + 1} = \rho \sigma_k$；
        \STATE $k \gets k + 1$；
    \ENDWHILE
    \end{algorithmic}
    \end{algorithm}
</div>

算法通过逐步增大罚因子来使得迭代点接近可行域，第 3 行的 $\mathop{\arg\min}$ 的含义如下面三种之一：

- $x^{k + 1}$ 是罚函数 $P_E(x, \sigma_k)$ 的全局极小解；
- $x^{k + 1}$ 是罚函数 $P_E(x, \sigma_k)$ 的局部极小解；
- $x^{k + 1}$ 不是罚函数 $P_E(x, \sigma_k)$ 的严格极小解，但近似满足**一阶最优性条件**即 $\nabla_x P_E(x^{k + 1}, \sigma_k) \sim 0$。

算法执行过程中 $\sigma_k$ 的选取需要小心：若增长太快，则子问题不易求解，太慢则需要的外迭代次数增多，另外**过小的罚因子可能导致迭代点发散**。

## 收敛性分析

为了讨论方便，假设对每个 $\sigma_k$，罚函数 $P_E(x, \sigma_k)$ 的最小值点都是存在的。

!!! note "二次罚函数法的收敛性 1"
    设 $k^{k + 1}$ 是 $P_E(x, \sigma_k)$ 的全局极小解，$\sigma_k$ 单调上升趋于无穷，则 $\{x^k\}$ 的每个极限点 $x^*$ 都是原问题的全局极小值解。

??? quote "证明"
    设 $\bar{x}$ 是原问题的全局极小解，即

    $$
    f(\bar{x}) \leqslant f(x), \quad \forall x \text{ 满足 } c_i(x) = 0, \quad i \in \mathcal{E}
    $$

    由定理条件，$x^{k + 1}$ 是 $P_E(x, \sigma_k)$ 的全局极小解，即 $P_E(x^{k + 1}, \sigma_k) \leqslant P_E(\bar{x}, \sigma_k)$，所以

    $$
    f(x^{k + 1}) + \frac{\sigma_k}{2} \sum_{i \in \mathcal{E}} c_i^2 (x^{k + 1}) \leqslant f(\bar{x}) + \frac{\sigma_k}{2} \sum_{i \in \mathcal{E}} c_i^2 (\bar{x}) = f(\bar{x})
    $$

    整理可得

    $$
    \sum_{i \in \mathcal{E}} c_i^2 (x^{k + 1}) \leqslant \frac{2}{\sigma_k}\left(f(\bar{x}) - f(x^{k + 1})\right)
    $$

    对上式取极限，有

    $$
    \lim_{k \to \infty} \sum_{i \in \mathcal{E}} c_i^2 (x^{k + 1}) = \sum_{i \in \mathcal{E}} c_i^2 (x^*) = 0 \implies f(\bar{x}) \geqslant f(x^*)
    $$

    由 $\bar{x}$ 最优性可知 $f(\bar{x}) = f(x^*)$，即 $x^*$ 也是全局极小值点。

以上定理表明，若可以**找到子问题的全局极小解**，则他们的极限点为原问题的最小值点。这个要求比较高，下面给出一个弱一点的定理。

!!! note "二次罚函数的收敛性 2"
    设 $f(x)$ 与 $c_i(x), i \in \mathcal{E}$ 连续可微，正数序列 $\varepsilon_k \to 0, \sigma_k \to \infty$，在 Algorithm 1 中，子问题的解 $x^{k + 1}$ 满足 $ \Vert \nabla_x P_E(x^{k + 1}, \sigma_k) \Vert \leqslant \varepsilon_k$，而对 $\{x^k\}$ 的任何极限点 $x^*$，都有 $\{ \nabla c_i(x^*), i \in \mathcal{E} \}$ 线性无关，则 $x^*$ 是等式约束优化问题的 KKT 点，且

    $$
    \lim_{k \to \infty} (-\sigma_k c_i(x^{k + 1})) = \lambda_i^*, \quad \forall i \in \mathcal{E}
    $$

    其中 $\lambda_i^*$ 是约束 $c_i(x^*) = 0$ 对应的拉格朗日乘子。

上述定理的证明略过，给出如下说明：

- 不管 $\{ \nabla c_i (x^*) \}$ 是否线性无关，通过 Algorithm 1 给出解 $x^k$ 的聚点总是 $\varphi(x) = \Vert c(x) \Vert^2$ 的一个稳定点。这说明即便没有找到可行解，我们也得到了一个使得约束 $c(x) = 0$ 违反度相对较小的一个解；
- 定理虽然不要求每一个子问题精确求解，但要获得原问题的解，子问题解的精度需要越来越高。

## 一般约束问题的二次罚函数法

对于不等式约束的优化问题

$$
\begin{aligned}
\min_x& & &f(x) \\
\text{s.t.}& & &c_i(x) \leqslant 0, \quad i \in \mathcal{I}
\end{aligned}
$$

若用原先的方法来定义罚函数，则也会惩罚 $c_i(x) < 0$ 的可行点，因此需要改进。

!!! note "不等式约束的二次罚函数"
    对不等式约束优化问题，定义其二次罚函数为

    $$
    P_I(x, \sigma) = f(x) = \frac{1}{2} \sum_{i \in \mathcal{I}} \tilde{c}_i^2(x)
    $$

    其中等式右端第二项称为惩罚项，

    $$
    \tilde{c}_i(x) = \max\{ c_i(x), 0 \}
    $$

    常数因子 $\sigma$ 称为罚因子。

注意到 $h(t) = (\max\{t, 0\})^2$ 是可导的，因此可以用梯度类算法求解子问题。

一般的约束优化问题可能既包含等式约束又包含不等式约束，它的形式为

$$
\begin{aligned}
\min_x& & &f(x) \\
\text{s.t.}& & &c_i(x) = 0, \quad i \in \mathcal{E} \\
 & & &c_i(x) \leqslant 0, \quad i \in \mathcal{I}
\end{aligned}
$$

针对这个问题，我们只需要将两种罚函数相加即可。

!!! note "一般约束的二次罚函数"
    对一般约束优化问题，定义二次罚函数

    $$
    P(x, \sigma) = f(x) + \frac{1}{2}\sigma \left[ \sum_{i \in \mathcal{E}} c_i^2(x) + \sum_{i \in \mathcal{I}} \tilde{c}_i^2(x) \right]
    $$

    其中等式右端第二项称为惩罚项，常数 $\sigma$ 为罚因子。
