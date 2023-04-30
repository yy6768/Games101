# Games101 作业2

## 目标

- 实现z-buffering
- 加分项-实现anti-aliasing



## 基本目标

- 实现bounding box
  - 非常简单，记录bounding box的左下和右上坐标即可
- 遍历box检查是否在三角型中心
  - 顺时针计算三角形三条边的向量
  - 顺时针计算三角形三个点到我们寻找的点的向量
  - 检查是否同号
- 插值深度值（没学，注释已经给出）
- 比较当前深度值和depth_buffer记录的最小深度，如果更小，更新当前的颜色和深度值



基本上非常简单不多说

绘制效果

遇到模型相反请参考

[闫令琪：Games101 现代计算机图形学-作业Assignment02解析 - 灰信网（软件开发博客聚合） (freesion.com)](https://www.freesion.com/article/4973968295/)

![image-20230423151410784](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423151410784.png)

## 提高目标

实现之后也不是很难，具体请参照lecture # 6介绍的原理

- 每个像素进行N*N采集（这里N=2）
- 判断每个采集点是否在内部（这里需要把判断的函数改成float）
- 对颜色取均值



- 因为必须记录所有小采样点的颜色，所以使用super_depth_buffering（如果不记录会产生黑边）
- 对于每个像素遍历4次，对4个采样点计算是否超过z值
- 最后对一个像素的4个采样点的颜色加权平均即可

![image-20230423152043046](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423152043046.png)

