# Games101 作业4

## 前言

- 源码位于：[yy6768/Games101: 学习Games101的笔记和自己的作业解决 (github.com)](https://github.com/yy6768/Games101)

## 要求

- 实现De CasteLjau算法
- 奖励分数：反走样



## De CasteLjau算法

![image-20230504150645003](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504150645003.png)



- 感觉不难只要实现一个简单的递归函数即可

  - 遍历所有i:=0-len-1, 每一次计算新的点，然后存储到`new_control_points`(一个`std::vector<cv::Point2f>`变量)，然后在递归即可

  - 直到只有一个控制点时返回该控制点

结果：

![image-20230504150738081](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504150738081.png)



## 提高：反走样

想了很久效果都不好，最后参考了[(149条消息) Games101：作业4解析（含提高部分）_games101 作业4答案-CSDN博客](https://blog.csdn.net/Q_pril/article/details/123818346)

它的思路不复杂但是很合理

- 先寻找距离最近的像素（round(x,y))
- 对于像素附近的其余3个点（4领域，8领域效果不好在文章中也详细说明了）[作业四得到这样的结果是否满足要求？ – 计算机图形学与混合现实在线平台 (games-cn.org)](https://games-cn.org/forums/topic/作业四得到这样的结果是否满足要求？/) 讨论区说明了如果取8领域会导致线条过粗导致锯齿明显
- 颜色的权重和距离成反比
- 取最大值（BBS中助教的更正说明了会影响之前的赋值）

![image-20230504155829514](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504155829514.png)

感觉效果不错