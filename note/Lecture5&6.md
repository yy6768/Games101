# Lecture 5& 6 光栅化

## Review

- 观测变换
  - 视图变换
  - 投影变换
    - 正交变换
    - 透视变换



## 课程内容

![image-20230415135237416](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415135237416.png)

- 视口变换
- 光栅化（Rasterzation）
  - 不同的光栅呈现
  - 三角型的光栅化

- 遮挡和可见性（下节课）



## 透视投影复习

![image-20230415135426010](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415135426010.png)

- 定义宽高比：Aspect ratio = width / height

- field-of-view 垂直可视角度
- 水平可视角度



![image-20230415135727035](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415135727035.png)

- 夹角：垂直可视角度/2
- 相机定义朝向-Z方向，所以距离是|n|



### MVP

![image-20230415135939423](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415135939423.png)

- 最终绘制在**屏幕（screen)**

- 屏幕的定义：
  - 像素二维数组
  - Size of the array:resolution（分辨率）
  - 屏幕光栅成像设备（Raster display)
    - 源于德语，把东西画在屏幕上就叫做光栅化
- Pixel：像素
  - 像素一般很复杂，但是在本门课固定为某种颜色，像素格内颜色无差异
  - 颜色可以有RGB三个值来表示（红绿蓝）

- 屏幕空间：（与虎书不同）

  - 像素坐标（x,y) x和y都是整数坐标

  ![image-20230415140358462](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415140358462.png)

  - 像素中心：(x+0.5,y+0.5)
  - 整个空间(0,0)到（width,height)
  - z无关
  - 将$[-1,1]^2-> [0,width] \times [0.height]$
  - viewport transform matrix(视口变换)

![image-20230415142509372](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415142509372.png)



## 绘制机器

- 屏幕绘制
- 激光切割金属



### 光栅显示设备 

- Oscilloscope（示波器）
  - 成像原理：阴极射线管
  - CRT

- 显示器
  - 显存/内存映射到屏幕上
- LCD（平板显示设备）
  - LCD，OLED（高分辨率）
  - 液晶显示，经过液晶扭曲成为水平成像



## 光栅化

### 三角形光栅化

- 任何其他图形可以拆解成三角形
- 三角形内部是平面的
- 三角形内外定义清晰，可以用叉乘判断内外
- 三角形只要定义定点的数值属性，可以使内部点“渐变”，（三角形插值）





在经过视口变换之后，我们知道了三角形变换到屏幕空间的位置，那么像素该如何显示呢？如何判断像素和三角形呢？

![image-20230415144633131](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415144633131.png)





### 采样

- 采样：把函数离散化的过程
- 给定x，多次求出f(x)
- 对像素空间进行采样

![image-20230415144759382](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415144759382.png)



#### 案例

- 问题给出三角形，判断像素中心是否在三角形内

- 定义函数inside(tri,x,y)在三角形内

- 然后运用采样

```c++
for (int x = 0;x < xmax;++x)
	for (int y = 0;y < ymax;++y)
	image[x][y] = inside(tri,x+0.5,y+0.5);
```

![image-20230415144906664](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415144906664.png)



如何定义inside函数，利用叉乘

![image-20230415145201330](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415145201330.png)



#### Edge cases

- 当点在边上的情况上不做处理/作为特殊情况处理
- 本门课不做处理
- OpenGL：top-left rule（左边和上边处理）

![image-20230415145410829](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415145410829.png)



### 加速方法

减小遍历范围

- 获得三个顶点的范围[x1,x2], [y1,y2]
- AABB规则

![image-20230415145608353](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415145608353.png)

其他优化方法

- 对三角形覆盖的所有区域，只找每一行的最左和最右的区域
- 适用于窄长三角形

![image-20230415145828710](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415145828710.png)



### 实际光栅化

![image-20230415150117291](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415150117291.png)

- 绿色居多，红蓝的比例差不多
  - 人眼对绿色最为敏感



### 

### 问题

- 采样中心点结果：

![image-20230415150415820](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415150415820.png)

- 实际情况：

![image-20230415150424864](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415150424864.png)

- 理想情况：

![image-20230415150714642](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230415150714642.png)



- 引入了锯齿：下节课介绍





## Antialiasing（反走样/抗锯齿）

### 采样理论

- aliasing：锯齿/走样
- 分辨率：像素组成
- 照片：感光元件获得的信息离散到相机的投影空间

![image-20230417222731196](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417222731196.png)

- artifact（瑕疵）：Error/Mistakes/Inaccuracy

  - 锯齿是一种artifact

  - Moire Pattern

    ![image-20230417223104702](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417223104702.png)

  -  Wagon wheel effect（时间采样问题）

    ![image-20230417223759035](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417223759035.png)



