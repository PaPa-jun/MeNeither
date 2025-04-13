# 集中不等式

在统计学习理论中，对于给定一个场景下提出的新模型或新策略，我们可能需要计算与证明我们的策略与理论上最好情况之间的差异，或称为遗憾值的上下界(regret bound)。这使得我们需要一系列相应的数学工具来支持我们的证明工作。

## 尾概率

!!! question
    设 $X_1, X_2, \cdots, x_n$ 是一个**独立同分布**的随机变量序列，假设其均值 $u = \mathbb{E}(X)$ 方差 $\sigma^2 = Var(X)$ 均存在。若采用如下估计量来估计 $\mu$：

    $$
        \hat{\mu} = \frac{1}{n}\sum_{i = 1}^n X_i
    $$

    考虑如下问题：
    
    1. 该估计量是否是一个**无偏估计**？
    2. $\mu$ 和 $\hat{\mu}$ 之间相差多远？

对于估计量的无偏性，可以从

$$
\mathbb{E}(\hat{\mu}) = \mathbb{E}\left( \frac{1}{n}\sum_{i = 1}^n X_i \right) = \frac{1}{n}\sum_{i = 1}^n \mathbb{E}(X_i) = \mu
$$

得出该估计是一个无偏估计的结论。为了衡量估计量与总体均值之间的差距，我们可以考虑**方差**即

$$
Var(\hat{\mu}) = Var\left( \frac{1}{n}\sum_{i = 1}^n X_i \right) = \frac{1}{n^2}\sum_{i = 1}^n Var(X_i) = \frac{\sigma^2}{n}
$$

这样的衡量方法在现实当中并不理想，因为它无法确定估计量是大部分情况都偏离还是仅有小部分情况偏离，从而我们需要引出**尾概率**这个概念。

!!! note "尾概率"
    若 $X$ 是一个均值为 $\mu$ 的随机变量，$\varepsilon$ 是一个常数则：

    - $P(X \geqslant \mu + \varepsilon)$ 称为其右尾概率；
    - $P(X \leqslant \mu - \varepsilon)$ 称为其左尾概率；
    - $P(\vert X - \mu \vert \geqslant \varepsilon)$ 称为其双尾概率。

从定义上可以很直观的看出，所谓尾概率衡量的就是估计量落在均值的一个领域外的概率大小，我们通常无法显示的计算这个概率，但是可以用一些工具给出一个合理的界。

## 马尔可夫不等式

马尔可夫不等式的**基本思想**是: 给定一个非负的随机变量 $X$，如果其期望(或均值)是一个较小的值，对于随机变量的采样出来的序列 $\{x_1, x_2, \cdots, x_n\}$ 中，我们观察到一个值较大的 $x_i$ 的概率是很小的。

!!! note "马尔可夫不等式"
    给定一个随机变量 $X > 0$ 和常数 $a > 0$，我们有
    $$
        P(X \geqslant a) \leqslant \frac{\mathbb{E}(X)}{a}
    $$
    经过代换 $a = k \cdot E(X)$ 可以得到另一种表达：$P\left(X \geqslant k \cdot E(X)\right) \leqslant \dfrac{1}{k}$。

??? quote "证明"
    设随机变量 $Y$，其定义为

    $$
    \begin{aligned}
        Y = \begin{cases}
            a & X \geqslant a \\
            0 & X < a
        \end{cases}
    \end{aligned}
    $$

    显然有 $E(X) \geqslant E(Y)$ 总是成立。根据离散随机变量的期望定义，有

    $$
        E(Y) = \sum_{i = 1}^n y_i \cdot P(y_i) = 0 \cdot P(X < a) + a \cdot P(X \geqslant a) = a \cdot P(X \geqslant a)
    $$

    结合 $E(X)$ 和 $E(Y)$ 的关系，有

    $$
        P(X \leqslant a) \leqslant \frac{E(X)}{a}
    $$

    考虑连续的情况，有

    $$
        \begin{aligned}
            E(X) &= \int_{0}^\infty x p(x)\,dx \\
            &\geqslant \int_{a}^\infty x p(x)\,dx \\
            &\geqslant \int_a^\infty a p(x)\,dx \\
            &= a P(X \geqslant a)
        \end{aligned}
    $$

