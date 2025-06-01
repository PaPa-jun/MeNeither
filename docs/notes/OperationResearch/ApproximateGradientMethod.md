---
katex: true
pseudocode: true
---

# 近似点梯度法

在机器学习、图像处理领域中，许多模型包含**两部分**：一部分是误差项，一般为光滑函数；另一部分是正则项，可能为非光滑函数，用来保证求解问题的特殊结构（例如 $\ell_1$ 范数正则化）。

$$
\min_{x \in \mathbb{R}^n} \psi (x) = f(x) + h(x)
$$

由于有非光滑的部分存在，我们可以考虑使用[次梯度法](./SubgradientMethod.md)求解，但是次梯度法**不能充分利用光滑部分的信息**，这时我们考虑**近似点梯度法**，也叫邻近点梯度法。

## 邻近算子

邻近算子事处理**非光滑**问题的一个非常有效的工具，当然该算子并不仅仅局限于非光滑函数。

!!! note "邻近算子"
    对于一个凸函数 $h$，定义它的**邻近算子**为

    $$
    \text{prox}_h(x) = \mathop{\arg\min}\limits_{u \in \mathbf{dom }h} \left\{ h(u) + \frac{1}{2} \Vert u - x \Vert^2 \right\}
    $$

可以看出，邻算子的目的是求解一个距 $x$ 不算太远的点，并使函数值 $h(u)$ 也相对较小。

??? tip "邻近算子是良定义的"
    如果 $h$ 是适当的闭凸函数，则对任意的 $x$，$\text{prox}_h(x)$ 的值存在且唯一。

根据最优性条件可以得出邻近算子与次梯度的关系。

!!! note "邻近算子与次梯度的关系"
    如果 $h$ 是适当的闭凸函数，则

    $$
    u = \text{prox}_h(x) \iff x - u \in \partial h(u)
    $$

??? quote "证明"
    若 $u = \text{prox}_h(x)$，则由最优性条件得 $0 \in \partial h(u) + (u - x)$，因此有 $x - u \in \partial h(u)$。

    反之，若 $x - u \in \partial h(u)$，则由次梯度的定义可得到

    $$
    h(v) \geqslant h(u) + (x - u)^\top (v - u), \quad \forall v \in \mathbf{dom }h
    $$

    两边同时加 $\dfrac{1}{2}\Vert v - x \Vert^2$，有

    $$
    \begin{aligned}
    h(v) + \frac{1}{2} \Vert v - x \Vert^2 &\geqslant h(u) + (x - u)^\top(v - u) + \frac{1}{2}\Vert v - x \Vert^2 \\
    & \geqslant h(u) + \frac{1}{2} \Vert u - x \Vert^2, \quad \forall v \in \mathbf{dom }h
    \end{aligned}
    $$

    因此我们得到 $u = \text{prox}_h(x)$。

用 $th$ 代替 $h$，上面的结论可以形式上可以写成

$$
u = \text{prox}_{th}(x) \iff u \in x - t\partial h(x)
$$

即**邻近算子的计算实际上可以看成是次梯度法的隐式格式**（向后迭代），也是近似点梯度法的迭代格式。

!!! example
    考虑 $\ell_1$ 范数，邻近算子 $u = prox_{th}(x)$ 的最优性条件为

    $$
    \begin{aligned}
    x - u \in t\partial h(u) = 
    \begin{cases}
    \{t\}, & u > 0 \\
    [-t, t], & u = 0 \\
    \{-t\}, & u < 0
    \end{cases}
    \implies
    u = 
    \begin{cases}
    x - t, & x > t \\
    x + t, & x < -t \\
    \text{sign}(x)\max\{\vert x \vert - t, 0\}, & x \in [-t, t]
    \end{cases}
    \end{aligned}
    $$

## 近似点梯度法

考虑复合优化问题

$$
\min \psi(x) = f(x) + h(x)
$$

其中函数 $f$ 为可微函数，其定义域 $\mathbf{dom }f = \mathbb{R}^n$，函数 $h$ 为**凸函数**，可以是非光滑的，并且一般计算此项的邻近算子并不复杂。

近似点梯度法的思想非常简单：注意到 $\psi(x)$ 有两部分，对于光滑部分 $f$ 做[梯度下降](./GradientDescent.md)，对于非光滑部分 $h$ 使用邻近算子，则近似点梯度法的迭代公式为

$$
x^{k + 1} = \text{prox}_{t_kh}(x^k - t_k \nabla f(x^k))
$$

其中 $t_k > 0$ 为每次迭代的步长，可以是一个常数或者由线搜索得出。

<div class="pseudocode">
    \begin{algorithm}
    \caption{近似点梯度法}
    \begin{algorithmic}
    \STATE \textbf{输入：}函数 $f(x), h(x)$，初始点 $x^0$；
    \WHILE{未达到收敛准则}
        \STATE $x^{k + 1} = \text{prox}_{t_kh}(x^k - t_k \nabla f(x^k))$；
    \ENDWHILE
    \end{algorithmic}
    \end{algorithm}
</div>

## 深入理解

根据邻近算子的定义，把迭代公式展开

$$
\begin{aligned}
x^{k + 1} &= \mathop{\arg\min}\limits_{u} \left\{ h(u) + \frac{1}{2t_k} \Vert u - x^k + t_k \nabla f(x^k) \Vert^2 \right\} \\
&= \mathop{\arg\min}\limits_{u} \left\{ h(u) + f(x^k) + \nabla f(x^k)^\top(u - x^k) + \frac{1}{2t_k}\Vert u - x^k \Vert^2 \right\}
\end{aligned}
$$

可以发现，近似点梯度法实质上就是将问题的光滑部分**线性展开**再加上二次项并保留非光滑部分，然后求极小来作为每一步的估计。

此外，根据邻近算子和次梯度的关系，邻近点梯度法的迭代格式可以写成

$$
x^{k + 1} = x^k - t_k \nabla f(x^k) - t_k g^k, \quad g^k \in \partial h(x^{k + 1})
$$

即对光滑部分做**显式**的梯度下降，关于非光滑部分做**隐式**的梯度下降。

## 收敛性分析


