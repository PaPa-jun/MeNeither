# 交替方向乘子法

本文考虑如下凸问题

$$
\begin{aligned}
\min_{x_1, x_2}& & &f_1(x_1) + f_2(x_2) \\
\text{s.t.}& & &A_1 x_1 + A_2 x_2 = b
\end{aligned}
$$

其中 $f_1, f_2$ 是适当的闭凸函数，但**不要求**是光滑的，$x_1 \in \mathbb{R}^n, x_2 \in \mathbb{R}^m, A_1 \in \mathbb{p \times n}, A_2 \in \mathbb{R}^{p \times m}, b \in \mathbb{R}^p$。这个问题的特点是**目标函数可以分成彼此分离的两块**，但是变量被线性约束结合在一起。

## 问题形式转化

这里给出一些例子说明如何把某些一般的问题转化为适用交替方向乘子法求解的标准形式。

!!! question "可分成两块的无约束优化问题"
    考虑无约束优化问题

    $$
    \min_x f_1(x) + f_2(x)
    $$

??? quote "转化为标准形式"
    为了将此问题转化为标准形式，需要将目标函数改成可分的形式。考虑引入一个新的变量 $x$ 并令 $x = z$，问题转化为

    $$
    \begin{aligned}
    \min_{x, z}& & &f_1(x) + f_2(z) \\
    \text{s.t.}& & &x - z = 0
    \end{aligned}
    $$

!!! question "带线性变换的无约束优化问题"
    考虑无约束优化问题

    $$
    \min_x f_1(x) + f_2(Ax)
    $$

??? quote "转化为标准形式"
    类似地，我们引入一个新的变量 $z$，令 $z = Ax$，则问题变为

    $$
    \begin{aligned}
    \min_{x, z}& & &f_1(x) + f_2(z) \\
    \text{s.t.}& & &Ax - z = 0
    \end{aligned}
    $$

!!! question "凸集上的约束优化问题"
    考虑约束优化问题

    $$
    \begin{aligned}
    \min_x& & & f(x) \\
    \text{s.t.}& & & Ax \in C
    \end{aligned}
    $$

    其中 $C \in \mathbb{R}^n$ 为凸集。
    
??? quote "转化为标准形式"
    对于集合约束 $Ax \in C$，我们可以用**示性函数** $I_C(\cdot)$ 将其添加到目标函数中，那么问题可以转化为

    $$
    \min_x f(x) + I_C(Ax)
    $$

    其中 $I_C(z)$ 是集合 $C$ 的示性函数，即

    $$
    \begin{aligned}
    I_C(z) = 
    \begin{cases}
    0, \quad z \in C \\
    + \infty, \quad \text{others.}
    \end{cases}
    \end{aligned}
    $$

    问题转化为**带线性变换的无约束优化问题**。

!!! question "全局一致性问题"
    考虑如下问题

    $$
    \min_x \sum_{i = 1}^N \phi_i(x)
    $$

??? quote "转化为标准形式"
    令 $x = z$，并将 $x$ 复制 $N$ 份，分别为 $x_i$，那么问题转化为

    $$
    \begin{aligned}
    \min_{x_i, z}& & & \sum_{i = 1}^N \phi_i(x_i) \\
    \text{s.t.}& & & x_i - z = 0, \quad i = 1, 2, \cdots, N
    \end{aligned}
    $$

    形式上，令 $x = (x_1^\top, x_2^\top, \cdots, x_N^\top)^\top$ 以及

    $$
    f_1(x) = \sum_{i = 1}^N \phi_i(x_i), \quad f_2(z) = 0
    $$

    则此问题可以化为

    $$
    \begin{aligned}
    \min_{x, z}& & & f_1(x) + f_2(z) \\
    \text{s.t.}& & & A_1x - A_2z = 0
    \end{aligned}
    $$

    其中矩阵 $A_1, A_2$ 定义为

    $$
    \begin{aligned}
    A_1 = 
    \left(\begin{array}{cccc}
    I \\
      & I \\
      &   & \ddots \\
      &   &   & I
    \end{array}\right), \quad
    A_2 = 
    \left(\begin{array}{c}
    I \\
    I \\
    \vdots \\
    I
    \end{array}\right)
    \end{aligned}
    $$

!!! question "共享问题"
    考虑如下问题

    $$
    \min_{x_i} \sum_{i = 1}^N f_i(x_i) + g\left( \sum_{i = 1}^N x_i \right)
    $$

??? quote "转化为标准形式"
    为了使目标函数可分，我们将 $g$ 的变量 $x_i$ 分别复制一份为 $z_i$，那么问题转化为

    $$
    \begin{aligned}
    \min_{x_i, z_i}& & & \sum_{i = 1}^N f_i(x_i) + g\left( \sum_{i = 1}^N z_i \right) \\
    \text{s.t.}& & & x_i - z_i = 0, \quad \forall i = 1, 2, \cdots, N
    \end{aligned}
    $$

    该问题也符合标准形式。

## 算法思想

下面给出交替方向乘子法的迭代格式，首先写出标准形式的增广拉格朗日函数

$$
\begin{aligned}
L_\rho(x_1, x_2, y) = &f_1(x_1) + f_2(x_2) + y^\top (A_1 x_1 + A_2 x_2 - b) + \\
&\frac{\rho}{2}\Vert A_1 x_1 + A_2 x_2 - b \Vert_2^2
\end{aligned}
$$

其中 $\rho > 0$ 为二次罚项的系数。回顾[增广拉格朗日函数法](./AugementedLagrangianMethod.md)，求解上述问题的迭代格式为

$$
\begin{aligned}
\begin{cases}
(x_1^{k + 1}, x_2^{k + 1}) = \mathop{\arg\min}\limits_{x_1, x_2} L_\rho (x_1, x_2, y^k) \\
y^{k + 1} = y^k + \tau \rho(A_1x_1^{k + 1} + A_2 x_2^{k + 1} - b)
\end{cases}
\end{aligned}
$$

其中 $\tau$ 为步长。在实际求解中，第一步迭代同时对 $x_1$ 和 $x_2$ 进行优化有时比较困难，而固定一个变量求解关于另一个变量的极小问题可能比较简单，因此我们可以考虑对 $x_1$ 和 $x_2$ **交替求极小**，这就是交替方向乘子法的基本思路。

我们可以总结出算法的迭代格式为

$$
\begin{aligned}
x_1^{k + 1} &= \mathop{\arg\min}\limits_{x_1} L_\rho(x_1, x_2^k, y^k) \\
x_2^{k + 1} &= \mathop{\arg\min}\limits_{x_2} L_\rho(x_1^{k + 1}, x_2, y^k) \\
y^{k + 1} &= y^k + \tau \rho(A_1x_1^{k + 1} + A_2 x_2^{k + 1} - b)
\end{aligned}
$$

其中 $\tau$ 为步长，通常取之于 $\left(0, \dfrac{1 + \sqrt{5}}{2}\right)$。

## 收敛性分析


