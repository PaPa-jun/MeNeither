# 线搜索方法

对于无约束优化问题：

$$
\min_{x \in \mathbb{R}^{n}} f(x)
$$

我们将求解过程比喻成下山的过程。为了寻找最低点，在点 $x$ 处需要确定如下两件事：下一步该沿着什么方向走；沿着这个方向走多远。这就构成了无约束优化问题的两个步骤即寻找搜索方向和确定搜索步长。

线搜索类算法的数学表述为：给定当前迭代点 $x^k$，首先通过某种算法选取向量 $d^k$，之后确定正数 $\alpha^k$，则下一步的迭代点可以写作：

$$
x^{k + 1} = x^k + \alpha_k d^k
$$

我们称 $d^k$ 为迭代点 $x^k$ 处的**搜索方向**，$\alpha^k$ 为相应的**步长**。这里要求 $d^k$ 是一个**下降方向**，即 $(d^k)^\mathbf{T}\nabla f(x^k) < 0$。这个下降性质保证了搜索过程会让目标函数值减小。

线搜索算法研究的是如何确定搜索步长即 $\alpha^k$ 的问题。首先研究这个问题是 $d^k$ 的选取千差万别但是 $\alpha^k$ 的选择在不同的算法中非常相似。考虑构造辅助函数：

$$
\varphi(\alpha) = f(x^k + \alpha d^k)
$$

表示迭代时，函数值随 $\alpha$ 的变化。先搜索的目标就是求解 $\alpha_k$ 使得 $\varphi(\alpha_k)$ 的值尽可能小，即找到合适的 $\alpha_k$ 使得 $f$ **充分下降**，同时又不能在寻找 $\alpha_k$ 上花费太多的计算量。一个自然的想法是寻找 $\alpha_k$ 使得

$$
\alpha_k = \arg \min_{\alpha > 0} \varphi(\alpha)
$$

即 $\alpha_k$ 为最佳步长。这种搜索算法被称为**精确线搜索算法**。但是由于这种算法的计算量太大，因此实际中很少使用，取而代之我们常用一种**非精确线搜索算法**，它不要求 $\alpha_k$ 是 $\varphi(\alpha)$ 的最小值点，只要求 $\varphi(\alpha_k)$ 满足一些特定条件。下面介绍这种方法。

## 线搜索准则

在非精确线搜索算法中，选取 $\alpha_k$ 需要 $\varphi(\alpha_k)$ 满足一定要求，这些要求被称为**线搜索准则**。且线搜索准则的合适与否直接影响了算法的收敛性，如果选取不合适的线搜索准则，可能导致算法无法收敛。

### Armijo 准则

首先引入 Armijo 准则，它是一个很常用的线搜索准则。引入 Armijo 准则的目的是保证每一步的迭代充分下降。

!!! note "Armjo 准则"
    设 $d^k$ 是点 $x^k$ 处的下降方向，若

    $$
    \varphi(\alpha) \leqslant \varphi(0) + c_1 \alpha \nabla f(x^k)^{\mathbf{T}}d^k
    $$

    则称步长 $\alpha$ 满足 Armijo 准则，其中 $c_1 \in (0, 1)$ 是一个常数。

Armijo 准则的几何含义很直观，它指的是点 $(\alpha, \varphi(\alpha))$ 必须在直线 

$$
\ell (\alpha) = \varphi(0) + c_1 \alpha \nabla f(x^k)^{\mathbf{T}}d^k
$$

的下方。注意到 $\varphi(\alpha) = f(x^k + \alpha d^k)$ 和 $\varphi(0) = f(x^k)$，且因为 $d^k$ 是下降方向，所以直线的斜率为负，因此满足 Armijo 条件的步长确实能让函数值下降。在实际应用中 $c_1$ 通常选择为一个很小的正数，例如 $10^{-3}$，这使得 Armijo 准则非常容易满足。但是仅仅使用 Armijo 准则无法保证算法的收敛性，这是因为 $\alpha = 0$ 显然满足这个要求，研究这样的步长是没有意义的。为此 Armijo 准则通常与其他准则一起使用。

### 回退法

