# 贪心算法

??? abstract annotate "重点"

    - 方法的基本思想和基本步骤；
    - 贪心算法的正确性保证：满足贪心选择性质；
    - 贪心算法与动态规划的比较；
    - 两种背包问题的最优性分析：最优子结构性质和贪心选择性质；
    - 算法设计。(1)

1.  小数背包、活动安排、找钱问题。

## 基本思想

贪心算法的思想很简单：从某一个初始解出发，通过一系列**贪心选择**——当前状态下的**局部最优**选择，逐步逼近给定目标，尽可能快地求得更好的解。

!!! warning ""
    算法不能保证对所有问题都达到**全局最优**，但是对某些具有**特殊结构**的问题，算法可以达到全局最优。

## 问题特征

当问题具有[最优子结构性质](./DynamicProgramming.md/#_3)和**贪心选择性**时可以利用贪心算法得到全局最优解，若不满足贪心选择性，算法只能得到近优解。

!!! note annotate "贪心选择性"
    问题可通过贪心选择达到全局最优。(1)

1.  并不是所有具有最优子结构性质的问题都可以采用贪心策略，往往可以利用最优子结构性质来证明贪心选择性。

| 动态规划 | 贪心算法 |
|: --- :|: --- :|
| 每一步的选择基于子问题的解 | 每一步利用贪心选择快速选择一个局部最优解 |
| 需要先求解子问题 | 贪心选择在子问题被求解之前就可以做出 |
| 通常采用自底向上的设计 | 通常采用自顶向下的设计 |
| 更慢、更复杂 | 更快、更简单 |

## 算法设计实例

!!! warning "后边再写。。问题比较简单"

### 小数背包

### 活动安排

### 货币找零


