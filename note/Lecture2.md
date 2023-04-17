# Lecture2

对应虎书 Ch2 和Ch5

课程分为4块：

- 光栅化

- 几何

- 光线传播和追踪

- 动画



## 课程内容

- 线性代数
  - 向量
  - 矩阵



## 向量

- notation:$\vec a \ or $ **a** or $\vec {AB} = B - A(起始点和终止点)$
- 属性：
  - 方向
  - 长度

### 向量的Normalization

- 长度（Magnitude） ：$||\vec a||$
- 单位长度：
  - 长度为1的向量
  - $\hat a = \vec a / ||\vec a||$
  - 通常表达方向



### 基本操作

- 向量求和
  - 图形上的解释
    - 平行四边形法则
    - 三角形法则
  - 代数解释
    - 坐标的相加
    - 笛卡尔坐标系

- 向量转置$A^T$

- dot product(点乘)：

  - $\vec a \cdot \vec b = ||\vec a||||\vec b || cos\theta (cos\theta 是两者夹角)$
  - 点乘产生的是一个数
  - 点乘可以用于求夹角
  - 交换律、结合律、分配律
  - $\vec a \cdot \vec b = \begin{pmatrix} x_a\\y_a \end{pmatrix} \cdot \begin{pmatrix} x_b\\y_b \end{pmatrix} = x_ax_b + y_ay_b$

- projection（投影）：

  - notation:$\vec b_{\perp} : $projection of b onto a
  - $\vec b_{\perp} = \vec b * cos\theta$
  - 可以分解向量到两个方向
  - 测量两个向量的方向是接近/原理
  - 给定一个方向可以看出方向是接近还是远离

- cross product（叉乘）：

  - 结果是向量且与原向量都正交

  - 结果方向又右手螺旋定则决定

  - $a \times b = -b \times a$

  - $|| a \times b ||  = ||a||||b|| sin\phi$

  - ![image-20230305233422599](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230305233422599.png)

  - 本课程假设使用右手系

  - 满足分配律和结合律

  - 代数表达：

    - 坐标表达
    - 矩阵形式表达

    ![image-20230305233619348](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230305233619348.png)

    - 用于判断向量的左右侧

    - 点在封闭图形的内测还是外侧

      （如果在内测，点P都在向量同侧，外部则至少一个向量左右侧不同）

      ![image-20230305233943294](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230305233943294.png)

- 正交基

  - 满足：
    - 单位向量
    - 两两正交
    - 可以把向量分解成多个方向（投影）



## 矩阵

- 定义：数组

- 矩阵乘积：mn维与np维才能乘（第一个矩阵列数与第二个矩阵行数）

  - 没有交换律
  - 结合律分配律都有（结合律比较有用）
  - 向量作为m*1的矩阵
  - $（AB）^T = B^TA^T$
  - 单位矩阵：对角线为，其余数为0 $I$
  - 矩阵的逆：$AA^{-1} = A^{-1}A = I$
    - $(AB)^{-1} = B^{-1}A^{-1}$

- 点乘/叉乘矩阵形式

  ![image-20230305235125977](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230305235125977.png)

  - a的转置×b就是点乘
  - 叉乘则需要先求伴随矩阵