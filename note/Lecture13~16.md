# Lecture 13 光线追踪

## 课程内容

- whitted-style Ray tracing



## 为什么需要光线追踪

- 光栅化（Rasterization）处理**全局**效果并不好，尤其是多次反射的部分
  - 很难处理软阴影
  - Glossy reflection（光面反射）（反射不止一次）
  - 间接光照（反射不止一次）
- 光栅化性能（效率）很高，但是质量很低

- 光线追踪比较准确，但是比较慢
  - 一般用于离线应用
  - 10K cpu小时来渲染一帧图

![image-20230505130453166](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505130453166.png)



## 光线

本门课对光线的定义：（不等于物理课对光线的定义）

- 光线沿着直线传播
- 光线和光线不会发生碰撞
- 光路可逆性：从光源不断经过反射折射到观察者视觉内，从视觉内发出一道光也能反向到达光源



![image-20230505130846390](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505130846390.png)



## 光线投射

- 光线投射：

  - 从相机视角生成图像（投射”光线“）
  - 检查每个像素能不能到达光源

  ![image-20230505131557532](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505131557532.png)



案例

- eye ray：相机投射出来的“光线”

- shadow ray：从物体上的点投射到广元的线

  ![image-20230505131906960](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505131906960.png)



## 递归（Whitted-Style） Ray tracing

![image-20230505132043832](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505132043832.png)

- 递归
  - 当年需要渲染74min
  - 现代PC 6s
  - GPU 1/30s



### 算法流程

- 因为会发生折射，所以折射点也作为光源计算新的弹射的点到光源的值
- 最后把递归到的所有值加到image plane(投影面)的像素上

![image-20230505132421209](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505132421209.png)

- secondary ray：弹射光线
- shadow ray ：照向光源

![image-20230505135403499](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505135403499.png)



## 光线和物体表面的交点

### 数学上的光线（ray equation）

- $r(t) = o + td ~ 0 \le t \lt \infin$
- 球面（sphere): $p:(p-c)^2 - R^2 = 0$

![image-20230505140919744](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505140919744.png)

- 解方程：$(o+td - c)^2 - R^2= 0$

- 解t的方程，得到：

![image-20230505141108632](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505141108632.png)



### 推广：与隐式表面的交点

![image-20230505141204436](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505141204436.png)

通过数值优化的方法求值



### 与显示表面求交点

![image-20230505141831202](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505141831202.png)

- 在平面上点一个点，其作为光源
  - 与平面交点有奇数个点：在内部
  - 有偶数个点：在外部

- 朴素的想法：
  - 每一个三角型计算一次（效率极低）



### 三角形和平面求交点

- 简化成平面求交
- 平面的数学定义
  - $p:(p-p')\cdot N = 0$

![image-20230505142157570](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505142157570.png)

- 求解

![image-20230505142357932](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505142357932.png)

- 得到光线和平面的交点，再检查是否在三角形内



### Moller Trumbore算法

- 通过重心坐标法求得平面内的点

- 三个变量可以通过三维线性方程组

![image-20230505142517457](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505142517457.png)



## 挑战

- 如果光线与每个物体有交点？那么计算很慢
- 平面多个像素每个都要检查

![image-20230505142912331](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505142912331.png)



## 光线追踪加速

### 包围盒（bounding volumes）

![image-20230505143113333](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505143113333.png)

- 使用相对简单的物体将复杂物体包围起来
- 光线如果不能碰到包围的物体，就更不需要考虑内部



AABB（轴对齐包围盒）

![image-20230505143355503](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505143355503.png)



### 判定光线与AABB求交

#### 二维

- 从o传播，无论如何可以求出对面进入的时间和出去的时间
- 可能得到负数时间
- 最终结果需要将2维的tmin和tmax的时间求出交集

![image-20230505143730188](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505143730188.png)

#### 三维

![image-20230505144213000](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505144213000.png)

- 求出交点，并且每一对都求出tmin和tmax
- 进入时间tenter  = min{tmin} 出去时间texit=min{tmax}
- 如果进入时间小于离开，那么光线于时间有交点

- 如果离开时间texit<0 那么就说明与光线没有交点

- 如果t_exit >=0 and t_enter <0
  - 点光源在盒子内部

![image-20230505144430569](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505144430569.png)



#### 为什么需要 Axis -Aligned

![image-20230505144543720](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230505144543720.png)





# Lecture 14 光线追踪（2）

## 课前公告

分享一些技术博客

- DLSS 技术（视频超分辨）
- RTXGI（实时全局光照技术）

- 之后的课程难度升高



## Review

- 为什么需要光线追踪
- whitted-style ray tracing
- Ray - object intersaction
- AABB（Axis-Aligned Bounding Box)

