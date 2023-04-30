# Lecture 7

## 课程内容

- 补充光栅化知识：深度测试
- 光照
- 基本着色
- 现代图形管线



## 可见性/遮挡

- 解决方法：**Z-buffering(深度缓存)**

### 绘制算法（画家算法）

- 由远及近绘制
- 覆盖远处的东西

![image-20230418224701829](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418224701829.png)

- 画家算法需要将物体按照深度进行排序
- 一定程度上画家算法可行：
  - 但是特殊情况不行：两两之间互相覆盖

![image-20230418225039596](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418225039596.png)

### Z-Buffering

- 线代广泛采用

- 思路：

  - 存储每一个像素最浅的深度（z-value）

  - 需要一个额外的图像存储像素的深度信息

    ![image-20230418225339519](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418225339519.png)

  - IMPORTANT：相机朝-z，所有前方的值，但是这里的深度z值是正的（单纯描述深度）

    ![image-20230418225511781](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418225511781.png)

- 案例：

  ![image-20230418225532628](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418225532628.png)

- 算法流程：

  - 初始化深度矩阵 = $\infin$
  - 将每个目标迭代：
    - 对于任何一个三角型，可以找到它的像素
    - 如果它的深度<记录的深度
      - 设置frame为当前像素的rgb
      - 设置z-level为当前深度

  ![image-20230418230022840](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418230022840.png)

- Z- buffer的复杂度：O(n)

  - 假设三角形不是特别大，所以假设扫描一个三角型的时间为常数时间
  - Z-buffer很好的性质：与顺序无关
  - 排序：浮点型与浮点型比较难度很大（深度完全一样怎么办？本门课不讨论）



## 复习

![image-20230418233514727](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418233514727.png)

- 模型变换
- 摄像机（view变换）
- 投影变换
- Rasterization

![image-20230418233830903](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418233830903.png)

即将学习：

- 着色（如何让图像更加真实）



## shading

- 定义：平行线或彩色块绘制的插图或图表变暗或着色。
- 本门课定义：不同的物体，运用不同的材质



### 最基础的着色模型（Blinn-Phong Reflectance Model)

- 三个茶杯有以下特征
  - 高光：（Specular highlights）
  - 漫反射：（Diffuse reflection）
  - 间接光照（Ambient lighting）

![image-20230418234402046](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418234402046.png)

### 定义

- 光照是有点光源产生（shading point）
- 观察者方向（单位向量）
- 法线方向（surface normal）（单位向量）
- 光照方向（light direction）（单位向量）
- 反射平面参数（颜色，shininess）

![image-20230418234709816](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418234709816.png)



- 着色不考虑阴影（任何点的着色情况不考虑其他物体的存在）（shading != shadow)

### Diffuse Reflection(漫反射)

- 光线被均匀反射到所有方向

  ![image-20230418235115260](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230418235115260.png)

- 多少光（energy）接收到？

  ![image-20230418235157676](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418235157676.png)

  - 以光线数量作为energy为例：（夹角决定了光线接受）
  - 接受到的能量$cos \theta = \vec I\cdot \vec n$

- 光衰减

  - 离中心近的球壳和中心远的球壳具有的能量相同
  - 定义中心光源强渡为I
  - 则远处的光源强度$I/r^2$

  ![image-20230418235804730](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230418235804730.png)

  

  

#### Lambertian (Diffuse) shading

- 定义：$L_d = k_d(I/r^2)max(0, n\cdot l)$(点乘为负数则没有光线进入平面)
- $k_d$:漫反射系数（黑色表面吸收所有光线($k_d=0$)
- 漫反射与观察者方向完全无关

![image-20230419000056766](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230419000056766.png)

![image-20230419000248723](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230419000248723.png)



# Lecture 8 

## Review

- Blinn-Phong的简单模型

  - Diffuse（漫反射）
  - Specular（本节课）
  - Ambient（本节课）

- 模型的所有向量假设在反射点出发，并且是单位向量

  ![image-20230422104136441](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230422104136441.png)



![image-20230422104316138](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230422104316138.png)

- 漫反射模型：
  - $k_d$：漫反射系数
  - 能量
  - 夹角余弦（单位向量使用点乘计算）



## 高光

- $\vec v 和 \vec R$足够接近的时候 （观察方向和其他时刻接近的时候）

![image-20230422104412309](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230422104412309.png)

- 推导：当v和R接近的时候，半程向量与发现接近

![image-20230423194049976](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230423194049976.png)

- 半程向量就是对角线方向的单位向量
  - 高光通常都是白的，所以$k_s$一般给出的是白色
  - 不考虑能量：因为blinn-phong模型是简化后的模型
  - 指数p：高光点必须要两者非常近，才认为是高光，所以我们希望两个夹角足够小，高光范围最够小（p通常选取100到200）

![image-20230423194542751](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423194542751.png)

![image-20230423194715954](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423194715954.png)



## 环境光照项（Ambient Term）

- 假设来自环境的光永远都是相同的（constant）

- 颜色都是相同的

  ![image-20230423194853536](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423194853536.png)

- 需要用全局光照的知识才能很好的处理全局光照



## Blinn-Phong

![image-20230423195024544](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423195024544.png)

## 着色频率（shading frequency）

- 着色频率：
  - 着色点如何选取？
  - 一个平面做一次shading/三个定点知道，通过插值/每一个像素上进行着色



