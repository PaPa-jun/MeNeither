---
katex: true
---

# 算法基础

??? abstract "重点"
    算法设计与分析的框架，介绍基础的排序算法和算法复杂度分析。

    - 插入排序算法
    - 算法复杂性及其度量
        - 时间复杂性和空间复杂性
        - 最坏、最好和平均情形复杂性
    - 插入排序的最坏、最好和平均时间
    - 归并排序算法及其时间复杂性

## 插入排序

插入排序是一个对于少量元素非常有效的算法。伪代码如下：

```python title="插入排序" linenums="1" hl_lines="5-8"
def InsertionSort(A):
    for j in range(1, len(A)):
        key = A[j]
        i = j - 1
        while i >= 0 and A[i] > key:
            A[i + 1] = A[i]
            i = i - 1
        A[i + 1] = key
```

算法从第 5 行开始执行插入，通过令大于 `A[j]` 的所有值后移实现将 `A[j]` **插入**到合适的位置。

### 算法的正确性

下面要说明插入排序算法的正确性，这通常涉及**循环不变式**的构造和证明。

!!! note "循环不变式"
    循环不变式通常用来说明算法的正确性，通过找到一个条件满足：

    - **初始化**：在循环的第一次迭代前，它为真；
    - **保持**：如果循环的某次迭代前它为真，那么在下次迭代之前它仍然为真；
    - **终止**：在循环终止时，不变式为我们提供了一个有用的性质，该性质有助于证明算法是正确的。

对于排序算法，可以发现一个显然的循环不变式，即**关键字** `A[j]` **之前的位置都是有序的**，我们可以实际考察：

- **初始化**：在循环的第一次迭代前，`A[j]` 前只有一个元素，显然有序；
- **保持**：如果循环的某次迭代前 `A[j]` 前的元素有序，则在本次迭代后，`A[j]` 被插入到它之前的合适的位置，显然也满足不变式；
- **终止**：在循环终止时，很容易发现整个数组都是有序的。

了解数学归纳法的读者可能发现，所谓的循环不变式在形式上与数学归纳法如出一辙。

## 分析算法

在算法设计中，分析算法的时间空间复杂度是一个非常重要的工作，帮助我们从算法本身的角度估计算法的大致运行时间。这里说从“算法本身估计”是指抛开不同的硬件平台对算法运行效果的影响，仅根据算法的语句执行次数来估计算法的运行时间。

在这种情况下，我们假定算法运行一条语句的时间为一个固定的常数，如此以来，算法处理的输入规模就成了除了设计以外，唯一形象算法运行时间的变量了。我们要研究的便是算法的运行时间与数据量之间的关系。

继续考虑插入排序算法，假定输入的数组 `A` 中包含 $n$ 个元素，则算法每一句的运行次数如下：

```python title="插入排序：语句运行次数统计" linenums="1"
def InsertionSort(A):
    for j in range(1, len(A)):          # n
        key = A[j]                      # n - 1
        i = j - 1                       # n - 1
        while i >= 0 and A[i] > key:    # sum(t_j), j = 1 to n - 1
            A[i + 1] = A[i]             # sum(t_j - 1), j = 1 to n - 1
            i = i - 1                   # sum(t_j - 1), j = 1 to n - 1
        A[i + 1] = key                  # n - 1
```

分析中 $t_j$ 表示对某次的 $j$ 执行的 `while` 循环的次数。那么该算法的时间复杂度即为将所有语句的运行次数乘上单次运行的时间代价并相加：

$$
T(n) = c_1n + c_2(n - 1) + c_3(n - 1) + c_4\sum_{j = 1}^{n - 1}t_j + c_5\sum_{j = 1}^{n - 1}(t_j - 1) + c_6\sum_{j = 1}^{n - 1}(t_j - 1) + c_7(n - 1)
$$

考虑 $t_j$ 的情况：如果输入的数组已经有序，则有 $t_j = 1, \forall j$ 成立，此时：

$$
T(n) = (c_1 + c_2 + c_3 + c_4 + c_7)n - (c_2 + c_3 + c_4 + c_7)
$$

给出了插入排序的最优时间。因为上式为一个线性函数，因此说插入排序的最优运行时间是数据规模的**线性函数**；如果输入的数据反向有序，则每次运行的 `t_j = j` 即有

$$
\sum_{j = 1}^{n - 1}t_j = \frac{(n - 1)n}{2} \qquad \sum_{j = 1}^{n - 1}(t_j - 1) = \frac{(n - 1)(n - 2)}{2}
$$

从而可以得出插入排序的最坏时间为：

$$
T(n) = an^2 + bn + c
$$

上式省略的具体的计算，只保留了主要的体现阶数的项。总之，最坏情况下，算法的运行时间是数据规模的**二次函数**。

上面的分析分别从最坏和最好的角度分析了排序算法，很多时候，我们也会关注算法的平均表现，此时需要从概率的角度分析数据的分布，由此判断语句的平均运行次数从而给出算法的平均时间复杂度。而最优和最差时间复杂度分别给出了算法开销的下界和上界，是非常有意义的。若考虑插入排序的平均情况，会发现平均情况下 $t_j = n / 2$，得出插入排序的平均情况也是一个**二次函数**。

