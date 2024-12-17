# 梯度下降法

梯度下降法属于最基础的梯度类算法，本质是利用函数的一阶导数信息选取下降方向 $d^k$。对于光滑函数 $f(x)$，在迭代点 $x^k$ 处，我们选择 $d^k = -\nabla f(x^k)$ 为下降方向，迭代格式为

$$
x^{k + 1} = x^k - \alpha_k \nabla f(x^k)
$$

步长 $\alpha^k$ 的选择可以依赖于[线搜索算法](./LinearSearch.md)，也可以是一个固定的常数。

## 收敛性

需要引入利普希茨连续的定义，如下

!!! note "利普希茨连续"
    若给定函数 $f$ 是可微函数，并且对任意定义域的点 $x, y$，梯度满足利普希茨连续（Lipschitz continuous），即存在 $L > 0$ 使得

    $$
    \Vert \nabla f(y) - \nabla f(x) \Vert \geqslant L \Vert y - x \Vert
    $$

    则称 $f$ 是梯度李氏连续，或者李氏光滑（$L$-光滑）的。

梯度利普希茨连续表明 $\nabla f(x)$ 的变化可以被自变量 $x$ 的变化所控制，满足该性质的函数具有很多好的性质，一个重要的性质是其具有**二次上界**。

!!! note "二次上界"
    设可微函数 $f(x)$ 的定义域 $\mathbf{dom}f = \mathbb{R}^n$，且为梯度 $L$-利普希茨连续的，则函数 $f(x)$ 有二次上界

    $$
    f(y) \leqslant f(x) + \nabla f(x)^\top (y - x) + \frac{L}{2} \Vert y - x \Vert^2, \quad \forall x, y \in \mathbf{dom}f
    $$

    **证明**. 对任意 $x, y \in \mathbf{R}^n$，构造辅助函数

    $$
    g(t) = f(x + t(y - x)), t \in [0, 1]
    $$
    
    显然 $g(0) = f(x)$，$g(1) = f(y)$ 以及

    $$
    g'(t) = \nabla f(x + t(y - x))^\top (y - x)
    $$

    由等式

    $$
    g(1) - g(0) = \int_0^1 g'(t)\,\mathrm{d}t
    $$

    可知

    $$
    \begin{align*}
    &f(y) - f(x) - \nabla f(x)^\top (y - x) \\
    =& \int_0^1 (g'(t) - g'(0))\,\mathrm{d}t \\
    =& \int_0^1 (\nabla f(x + t(y - x)) - \nabla f(x))^\top (y - x) \mathrm{d}t \\
    \leqslant & \int_0^1 \Vert \nabla f(x + t(y - x)) - \nabla f(x) \Vert \Vert y - x \Vert \mathrm{d}t \\
    \leqslant & \int_0^1 L \Vert y - x \Vert^2 t \mathrm{d}t = \frac{L}{2} \Vert y - x\Vert^2
    \end{align*}
    $$

    其中最后一行的不等式利用了梯度利普希茨连续的条件，整理后即为二次上界定理中的式子。定理对于 $f(x)$ 定义域的要求可以减弱为 $\mathbf{dom}f$ 是凸集，以满足证明中 $g(t)$ 在 $t\in [0, 1]$ 时有定义。

上述定理说明 $f(x)$ 可被一个二次函数上界所控制，即要求 $f(x)$ 的增长速度不超过二次。


### 非凸函数上的收敛性

梯度法的另一个理解是按照下式迭代

$$
x_{k + 1} = \arg\min_y f(x_k) + \nabla f(x_k)^\top (y - x_k) + \frac{L}{2}\Vert y - x_k \Vert^2
$$

注意到，右边的式子就是利普希茨连续函数的二次上界，是一个关于 $y$ 的二次函数，令导数为零可以求得闭式解即

$$
x_{k + 1} = x_k -  \frac{1}{L} \nabla f(x)
$$

符合梯度下降的迭代格式，其中步长为 $\dfrac{1}{L}$，且 $L$ 为利普希茨常数。下面可以给出一个非凸函数上梯度下降法的收敛性质

