# 数据链路层协议

本文介绍几个基本的数据链路层协议。

## 基本数据链路层协议

### Utopia 协议

Utopia（乌托邦）协议是一种完全理想化的协议，工作在完全理想化的环境中：

- 数据单向传输，收发双方的网络层一直处于就绪状态；
- 处理时间可忽略不计，接收缓冲空间无限大（无需任何流量控制）；
- 信道不会损坏或丢失帧（无需任何差错控制）。

=== "发送端"

    ```c linenums="1" hl_lines="7-9"
    void sender()
    {
        frame s;
        packet buffer;
        while (true)
        {
            from_network_layer(&buffer);    //拿到数据包
            s.info = buffer;                //成帧
            to_physical_layer(&s);          //调用物理层服务实现发送
        }
    }
    ```

=== "接受端"

    ```c linenums="1" hl_lines="7-9"
    void receiver()
    {
        frame r;
        event_type event;
        while (true)
        {
            wait_for_event(&event);     // 等待接收到数据
            from_physical_layer(&r);    // 接收数据
            to_network_layer(&r.info);  //传给网络层
        }
    }
    ```

协议只需成帧处理，无需做其它任何处理。

### 停-等协议

在理想条件基础上，增加流量控制，修改假设：

- 数据单向传输，收发双方的网络层一直处于就绪状态；
- **接收端需要一定的接收处理时间，接收缓冲只能存放一个帧；**
- 信道不会损坏或丢失帧（无需任何差错控制）。

为了防止发送快于接收而造成数据丢失，发送端在发送一帧后必须**停止发送**，**等待**接收端发回的反馈确认短帧；接收端在收到一个帧并发送网络层后，需向发送端发一反馈**确认短帧**（不需包含任何信息，因为信道是无差错的），表示可发新帧。

=== "发送端"

    ```c linenums="1"
    void sender()
    {
        frame s;
        packet buffer;
        event_type event;
        while (true)
        {
            from_network_layer(&buffer);    // 拿到数据包
            s.info = buffer;                // 成帧
            to_physical_layer(&s);          // 调用物理层服务实现发送
            wait_for_event(&event);         // 等待事件
        }
    }
    ```

=== "接收端"

    ```c linenums="1"
    void receiver()
    {
        frame r, s;
        event_type event;
        while (true)
        {
            wait_for_event(&event);     // 等待事件
            from_physical_layer(&r);    // 接收数据帧
            to_network_layer(&r.info);  // 传给网络层
            to_physical_layer(&s);      // 调用物理层服务实现发送
        }
    }
    ```

由于需要反馈，且帧的发送和反馈是严格交替进行的，所以一般采用**半双工**信道。

### 有噪音信道的停-等协议

进一步考虑实际的会出错的信道，帧既可能损坏（接收端可通过校验检查出错误），也可能完全丢失。

发送端仍通过接收端的反馈来决定怎么做。但由于帧会丢失，发送端可能收不到反馈的确认帧，因此发送端必须引入**超时机制**（time out），即增加一个定时计数器，在一定时间后对没有确认的帧进行重发，也称作 ARQ（Automatic Retransmit reQuest）。

!!! note "时间值应选择稍大于两倍端到端的信号传输时间和接收端的接收处理时间之和。"

当接收端的反馈确认帧丢失时，必须通过为帧编制序号来解决重复帧的问题。

!!! note "停等协议帧序号"
    帧的序号位数应尽量的短从而少占用帧头的空间，在简单停-等协议中只需 1 个比特位（“0”-“1”，“1”-“0”）即可。这是由于在本协议中，发送端每发送一个帧都是建立在此帧之前的所有帧都已正确发送的基础上，只需区分相邻的两个连续帧即可避免重复的可能。

收发双方都需维护各自的帧序号（sequence number）。发送端维护的帧序号 $N(S)$ 表示当前所发帧的序号，接收端维护的帧序号 $N(R)$ 表示接收端当前所期待接收的帧序号。