考虑随机变量 $Y$ 服从 $[0, 4]$ 上的均匀分布，则

$$
E(Y) = \int_0^4 \frac{x}{4}\,dx = 2
$$

根据马尔可夫不等式，有

$$
E(Y \geqslant 2) \leqslant \frac{2}{2} = 1 \quad E(Y \geqslant 3) \leqslant \frac{2}{3} \quad E(Y \geqslant 4) \leqslant \frac{1}{2}
$$

对比真实概率

$$
E(Y \geqslant 2) = \int_2^4 \frac{1}{4} = \frac{1}{2} \quad E(Y \geqslant 3) = \int_3^4 \frac{1}{4} = \frac{1}{4} \quad E(Y \geqslant 4) = \int_4^4 \frac{1}{4} = 0
$$

可见利用马尔可夫不等式得到的界是过于宽松的。

## 切比雪夫不等式

!!! tip "在前面的 马尔可夫不等式, 我们的考虑点主要是基于随机变量 $ X $ 的期望；而 切比雪夫不等式 主要考虑的点主在于方差。"

切比雪夫不等式的**基本思想**是：如果随机变量 $X$ 方差比较小，那给定其抽样样本 $\{x_1, x_2, \cdots, x_n\}$，其偏离期望的概率也应该很小。

!!! note "切比雪夫不等式"
    设随机变量 $X$，其总体均值为 $\mu$，方差为 $\sigma$，对给定常数 $a > 0$ 有

    $$
        P(\vert X - \mu \vert \geqslant a) \leqslant \frac{\sigma^2}{a^2}
    $$
    
    令 $a^2 = k \cdot \sigma$ 则 $P(\vert X - \mu \vert^2 \geqslant k \cdot \sigma) \leqslant \dfrac{1}{k}$。

??? quote "证明"
    设随机变量 $Y = \vert X - \mu \vert^2$ 带入 马尔可夫不等式 有

    $$
        P(\vert X - \mu \vert^2 \geqslant a^2) \leqslant \frac{E\left( X - \mu \right)^2}{a^2} = \frac{\sigma^2}{a^2} \implies P(\vert X - \mu \vert \geqslant a) \leqslant \frac{\sigma^2}{a^2}
    $$

切比雪夫不等式相比马尔可夫不等式进一步利用了方差的信息，得到的界相对更紧一些。

## 切尔诺夫界

!!! note "矩母函数"
    假设 $X$ 为一个随机变量，若存在 $h > 0$ 使得对任意的 $\lambda \in [0, h]$，$\mathbb{E}(e^{\lambda X})$ 均存在，则称 $X$ 存在矩母函数

    $$
        M_X(\lambda) = \mathbb{E}(e^{\lambda X})
    $$

    若一个随机变量的矩母函数不存在，则称其为**重尾的**，否则称其为**轻尾的**。

参考切比雪夫不等式的推导，我们将随机变量 $e^{\lambda (X - \mu)}$ 带入马尔可夫不等式，就可以得到切尔诺夫界：

$$
P((X - \mu) \geqslant \varepsilon) = P\left( e^{\lambda(X - \mu)} \geqslant e^{\lambda \varepsilon} \right) \leqslant \frac{\mathbb{E}\left( e^{\lambda (X - \mu)} \right)}{e^{\lambda \epsilon}}
$$

对于 $\forall \lambda \in [0, h]$ 成立。

!!! note "切尔诺夫界"
    对于任意的随机变量 $X$，假设其均值存在且为 $\mu$，并且矩母函数 $M_X(\lambda), \lambda \in [0, h]$ 存在，则 $X$ 的切尔诺夫界为

    $$
        P\left( (X - \mu) \geqslant \varepsilon \right) \leqslant \inf_{\lambda \in [0, h]} \frac{\mathbb{E}\left( e^{\lambda (X - \mu)} \right)}{e^{\lambda \epsilon}}
    $$

定义的左侧为 $X$ 的**右尾概率**，右侧的分子可以看作 $X$ 去中心化之后的矩母函数。

