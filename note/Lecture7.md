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

- $\vec v 和 \vec R$足够接近的时候 

![image-20230422104412309](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230422104412309.png)