=== "发送端"

    ```c linenums="1"
    void sender()
    {
        seq_nr next_frame_to_send;
        frame s;
        packet buffer;
        event_type event;
        next_frame_to_send = 0;
        from_network_layer(&buffer);                // 从网络层拿到数据包
        while (true)
        {
            s.info = buffer;                        // 成帧
            s.seq = next_frame_to_send;             // 设置帧序号
            to_physical_layer(&s);                  // 调用物理层服务实现发送
            start_timer(s.seq);                     // 开始计时
            wait_for_event(&event);                 // 等待事件
            if (event == frame_arrival)             // 收到确认帧
            {
                from_physical_layer(&s);            // 接收确认帧
                if (s.ack != next_frame_to_send)    // 可以发送下一个帧
                {
                    stop_timer(s.seq);              // 停止定时计数器
                    from_network_layer(&buffer);    // 准备发送下一个帧
                    inc(next_frame_to_send);        // 帧序号取反
                }
                else                                // 期待接收上一个帧
                {
                    to_physical_layer(&s);          // 重发帧
                    start_timer(s.seq);             // 重新计时
                }
            }
            else if (event == timeout)              // 超时
            {
                to_physical_layer(&s);              // 重发帧
                start_timer(s.seq);                 // 重新开始计时
            }
        }
    }
    ```

=== "接收端"
        
    ```c linenums="1"
    void receiver()
    {
        seq_nr frame_expected;
        frame r, s;
        event_type event;
        from_expected = 0;                      // 期待序号置零
        while (true)
        {
            wait_for_event(&event);             // 等待事件
            if (event == frame_arrival)         // 收到数据帧
            {
                from_physical_layer(&r);        // 接收数据帧
                if (r.seq == frame_expected)    // 检查序号是否匹配
                {
                    to_network_layer(&r.info);  // 传给网络层
                    inc(frame_expected);        // 期待序号取反
                }
                s.ack = frame_expected;         // 确认帧序号
                to_physical_layer(&s);          // 发送确认帧
            }
        }
    }
    ```

然而，在时延大的信道（如卫星通信）中，停-等协议的效率是很低的。

!!! example
    考虑两个地面站通过卫星通信，典型的传播时间约为 $270 \text{ ms}$。假设一个帧的发送时间为 $20 \text{ ms}$，则从发送站开始发送算起，经 $20 \text{ ms} + 270 \text{ ms} = 290 \text{ ms}$，数据帧才能到达目的站。假设不考虑目的站的处理时间，且认为确认帧非常短，其发送时间可忽略不计，则又需 270 ms 确认帧才能被发送站收到。因此信道的利用率为

    $$
    \text{信道利用率} = \frac{20 \text{ ms}}{290 \text{ ms} + 270 \text{ ms}} = \frac{1}{28}
    $$

    非常低。这是由于每发一个帧之前都必须等待前一个帧的确认帧所造成的。

## 滑动窗口协议

滑动窗口协议是一种非常可靠、适用于各种条件的通用流量控制协议，特别是在效率、复杂性及对缓冲区的需求等方面可作灵活调配。

### 滑动窗口

**发送窗口**是发送端允许在**未收到确认的情况下连续发送的帧的序号集合**。允许连续发送的帧的数量称为发送**窗口尺寸**，表示为 $W$。发送端必须有 $W$ 个输出缓冲区来存放 $W$ 个数据帧的副本以备数据帧的重发。当发送端收到发送窗口下沿帧的肯定确认时，**将发送窗口整体向前滑动一个序号**，并从输出缓冲区中删除相应的数据帧副本。

**接收窗口**是接收端允许接收的帧的序号集合。允许接收的帧的数量称为接收窗口尺寸。接收端必须设置相应数量的输入缓冲区来支持接收窗口。

对于接收端收到的帧，如果其序号落在接收窗口外，则该帧会被**直接丢弃**。只有落在接收窗口内的帧才会被接收端进行校验处理。如果校验正确：

- 当接收的帧不是接收窗口下沿帧时，必须暂存在输入缓冲区，不能交给网络层。
- 当接收到接收窗口下沿帧时，会将其连同后面连续的若干个检验过的正确帧**按顺序**交给网络层，并在发回确认帧的同时将接收窗口**向前滑动**相应的数量。

