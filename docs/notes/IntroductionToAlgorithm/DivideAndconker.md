---
katex: true
---

# 分治策略

??? abstract "重点"

    - 替换法：
        - 猜测解：数学归纳法证明
        - 变量变换法
    - 迭代法：
        - 展开法
        - 递归数法
    - 主定理
    - 补充：递归与分治法
        - 递归设计技术
        - 递归程序的非递归化
        - 算法设计
    - 算法
        - Fibonacci 数
        - 生成全排列
        - 二分查找
        - 大整数乘法
        - Stranssen 矩阵乘法
        - 导线和开关

在前面的[算法基础](./Fundation.md)部分，我们已经看到了分治策略在解决算法问题时的优势，本章详细介绍算法设计当中的分治策略。

## 最大子数组问题

最大子数组问题的目标为，寻找数组 `A` 中**和**最大的非空连续子数组。这个问题可以很好的利用分治策略来解决：考虑将数组 `A[low...high]` 划分成规模尽可能相等的两个子数组 `A[low...mid]` 和 `A[mid+1...high]`，那么 `A[low...high]` 中的最大子数组必定以下面三种情况之一出现：

- 最大子数组完全在 `A[low...mid]` 中；
- 最大子数组完全在 `A[mid+1...high]` 中；
- 最大子数组横跨 `A[low...mid]` 和 `A[mid+1...high]`。

对于前两种情况，可以直接递归地求解，其问题形式与原问题一样，只是规模更小；对于第三种情况，我们只需要找出形如 `A[i...mid]` 和 `A[mid+1...j]` 的最大子数组然后合并即可。当三种情况的最大子数组都被找出时，原问题的最大子数组就是三者中较大的那一个，如此就解决了问题。

下面看一下具体的算法

```python title="最大子数组问题的分治策略实现"
def MaxSubarray(A, low, high):
    if high == low:
        return (low, high, A[low])      # 基本情况：只有一个元素
    
    # 否则
    mid = (low + high) // 2
    (left_low, left_high, left_sum) = MaxSubarray(A, low, mid) # 递归求解左子数组
    (right_low, right_high, right_sum) = MaxSubarray(A, mid + 1, high) # 递归求解右子数组
    (cross_low, cross_high, cross_sum) = MaxCrossSubarray(A, low, mid, high) # 寻找跨越中间点的最大子数组

    # 合并
    if left_sum >= right_sum and left_sum >= cross_sum:
        return (left_low, left_high, left_sum)
    elif right_sum >= left_sum and right_sum >= cross_sum:
        return (right_low, right_high, right_sum)
    else:
        return (cross_low, cross_high, cross_sum)

def MaxCrossSubarray(A, low, mid, high):
    # 初始化左边最大值为负无穷大
    left_sum = float('-inf')
    sum = 0
    max_left = mid
    for i in range(mid, low - 1, -1):   # 从 mid 向 low 方向遍历
        sum += A[i]
        if sum > left_sum:
            left_sum = sum
            max_left = i

    # 初始化右边最大值为负无穷大
    right_sum = float('-inf')
    sum = 0
    max_right = mid + 1
    for j in range(mid + 1, high + 1):  # 从 mid + 1 向 high 方向遍历
        sum += A[j]
        if sum > right_sum:
            right_sum = sum
            max_right = j

    return (max_left, max_right, left_sum + right_sum)
```

下面分析一下这个算法的运行时间。对于基本情况，当数组中只有一个元素时，算法的运行时间显然是常数级别，否则进行递归。另外 `MaxCrossSubarray` 算法需要对传入的数组做一次遍历，因此每次递归运行时间是线性的，由此得到下面的递归式

$$
\begin{aligned}
T(n) = 
\begin{cases}
\Theta(1) & n = 1 \\
2T\left(\dfrac{n}{2}\right) + \Theta(n) & n > 1
\end{cases}
\end{aligned}
$$

之后的部分会介绍该递归式的求解。
