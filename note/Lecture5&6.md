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