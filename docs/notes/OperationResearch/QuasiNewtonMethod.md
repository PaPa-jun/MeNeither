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