!!! note "梯度下降法非凸函数上的收敛性"
    若 $f$ 是 $L$-利普希茨光滑函数并且 $f$ 有最小值 $f^*$，则选取步长 $\alpha_k = \frac{1}{L}$，我们有 $\lim\limits_{k \to \infty} \Vert \nabla f(x_k) \Vert = 0$。

    **证明**. 由李氏光滑性，当选取 $\alpha = \frac{1}{L}$ 为步长时，$f(x_k)$ 的一个二次上界为

    $$
    q_{x_k}(y) = f(x_k) + \nabla f(x_k)^\top (y - x_k) + \frac{1}{2\alpha} \Vert y - x_k \Vert^2
    $$

    所以有

    $$
    f(x_{k + 1}) \leqslant q_{x_k}(x_{k + 1}) = q_{x_{k}}(x_k) - \frac{1}{2L} \Vert \nabla f(x_k) \Vert^2 = f(x_k) - \frac{1}{2L}\Vert \nabla f(x_k) \Vert^2
    $$

    因此有

    $$
    \lim_{n \to \infty}\sum_{k = 1}^{n} \frac{1}{2L} \Vert \nabla f(x_k) \Vert^2 = \lim_{k \to \infty} f(x_0) - f(x_k) = f - f^* < \infty \implies \lim\limits_{k \to \infty} \Vert \nabla f(x_k) \Vert = 0
    $$

这就证明了梯度下降法在一般函数上的收敛性。

### 凸函数上的收敛性

!!! note "梯度法在凸函数上的收敛性"
    设函数 $f(x)$ 为凸的梯度 $L$-利普希茨连续函数，$f^* = f(x^*) = \inf_x f(x)$ 存在且可达。如果步长 $\alpha_k$ 取为常数 $\alpha$ 且满足 $0 < \alpha \leqslant \dfrac{1}{L}$，那么由迭代

    $$
    x_{k + 1} = x_k - \alpha \nabla f(x)
    $$

    得到的点列 $\{x_k\}$ 的函数值收敛到最优值，且在函数值的意义下收敛速度为 $\mathcal{O}\left(\dfrac{1}{k}\right)$。

    **证明**. 因为函数 $f$ 是利普希茨可微函数，对任意的 $x$，根据二次上界

    $$
    f(x - \alpha \nabla f(x)) \leqslant f(x) - \alpha \left(1 - \frac{L \alpha}{2}\right) \Vert \nabla f(x) \Vert^2
    $$

    记 $\widetilde{x} = x - \alpha \nabla f(x)$ 并限制 $0 < \alpha \leqslant \dfrac{1}{L}$，我们有

    $$
    \begin{align*}
    f(\widetilde{x}) &\leqslant f(x) - \dfrac{\alpha}{2} \Vert \nabla f(x) \Vert^2 \\
    & \leqslant f^* + \nabla f(x)^\top (x - x^*) - \dfrac{\alpha}{2} \Vert \nabla f(x) \Vert^2 \\
    &= f^* + \dfrac{1}{2\alpha} \left( \Vert x - x^* \Vert^2 - \Vert x - x^* - \alpha \nabla f(x) \Vert^2 \right) \\
    &= f^* + \dfrac{1}{2\alpha} \left( \Vert x - x^* \Vert^2 - \Vert \widetilde{x} - x^* \Vert^2 \right)
    \end{align*}
    $$

    其中第二个不等式为 $f$ 的凸性。在上式中取 $x = x^{i-1}, \widetilde{x} = x^i$ 并将不等式对 $i = 1, 2, \dots, k$ 求和得到

    $$
    \begin{align*}
    \sum_{i = 1}^{k} \left( f(x^i) - f^* \right) &\leqslant \dfrac{1}{2\alpha} \sum_{i = 1}^k \left( \Vert x^{i - 1} - x^* \Vert^2 - \Vert x^i - x^* \Vert^2 \right) \\
    &= \dfrac{1}{2\alpha}\left( \Vert x^0 - x^* \Vert^2 - \Vert x^k - x^* \Vert^2 \right) \\
    &\leqslant \dfrac{1}{2\alpha} \Vert x^0 - x^* \Vert^2
    \end{align*}
    $$

    根据二次上界得知 $f(x^i)$ 是非增的，所以

    $$
    f(x^k) - f^* \leqslant \dfrac{1}{k} \sum_{i = 1}^k \left( f(x^i) - f^* \right) \leqslant \dfrac{1}{2k\alpha} \Vert x^0 - x^* \Vert^2
    $$

如果函数 $f$ 还是 $m$-强凸函数，则梯度法的收敛速度会进一步提升为 $\mathcal{Q}$-线性收敛。

### 强凸函数上的收敛性

