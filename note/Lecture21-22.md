# Lecture 21

## 课程内容

- 计算机动画
  - 历史
  - keyframe animation（关键帧）
  - Physical simulation（物理模拟）
  - Kinematics
  - Rigging



## 动画

- 让东西动起来
- 图按照一定速度进行播放

- 动画的质量和图的数量有关
  - 电影24帧
  - 游戏30fps
  - 虚拟现实帧率要求非常高：90fps



### 动画历史



- 1878年第一部电影

- 数字动画1963

![image-20230606223935284](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606223935284.png)

- 最早的计算机动画
- 侏罗纪公园（1993） 电脑生成的恐龙
- 玩具总动员（1995）光栅化
- "Cloudy With a Chance of Meatballs"(2009)
- 冰雪奇缘2 各种细节 火焰、风雪、光线的各种细节技术



## 关键帧

![image-20230606224305739](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606224305739.png)

- 中间补充关键帧

- 中间可以进行插值

  ![image-20230606224503163](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606224503163.png)

- 线性插值/几何插值

  ![image-20230606224618409](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606224618409.png)



## 物理模拟

- newton‘s Law：$F= ma$

基于物理的动画：

![image-20230606224735862](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606224735862.png)



## 质点弹簧系统（Mass Spring Rope）

- 一系列质点通过弹簧连接

  ![image-20230606225235891](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606225235891.png)

![image-20230606225409687](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606225409687.png)

- 问题：永远都能震动下去

- 导数的表示

![image-20230606225459593](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606225459593.png)

引入能量损失：

![image-20230606225533819](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606225533819.png)

问题：所有点都会同步停下来，无法描述内部的阻力



![image-20230606225625989](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606225625989.png)

添加内部的阻力：

- 首先内部摩擦里和相对作用有关
- 旋转类的相对速度不应该被损耗，所以通过点乘（投影）消除垂直的力的作用

![image-20230606230245012](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230245012.png)

建模

但是上述布料建模的问题：

- 无法解决切变（拉）
- 无法解决out-of-plane的问题

![image-20230606230307092](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230307092.png)



解决切变

![image-20230606230346148](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230346148.png)

虽然解决了切变，但是引入了不对称（旋转以后效果不一样，和布料性质不同）



![image-20230606230424023](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230424023.png)

虽然能解决了不对称，但是不能解决out-of-plane（不在平面内的力，例如折叠，弯曲）



![image-20230606230514936](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230514936.png)

引入skip connection 越过一个点再连接（弱连接），可以抵抗非平面的弯折



## 有限元方法

![image-20230606230714794](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230714794.png)



## Particle system（粒子系统）

- 非常容易实现
- 可拓展

问题

- 会引入引力的问题，需要使用一些加速结构

- 可以模拟流体

![image-20230606230910247](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606230910247.png)



算法：

![image-20230606231014698](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231014698.png)



### 粒子系统力

- 引力，电磁力
- 斥力，弹簧力
- 粘滞力，阻力
- 碰撞



#### 引力

![image-20230606231212329](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231212329.png)



### Simulated Flocking as an ODE

- 鸟类的吸引
- 鸟类的斥力
- 考虑鸟群的平均飞行方向

![image-20230606231427432](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231427432.png)



## Forward Kinematics 运动学

![image-20230606231535623](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231535623.png)

- pin 1维旋转
- ball（bone就是这种） 2d旋转
- prismatic joint 可以拉长



计算尖端p的位置：

![image-20230606231713631](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231713631.png)



问题

- 非常的物理
- 对艺术家不友好



## 逆运动学 Inverse Kinematics

![image-20230606231833879](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231833879.png)

- 逆运动学：控制终端p，计算中间的变换

![image-20230606231907967](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606231907967.png)

问题：解可能不唯一

解决方案：

![image-20230606232001345](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232001345.png)

- 通过优化方法：梯度下降、newtown法等方法



## Rigging（绑定）

- 对于一个形状的控制（类似木偶）

![image-20230606232131076](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232131076.png)

- 添加控制节点
- 添加软节点，蒙皮等等



### blend shapes