??? question "计算 $X \sim \mathcal{N}(\mu, \sigma^2)$ 的切尔诺夫界。"
    首先计算 $X$ 的矩母函数：

    $$
    \begin{aligned}
        M_X(\lambda) &= \mathbb{E}\left( \exp(\lambda X) \right) \\
        &= \int_{-\infty}^{\infty} \frac{\exp(\lambda X)}{\sqrt{2 \pi} \sigma} \exp\left( -\frac{(X - \mu)^2}{2\sigma^2} \right) \\
        &= \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(\lambda X -\frac{(X - \mu)^2}{2\sigma^2} \right) \\
        &= \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(\frac{2\sigma^2 \lambda X - X^2 + 2\mu X - \mu^2}{2\sigma^2} \right) \\
        &= \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(- \frac{- 2X(\sigma^2 \lambda + \mu) + X^2 + \mu^2}{2\sigma^2} \right) \\
        &= \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(- \frac{X^2 - 2X(\sigma^2 \lambda + \mu) + (\sigma^2 \lambda + \mu)^2 - (\sigma^2 \lambda + \mu)^2 + \mu^2}{2\sigma^2} \right) \\
        &= \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(- \frac{\left[X - (\sigma^2 \lambda + \mu)\right]^2 - (\sigma^2 \lambda)^2 - 2\mu(\sigma^2 \lambda)}{2\sigma^2} \right) \\
        &= \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(- \frac{\left[X - (\sigma^2 \lambda + \mu)\right]^2}{2\sigma^2} \right) \cdot \exp\left( \frac{(\sigma^2 \lambda)^2 + 2\mu(\sigma^2 \lambda)}{2\sigma^2} \right) \\
        &= \exp\left( \frac{(\sigma^2 \lambda)^2 + 2\mu(\sigma^2 \lambda)}{2\sigma^2} \right) \int_{-\infty}^{\infty} \frac{1}{\sqrt{2 \pi} \sigma} \exp\left(- \frac{\left[X - (\sigma^2 \lambda + \mu)\right]^2}{2\sigma^2} \right) \\
        &= \exp\left(\mu \lambda + \frac{\sigma^2 \lambda^2}{2} \right)
    \end{aligned}
    $$

    上式最后一步基于概率密度函数积分为 1 的性质。将其带入切尔诺夫界，有

    $$
    \begin{aligned}
        &\frac{\mathbb{E}(e^{\lambda (X - \mu)})}{e^{\lambda \varepsilon}} = \frac{\mathbb{E}(e^{\lambda X})}{e^{\lambda (\mu + \varepsilon)}}
        = \frac{e^{\left(\mu \lambda + \frac{\sigma^2 \lambda^2}{2} \right)}}{e^{\lambda (\mu + \varepsilon)}} = \exp \left( \frac{\sigma^2\lambda^2}{2} - \lambda \varepsilon \right) \\
        \implies & \inf_{\lambda \in [0, h]}\frac{\mathbb{E}(e^{\lambda (X - \mu)})}{e^{\lambda \varepsilon}} \iff \arg\min_{\lambda \in [0, h]} \frac{\sigma^2\lambda^2}{2} - \lambda \varepsilon \implies \lambda = \frac{\varepsilon}{\sigma^2}
    \end{aligned}
    $$

    将解出的 $\lambda$ 回带得到一维正态分布的切尔诺夫界为 $\exp\left( -\dfrac{\varepsilon^2}{2\sigma^2} \right)$。

## 次高斯性

上一节的我们推导出了高斯分布的切尔诺夫界，事实上有很多分布都会满足这个界。

!!! note "次高斯"
    假设 $X$ 是一个均值为 $\mu = \mathbb{E}(X)$ 的随机变量，若存在 $\sigma > 0$ 使得

    $$
        \mathbb{E}(e^{\lambda(X - \mu)}) \leqslant e^{\frac{\sigma^2\lambda^2}{2}}\, \quad \forall \lambda \in \mathcal{R}
    $$

    则称它为 $\sigma$-次高斯的，其中 $\sigma$ 称作次高斯参数。

