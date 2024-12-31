# 增广拉格朗日函数法

回顾[罚函数法的收敛性分析](./PenaltyFunctionMethod.md/#_3)，在等式约束的罚函数算法执行过程中，通过求解子问题使得 $\nabla_x P_E(x^{k + 1}, \sigma_k) \to 0$，有

$$
c_i(x^{k + 1}) \to - \frac{\lambda_i^*}{\sigma_k}, \quad \forall i \in \mathcal{E}
$$

为了保证可行性 $c_i(x) = 0$，罚因子必须**趋于正无穷**。此时，子问题因条件数爆炸而难以求解。增广拉格朗日法通过对二次罚函数进行修正，使得对优先的罚因子，得到的逼近最优解也是可行的。

## 等式约束问题

对于等式约束优化问题

$$
\begin{aligned}
\min_x& & &f(x) \\
\text{s.t.}& & &c_i(x) = 0, \quad i \in \mathcal{E}
\end{aligned}
$$

定义其**增广拉格朗日函数**。

!!! note "等式约束增广拉格朗日函数"
    对于等式约束，其增广拉格朗日函数为

    $$
    L_{\sigma}(x, \lambda) = f(x) + \sum_{i \in \mathcal{E}} \lambda_i c_i(x) + \frac{1}{2}\sigma \sum_{i \in \mathcal{E}} c_i^2(x)
    $$

    其中 $\sigma$ 为罚因子。

即在拉格朗日函数的基础上，添加约束的二次罚函数。在第 $k$ 步迭代，给定罚因子 $\sigma_k$ 和乘子 $\lambda^k$，增广拉格朗日函数 $L_{\sigma_k}(x, \lambda^k)$ 的最小值点 $x^{k + 1}$ 满足

$$
\nabla_x L_{\sigma_k}(x^{k + 1}, \lambda^k) = \nabla f(x^{k + 1}) + \sum_{i \in \mathcal{E}} (\lambda_i^k + \sigma_k c_i(x^{k + 1})) \nabla c_i(x^{k + 1}) = 0
$$

优化问题的最优解 $x^*$ 以及相应的乘子 $\lambda^*$ 应满足

$$
\nabla f(x^*) + \sum_{i \in \mathcal{E}} \lambda_i^* \nabla c_i (x^*) = 0
$$

为了使增广拉格朗日函数法产生的迭代点列收敛到 $x^*$，需要保证上面两个式子在最优解处的**一致性**。因此，对于充分大的 $k$ 有

$$
\lambda_i^* \approx \lambda_i^k + \sigma_i c_i(x^{k + 1}) \implies c_i (x^{k + 1}) \approx \frac{1}{\sigma_k}(\lambda_i^* - \lambda_i^k)
$$

所以当 $\lambda_i^k$ 足够接近 $\lambda_i^*$ 时，点 $x^{k + 1}$ 处的约束违反度会远小于 $\dfrac{1}{\sigma_k}$。

在本文的开头说明使用二次罚函数法时，约束违反度正比于 $\dfrac{1}{\sigma_k}$，即增广拉格朗日函数法可以通过有效地更新乘子来降低约束违反度。而上式表明，乘子的一个有效的**更新格式**为

$$
\lambda_i^{k + 1} = \lambda_i^k + \sigma_k c_i(x^{k + 1}), \quad \forall i \in \mathcal{E}
$$

从而给出增广拉格朗日函数法。

<div class="pseudocode">
    \begin{algorithm}
    \caption{等式约束问题的增广拉格朗日函数法}
    \begin{algorithmic}
    \STATE 选取初始点 $x^0$，乘子 $\lambda^0$，罚因子 $\sigma_0 > 0$，罚因子更新常数 $\rho > 0$，约束违反度常数 $\varepsilon > 0$ 和精度要求 $\eta_k > 0$，并令 $k \gets 0$；
    \FOR{$k = 0, 1, 2, \dots$}
        \STATE 以 $x^k$ 为初始点，求解 $\min_x L_{\sigma_k}(x, \lambda^k)$ 得到满足精度条件 $\Vert \nabla_x L_{\sigma_k}(x, \lambda^k) \Vert \leqslant \eta_k$ 的解 $x^{k + 1}$；
        \IF{$\Vert c(x^{k + 1}) \Vert \leqslant \varepsilon$}
            \STATE 返回近似解 $x^{k + 1}$，$\lambda^k$，终止迭代；
        \ENDIF
        \STATE 更新乘子：$\lambda^{k + 1} = \lambda^k + \sigma_k c(x^{k + 1})$；
        \STATE 更新罚因子：$\sigma_{k + 1} = \rho \sigma_k$；
    \ENDFOR
    \end{algorithmic}
    \end{algorithm}
</div>

??? tip
    - 算法中第 3 行的意义为，可以非精确求解关于 $x$ 的子问题；
    - 算法第 4 行为终止条件：若找到一个可行点且为子问题的解，则终止迭代；
    - $\sigma_k$ 增长不宜过快：
        1. 随着罚因子 $\sigma_k$ 的增大, 可见 $L_{\sigma_k}(x, \lambda^k)$ 关于 $x$ 的海瑟矩阵的条件数也将增大, 这将导致数值困难；
        2. $\sigma_k$ 与 $\sigma_{k + 1}$ 接近时，$x^k$ 可以作为求解 $x^{k + 1}$ 的初始点, 以加快收敛。
    - $\sigma_k$ 不应增长过慢: 算法整体的收敛速度将变慢(惩罚不足)；
    - 在实际中, 我们应该控制 $\sigma_k$ 的增长维持在一个合理的速度区间内，一个简单的方法是维持 $\rho \in [2, 10]$.

## 收敛性分析

我们阐述由增广拉格朗日函数法导出的极小值点和原问题的极小值点有什么关系。实际上, 增广拉格朗日函数在一定条件下将成为精确罚函数。

!!! note "严格局部极小解定理"
    设 $x^*$，$\lambda^*$ 分别为等式约束问题的**局部极小解**和相应的乘子，并且在点 $x^*$ 处 LICQ 和二阶充分条件成立。那么，存在一个有限大的常数 $\bar{\sigma}$，使得对任意的 $\sigma \geqslant \bar{\sigma}$，$x^*$ 都是 $L_{\sigma}(x, \lambda^*)$ 的严格局部极小解。反之，如果 $x^*$ 为 $L_{\sigma}(x, \lambda^*)$ 的局部极小解且满足 $c_i(x^*) = 0, i \in \mathcal{E}$，那么 $x^*$ 为等式约束问题的局部极小解。

??? quote "证明"

    因为 \( x^* \) 为问题等式约束优化问题的局部极小解且二阶充分条件成立，所以

    \[
    \nabla_x L(x^*, \lambda^*) = \nabla f(x^*) + \sum_{i \in E} \lambda_i^* \nabla c_i(x^*) = 0,
    \]

    \[
    u^T \nabla_{xx}^2 L(x^*, \lambda^*) u > 0, \forall u \in \{u | \nabla c(x^*)^T u = 0\}.
    \]

    这里

    \[
    \nabla c(x^*) := (\nabla c_1(x^*), \nabla c_2(x^*), \ldots, \nabla c_{|E|}(x^*)) \in \mathbb{R}^{n \times |E|}.
    \]

    对比 \( L_\sigma(x^*, \lambda^*) \) 和 \( L(x^*, \lambda^*) \) 的表达式，由 \( c_i(x^*) = 0, i \in E \)，得

    \[
    \nabla_x L_\sigma(x^*, \lambda^*) = \nabla_x L(x^*, \lambda^*) = 0,
    \]

    \[
    \nabla_{xx}^2 L_\sigma(x^*, \lambda^*) = \nabla_{xx}^2 L(x^*, \lambda^*) + \sigma \sum_{i \in E} \nabla c_i(x^*) \nabla c_i(x^*)^T.
    \]

    为了证明 \( x^* \) 是 \( L_\sigma(x^*, \lambda^*) \) 的严格局部极小解，只需证对于充分大的 \( \sigma \) 成立

    \[
    \nabla_{xx}^2 L_\sigma(x^*, \lambda^*) \succ 0.
    \]

    假设该结论不成立，则对任意 \( k \) 以及 \( \sigma_k > 0 \)，存在 \( u_k \) 满足 \( \|u_k\| = 1 \)，且满足：

    \[
    u_k^T \nabla_{xx}^2 L_{\sigma_k}(x^*, \lambda^*) u_k = u_k^T \nabla_{xx}^2 L(x^*, \lambda^*) u_k + \sigma_k \left\| \nabla c(x^*)^T u_k \right\|^2 \leq 0,
    \]

    则

    \[
    \left\| \nabla c(x^*)^T u_k \right\|^2 \leq -\frac{1}{\sigma_k} u_k^T \nabla_{xx}^2 L(x^*, \lambda^*) u_k \rightarrow 0, \quad \sigma_k \rightarrow \infty.
    \]

    因为 \( \{u_k\} \) 为有界序列，必存在聚点，设为 \( u \)。那么

    \[
    \nabla c(x^*)^T u = 0, \quad u^T \nabla_{xx}^2 L(x^*, \lambda^*) u \leq 0.
    \]

    这与证明开头的两个式子矛盾，故结论成立。

    反之，若 \( x^* \) 满足 \( c_i(x^*) = 0 \) 且为 \( L_\sigma(x, \lambda^*) \) 的局部极小解，那么对于任意与 \( x^* \) 充分接近的可行点 \( x \)，我们有

    \[
    f(x^*) = L_\sigma(x^*, \lambda^*) \leq L_\sigma(x, \lambda^*) = f(x),
    \]

    因此，\( x^* \) 为原问题的一个局部极小解，证毕。

通过进一步假设乘子点列的有界性以及收敛点处的约束品性，我们可以证明算法迭代产生的点列 $\{x^k\}$ 会有子列收敛到原问题的一阶稳定点。

!!! note "增广拉格朗日函数法的收敛性"
    假设乘子列 $\{\lambda^k\}$ 是有界的，罚因子 $\sigma_k \to +\infty, k \to \infty$，Algorithm 1 中精度 $\eta_k \to 0$。迭代点列 $\{x^k\}$ 的一个子序列 $\{x^{k_j + 1}\}$ 收敛到 $x^*$，并且在点 $x^*$ 处 LICQ 成立。那么存在 $\lambda^*$，满足

    $$
    \lambda^{k_j + 1} \to \lambda^*, \quad j \to \infty \\
    \nabla f(x^*) + \nabla c(x^*)\lambda^* = 0, \quad c(x^*) = 0
    $$

## 一般约束问题

对于一般约束优化问题

$$
\begin{aligned}
\min_x& & &f(x) \\
\text{s.t.}& & &c_i(x) = 0, \quad i \in \mathcal{E} \\
 & & &c_i(x) \leqslant 0, \quad i \in \mathcal{I}
\end{aligned}
$$

也可以定义其拉格朗日函数以及设计对应的增广拉格朗日函数法。

### 增广拉格朗日函数

对于一般约束优化问题，通过引入**松弛变量**可以得到如下等价形式：

$$
\begin{aligned}
\min_{x, s}& & &f(x) \\
\text{s.t.}& & &c_i(x) = 0, \quad i \in \mathcal{E} \\
 & & &c_i(x) + s_i = 0, \quad i \in \mathcal{I} \\
 & & &s_i \geqslant 0, \quad i \in \mathcal{I}
\end{aligned}
$$

保留非负约束(1)，可以构造拉格朗日函数
{ .annotate }

1.  在拉格朗日函数的定义中，往往倾向于将简单的约束（比如非负约束、盒子约束等）保留，对于复杂的约束引入乘子。

$$
L(x, s, \lambda, \mu) = f(x) + \sum_{i \in \mathcal{E}}\lambda_i c_i(x) + \sum_{i \in \mathcal{I}} \mu_i (c_i(x) + s_i), \quad s_i \geqslant 0, i \in \mathcal{I}
$$

记一般约束优化问题的二次罚函数为 $p(x, s)$，则

$$
p(x, s) = \sum_{i \in \mathcal{E}}c_i^2(x) + \sum_{i \in \mathcal{E}}(c_i(x) + s_i)^2
$$

我们构造增广拉格朗日函数如下

$$
L_{\sigma}(x, s, \lambda, \mu) = f(x) + \sum_{i \in \mathcal{E}}\lambda_i c_i(x) + \sum_{i \in \mathcal{I}} \mu_i (c_i(x) + s_i) + \frac{\sigma}{2}p(x, s), \quad s_i \geqslant 0, i \in \mathcal{I}
$$

其中 $\sigma$ 是罚因子。

### 增广拉格朗日函数法

在第 $k$ 步迭代中，给定乘子 $\lambda^k, \mu^k$ 和罚因子 $\sigma_k$，我们需要求解如下问题：

$$
\min_{x, s} L_{\sigma_k}(x, s, \lambda^k, \mu^k), \quad \text{s.t.} \quad s \geqslant 0
$$

以得到 $x^{k + 1}, s^{k + 1}$。这里介绍一种求解该问题的方法。

首先，固定 $x$，关于 $s$ 的子问题可以表示为

$$
\min_{s \geqslant 0} \sum_{i \in \mathcal{I}} \mu_i (c_i(x) + s_i) + \frac{\sigma_k}{2} \sum_{i \in \mathcal{I}} (c_i(x) + s_i)^2
$$

容易解得

$$
s_i = \max\left\{ -\frac{\mu_i}{\sigma_k} - c_i(x), 0 \right\}, \quad i \in \mathcal{I}
$$

将其带入 $L_{\sigma_k}$，我们有

$$
\begin{aligned}
L_{\sigma_k}(x, \lambda^k, \mu^k) = &f(x) + \sum_{i \in \mathcal{E}} \lambda_i c_i(x) + \frac{\sigma_k}{2} \sum_{i \in \mathcal{E}} c_i^2(x) + \\
&\frac{\sigma_k}{2}\sum_{i \in \mathcal{I}}\left(\max\left\{ \frac{\mu_i}{\sigma_k} + c_i(x), 0\right\}^2 - \frac{\mu_i^2}{\sigma_k^2} \right)
\end{aligned}
$$

其为关于 $x$ 的连续可微函数（如果 $f(x), c_i(x), i \in \mathcal{I}\cup\mathcal{E}$ 连续可微）。因此，优化问题等价于

$$
\min_{x \in \mathbb{R}^n} L_{\sigma_k}(x, \lambda^k, \mu^k)
$$

并且可以利用梯度法求解。

下面讨论如何更新**对偶乘子**。对于一般约束优化问题，其最优解 $x^*, s^*$ 和乘子 $\lambda^*, \mu^*$ 需满足 KKT 条件：

$$
\begin{aligned}
&0 = \nabla f(x^*) + \sum_{i \in \mathcal{E}} \lambda^*_i \nabla c_i(x^*) + \sum_{i \in \mathcal{I}} \mu_i^* \nabla c_i(x^*) \\
&\mu_i^* \geqslant 0, \quad i \in \mathcal{I} \\
&s_i^* \geqslant 0, \quad i \in \mathcal{I}
\end{aligned}
$$

增广拉格朗日函数的最优解 $x^{k + 1}, s^{k + 1}$ 需满足

$$
\begin{aligned}
0 = &\nabla f(x^{k + 1}) + \sum_{i \in \mathcal{E}} \left( \lambda_i^k + \sigma_k c_i (x^{k + 1}) \right) \nabla c_i(x^{k + 1}) + \\
&\sum_{i \in \mathcal{I}}(\mu^k_i + \sigma_k(c_i(x^{k + 1}) + s_i^{k + 1})) \nabla c_i(x^{k + 1}) \\
s_i^{k + 1} = &\max\left\{ -\frac{\mu_i^k}{\sigma_k} - c_i(x^{k + 1}), 0 \right\}, \quad i \in \mathcal{I}
\end{aligned}
$$

对比两个条件，可知乘子的更新格式为

$$
\begin{aligned}
&\lambda_i^{k + 1} = \lambda_i + \sigma_k c_i(x^{k + 1}), \quad i \in \mathcal{E} \\
&\mu_i^{k + 1} = \max\{ \mu_i^k + \sigma_k c_i(x^{k + 1}), 0 \}, \quad i \in \mathcal{I}
\end{aligned}
$$

对于等式约束，约束违反度定义为

$$
v_k(x^{k + 1}) = \sqrt{\sum_{i \in \mathcal{E}} c_i^2(x^{k + 1}) + \sum_{i \in \mathcal{I}}\left( c_i(x^{k + 1}) + s_i^{k + 1} \right)^2}
$$

根据前面的计算消去 $s$，约束违反度为

$$
v_k(x^{k + 1}) = \sqrt{\sum_{i \in \mathcal{E}} c_i^2(x^{k + 1}) + \sum_{i \in \mathcal{I}}\max\left\{ c_i(x^{k + 1}), -\frac{\mu_i^k}{\sigma_k} \right\}^2}
$$

综上所属，可以给出一般约束优化问题的增广拉格朗日函数法。

<div class="pseudocode">
    \begin{algorithm}
    \caption{一般约束问题的增广拉格朗日函数法}
    \begin{algorithmic}
    \STATE 选取初始点 $x^0$，乘子 $\lambda^0, \mu^0$，罚因子 $\sigma_0 > 0$，约束违反度常数 $\varepsilon > 0$，精度常数 $\eta > 0$，以及常数 $0 < \alpha \leqslant \beta \leqslant 1$ 和 $\rho > 1$，令 $\eta_0 = \dfrac{1}{\sigma_0}, \varepsilon_0 = \dfrac{1}{\sigma_0^\alpha}$ 以及 $k \gets 0$；
    \FOR{$k = 0, 1, 2, \dots$}
        \STATE 以 $x^k$ 为初始点，求解 $\min_x L_{\sigma_k}(x, \lambda^k, \mu^k)$ 得到满足精度条件 $\Vert \nabla_x L_{\sigma_k}(x, \lambda^k, \mu^k) \Vert_2 \leqslant \eta_k$ 的解 $x^{k + 1}$；
        \IF{$v_k(x^{k + 1}) \leqslant \varepsilon$}
            \IF{$v_k(x^{k + 1}) \leqslant \varepsilon$ \and $\Vert \nabla_x L_{\sigma_k}(x^{k + 1}, \lambda^k, \mu^k) \Vert_2 \leqslant \eta$}
                \STATE 得到逼近解 $x^{k + 1}, \lambda^k, \mu^k$，终止迭代；
            \ENDIF
            \STATE 更新乘子：$\lambda_i^{k + 1} = \lambda_i + \sigma_k c_i(x^{k + 1}), \quad i \in \mathcal{E}$；
            \STATE 更新乘子：$\mu_i^{k + 1} = \max\{ \mu_i^k + \sigma_k c_i(x^{k + 1}), 0 \}, \quad i \in \mathcal{I}$；
            \STATE 罚因子不变：$\sigma_{k + 1} = \sigma_k$；
            \STATE 减小问题求解误差和约束违反度：$\eta_{k + 1} = \dfrac{\eta_k}{\sigma_{k + 1}}, \varepsilon_{k + 1} = \dfrac{\varepsilon_k}{\sigma_{k + 1}^\beta}$
        \ELSE
            \STATE 乘子不变：$\lambda^{k + 1} = \lambda^k, \mu^{k + 1} = \mu^k$；
            \STATE 更新罚因子：$\sigma_{k + 1} = \rho \sigma_k$；
            \STATE 调整问题求解误差和约束违反度：$\eta_{k + 1} = \dfrac{1}{\sigma_{k + 1}}, \varepsilon_{k + 1} = \dfrac{1}{\sigma_{k + 1}^\alpha}$；
        \ENDIF
    \ENDFOR
    \end{algorithmic}
    \end{algorithm}
</div>

??? tip
    在 Algorithm 2 中，我们利用约束违反度的大小判断参数的更新方式：

    - 若 $v_k(x^{k + 1})$ 满足精度条件，则进行乘子的更新，并提高子问题求解精度，罚因子不变；
    - 若不满足，则不进行乘子的更新，并适当增大罚因子以便得到约束违反度更小的解。