- 信号变换太快，导致无法完整采样->Artifact



## Antialiased Sampling

- pre-filter:采样前先模糊化
- 再采样

![image-20230417224059732](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417224059732.png)

- 先采样，在进行filter，不能得到正确的结果

![image-20230417224302285](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417224302285.png)



### why

- 怎么理解进行模糊操作来进行反走样
- 为什么先采样在做filter不能得到正常结果？



### 频域（frequency domain)

![image-20230417224553107](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417224553107.png)

- 调整x前的系数，得到不同频率的余弦波：$f = 1 / T(周期)$



### 傅里叶级数变换

#### 傅里叶展开

- 任何函数的某个区间可以写作正弦和余弦函数的组合再添加一个常数项

![image-20230417225105714](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417225105714.png)



#### 傅里叶变换

- 给定任何一个函数，通过复杂的函数变换变为另外一个函数，也可以通过一个逆变换变为原来的函数：傅里叶变换/逆傅里叶变换
- 傅里叶函数在这里的目的就是提取函数对应的不同的频段

![image-20230417225223817](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417225223817.png)

![image-20230417225511090](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417225511090.png)

- f1到f5频率逐渐降低，同等频率的采样，采样f1可以大致复原f1,f5很难复原
- 走样的正式定义：对于两个频率不同的函数，通过相同频率的采样，得到的结果一致

![image-20230417225714316](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417225714316.png)



### 滤波

- 傅里叶变化可以把函数从实域变化为频域

  - 右边可以发现低频信息集中在中间，高频信息集中在外围

  ![image-20230417225915075](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417225915075.png)

- 滤波：去掉一些特定的频率

  - 通过对上述右图进行过滤（抹除低频率信号）得到下方右图，再通过逆傅里叶变换得到下左图，高频一般储存边界信息

  ![image-20230417230116604](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417230116604.png)

  - 只留下低频信息（低通滤波器）

    ![image-20230417230322836](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417230322836.png)

  - 去除高频信息和低频信息，保留某段中频信息

    ![image-20230417230451527](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417230451527.png)

- 滤波=卷积=平均

  - 卷积：对滤波器作点乘，实际上是对邻居的加权平均

    ![image-20230417230651729](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417230651729.png)

    ![image-20230417230740826](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417230740826.png)



#### 卷积

![image-20230417230907007](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417230907007.png)

- 在实域作卷积=在频域作乘积

  ![image-20230417231112879](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417231112879.png)



#### 滤波器（Box Filter）

- 上述卷积 1/9 $1^{3*3}$的矩阵实际上就是把每个像素变成周围像素的平均值，相当于把图片模糊化
- 越大box的模糊（极限情况下，box为一个像素，那么相当于没有做filter)



### Sample = Repeating Frequency Contents

- 什么叫采样：乘上只在固定值上为1的函数（冲击函数）
  - 左实域，右频域
  - 采样相当于重复原始信号的频谱（关注右3图）

![image-20230417231639626](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417231639626.png)

- 采样的越稀疏，搬移的过于密集，导致频谱混叠，最后就发生了走样

  ![image-20230417232000224](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417232000224.png)

### 反走样

- 增加采样率

  - 频谱搬移间隔增大，减少频谱偏移的重叠，最终做到反走样

- 先做模糊，再做采样

  - 模糊：低通滤波，去除高频滤波

  - 导致截断了频谱重叠的部分

    ![image-20230417232247412](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417232247412.png)

  - solution：

    - 卷积：f(x,y) 使用1-pixel的box进行卷积
    - 采样

    ![image-20230417232532673](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417232532673.png)

  - 原始三角形边缘的像素可能出现上述的情况，经过filter得到情况变成下面一行



### MSAA （antialiasing by sampling）

- 用更多采样点来反走样
- 使用更多小的像素来划分像素，再求出平均

![image-20230417232718483](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417232718483.png)



- solution：

  - 分解成小像素

  - 对n*n的小像素进行平均（卷积）

    ![image-20230417232940025](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417232940025.png)

  - 进行采样

- 效果不错

  ![image-20230417233155874](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230417233155874.png)

### 思考

- No free lunch
  - 增大了计算量
  - 线代使用一些复用样本得到更好的效果
- 其他抗锯齿方案
  - FXAA（快速近似抗锯齿）
    - 得出有锯齿的图
    - 然后找到边界
    - 替换锯齿边界
  - TAA（刚刚兴起）
    - 视频中广泛使用
    - 利用上一帧的像素
    - 相当于把MSAA的样本放到时间上
- 相似概念（super resolution）
  - 从低分辨率到高分辨率
  - 本质上也是采样不足的问题
  - DLSS方法（Deep learning Super Sampling）