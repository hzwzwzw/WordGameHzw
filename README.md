### 连字成词、下落、确保有解

这篇文章讨论的问题来自于面向对象程设（雍）的大作业中的*游戏5.14_连字成词*选题。我挑题目的主要想法是避开难度集中在可视化界面之类无意义工作的项目，尽可能挑选思考性更强的题。看到这道题后在脑子里稍微构思了一下，觉得这题表面简单细节却不失复杂，于是就定下了选题。

但开始仔细思考时才意识到，这题比我预想的还要复杂不少。所以在这篇文章里介绍一下我的思路，有一些巧思，也有一些妥协，在我个人看来算是一个在实现难度和质量之间达到了比较好的平衡的设计。

#### 题目简介

为了避免表意不清和不必要的麻烦，以下是我结合原题和我自己所期的实现目标进行的题目概括：

> 这是一个消消乐类的游戏，它满足以下特点：
>
> 1. 游戏地图由8*8的方格组成，这些方格不是单纯的色块，而是一个个汉字。它的消除操作不是消去同色的区域，而是消去一组对应着一个成语的四个方格。
> 2. 一个成语所对应的四个方格不仅要相邻，还必须满足四个字恰好按顺序连成直线或折线。换句话说，原成语中相邻的字，其方格也必须相邻。
> 3. 完成一次消除操作后，上方的色块会向下掉落，填充空白区域。
> 4. 游戏地图随机生成，而且必须有解。

#### 基本图形

这个游戏既像消消乐，又颇有一些俄罗斯方块的味道——每个成语由四个方格组成，这四个方格拼成的形状与俄罗斯方块中的方块类似。

因此我们可以先把这些基本图形罗列出来，方便后续生成游戏地图。如图所示，我将图形分为了“有底”与“无底”两类，“无底”的图形在没有其他图形的方格的支撑时，会因为自身的某些方格向下掉落导致相对位置改变，进而导致图形变化而变得无解；而“有底”的图形可以独自稳定存在。这里涉及到一个问题，这个游戏中，成语四个字的顺序固定，因而图形中各个方块的相对位置不能有一点变化，否则就会破坏相邻关系。至于为什么要这样分类，后面再做解释。

顺带一提，"T"形的图形不满足游戏要求，因为这样排列的四个字不可能按顺序连接起来。

#### 确保有解？

对“必须有解”可以有两个角度的理解，一种是“必定存在一种解法，但玩家可能选择其他的消除顺序，如果选择错误将导致游戏失败”，另一种是“不论玩家选择怎样的消除顺序，总能完成游戏”。我起初打算将这两种游戏模式都实现，分别作为游戏的困难和简单难度。但需要提前说明的是，困难模式显然比较容易设计，而想让玩家“总能完成游戏”的简单模式在设计上则存在不少障碍，因而我最后并没有完整实现后者，而是只在理论上尽可能降低了玩家错误操作导致游戏失败的可能性（也算是相对简单了……）。

##### 困难模式

我们先基于困难模式进行讨论，即“必定存在一种解法，但玩家可能因为选择的消除顺序错误而导致游戏失败”。既然只需要存在一种解法，那么显然我们可以直接逆向生成地图，也就是用各种基本图形（包括有底和无底的）“摞”成一个8*8的地图，就解决了问题。

这里的“摞”并不是把基本图形一个个从下往上堆——直接堆会导致图形之间有空隙。而即便考虑了重力向下掉落之后，这样的做法也不完美：很可能只有最下方没有发生过部分掉落（即没有相对运动）的基本图形可以消去，上面的图形一片混乱，玩家只要不停消除最底层的图形就可完成游戏（如果不考虑“无底”图形在落到底层后无法消除的情况的话）。

具体地说，正确的“摞”法应该是：首先在底层铺上一层“有底”类基本图形（确保这些图形最后一定可以消掉），然后每次要插入一个基本图形，都随机选择地图上的一块区域——这块区域大概率已经有了先前放入的其他基本图形的方块，那就把它们向上移动，为新的图形腾出空间放入。如此，既可充分打乱地图，也可保证游戏有至少一种解，这个解即为插入基本图形的逆过程。

这种摞法基于一种想法：尽可能把新的基本图形插入到已有其他方块的区域，通过其他方块的支撑避免这个新的基本图形变形——这样才能确保每一步的操作都是可逆的，而不是发生插入的图形悬空，导致直接下落变成无解的状况，如图所示。

##### 简单模式

再考虑简单模式，即“不论玩家选择怎样的消除顺序，总能完成游戏”。经过上面的分析，自然的想法已经明确了，似乎只要只使用“有底”的基本图形生成，就可以保证不论玩家如何消除，地图最底端的基本图形总是可以消去，总是有解的目标就达成了——然而事实并非如此。如下图所示，图形之间是可能存在交错的。如果先消除了白色图形，那么红蓝就会成为一个死局。如果我们想要通过“摞”的方法保证地图的趣味性的话，这个问题是难以避免的。

当然我们还是可以牺牲游戏性，真的通过“把基本图形一个个从下往上堆”的方法生成地图，如此的确能避免死局，但就太没意思了。

此外，还有一个隐藏的棘手问题——我们要消除的是成语，组成成语的四个字并不是非要在同一个基本图形内的。例如，如果两个相邻的成语有相同的字，如“一诺千金”和“唯唯诺诺”，那么玩家有可能在消除前者时选择了后者的某一个“诺”字，那么游戏应该判定这种消除方法为合法；再例如，如果两个相邻的成语分别是“一诺千金”和“字字珠玑”，那么玩家有可能框选“一字千金”消除，游戏也应该判定这种消除方法为合法。但这两种操作完全背离了游戏生成路径，很可能导致游戏进入无解的境地。至于解决办法，前者可以通过取巧，即在生成成语时禁止两个含有同字的图形太过接近，以尽可能规避这种情况发生；而后者就没有很好的解决办法了。

综合这两个问题考虑，我觉得与其让简单模式做到“确保玩家必胜”，不如~~去除大部分地图的难度，但保留一些无解的可能，这样才知道你玩的是消除游戏~~。是故意的.jpg

#### 地图生成

还有一个大问题需要考虑——我们的地图是8*8的，如果完全随机“摞”图形，很可能无法完全平铺整个地图，而会在部分列多出或少掉几格。解决这个问题的方法是，
