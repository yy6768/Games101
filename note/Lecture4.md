# Lecture 4

## 关于变换的补充

-  旋转$\theta$角和旋转-$\theta$角为逆

![image-20230412191005367](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412191005367.png)

- 逆矩阵= 转置矩阵为正交矩阵



## 课程内容

- 三维变换
- 观测变换
  - 视图变换
  - 投影变换
    - 正交投影
    - 透视投影



## 3维变换

- 使用齐次坐标：

![image-20230412191341583](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412191341583.png)



### 缩放

- $S(s_x,s_y,s_z):diag(s_x,s_y,s_z,1)$



### 平移

![image-20230412191557741](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412191557741.png)

### 旋转

![image-20230412191629568](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412191629568.png)

- 简单情况下是绕轴进行旋转
- $R_y$因为叉乘是负所以相反



复杂旋转

- 任意3D角度的旋转可以分解成简单旋转
- $R_{xyz} (\alpha,\beta,\gamma) = R_x(\alpha)R_{y}(\beta)R_z(\gamma)$

- $\alpha, \beta,\gamma$被称为欧拉角



#### Rodrigues’Rotation Formula

- $R(n,\alpha) = cos(\alpha)I + (1-cos(\alpha))nn^T + sin(\alpha)\begin{pmatrix}0 & -n_2 & n_y\\n_z & 0 & -n_x\\-n_y & n_x & 0\end{pmatrix}$

- 旋转轴n

- 证明：

[三维空间绕任意轴旋转矩阵的推导 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/56587491)

![51ab6dbdcf9232397fe6e6aca6a720b](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/51ab6dbdcf9232397fe6e6aca6a720b.jpg)

![d6c8dfee533c5eeede00e9d8bfeab3f](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/d6c8dfee533c5eeede00e9d8bfeab3f.jpg)

## Viewing transformation

- 视图变换

- 拍照片：
  - 选址+安排人（model transformation）
  - 选好角度（view transformation）
  - 茄子（projection transformation)



- 如何确定相机的摆放：
  - Position：$\vec e$
  - Look-at :$\hat g$
  - up-direction :$\hat t$

![image-20230412193156732](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412193156732.png)

- key observation
  - 当照片和所有物体一起移动，照片是相同的（不变换的）
- 约定俗成：相机一定在原点，沿着-z看，up-direction :z

![image-20230412193346600](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412193346600.png)

### 相机变换:star:

- $M_{view}= R_{view}T_{view}$
- 使用逆矩阵先计算出$R^{-1}_{view}$，因为由x轴旋转到g$\times$t 比$g\times t->X$轴更好算

![image-20230412193836130](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412193836130.png)



## Projection Transformation

- 透视投影与正交投影最大的区别是是否有近大远小的性质
- 透视投影连接相机为四棱锥
- 正交投影投影在某个平面

![image-20230412194447609](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412194447609.png)

#### Orthographic projection（正交投影）

- 假设相机朝向-Z，Y是向上方向
- 正交投影丢弃Z坐标直接得到投影
- 对投影的大小重新缩放[-1,1]

![image-20230412194644983](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412194644983.png)

- 通常
  - 我们将一个cuboid $[l,r]\times[b,t]\times[f,n]$变换成“canonical(正则)”的正方体
    - l,r 定义左和右
    - b,t 定义底部和上
    - z轴定义“远和近”（右手系）
    - opengl是左手系，得到“近和远”
    - 先平移再缩放 
  - Transformation matrix
  - ![image-20230412195206315](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412195206315.png)

#### Perspective projection（透视投影）

- 在图形学、艺术、Visual system中非常常用
- 远处的物体会变得很小
- 实际平行线在投影面不是平行的（Euclid was wrong?)



- 齐次坐标中的最后一维一般为1，如果不为1，其他维度同时除以最后一维，表示的点相同



- 怎么做透视投影
  - 收缩远平面
  - 再做正交投影
- 做法的几个性质
  - 近平面不变
  - 中心点不变

![image-20230412200215472](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412200215472.png)



- 挤压（收缩）

  - 通过相似三角形可以计算$y' = \frac n z y$

  ![image-20230412200609472](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412200609472.png)

![image-20230412201051885](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412201051885.png)

利用上述齐次坐标性质，就可以确认3行的结果

![image-20230412201804044](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412201804044.png)



对于z坐标

- 任何点在近平面z不会发生变化
- 任何点在远平面z不会发生变化

近屏幕所以变换矩阵的第三行一定满足（00AB）的形式

![image-20230412202237265](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412202237265.png)

![image-20230412202443644](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412202443644.png)

- 根据远平面的中心点也可以构成一个方程（f，1）中心点不变

![image-20230412202530728](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230412202530728.png)



### 思考题