## 设计算法

我们可以选择的算法设计技术有很多，插入排序选择了**增量**的方法：通过插入 `A[j]` 产生排序好的子数组。下面我们讨论一种称为**分治法**的设计策略。

### 分治法

分治法采用**递归**的程序设计，在这种设计中，算法会频繁地跳用自身去处理问题。分支法就是这样的算法，它首先将原问题分解为结构相同但规模更小的若干个子问题，然后在解决这些子问题，最后将解决好的子问题合并得到完整问题的解。

下面考虑一种采用分治法设计的排序算法，称为**归并排序**。它完全遵循分治法的设计逻辑：

- **分解**：分解带排列的 $n$ 个元素的序列成各具 $n/2$ 个元素的两个子序列；
- **解决**：使用归并排序**递归地**解决这两个子问题；
- **合并**：合并两个已排序的子序列以产生父问题的答案。

当待排序的序列长度为 $1$ 时，递归算法开始“回升”，在这种情况下不需要做任何工作，因为长度为 $1$ 的序列显然是有序的。

下面让我们来看看归并排序的代码：

```python linenums="1" title="归并排序" hl_lines="4-6 16 17 20-26"
def MergeSort(A, p, r):
    if p < r:
        q = (p + r) // 2
        MergeSort(A, p, q)
        MergeSort(A, q + 1, r)
        Merge(A, p, q, r)

def Merge(A, p, q, r):
    INF = max(A) + 1
    m = q - p + 1
    n = r - q

    L = A[p : q + 1]
    R = A[q + 1 : r + 1]
    
    L.append(INF)
    R.append(INF)

    i = j = 0
    for k in range(p, r + 1):
        if L[i] <= R[j]:
            A[k] = L[i]
            i += 1
        else:
            A[k] = R[j]
            j += 1
```

在 `MergeSort` 中，算法首先将数组 `A` 分割成两个规模为 $n / 2$ 的子问题，然后分别递归调用 `MergeSort`，这里很好地体现了**分治**的思想。子问题处理完成之后，算法将调用 `Merge` 将子数组合并，合并的方法是归并排序的精髓。在 $16, 17$ 行，算法给子数组末尾设置了一个**哨兵**节点，如此避免合并过程中需要检查空数组的问题。在合并的过程中，算法一次从两个子数组中取出一个数，比较大小，将满足顺序的那个放入父数组，这样每个问题合并时只需做一次遍历即可。

### 归并排序的分析

考虑归并排序的正确性，该算法的正确性关键在 `Merge` 算法的正确性，只要 `Merge` 算法正确，则归并排序能够正确将数组排序。对于 `Merge` 算法，可以得发现有这样一个循环不变式：在每次迭代时数组 `A[p...k-1]` 按从小到大（从大到小）的顺序包含 `L[1...m-1]` 和 `R[1...n-1]` 中的元素，且 `L[i]` 和 `R[j]` 一定是各自数组中，没有复制回 `A[p...k-1]` 数组的最小（大）的元素：

- **初始化**：第一次迭代时，数组 `A[p...k-1]` 为空，且 `L` 和 `R` 都是有序的，第一个元素一定是最小（大）的，所以满足循环不变式；
- **保持**：若某次迭代前，循环不变式被满足，则下一次迭代时，从 `L` 和 `R` 中选出的数一定是大于 `A` 中所有数的，因此添加到 `A` 尾部之后，`A` 仍然保持有序，且 `L` 和 `R` 也仍然保持有序；
- **终止**：在循环终止时，`A` 已经有序，且 `L` 和 `R` 中，除了最大（小）的两个哨兵节点外，都已经放入了 `A`，算法正确排序。

如此就证明了归并排序的正确性。接下来考虑归并排序的时间复杂度，依照前面插入排序的分析，且我们已知 `Merge` 时只需要对整个数组遍历一次，也就是线性的时间，因此可以给出下面的式子：

$$
\begin{align*}
T(n) = 
\begin{cases}
c & n = 1 \\
2T(n / 2) + cn & n > 1
\end{cases}
\end{align*}
$$

可以画出下面这样一棵**递归树**：

``` mermaid
graph TD;
    A(("$$cn$$")) --> B(("$$cn/2$$"))
    A --> C(("$$cn/2$$"))
    B --> D(("$$cn/4$$"))
    B --> E(("$$cn/4$$"))
    C --> F(("$$cn/4$$"))
    C --> G(("$$cn/4$$"))
```

可以看出，每一层递归给出的总的时间都为 $cn$，且树的高度为 $\lg{n}$，因此算法的运行时间为 $cn\lg{n} + cn$。

## 总结

本章是算法的基本介绍章节，虽然如此，我们也得到了一些非常有用的结论：

- 插入排序的最坏、最好、平均时间复杂度（省略低阶项）分别为 $cn^2$，$cn$，$cn^2$；
- 归并排序的时间复杂度（省略低阶项）为 $cn\lg{n}$。