![image-20230506130344535](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506130344535.png)



## 课程内容

- 均匀格子（Uniform grids）
- 空间划分（Spatial partition）



## Uniform  grids

-  找出场景的包围盒

- 创建盒子（将包围盒进行划分）

- 场景预处理（存储与物体相交的格子）

   左上角第二格错了没涂颜色

![image-20230506130737869](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506130737869.png)

- 判断光线是否和盒子相交
  - 如何光栅化一条线（bresenham算法)
- 判断光线是否和物体相交

![image-20230506130830937](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506130830937.png)

- 格子不能太稀疏，也不能太密集

  ![image-20230506131350726](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506131350726.png)



### Teapot in a stadium

场景过大，物体过小，分布不均匀，不适合使用格子算法

![image-20230506131511896](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506131511896.png)



## 空间划分

- Oct-Tree （八叉树）（二维-四叉树）

- **KD-tree**

  每一次划分只沿着某一个轴进行划分，交替进行划分

- BSP-Tree

  对空间进行划分，每一次选择一个方向（不是沿正交方向上划分，计算量大）

![image-20230506131746951](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506131746951.png)

### KD树

- 预处理
  - 交替进行划分：第一次数值划分得到蓝色区域1，第二次沿水平方向得到2…… 

![image-20230506132450677](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506132450677.png)

- 数据结构

  - 内部节点沿轴划分（x,y,z)
  - 划分的位置（需要特殊的位置选取，有不同的技巧）
  - 存储两个子区域的指针（不存储实际的物体）
  - 叶子节点存储一系列物体

  ![image-20230506132613894](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506132613894.png)

- 光线追踪

  - 发现与A有交点

  - 发现与1号区域有交点（仅供演示），1号区域仍要递归

    ![image-20230506133200966](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506133200966.png)

  - 发现B也有交点，然后需要和左右节点都进行判断

  - 发现与3有交点，3是叶子节点，需要与所有物体进行判断



- KD树的问题很多

  - KD树的建立很难，需要考虑三角形与格子求交（难）

  - 在kd树描绘的边界的物体与多个kd树的多个叶子节点都有交际，也就是kd树的边界的物体有大量的冗余存储

    ![image-20230506133802206](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506133802206.png)

    

## Object Partitions &Bounding volume Hierarchy（BVH）

- 把物体划分成多个部分
- 对新生成的划分求包围盒（不会出现三角形需要和包围盒求交）

![image-20230506134319783](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506134319783.png)



如何划分节点？

- 选择不同的维度
- 技巧1：每次沿长度最长的轴就行划分
- 技巧2：取中值的那个物体（如果区域内有n个三角形，我们找到n/2的三角形的坐标作为划分的轴的值）O(n)

- 中止个数：当区域内只有不超过指定的个数的物体就停止划分

![image-20230506134919906](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506134919906.png)

![image-20230506134957928](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506134957928.png)

对比

![image-20230506135138468](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506135138468.png)



## 辐射度量学（radiometry）

- 在作业3中，光线强度为10，10是什么
- 作业5中，有人会觉得这是真实的照片吗？
- PBR：（**Physically Based Rendering)** 基于物理的渲染

![image-20230506135552020](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506135552020.png)

- 定义：
  - 定义了光照的度量属性（4个物理量）
    - Radiant flux
    - intensity
    - irradiance
    - radiance

![image-20230506135905426](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506135905426.png)

### Radiant Energy & Flux

- 定义：
  - Radiant Energy：光源/辐射的能量（以焦耳为单位）
  - Radiant flux ：单位时间的光源的能量（Watt/lumen（光学常用）为单位）

![image-20230506140207079](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506140207079.png)

flux的另一解释- 单位时间光子通过传感器的数量

### 其他

![image-20230506140543252](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506140543252.png)

#### radiant intensity

- 定义：每立体角光源发射出的power/flux 

![image-20230506140714610](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506140714610.png)

- 立体角的定义：
  - 类比平面角
    - $\theta = \frac l r$
    - 圆周角：$2\pi$
  - ![image-20230506140926216](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506140926216.png)
  - $\Omega = \frac A {r^2}$
  - 球体立体角$4\pi$
- 单位立体角

![image-20230506141155592](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506141155592.png)

- 对于均匀的点光源
  - 直接除以$4\pi$得到intensity

![image-20230506141407341](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230506141407341.png)