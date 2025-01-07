# 随机梯度下降法

我们将主要考虑随机优化问题

$$
\min_{x \in \mathbb{R}} f(x) = \frac{1}{N} \sum_{i = 1}^N f_i (x)
$$

其中 $f_i(x)$ 对应第 $i$ 个样本的损失函数。假设每个样本的损失函数是凸的、可微的，因此可以运用梯度下降法

$$
x^{k + 1} = x^k - \alpha_k \nabla f(x^k)
$$

来求解原始的优化问题。其中

$$
\nabla f(x^k) = \frac{1}{N} \sum_{i = 1}^N \nabla f_i(x^k)
$$

在绝大多数情况下，我们不能通过化简的方式得到 $\nabla f(x^K)$ 的表达式，要计算这个梯度必须计算出所有的 $\nabla f_i (x^k), i = 1, 2, \cdots, N$ 然后将它们相加。然而在机器学习中，采集到的样本量是巨大的，因此计算 $\nabla f(x^k)$ 需要**非常大的计算量**。

## 算法思想

为了减少传统梯度下降法的计算量，我们考虑迭代格式

$$
x^{k + 1} = x^k - \alpha_k \nabla f_{s_k}(x^k)
$$

其中 $s_k$ 是从 $\{ 1,2,\cdots, N \}$ 中**随机等可能**地抽取的一个样本，$\alpha_k$ 称为步长。

通过对比传统算法可知，随机梯度算法**不去计算全梯度** $\nabla f(x^k)$，而是从众多样本中随机抽出一个样本 $s_i$，然后紧紧计算这个样本处的梯度 $\nabla f_{s_k}(x^k)$，以此作为 $\nabla f(x^k)$ 的近似。

实际计算中每次只抽取一个样本 $s_k$ 的做法比较极端，常采用的形式是**小批量**随机梯度法，即随机选择一个元素个数很少的**集合** $\mathcal{I}_k \subset \{1, 2, \cdots, N\}$，然后执行迭代格式

$$
x^{k + 1} = x^k - \frac{\alpha_k}{\vert \mathcal{I}_k \vert} \sum_{s \in \mathcal{I}_k} \nabla f_s (x^k)
$$

其中 $\vert \mathcal{I}_k \vert$ 表示 $\mathcal{I}_k$ 中元素的个数。

??? tip "随机次梯度法"
    当 $f_i(x)$ 是凸函数但是不一定可微时，可以用 $f_i(x)$ 的次梯度代替梯度进行迭代。这就是随机次梯度法，有迭代格式

    $$
    x^{k + 1} = x^k - \alpha_k g^k
    $$

    其中 $\alpha_k$ 为步长，$g^k \in \partial f_{s_k}(x^k)$ 为随机次梯度，其**期望**为真实梯度。

## 动量方法

传统梯度法在问题比较病态时收敛速度非常慢，随机梯度下降法也有类似的问题。为了克服这一缺陷，人们提出了**动量方法**（momentum），旨在加速学习。

$$
\begin{aligned}
v^{k + 1} &= \mu_k v^k + \alpha_k \nabla f_{x_k}(x^k) \\
x^{k + 1} &= x^k + v^{k + 1}
\end{aligned}
$$

从形式上看，动量方法引入了一个速度变量 $v$，它代表参数移动的方向和大小。在计算完当前点的随机梯度后，我们并**不完全相信**这个方向，而是将其和上一步更新方向做**线性组合**来得到新的更新方向。

## Nesterov 加速算法

针对光滑问题的 Nesterov 加速算法迭代的随机版本为

$$
\begin{aligned}
y^{k + 1} &= x^k + \mu_k (x^k - x^{k - 1}) \\
x^{k + 1} &= y^{k + 1} - \alpha_k \nabla f_{s_k} (y^{k + 1})
\end{aligned}
$$

其中 $\mu_k = \dfrac{k - 1}{k + 2}$，步长 $\alpha_k$ 是一个固定值或者由线搜索确定。若设第 $k$ 步迭代时有速度变量

$$
v^{k} = x^k - x^{k - 1}
$$

结合原始 Nesterov 加速算法的两步迭代可以得到

$$
x^{k + 1} = x^k + \mu_k (x^k - x^{k - 1}) - \alpha_k \nabla f_{s_k}(x^k + \mu_k(x^k - x^{k - 1}))
$$

如果定义有关 $v^{k + 1}$ 的迭代式

$$
v^{k + 1} = \mu_k v^k - \alpha_k \nabla f_{s_k}(x^k + \mu_k v^k)
$$

则得到关于 $x^k$ 和 $v^k$ 的等价迭代

$$
\begin{aligned}
v^{k + 1} &= \mu_k v^k - \alpha_k \nabla f_{s_k}(x^k + \mu_k v^k) \\
x^{k + 1} &= x^k + v^{k + 1}
\end{aligned}
$$

与动量方法相比，二者的主要区别在于梯度的计算上。Nesterov 加速算法**先对点施加速度的作用**，再求梯度，这可以理解为对标准动量方法做了一个矫正。

## AdaGrad

在一般的随机梯度法中，**调参**是一个很大的难点，参数设置的好坏对算法的性能有显著的影响，
