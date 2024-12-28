# 对偶理论
考虑一个基本的优化问题：

$$
\begin{aligned}
&\min_{x \in \mathbb{R}^n} && f(x) \\
&\text{s.t.} && c_i(x) = 0, & i \in \mathcal{E}\\
&&& c_i(x) \leqslant 0, & i \in \mathcal{I}
\end{aligned}
$$

下面我们将讨论该类型优化问题的拉格朗日对偶问题。

## 拉格朗日函数

考虑其**拉格朗日函数**，其基本思想是为每个约束指定一个**拉格朗日乘子**，以乘子为加权系数将约束增加到目标函数中：

$$
\begin{align*}
L(x, \lambda, \mu) = f(x) + \sum_{i \in \mathcal{I}}\lambda_i c_i(x) + \sum_{i \in \mathcal{E}} \mu_i c_i(x), \quad \mu_i \geqslant 0
\end{align*}
$$

上式约束条件的目的是为了保证 $f(x)$ 在原优化问题定义下的可行点处的取值**大于或等于**拉格朗日函数的取值。即拉格朗日函数是原优化问题的一个**下界**。

## 对偶函数

对拉格朗日函数 $L(x, \lambda, \mu)$ 中的 $x$ 取下确界可以定义**拉格朗日对偶函数**，这一函数在对偶理论中起到关键性作用。

!!! note "拉格朗日对偶函数"
    拉格朗日对偶函数 $ g: \mathbb{R}^m_+ \times \mathbb{R}^p \to [-\infty, +\infty )$ 是拉格朗日函数 $L(x, \lambda, \mu)$ 对于 $\lambda \in \mathbb{R}^m_+, \mu \in \mathbb{R}^p$ 关于 $x$ 的下确界：

    $$
    g(\lambda, \mu) = \inf_{x \in \mathbb{R}^n} L(x, \lambda, \mu)
    $$

## 弱对偶定理

根据上面的定义，可以很容易得出一个结论即

!!! note "弱对偶定理"
    对于任意的 $\mu \geqslant 0$ 和 $\lambda$，拉格朗日对偶函数给出了优化问题

    $$
    \begin{aligned}
    &\min_{x \in \mathbb{R}^n} && f(x) \\
    &\text{s.t.} && c_i(x) = 0, & i \in \mathcal{E}\\
    &&& c_i(x) \leqslant 0, & i \in \mathcal{I}
    \end{aligned}
    $$

    最优值的一个下界，即

    $$
    g(\lambda, \mu) \leqslant p^*, \mu \geqslant 0
    $$

## 对偶问题

那么一个自然的问题就是，从拉格朗日对偶函数获得的下界中，哪个是最优的？为了求解该最优的下界，便有如下拉格朗日对偶问题：

$$
\max_{\lambda, \mu \geqslant 0} g(\lambda, \mu) = \max_{\lambda, \mu \geqslant 0} \inf_{x \in \mathbb{R}^n} L(x, \lambda, \mu)
$$

即对偶问题是**最大化拉格朗日函数的下确界**。

## 强对偶原理

当固定 $(\lambda, \mu)$ 时，拉格朗日函数关于 $x$ 可能无界，那么对偶函数在 $(\lambda, \mu)$ 处的取值为 $-\infty$，此时对偶函数提供的下界时无意义的。因此我们规定拉格朗日对偶函数的定义域为

$$
\mathbf{dom } g = \{ (\lambda, \mu) \mid \mu \geqslant 0, g(\lambda, \mu) > -\infty \}
$$

当 $(\lambda, \mu) \in \mathbf{dom } g$ 时，称其为**对偶可行解**。记对偶问题的最优值为 $q^*$，如果有 $p^* = q^*$，则称**强对偶原理**成立。 

## 线性规划的对偶问题

下面我们具体地看看线性规划中的对偶问题。考虑线性规划

$$
\begin{aligned}
&\min_{x} && c^\top x \\
&\text{s.t.} && Ax = b\\
&&& x \geqslant 0
\end{aligned}
$$

写出拉格朗日函数

$$
L(x, \lambda, \mu) = c^\top x + \lambda^\top (Ax - b) - \mu^\top x, \mu \geqslant 0
$$

固定 $(\lambda, \mu)$，则 $L(x, \lambda, \mu)$ 是一个关于 $x$ 的一次方程，求导有

$$
\nabla_x L(x, \lambda, \mu) = c + A^\top \lambda - \mu
$$

因此原问题对偶函数即拉格朗日函数下确界为

$$
\begin{align*}
    g(\lambda, \mu) = 
    \begin{cases}
        -\lambda^\top b, & A^\top \lambda + c - \mu = 0, \mu \geqslant 0 \\
        -\infty, & \text{others.}
    \end{cases}
\end{align*}
$$

得到对偶问题

$$
\begin{aligned}
&\max_{\lambda, \mu} && -\lambda^\top b \\
&\text{s.t.} && A^\top \lambda + c = \mu\\
&&& \mu \geqslant 0
\end{aligned}
$$