### 回退 $n$ 协议

在回退 $n$ 协议中，发送窗口的尺寸 $W_s$ 大于 1，而接收窗口的尺寸 $W_r$ 等于 1。由于 $W_r = 1$，接收端只能按顺序接收数据帧。一旦某个帧出错或丢失，**接收端会丢弃该帧及其所有后续帧**，不作任何确认。发送端超时后需重发出错或丢失的帧及其后续所有帧。

发送端需要为每个待确认的帧设置一个定时计数器。发送窗口的尺寸 $W_s$ 不能超过 $2^n - 1$（其中 $n$ 为序号的编码位数），**否则会导致接收端无法分辨新旧数据帧**。

!!! note 
    回退 $n$ 协议只要求发送端保持一定数量的缓存来保存未确认的数据帧，对接收端没有缓存要求。但在误码率高的情况下，会显著降低信道的利用率。


### 选择重传协议

选择重传协议中，发送和接收窗口的尺寸都大于 1。由于接收窗口的尺寸大于 1，接收端可存储坏帧之后的其它数据帧（落在接收窗口），接收端对错帧发否定确认帧，因此发送端只需重发出错的帧，而不需重发其后的所有后续帧。

接收端正确收到重发的帧后，可对其后连续的已接收的正确帧作一次**总体确认**（最大序号的确认），并交送网络层。大大提高了信道的利用率。

接收窗口的尺寸不能超过 $2^n - 1$（即序号范围的 1/2），否则可能造成**帧的重叠**。

!!! note
    发送窗口的尺寸一般和接收窗口的尺寸相同，发送端为每一个输出缓存区设置一个定时计数器，定时器一旦超时，相应输出缓存区中的帧就被重发。

## 面向位的协议

高级数据链路控制（High-Level Data Link Control，HDLC）是由国际标准化组织制定的面向位的有序链路层协议。它是为非平衡的链路级操作而研制的，采用主从结构，链路上一个主站控制多个从站。主站向从站发命令，从站向主站返回响应。

HDLC 中只有一个地址域，即从站的地址。在命令帧中，它是目的地址；在响应帧中，它是源地址。

### 帧的格式

| 字段名称 | 描述 |
| --- | --- |
| 标志字段 | 用于帧的同步，通常为 `01111110`，在数据位有 5 个连续的1出现时，就插入 1 个 0（参考[比特填充的标志比特法](./DataLinkLayer.md/#_6)）|
| 地址字段 | 在命令帧中表示目的地址，在响应帧中表示源地址，全 1 为广播地址，全 0 为测试地址 |
| 控制字段 | 用于控制帧的类型和序号 |
| 信息字段 | 包含实际传输的数据 |
| 帧校验序列（FCS） | 用于错误检测 |

### 控制字段

控制字段用于区分不同类型的帧，并包含序号信息。HDLC 定义了三种类型的帧：

| 帧类型 | 描述 |
| --- | --- |
| 信息帧（I 帧） | 用于传输用户数据和控制信息 |
| 监控帧（S 帧） | 用于流量控制和错误控制 |
| 无编号帧（U 帧） | 用于链路管理 |

### 监控帧

监控帧用于流量控制和错误控制，包含以下几种类型：

| 代码 | 类型 | 描述 |
| --- | --- | --- |
| 00 | 接收就绪（RR）| 表示接收方已准备好接收更多数据 |
| 01 | 接收不就绪（RNR）| 表示接收方暂时无法接收数据 |
| 10 | 拒绝（REJ）| 表示接收方检测到错误，需要重传 |
| 11 | 选择重传（SREJ）| 表示接收方请求重传特定帧 |

### 无序号帧

无序号帧用于链路管理和控制，包含以下几种类型：

| 类型 | 描述 |
| --- | --- |
| 设置（SET） | 用于初始化链路 |
| 断开（DISC） | 用于断开链路 |
| 无编号确认（UA） | 用于确认无编号帧 |
| 帧拒绝（FRMR） | 用于报告帧格式错误 |

HDLC 协议通过这些机制实现了高效、可靠的数据链路层通信。