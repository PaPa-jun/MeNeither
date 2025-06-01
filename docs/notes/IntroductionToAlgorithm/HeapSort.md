---
katex: true
---

# 堆排序

??? abstract "重点"
    - 二插堆的概念和储存结构；
    - 堆的性质和种类；
    - 堆的操作：建堆，整堆；
    - 堆排序算法和时间复杂度；
    - 优先队列及其维护操作。

堆排序是一种高效的排序算法，利用**堆**这一数据结构。堆排序的时间复杂度为 $O(n\lg n)$，且与插入排序一样，堆排序具有**空间原址性**即任何时候都只需要常数个额外的空间储存临时数据。

## 堆

堆这一数据结构是一个近似的**完全二叉树**，可以考虑用数组来储存。也就是说，堆作为一刻二叉树，除了最后一层，其余层都是满的，且最后一层从左向右填充。用数组存储使我们可以在 $\Theta(1)$ 的时间内实现求子节点或父节点的下标。

```python title="求子节点或父节点下标" linenums="1"
def Parent(i):
    return i // 2

def Left(i):
    return i * 2 + 1

def Right(i):
    return (i + 1) * 2
```

!!! note "二叉树的数组储存"
    假设数组 `A` 表示一颗完全二叉树，则 `A[0]` 表示根节点，紧随着两个是根节点的两个子节点，然后再是下一层的四个节点，以此类推。

堆分为两类，即最大堆和最小堆，分别满足下面两个性质

$$
A[Parent(i)] \geqslant A[i] \qquad A[Parent(i)] \leqslant A[i]
$$

即，除了根节点之外，所有节 $i$ 点都必须满足：父节点的关键字值大于（小于）等于 $i$ 的关键字的值。

!!! note ""
    除非特殊说明，下文均默认以最大堆为例。

## 堆的维护

堆的维护主要表现为两种操作，即整堆和建堆。前者保证堆时刻满足其性质，后者将一个无需的数组建立成满足堆性质的数组。

### 整堆

整堆操作令待维护节点逐级下降，以满足堆的性质：

```python title="整堆" linenums="1" hl_lines="11"
def MaxHeapfy(A, i):
    l = Left(i)
    r = Right(i)
    largest = i
    if l <= A.heap_size and A[l] > A[largest]:
        largest = l
    if r <= A.heap_size and A[r] > A[largest]:
        largest = r
    if largest != i:
        A[i], A[largest] = A[largest], A[i]
        MaxHeap(A, largest) # 逐级下降
```

整堆操作的最坏情况发生在对根节点做整堆，根节点最小且此时下降方向的子树是满的，可以列出如下递归式

$$
T(n) \leqslant T(2n/3) + \Theta(1)
$$

根据[主定理](./DivideAndconker.md)有 $n^{\log_{3/2}1} = 1$，因此整堆操作的时间复杂度为 $O(\lg n)$。

### 建堆

建堆操作利用整堆操作，自底向上让整个数组都满足性质：

```python title="建堆" linenums="1"
def BuildMaxHeap(A):
    heap_size = len(A)
    for i in reversed(range(heap_size // 2)): # 自底向上，遍历叶节点
        MaxHeapify(A, i, heap_size)
```

建堆操作对每个非叶节点执行一次整堆，对于有 $n$ 个节点的堆，其高度为 $h = \lg n$，也就是说整堆操作的时间复杂度为 $O(h)$，我们可以将建堆操作的总代价表示为

$$
\sum_{h = 0}^{\lg n} \frac{n}{2^{h + 1}} O(h) = O(n)
$$

因此我们可以在线性时间内把一个无序数组建立成最大堆。

## 利用堆的排序算法

利用堆的性质，我们可以在 $\Theta(1)$ 的时间内得到数组的最大元素，因此只需要依次取最大元素，然后将其放到数组中合适的位置并从堆中删去，维护新堆的性质之后重复即可完成排序：

```python linenums="1"
def HeapSort(A):
    BuildMaxHeap(A)
    for i in reversed(range(1， len(A))):
        A[0], A[i] = A[i], A[0]
        A.heap_size -= 1
        MaxHeapfy(A, 0)
```

堆排序的时间复杂度为 $O(n\lg n)$，因为每次调用 `MaxHepfy` 的代价为 $O(\lg n)$，调用 $n - 1$ 次的代价为 $O(n\lg n)$。

## 优先队列

利用堆可以实现一种名为**优先队列**的数据结构。同样的分为最大优先队列和最小优先队列，下面以最大优先队列为例。其操作共有四个分别为

- 把元素 $x$ 插入队列中：`Insert(A, x)`；
- 返回队列中最大的值：`Maximum(A)`；
- 返回并去除队列中最大的值：`ExtractMax(A)`；
- 将元素 $x$ 的关键字**增加到** $k$，要求 $k\geqslant x$：`IncreaseKey(A, x, k)`。

上述操作利用堆的性质都很好实现：

```python linenums="1"
def Maximum(A):
    return A[0]

def ExtractMax(A):
    if A.heap_size < 1:
        raise Exception("Heap underflow.")
    max = A[0]
    A[0] = A[A.heap_size - 1]
    A.heap_size -= 1
    MaxHeapfy(A, 0)
    return max

def IncreaseKey(A, i, k):
    if k < A[i]:
        raise Exception("k must be bigger than A[i]")
    A[i] = k
    while i > 0 and A[Parent(i)] < A[i]:
        A[i], A[Parent(i)] = A[Parent(i)], A[i]
        i = Parent(i)

def Insert(A, k):
    A.heap_size += 1
    A[A.heap_size - 1] = -float("inf")
    IncreaseKey(A, A.heap_size - 1, k)
```

上述操作中比较有意思的是最后的插入操作，初始化一个最小的节点放在末尾，然后利用 `IncreaseKey` 实现堆的性质的维护。

