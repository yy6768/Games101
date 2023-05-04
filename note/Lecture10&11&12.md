# Lecture 10 几何（1）

## 课程内容

- 纹理的应用（环境光照）
- 几何（基本表示）



## 纹理应用

- 在现代GPU的加持下，纹理=内存+范围查询（过滤）



### Environment map

![image-20230426134744148](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426134744148.png)

- 把环境作为贴图进行反射（左边环境贴图，右边茶壶反射环境光）

- 纹理用来反射环境光

- 犹他茶壶（类似Stanford 兔子和stanford龙，以及康奈尔盒子）

#### Spherical Map

球体记录环境光照

#### CubeMap

- 描述整个球的位置会出现在极点扭曲的问题

![image-20230426135614030](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426135614030.png)

- 所以就出现了Cube Map

  ![image-20230426135651381](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426135651381.png)

- 一个立方体有六个表面，切都是均匀的，所以很少有扭曲的情况发生
- 但是计算量变得更大，原有的球形可以迅速地求出光线方向向量，而Cube Map需要先找到记载在正方形的哪一个面



### 凹凸贴图

- 定义物体表面上的高度
- 高度改变->法线方向发生变换->产生光线反射发生变换

![image-20230426135951387](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426135951387.png)



#### 法线贴图

- 定义一个纹理，对任何三角形的法线进行扰动
- 通过高度变换来定义法线

![image-20230426140336899](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426140336899.png)

- 计算导数->垂直方向法线向量

![image-20230426140736311](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426140736311.png)

##### 3D情况

- 对两个方向求导，在计算倒数，然后归一化

![image-20230426141038341](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426141038341.png)

#### 位移贴图

- 位移贴图，对定点位置进行修改

- 凹凸贴图没有改变几何，所以在边缘处与原图完全一致）
- 位移贴图需要三角型粒度足够细（定点间的间隔<纹理之间的高度变化）

![image-20230426143226884](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426143226884.png)



### 3D Procedural Noise + Solid Modeling

3维纹理+ 固体模型

- 没有实际纹理图，但是定义了三维纹理噪声的函数

![image-20230426144105103](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426144105103.png)

### 阴影

通过纹理贴图进行阴影计算

![image-20230426144538789](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426144538789.png)



## 几何

### 基本表述法

- 隐式几何

- 显示几何



#### 隐式几何

- 基于对点进行分类

  - points满足某些特定的关系
  - 例如球：$x^2+y^2+z^2 = 1$

  ![image-20230426145637232](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426145637232.png)

- 隐式几何的优缺点
  - 可以快速判断点是否在某个表面上
  - 难以遍历表面上的所有店



#### 显示几何

- 通过参数映射的方式将定义
  - 二维的所有点定义为3维表面

![image-20230426150047623](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426150047623.png)

- 显示几何难以判断点是否在面上
- 但是可以遍历范围的点来快速遍历所有表面

![image-20230426150257566](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426150257566.png)



### 隐式表示方法

#### 数学公式表示

- 不直观

![image-20230426150513004](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426150513004.png)

#### CSG（构造固体几何法）

- 通过Boolean运算结合隐式几何

![image-20230426150805378](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426150805378.png)



#### 距离函数（distance function）

- 距离函数：空间中的任意一个点到目标平面的最小的距离（可以是负的）

  ![image-20230426151022270](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426151022270.png)

  ![image-20230426151052583](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426151052583.png)

- 混合边界
- 计算A的距离函数SDF（A)，计算B的距离函数SDF（B)，blend计算出混合的SDF函数



#### Level Set Methods

- 距离函数有时不方便用数学表达
- 水平集的思想完全一致，但通过离散的格子表达距离

- 有点像等高线

![image-20230426151721498](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426151721498.png)

#### 分型集合（Fractals）

![image-20230426152103022](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230426152103022.png)





# Lecture 11 几何（2）

## 课程内容

- **曲线**和曲面
- Bezier曲线
- De Casterljau‘算法
- B-splines



## 显示几何

### 点云

- 最简单的方法：给定一个点（x,y,z)列表，只要我给的点够多，物体的表面就越细节
- 但是对于复杂的模型，需要的点非常多

![image-20230502122446683](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502122446683.png)



### Polygon Mesh

- 存储三角形或者多边形来构成平面
- 容易处理/模拟
- 相对比较复杂
- 可能是当前使用的最多的方法

![image-20230502122644363](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502122644363.png)



### Wavefront object file（.obj) 文件类型

![image-20230502122934227](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502122934227.png)

- 定义点和三角形
- 法线方向
- f来定义三角形的连接关系



## 曲线

- 例子
  - 高新园区宣传片的相机运动曲线
  - Maya建模软件的物体动画运动曲线
  - 字体的曲线



### Bezier Curve

用一系列的控制点来描述曲线的走向

![image-20230502123419640](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502123419640.png)



#### de Casteljau

- 给出时间t，时间t在[0,1]范围内，
- de Casteljau算法描述了一个方法绘制曲线
  - 3个点形成两条线段，根据时间t找到$b_0^1$,$b_1^1$

![image-20230502150511540](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502150511540.png)

![image-20230502151626655](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502151626655.png)



- 递归调用

  ![image-20230502150624877](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502150624877.png)

![image-20230502151931907](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502151931907.png)



#### Bezier Curve代数公式

![image-20230502152357706](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502152357706.png)

### 