在优化算法中，寻找一个满足 Armijo 准则的步长是比较容易的，一个常用的算法是**回退法**。给定初值 $\hat{\alpha}$，回退法通过不断以指数方式缩小试探步长，找到第一个满足 Armijo 准则的点。具体来说，回退法选取

$$
\alpha_k = \gamma^{j_0} \hat{\alpha}
$$

其中

$$
j = \min \{ j = 0, 1, \cdots | f(x^k + \gamma^j \hat{\alpha} d^k) \leqslant f(x^k) + c_1 \gamma^j \hat{\alpha}\nabla f(x^k)^{\mathbf{T}}d^k \}
$$

参数 $\gamma \in (0, 1)$ 是一个给定的实数。标准算法如下：

<div class="pseudocode">
    \begin{algorithm}
    \caption{线搜索回退法}
    \begin{algorithmic}
    \STATE 选择初始步长 $\hat{\alpha}$，参数 $\gamma, c \in (0, 1)$；
    \STATE 初始化 $\alpha \gets \hat{\alpha}$；
    \WHILE{$f(x^k + \alpha d^k) > f(x^k) + c\alpha \nabla f(x^k)^{\mathbf{T}d^k}$}
        \STATE 令 $\alpha \gets \gamma \alpha$;
    \ENDWHILE
    \STATE 输出 $\alpha_k = \alpha$.
    \end{algorithmic}
    \end{algorithm}
</div>

算法从大到小地搜索满足条件的 $\alpha$，返回满足要求的尽可能大的 $\alpha$，且由于 $\alpha = 0$ 总是满足 Armijo 条件，因此算法不会无休止地迭代。

### Armijo-Goldstein 准则

为了克服 Armijo 准则的缺陷，即防止 $\alpha^k$ 太小，我们需要引入其他准则。参考 Armijo 设置了一条直线使得 $\alpha$ 在直线下方，我们也可以设置第二条直线，使得 $\alpha$ 一定在直线上方，就是 Armijo-Goldstein 准则。

!!! note "Armijo-Goldstein 准则"
    设 $d^k$ 是点 $x^k$ 处的下降方向，若

    $$
    \begin{align*}
    &\varphi(\alpha) \leqslant \varphi(0) + c \alpha \nabla f(x^k)^{\mathbf{T}}d^k \\
    &\varphi(\alpha) \geqslant \varphi(0) + (1 - c) \alpha \nabla f(x^k)^{\mathbf{T}}d^k
    \end{align*}
    $$

    则称步长 $\alpha$ 满足 Armijo-Goldstein 准则，其中 $c \in \left(0, \dfrac{1}{2}\right)$。

Armijo-Goldstein 准则的几何含义就是将 $\alpha$ 限制在了两条直线

$$
\begin{align*}
&\ell_1(\alpha) = \varphi(0) + c \alpha \nabla f(x^k)^{\mathbf{T}}d^k \\
&\ell_2(\alpha) \geqslant \varphi(0) + (1 - c) \alpha \nabla f(x^k)^{\mathbf{T}}d^k
\end{align*}
$$

之间，这两条直线以 $\varphi(0)$ 为交点，斜率上 $|\ell_1| \leqslant |\ell_2|$，它确实避免了选择过小的 $\alpha$。

### Arimjo-Wolfe 准则

Armijo-GoldStein 准则能够使得函数值充分下降，但是它可能避开了 $\alpha$ 的最优值。为此我们引入 Armijo-Wolf 准则。

!!! note "Armijo-Wolf 准则"
    设 $d^k$ 是点 $x^k$ 处的下降方向，若

    $$
    \begin{align*}
    &\varphi(\alpha) \leqslant \varphi(0) + c_1 \alpha \nabla f(x^k)^{\mathbf{T}}d^k \\
    &\nabla f(x^k + \alpha d^k)^{\mathbf{T}}d^k \geqslant c_2 \nabla f(x^k)^{\mathbf{T}}d^k
    \end{align*}
    $$

    则称步长 $\alpha$ 满足 Armijo-Wolf 准则，其中 $c_1, c_2 \in (0, 1)$ 为给定的常数且 $c_1 < c_2$。