![image-20230606232319727](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232319727.png)

- 之前角色有不同的表面
- 可以通过控制点进行插值



## Motion capture（动作捕捉）

![image-20230606232433769](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232433769.png)

好处

- 贴近真实

问题：

- 服装、道具、摄像机成本大
- 现实的表现不符合艺术的目标（有些动画超脱现实）

其他的动捕方式

- marker（贴片）
- 高频相机捕捉

![image-20230606232727198](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232727198.png)



动作数据

![image-20230606232801507](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232801507.png)



恐怖谷效应

![image-20230606232832645](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232832645.png)



## 动画生产线

![image-20230606232953463](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230606232953463.png)

- VFX：可视化效果



# Lecture 22

## 课程内容

- 显示欧拉方法
- 稳定性的提高方法
- 刚体模拟
- 流体模拟



## Simple particle simulation

理想情况：粒子如何在速度场中运动（匀速度，多粒子）

场：有位置有方向

![image-20230607083811399](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607083811399.png)



常微分方程

![image-20230607083904967](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607083904967.png)



## 欧拉方法（Euler‘s method)

![image-20230607084041506](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607084041506.png)

- 简单迭代
- 精度很低
- 比较不稳定

![image-20230607084256510](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607084256510.png)



稳定性：

- 螺旋速度场：欧拉方法计算的路径无论如何一定会离开速度场，但是正常模拟最后会离开速度场
- 第二个例子（左下图）正确模拟应该趋近于水平，而模拟后震荡很大

![image-20230607084412637](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607084412637.png)



数值方法遇到的问题：

- 误差
  - 在某个时间戳不真实不准确，导致准确性降低
  - 误差在图形学不是至关重要的
- Instability
  - 误差被组合，导致**diverge**
  - 数值方法需要解决的最重要的问题



## Combating Instability

![image-20230607084842133](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607084842133.png)



### 中点法

- 计算欧拉方法到达的a
- 计算原始点和a点的中点b的导数来更新
- 使用b点的导数来更新原始的点，最后到达c点

![image-20230607091337657](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607091337657.png)



![image-20230607091533121](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607091533121.png)



### 调整步长

![image-20230607091826284](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607091826284.png)

- 不断二分，如果差距不大，就不需要再分时间
- 直到2分点和计算点差距不大



### 隐式欧拉方法

![image-20230607091919016](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607091919016.png)

- 用下一个时间戳的导数（位移使用下一个时间段速度，速度使用下一个时间段的加速度）
- 使用root-finding算法（例如牛顿法）
- 通常有着很好的稳定性



稳定性衡量：局部稳定性误差和每一步的误差的关系

- 隐式欧拉方法的全局误差是一阶的
- 阶越大越好

![image-20230607092351542](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607092351542.png)



### Runge-kutta families(隆格库塔方法)

![image-20230607092722267](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607092722267.png)

- 数值分析课程的内容



### Position-based/verlet integration(非物理)

![image-20230607092918648](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607092918648.png)

- 作业8会有详细细节
- 思想大概就是弹簧超过一定距离后变成劲度系数无限大的弹簧



## 刚体模拟

![image-20230607093057591](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607093057591.png)

- 考虑更多的物理量：旋转角度，速度，角速度，角动量，力矩



## 流体模拟

![image-20230607093512304](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607093512304.png)

- 水通过微笑的小球（rigid-body 刚体）模拟
- 水是不能被压缩的（密度恒定）
- 通过密度保持不变的性质，每个时间戳计算每个部分的密度，如果密度改变，修正粒子的位置
- 需要知道密度的梯度，梯度需要为0



## 拉格朗日方法

- 拉格朗日核心是质子，每一次只针对一个质子
- 欧拉方法（不是上面的欧拉方法），针对的是网格，计算网格中间的密度来考虑物质的变化

![image-20230607094136043](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607094136043.png)



## 物质点法MPM（混合方法）

![image-20230607094334843](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607094334843.png)

- 拉格朗日：考虑例子的材质属性
- 欧拉考虑网格密度
- 混合，例子将属性值添加到网格中，网格更新，并且将效果返回给例子



