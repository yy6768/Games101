# Lecture 3

- Assignment 0 
- 不能使用qq邮箱注册



## Last lecture

- 向量
- 点乘
- 叉乘
- 矩阵



## Topic

- 变换
  - 为什么要变化
  - 2D变换
  - 齐次坐标
  - 计算变换
  - 3维变换



## 为什么要学习变换

- 相机
- 计算机动画
- 光栅化



## 变换

### 缩放

![image-20230324221119350](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324221119350.png)

$\begin{bmatrix}x'\\y' \end{bmatrix} =\begin{bmatrix}s& 0\\0 &s \end{bmatrix}\begin{bmatrix}x'\\y' \end{bmatrix} $





![image-20230324221302643](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324221302643.png)





### 反射

![image-20230324221415492](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324221415492.png)



### 切变

![image-20230324221708934](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230324221708934.png)



### 旋转

![image-20230324221905282](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324221905282.png)



- 推导：

  - 前后坐标关系

    $\begin{bmatrix} x'\\y'\end{bmatrix} = A \begin{bmatrix} x\\y\end{bmatrix}$

  - 求解A的方程组（4个未知数）

- 旋转举证的结果性质：
  - 正交矩阵 $R_{-\theta} = R_{\theta}	^T$

## 齐次坐标（**Homogeneous coordinates**）

### 为什么需要齐次坐标？

- 考虑特殊的变换（平移），**平移无法使用矩阵的表示法**：

  $\begin{bmatrix} x'\\y'\end{bmatrix} = \begin{bmatrix} a & b\\c & d\end{bmatrix}\begin{bmatrix} x\\y\end{bmatrix} + \begin{bmatrix} t_x\\t_y\end{bmatrix}$

- 不希望平移变化表示为特殊的变换



## 引入齐次坐标

- 定义：将2维点中添加一个维度
- 平移变换可以表示为如下形式

![image-20230324223158693](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324223158693.png)

- 验证操作的合法性：

  ![image-20230324223503257](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324223503257.png)

![image-20230324223644219](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324223644219.png)

- 定义：$\begin{pmatrix}x\\y\\w\end{pmatrix} = \begin{pmatrix}\frac x w\\\frac y w\\1\end{pmatrix} $

- 两点相加为中点



## 常见变换

![image-20230324223908982](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324223908982.png)



### 逆变换

- $M^{-1}$表示新位置将变换变回原位置
- ![image-20230324224109108](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324224109108.png)



## 变换的组合

![image-20230324224437566](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324224437566.png)

- 操作不满足交换律
- 操作满足结合律——可以实现将矩阵相乘（复杂的变换也可以合成单个的变换）



## 变换的分解

- 示例：以任意点为中心进行旋转
  - 先沿着中心点反方向移动
  - 旋转
  - 回到旋转中心点

![image-20230324225011938](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324225011938.png)



## 三维变换

![image-20230324225248786](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324225248786.png)

![image-20230324225259534](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230324225259534.png)

- 先应用变换再平移