!!! tip "次高斯随机变量的切尔诺夫界"
    若随机变量 $X$ 为 $\sigma$-次高斯的，则 $X$ 满足

    $$
    P\left( (X - \mu) \geqslant \varepsilon \right) \leqslant e^{-\frac{\varepsilon^2}{2 \sigma^2}}
    $$

    对 $\forall \varepsilon \geqslant 0$ 成立。

??? quote "证明"
    由于 $(X - \mu) \geqslant \varepsilon \iff \exp(\lambda(X - \mu)) \geqslant \exp(\lambda \varepsilon)$ 可以得到

    $$
    \begin{aligned}
        P((X - \mu) \geqslant \varepsilon) &= P(\exp(\lambda(X - \mu)) \geqslant \exp(\lambda \varepsilon)) \\
        &\leqslant \mathbb{E}(\exp(\lambda(X - \mu)))\exp(-\lambda \varepsilon) \\
        &\leqslant \exp\left(\frac{\sigma^2\lambda^2}{2} - \lambda \varepsilon\right)
    \end{aligned}
    $$

    这里得到的就跟上节最后的例子一样了，解优化问题得到切尔诺夫界为 $\exp\left( -\dfrac{\varepsilon^2}{2\sigma^2} \right)$。

次高斯随机变量在机器学习中常用到，一个典型的例子就是**所有的有界随机变量都是次高斯的**。

??? tip "次高斯随机变量的性质"
    假设 $X$ 是 $\sigma$-次高斯的随机变量，$X_1$ 和 $X_2$ 相互独立，分别为 $\sigma_1, \sigma_2$ 次高斯，则有：

    1. $Var(X) \leqslant \sigma^2$；
    2. $\forall c$ 有 $cX$ 是 $\vert c \vert \sigma$-次高斯的随机变量；
    3. $X_1 + X_2$ 是 $\sqrt{\sigma_1^2 + \sigma_2^2}$-次高斯的。

## 霍夫丁界

下面介绍的**霍夫丁引理**保证了有界随机变量的次高斯性。

!!! note "霍夫丁引理"
    设 $X$ 是一个均值为 $\mu$ 的随机变量，若 $a \leqslant X \leqslant b$ 几乎处处成立，则 $X$ 是次高斯的，其次高斯参数为 $\sigma = \frac{b - a}{2}$。

之前介绍的尾概率界都是**单个**随机变量的界，下面要引入的**霍夫丁界**描述了**有限个独立的随机变量求和**的尾概率界。

!!! note "霍夫丁界"
    若随机变量 $X_1, X_2, \cdots, X_n$ 相互独立，且 $X_i$ 的均值为 $\mu_i$，次高斯参数为 $\sigma_i$ 则对任意 $\varepsilon > 0$ 有

    $$
        P\left( \sum_{i = 1}^n(X_i - \mu_i) \geqslant \varepsilon \right) \leqslant \exp\left( -\frac{\varepsilon^2}{2 \sum_{i = 1}^n \sigma_i^2} \right)
    $$

??? quote "证明"
    根据次高斯随机变量的性质 $\sum\limits_{i = 1}^n X_i$ 为 $\sqrt{\sum\limits_{i = 1}^n \sigma_i^2}$-次高斯的随机变量。又由期望的线性性有

    $$
        \mathbb{E}\left( \sum_{i = 1}^n X_i \right) = \sum_{i = 1}^n \mathbb{E}(X_i) = \sum_{i = 1}^n \mu_i
    $$

    根据次高斯随机变量的切尔诺夫界，有

    $$
        P\left( \sum_{i = 1}^n (X_i - \mu_i) \geqslant \varepsilon \right) \leqslant \exp\left( -\frac{\varepsilon^2}{2 \sum_{i = 1}^n \sigma_i^2} \right)
    $$

结合霍夫丁引理，我们可以得到有界随机变量的霍夫丁界。

!!! note "推论"
    若随机变量 $X_1, X_2, \cdots, X_n$ 相互独立，且 $X_i \in [a, b], \forall i \in 1, 2, \cdots n$，则

    $$
        P\left( \sum_{i = 1}^n (X_i - \mu_i) \geqslant \varepsilon \right) \leqslant \exp\left( -\frac{\varepsilon^2}{n(b - a)^2} \right)
    $$
