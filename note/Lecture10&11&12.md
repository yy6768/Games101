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
