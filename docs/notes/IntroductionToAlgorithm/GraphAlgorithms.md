# 图算法

??? abstract "重点"
    1. BFS 和DFS 算法：
        - 白色、灰色和黑色结点概念和作用；
        - DFS 遍历过程中搜索边的种类和识别；
        - 计算过程及其时间复杂度。
    2. 最小生成树：
        - 安全边概念和一般算法（Generic algorithm）；
        - Kruskal 算法和 Prim 算法的计算过程和计算复杂性；
        - 两种贪心算法的贪心策略和贪心选择性质。
    3. 单源最短路径（略） ：
        - 单源最短路径 $\delta(s, v)$ 和短路径上界 `d[v]` 概念；
        - 边松弛技术及其一些性质；
        - 三种问题算法的计算过程及其时间复杂度：Bellman-Ford 算法、DAG 算法和 Dijkstra 算法。
    4. 所有点对最短路径（略） ：
        - 为什么能转换为矩阵乘法？
        - 基于矩阵乘法的较慢和快速算法的时间复杂度；
        - Floyd-Warshall Algorithm 的思路和时间复杂度；
        - Johnson Algorithm 适应的问题及其时间复杂度。

图是一种十分重要的数据结构，由节点和边组成。我们用 $G = (V, E)$ 表示一个图，其中 $V$ 和 $E$ 分别为图的节点集合和边集合。

## 图的逻辑表示

有两种方法来表示一个图结构，一种是**邻接链表**一种是**邻接矩阵**。前者适用于稀疏图，后者适用于稠密图。

=== "图"

    ```mermaid
    graph LR
        A((1)) --> B((2))
        B --> C((3))
        B --> D((4))
        C --> D
        D --> E((5))
        E --> A
        E --> B
    ```

=== "邻接链表"

    ```mermaid
    graph LR
        A((1)) --> F((2))
        B((2)) --> G((3)) --> H((4))
        C((3)) --> I((4))
        D((4)) --> J((5))
        E((5)) --> K((1)) --> L((2))
    ```

=== "邻接矩阵"

    $$
    \begin{aligned}
    \left(\begin{array}{ccccc}
    0 & 1 & 0 & 0 & 0 \\
    0 & 0 & 1 & 1 & 0 \\
    0 & 0 & 0 & 1 & 0 \\
    0 & 0 & 0 & 0 & 1 \\
    1 & 1 & 0 & 0 & 0
    \end{array}\right)
    \end{aligned}
    $$

邻接链表的空间需求为 $\Theta(V + E)$，邻接矩阵空间需求为 $\Theta(V^2)$。

## 广度优先搜索

广度优先搜索按照从源节点开始，系统性地探索当前节点的**所有邻居**，按照**广度优先**的原则遍历图。

搜索过程将节点分为白色、灰色和黑色三种，白色代表节点**尚未被发现**，灰色代表节点已经被发现但是**其邻居可能还没被发现**，最后黑色说明**该节点及其所有邻居**都已被发现。

```python title="广度优先搜索（BFS）" linenums="1"
def BFS(G, s):
    for u in G.V - [s]:
        u.color = 'white'
        u.d = float('inf')
        u.pi = None
    s.color = 'gray'
    s.d = 0
    s.pi = None
    Q = Queue()
    Q.put(s)
    while not Q.empty():
        u = Q.get()
        for v in G.E[u]:
            if v.color == 'white':
                v.color = 'gray'
                v.d = u.d + 1
                v.pi = u
                Q.put(v)
        u.color = 'black'
```

广度优先搜索的总运行时间为 $O(V + E)$，是图的邻接链表大小的线性函数。

### 广度优先搜索树



## 深度优先搜索

深度优先搜索按照从源节点开始，尽可能深地探索每一个分支，直到不能继续为止，然后回溯并继续探索下一个分支。

搜索过程同样将节点分为白色、灰色和黑色三种，白色代表节点**尚未被发现**，灰色代表节点已经被发现但是**其邻居可能还没被发现**，最后黑色说明**该节点及其所有邻居**都已被发现。

```python title="深度优先搜索（DFS）" linenums="1"
def DFS(G):
    for u in G.V:
        u.color = 'white'
        u.pi = None
    time = 0
    for u in G.V:
        if u.color == 'white':
            DFSVisit(G, u, time)

def DFSVisit(G, u, time):
    time += 1
    u.d = time
    u.color = 'gray'
    for v in G.E[u]:
        if v.color == 'white':
            v.pi = u
            DFSVisit(G, v, time)
    u.color = 'black'
    time += 1
    u.f = time
```

深度优先搜索的总运行时间为 $O(V + E)$，同样是图的邻接链表大小的线性函数。

### 深度优先搜索森林

## 最小生成树


