# 中位数和顺序统计量

在一个由 $n$ 个元素组成的集合中，第 $i$ 个**顺序统计量**是该集合中第 $i$ 小的元素。本文讨论从一个 $n$ 个**互异**的元素构成的集合中**选择**第 $i$ 个顺序统计量的问题。

??? abstract "重点"
    
    - 最大和最小值的求解方法；
    - 期望时间为线性的选择算法；
    - 最坏时间为线性的选择算法及其时间分析。

## 最小值和最大值

考虑在一个有 $n$ 个元素的集合中，如何找到最小的元素？一个显然的算法如下

```python linenums="1" title="线性查找最小自值"
def Minimum(A):
    min = A[0]
    for i in range(1, len(A)):
        if A[i] < min:
            min = A[i]
    return A[i]
```

这样可以通过 $n - 1$ 次比较找到最小值，同理也可以找到最大值，且可以证明，这个算法在当前场景下是**最优**的。

如果改变一下场景，要求**同时找到最大值和最小值**，则可以有一种至多比较 $3 \lfloor n/2 \rfloor$ 次的算法。算法的基本思想是：将数组元素**两两分组**，然后在每一对中分别找出较大值和较小值。这样每一对元素只需比较一次即可确定一个较大值和一个较小值。然后**分别对所有的较大值和较小值进行比较**，找出整体的最大值和最小值。

```python linenums="1" title="同时查找最大值和最小值"
def MinMax(A):
    if len(A) % 2 == 0:             # 如果 A 中元素有偶数个
        min_val = min(A[0], A[1])
        max_val = max(A[0], A[1])
        i = 2
    else:                           # 如果 A 中元素有奇数个
        min_val = max_val = A[0]
        i = 1

    while i < len(A) - 1:
        if A[i] < A[i + 1]:         # 比较元素对中的大小关系
            min_val = min(min_val, A[i])
            max_val = max(max_val, A[i + 1])
        else:
            min_val = min(min_val, A[i + 1])
            max_val = max(max_val, A[i])
        i += 2                      # 成对处理

    return min_val, max_val
```

通过这种方法，我们可以将比较次数减少到最多 $3 \lfloor n/2 \rfloor$ 次。

## 期望为线性时间的选择算法

一般选择问题选择数组中的第 $i$ 个顺序统计量。根据顺序统计量的定义，可以考虑将数组按照 $i$ 为**主元**进行划分，这就不禁令人想到快速排序中的 [Partition 算法](./QuickSort.md/#_3)，这个算法实现的就是将数组划分成大于主元和小于主元的两个部分。

因此我们可以借鉴快速排序算法，利用[随机化的 Partition 算法](./QuickSort.md/#_8)，递归地寻找顺序统计量。

```python title="选择算法" linenums="1"
def RandomizedSelection(A, p, r, i):
    if p == r:
        return A[p]
    q = RandomizedPartition(A, p, r)
    k = q - p + 1
    if i == k:          # 找到
        return A[q]
    elif i < k:         # 仅递归划分一边
        return RandomizedSelection(A, p, q - 1, i)
    else:
        return RandomizedSelection(A, q + 1, r, i - k)
```

算法的期望时间复杂度是 $O(n)$，但是最坏情况下，算法每次划分选择的都是最大的元素为主元，此时的复杂度为 $\Theta(n^2)$。

## 最坏情况线性时间的选择算法

虽然随机化选择算法在期望时间上是线性的，但在最坏情况下时间复杂度仍然是 $O(n^2)$。为了保证最坏情况下的线性时间复杂度，我们可以使用一种称为 **Median of Medians** 的方法。

该算法的基本思想是将数组划分为若干小组，每组包含 5 个元素，然后对每组进行排序并找出中位数。接着递归地找出这些中位数的中位数，作为主元调用修改的 Partition 算法进行划分。这样可以保证划分的平衡性，从而保证最坏情况下的线性时间复杂度。

```python title="最坏情况线性时间选择算法" linenums="1"
def Selection(A, p, r, i):
    if p == r:
        return A[p]
    mid = MedianOfMedians(A, p, r)
    q = Partition(A, p, r, mid)
    k = q - p + 1
    if i == k:
        return A[q]
    elif i < k:
        return Selection(A, p, q - 1, i)
    else:
        return Selection(A, q + 1, r, i - k)

def MedianOfMedians(A, p, r):
    n = r - p + 1
    if n <= 5:
        return InsertionSort(A, p, r)[n // 2]
    medians = []
    for i in range(p, r + 1, 5):    # 划分数组
        sub_right = i + 4 if i + 4 <= r else r
        median = InsertionSort(A, i, sub_right)[(sub_right - i) // 2]
        medians.append(median)
    # 递归调用 Selection 找到中位数的中位数
    return Selection(medians, 0, len(medians) - 1, len(medians) // 2)

def InsertionSort(A, p, r):
    for i in range(p + 1, r + 1):
        key = A[i]
        j = i - 1
        while j >= p and A[j] > key:
            A[j + 1] = A[j]
            j -= 1
        A[j + 1] = key
    return A[p:r + 1]

def Partition(A, p, r, pivot):
    i = p - 1
    for j in range(p, r):
        if A[j] < pivot:
            i += 1                  # 时刻保证 i 指向左侧子数组的最后一个位置
            A[i], A[j] = A[j], A[i] # 将小于主元素的数交换到左侧子数组的位置
        if A[j] == pivot:
            k = j                   # 记录主元的位置
    A[i + 1], A[k] = A[k], A[i + 1] # 交换主元到 i + 1
    return A[i + 1]
```

可以看到，修改的 Partition 算法把主元也作为一个传入参数进行处理。通过这种方法，我们可以保证选择算法在最坏情况下的时间复杂度为 $O(n)$。
