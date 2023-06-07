# Lecture 19 

## 课程内容

- 相机
- 棱镜
- 光场



## 图像= 合成+捕捉

![image-20230531113630457](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531113630457.png)



## 相机

### 针孔相机（Pinholes) 和镭射相机

![image-20230604213140526](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604213140526.png)

![image-20230604213309723](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604213309723.png)

传感器智能收集各个方向的光，而不只是收集某个点的光



## FOV 视场

![image-20230604213559194](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604213559194.png)



- f:焦距（只是沿用凸透镜的说法）

![image-20230604214402213](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604214402213.png)

- 焦距指的是对应36*24mm的胶片的f大小，手机的焦距大于手机厚度是因为胶片大小很小

- 传感器概念可以和胶片不一致，但是对于本门课是一致的



## Exposure 曝光

![image-20230604214802877](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604214802877.png)

- Exposure = time * irradiance
- 时间跨度获得的irradiance





### 相关概念

- Aperture size：通过f-stop来控制光圈的大小
- Shtter speed ：快门，快门速度快，时间短，曝光少
- ISO gain:感光度,因子数，可以理解成灵敏度，可以在硬件或者软件实现调节

![image-20230604215548462](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604215548462.png)

- f-stop越大，光圈越小
- shutter-speed其实就是时间反比（ms-1)

- 增大ISO会放大图像光，也会放大噪声



### ISO

![image-20230604223100753](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604223100753.png)



### F-number(F-stop)

![image-20230604223154621](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604223154621.png)

- FN或者写作F/N
- 非正式理解：F数就是光圈的直径的倒数



### 快门

- 半开

![image-20230604223355895](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604223355895.png)

- 全开

![image-20230604223402863](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604223402863.png)

- 运动模糊：在快门打开时间内，物体发生的相关运动，传感器都记录下来，出现运动模糊

  ![image-20230604223503620](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604223503620.png)



### 快门速度的边缘效应

Rolling shutter：超级高速的东西出现扭曲（螺旋桨）

![image-20230604224759624](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604224759624.png)



![image-20230604224904511](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604224904511.png)



### 高速摄影

![image-20230604225120278](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604225120278.png)‘

- 更大的光圈（通常）/更大的ISO



### 超低速摄影

![image-20230604225225359](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604225225359.png)



## Thin Lens Approximation（薄棱镜近似）

![image-20230604225506297](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604225506297.png)

研究的是理想化的薄棱镜

![image-20230604225525621](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604225525621.png)

### 棱镜方程

![image-20230604225644491](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604225644491.png)

物距和相距之和为常数

![image-20230604230144256](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230604230144256.png)



### Defocus Blur

![image-20230604230312496](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604230312496.png)

- 成像平面与交点的距离有偏差

![image-20230604231031507](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604231031507.png)

F数的定义：

F除以A：焦距除以光圈的大小

![image-20230604231208330](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604231208330.png)

## Ray Tracing Ideal Thin Lenses

- 模拟薄棱镜
- 设定方案：
  - 选择sensor大小，透镜的焦距和光圈大小
  - 设定物距$z_0$

![image-20230604231548353](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604231548353.png)

- 选定像素点x'
- 随机在透镜上采样x''
- 可以根据物距和焦距计算物体plane上的x'''
- 计算光线x''->x'''



## Depth of Field(景深)

![image-20230604231850637](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604231850637.png)

COC很小的范围内，在成像平面看到的景都是锐利的

![image-20230604232030045](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604232030045.png)

![image-20230604232108663](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230604232108663.png)