- 下列曲线可以得到一个n阶berizer曲线

  - bernstein多项式

  ![image-20230502152457471](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502152457471.png)

  - 可以在空间中生成bezier曲线

    

![image-20230502152636809](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230502152636809.png)



#### 贝塞尔曲线的性质

1. 规定了必须过起点和终点
2. 起始方向确定：在3维的空间如下
3. 在仿射变换下，可以直接对不同的定点做仿射变换，得到的beizier曲线一致（投影不行）
4. 凸包性质：无论如何不能超过凸包（给定控制点形成的凸包）

![image-20230502153109934](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502153109934.png)

![image-20230502153406062](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502153406062.png)



#### piecewise bezier curve曲线

![image-20230502153537955](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502153537955.png)

- 一次用多个点很难控制beizier曲线

- 我们可以逐段的定义berizer曲线

  - 通常4个点可以得到一段beizier曲线
  - photoshop的钢笔工具的原理

  ![image-20230502153623880](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502153623880.png)

- 连续性

  c0连续：两个线的端点在一起（点挨在一起）

  c1连续：c0连续的基础上切线方向相反

  高阶连续（曲率连续）

  ![image-20230502154135800](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502154135800.png)

### 其他曲线

#### 样条

由一系列的控制点定义的连续的曲线，导数也是连续的

![image-20230502154449917](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502154449917.png)

#### B-样条

- 基样条
- 有局部性

![image-20230502154617951](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502154617951.png)

#### 更多

![image-20230502154741147](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502154741147.png)

## 曲面

Bezier 曲面

![image-20230502154920341](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502154920341.png)

![image-20230502155123518](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502155123518.png)

- 4*4的控制点得到的（类似于二插值一样的）
  - 水平方向扫描得到一条bezier曲线，
  - 竖直方向上选取这四个bezier曲线上的点做为新的控制点然后绘制出竖直方向的bezier曲线，最后形成曲面

![image-20230502155239470](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230502155239470.png)

更多可以参考高阶的课程

## Mesh Operations：集合处理

- Mesh subdivision（网格的细分）
- Mesh simplification 
- Mesh regularzation



# Lecture 12 

## 小插曲

2019年图灵奖

![image-20230503182222956](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230503182222956.png)



## 三角形面的处理方法

### 三角形划分

![image-20230504121950689](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504121950689.png)

### 三角形简化

![image-20230504122039976](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504122039976.png)

## loop subdivision

- 引入更多三角形
- 改变这些三角形的位置

![image-20230504122354004](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504122354004.png)

步骤：

- 将三角形划分成四个
- 将新的定点变换到新的位置（update步（1））

![image-20230504122809523](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504122809523.png)

- 将老的定点变换到新的位置：

  ![image-20230504122931624](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504122931624.png)

  - n是定点度（定点连接的边的数量）



### Catmull-Clark subdivision

- Non-quad face：非四边型面

- Extraordinary vertex（奇异点）：度不是4

  ![image-20230504123606716](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504123606716.png)

- 问题

  - 经过一次细分之后，还有多少个奇异点呢？奇异点数会增加
  - 它们的度如何变化？
  - 有多少非四边形面？全部非四边形面消失，增加奇异点数

不要求掌握：更新规则：

上半部分：新的点分成面点和边点进行更新

下半部分：老的点进行更新

![image-20230504124451552](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504124451552.png)



## 三角形面简化

### 边坍缩（Collapsing an edge)

![image-20230504125540289](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504125540289.png)

#### 二次误差度量（Quadric Error Metrics）

![image-20230504125637667](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504125637667.png)

- 求平均得到左边的图（问题在于新的三角形远小于原来的图形）
- 二次误差度量：寻找一个点，它到上半部分3个点的平方距离最小

![image-20230504130046034](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504130046034.png)

- 对一条边进行坍缩，会影响其他边
- 先取最小的二次误差，并且动态更新最小的值

局部最优=全局最优？

- 贪心思想：局部最优的二次度量误差已经有较好的效果

![image-20230504130214101](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504130214101.png)





## 光线追踪

### shadow mapping

- 图像空间的算法
- 核心思想：如果点不在阴影里，那么点必须在光源照亮且被相机照到
- 本课程只涉及点光源（阴影生成会有明显的边界）

#### 步骤

- 从光源开始渲染

![image-20230504131020773](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504131020773.png)

- 从摄像机方向观察

  ![image-20230504131537215](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504131537215.png)

- 将摄像机看到的点投影回光源

![image-20230504131114988](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504131114988.png)

### 案例

![image-20230504131916924](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504131916924.png)

- 从光源看向场景（其实不考虑着色）

- 记录深度图

  ![image-20230504131957683](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504131957683.png)

- 从相机观察，并且投影回光源

![image-20230504132029848](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504132029848.png)

- 图像为什么这么脏（dirty）？

  shadow mapping本身存在的问题：

  - 浮点数的精度问题
  - shadow-map本身也需要分辨率
  - 需要做两次渲染
  - 只能做硬阴影

- 解决方法：

  - 比较最值
  - 添加bias（偏差）

![image-20230504133253987](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504133253987.png)

#### 硬阴影和软阴影

![image-20230504133500516](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504133500516.png)

- 软阴影：有过度的阴影，越靠近物体越硬，越靠近边界越软
- 本影，半影（Umbra，Pernumbra）

![image-20230504133642517](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230504133642517.png)