再给出收敛性定理之前，我们需要下面的定理来揭示凸的梯度 $L$-利普希茨连续函数的另一个重要的性质。

!!! note "引理"
    设函数 $f(x)$ 是 $\mathbb{R}^n$ 上的凸可微函数，则一下结论等价：

    - $f$ 的梯度为 $L$-利普希茨连续的；
    - 函数 $g(x) := \dfrac{L}{2} x^\top x - f(x)$ 是凸函数；
    - $\nabla f(x)$ 有**余强制性**，即对任意的 $x, y \in \mathbb{R}^n$，有

    $$
    \left( \nabla f(x) - \nabla f(x) \right)^\top (x - y) \geqslant \dfrac{1}{L} \Vert \nabla f(x) - \nabla f(y) \Vert^2
    $$

    证明略去。
    
上述引理的第三条余强制性其实是对单调性的一种加强，若函数 $f(x)$ 为单调函数，则其函数值的变化与自变量的变化同号，即

$$
\left( f(x) - f(x) \right)^\top (x - y) \geqslant 0
$$

但是余强制性把不等式右侧的 $0$ 换成了一个二次的非负项，是一种加强。引理表明若函数 $f(x)$ 是凸函数，则函数梯度 $L$-利普希茨连续、二次上界（引理的证明需要）和余强制性等价，只要知道其中一个就可以推出另外两个。

接下来给出梯度法在强凸函数下的收敛性

!!! note "梯度法在强凸函数上的收敛性"
    设函数 $f(x)$ 为 $m$-强凸函数的梯度 $L$-利普希茨连续函数，$f^* = f(x^*) = \inf_x f(x)$ 存在且可达。如果步长 $\alpha_k$ 取为常数 $\alpha$ 且满足 $0 < \alpha \leqslant \dfrac{2}{m + L}$，那么由迭代

    $$
    x_{k + 1} = x_k - \alpha \nabla f(x)
    $$

    得到的点列 $\{x_k\}$ 的函数值收敛到最优值，且为 $\mathcal{Q}$ 线性收敛。

    **证明**. 首先根据 $f$ 强凸且 $\nabla f$ 利普希茨连续，可得

    $$
    g(x) = f(x) - \dfrac{m}{2}x^\top x
    $$

    为凸函数且 $\dfrac{L - m}{2} x^\top x - g(x)$ 为凸函数。由引理知 $g(x)$ 是梯度 $(L - m)$-利普希茨连续的。再次利用引理可得关于 $g(x)$ 的余强制性

    $$
    \left( \nabla g(x) - \nabla g(x) \right)^\top (x - y) \geqslant \dfrac{1}{L - m} \Vert \nabla g(x) - \nabla g(y) \Vert^2 
    $$
    
    带入 $g(x)$ 的表达式，可得

    $$
    \left( \nabla g(x) - \nabla g(x) \right)^\top (x - y) \geqslant \dfrac{mL}{m + L} \Vert x - y \Vert^2 + \dfrac{1}{m + L} \Vert \nabla f(x) - \nabla f(y) \Vert^2
    $$

    然后我们估计在固定步长下梯度法的收敛速度。设步长 $\alpha \in \left( 0, \dfrac{2}{m + L} \right)$，则

    $$
    \begin{align*}
    \Vert x^{k + 1} - x^* \Vert^2_2 &= \Vert x^k - \alpha \nabla f(x^k) - x^* \Vert^2 \\
    &= \Vert x^k - x^* \Vert^2 - 2\alpha \nabla f(x^k)^\top (x^k - x^*) + \alpha^2 \Vert \nabla f(x^k) \Vert^2 \\
    &\leqslant \left( 1 - \alpha\dfrac{2mL}{m + L} \right) \Vert x^k - x^* \Vert^2 + \alpha \left( \alpha - \dfrac{2}{m + L} \right) \Vert \nabla f(x^k) \Vert^2 \\
    &\leqslant \left( 1 - \alpha \dfrac{2mL}{m + L} \right) \Vert x^k - x^* \Vert^2
    \end{align*}
    $$

    注意到 $\nabla f(x^*) = 0$ 因此

    $$
    \Vert x^k - x^* \Vert^2 \leqslant c^k \Vert x^0 - x^* \Vert^2, c = 1 - \alpha \dfrac{2mL}{m + L} < 1
    $$

    即在强凸函数的条件下，梯度法是 $\mathcal{Q}$ 线性收敛的。