- flat shading

![image-20230423195707934](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230423195707934.png)

- Grouand shading
  - 每一个三角形定点进行插值

![image-20230423195734128](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423195734128.png)

- Phone Shading

  - Compute full shading model at each pixel

  ![image-20230423195836751](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423195836751.png)

- 几何足够复杂的情况下，采用相对简单的模型也可以获得很好的效果

![image-20230423200125203](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423200125203.png)



## 法线向量

- 任何一个定点与多个三角形就行关联，定点的法线被定义为 **连接的三角形的法线的面积加权平均**

![image-20230423200513469](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423200513469.png)

- 重心坐标插值

![image-20230423200610490](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423200610490.png)



## 实时渲染管线

### 管线结构

![image-20230423201003707](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423201003707.png)

- 
- shading发生在顶点处理（grouand shading), fragment处理（phong shading)（shader）
- Texture mapping（质感）

![image-20230423201503696](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423201503696.png)



### shader

- 在vertex（vertex shader）和fragment processing stages（pixel shader）进行编程
- 描述对单个顶点的操作的描述
- GLSL 着色语言（最简单phong模型漫反射）

![image-20230423201736851](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423201736851.png)

![image-20230423202651926](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423202651926.png)

图形管线的硬件实现

![image-20230423202744270](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423202744270.png)

- 独立显卡，集成显卡
- GPU可以看作异构的多核处理器



## 纹理映射

- 两个不同的光源
- 任何一个点的属性可以被定义（$k_d$不同)

![image-20230423203323846](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423203323846.png)



- 声明：任何三维物体的表面都是二维的



![image-20230423203650869](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423203650869.png)

- 在物体上如何映射纹理？纹理图片映射到三角型的定点

- 几何重大的研究方向：parameters（参数化）



### 纹理坐标

- 纹理定义一个坐标系（u，v)
- (u,v)的范围u在[0,1]，v在[0,1]

![image-20230423204100062](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423204100062.png)

- 不同位置可以映射相同的纹理，纹理可以在物体表面运用多次

- tiled（wang）纹理的无缝衔接的合成

  ![image-20230423204244221](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230423204244221.png)



# Lecture 9

## 课程内容

- 重心坐标
- 纹理
- 纹理应用



## 重心坐标（插值）

![image-20230425201037945](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425201037945.png)

- 获得流畅的变换（在三角型内部）
- 详述每一个顶点的值

重心坐标：

$(x,y) = \alpha A + \beta B + \gamma C $

$\alpha + \beta + \gamma = 1$

![image-20230425201133150](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425201133150.png)

顶点自己的坐标：

$(\alpha, \beta, \gamma) = (1,0,0)$

$(x,y) = \alpha A + \beta B + \gamma C = A$



几何学解释：面积

![image-20230425201513368](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425201513368.png)

推导

![image-20230425201821574](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425201821574.png)

对三角型进行线性插值

（三个顶点的值计算出来后，可以计算出三角形内均匀的任意一个值）

![image-20230425202005288](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425202005288.png)

- 重心坐标并不是在投影中不变，所以需要现在三维空间中计算插值，再进行投影



## 应用纹理

对于每一个光栅化的屏幕：

- 任何一个点计算纹理坐标
- 对纹理进行采样（插值，得到颜色）

![image-20230425202313914](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425202313914.png)

## 纹理放大

邻近，双线性，双向三次插值

![image-20230425202520052](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425202520052.png)

- 邻近：取最近的一个像素的颜色

### 双线性插值

![image-20230425202810401](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425202810401.png)

- 线性插值$lerp(x,v_0,v_1)= v_0 + x(v_1 - v_0)$

![image-20230425202916398](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425202916398.png)

- 对两条边进行插值

![image-20230425202932121](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425202932121.png)

- 再对两条边插值的两个点在进行插值



### 纹理过大

![image-20230425232601242](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425232601242.png)

- 右边图片出现走样
- 远处一个纹理代表大片区域

![image-20230425232749774](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425232749774.png)

- 超采样可以解决，但是消耗过大



- 纹理过大其实是因为一个较大的纹理只通过一个像素点进行采样，采样频率过小
- 如果不进行采样？范围查询

![image-20230425233006258](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425233006258.png)



### Mipmap

- 进行范围查询
- 只能做 **近似**的 **方形**范围查询

![image-20230425233454816](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425233454816.png)

- 构建Mipmap（金字塔） 变为原来存储量的3/4倍

- 

![image-20230425233844987](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425233844987.png)

- 通过正方型近似得到纹理区域，正方形的边长可以通过微分进行计算

![image-20230425234017722](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425234017722.png)

- L=4 ，则它在金字塔上的第二层对应一个像素，

问题：不连续

![image-20230425234346065](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425234346065.png)



### 三线性插值

- 层与层之间进行一次插值

![image-20230425234432073](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425234432073.png)

![image-20230425234620963](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425234620963.png)

- 渐变效果很好

- 问题：mipmap远处及其模糊

![image-20230425234655203](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425234655203.png)

### Anisotropic Filter（各向异性过滤）

- Anisotropic Filter:不按照1：1的方式压缩，可以对应一个矩形区域，不一定是正方形区域

- EWA过滤，把不规则的形状拆解成圆形

![image-20230425235116657](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230425235116657.png)



### 