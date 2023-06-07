# Lecture 20

## 课程内容

- 光场
- 颜色

## Final project

- Aesthetics （美学）
- 不在乎写报告



![image-20230605094334513](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605094334513.png)



## 光场（Light field = Lumigraph）

- 如何描述人看到的东西？全光函数

![image-20230605095056965](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095056965.png)

- 定义：人眼所能看到的所有东西

![image-20230605095122842](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095122842.png)

从某一个点，$\theta,\phi$的角度看到的世界

![image-20230605095140822](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095140822.png)

引入光的波长

![image-20230605095233573](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095233573.png)

引入时间，就是电影

![image-20230605095257445](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095257445.png)

引入相机坐标，就是全息电影

![image-20230605095330633](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095330633.png)

真实的世界



### 光线

![image-20230605095531307](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095531307.png)

![image-20230605095515417](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605095515417.png)

重新定义（2个点定义光线）



光场：全光函数的一部分（面积），四维函数

- 两个数表示方向
- 两个数表示位置



简化成包围盒

![image-20230605100023168](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605100023168.png)

简化成2d position + 2dposition

![image-20230605100341859](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605100341859.png)



![image-20230605100425156](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605100425156.png)

### 生物仿生学：苍蝇眼-光场摄像机

![image-20230605100825155](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605100825155.png)



## 光场摄像机

![image-20230605100959698](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605100959698.png)

- 先拍照-根据光场原理动态的调

![image-20230605101204031](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605101204031.png)

- 把像素替换成微透镜（MicroLens)

- 选取特殊方向的光线，来虚拟的移动摄像机的位置（不需要重新拍和计算，只需要记录光场）

![image-20230605101601846](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605101601846.png)



## 颜色

![image-20230605102500847](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605102500847.png)

- 光谱：

  ![image-20230605102644805](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605102644805.png)

PSD（谱功率密度）

![image-20230605103047616](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605103047616.png)

不同波长处的光线强度

![image-20230605102954000](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605102954000.png)



- 颜色是人类的感觉
- 不同波长的光不是颜色



### 颜色的生物学

![image-20230605103904408](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605103904408.png)

透镜——晶状体

视网膜——屏幕



视杆细胞（Rod）：得到灰度图

视锥细胞（Cones）：得到颜色

![image-20230605104035634](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605104035634.png)

不同种类的视锥细胞

![image-20230605104309454](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605104309454.png)



### 颜色感知

SML三种视锥细胞得到不同的光，在累积起来

![image-20230605104328354](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605104328354.png)

![image-20230605104433003](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605104433003.png)



### 同色异谱 Metamerism

- 人的感知相同颜色，但是实际光谱不同

![image-20230605112522113](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605112522113.png)

## 颜色混合

使用加色系统：通过线性组合颜色得到其他颜色

![image-20230605112704082](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605112704082.png)

## CIE RGB系统

![image-20230605112937325](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605112937325.png)

通过波长（横坐标）描述每种光需要的三原色的光

![image-20230605113829584](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605113829584.png)

### 色域

颜色空间显示的色域

![image-20230605114136904](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605114136904.png)



![image-20230605114145252](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605114145252.png)



## HSV 颜色空间

- H : 色调
- S：饱和度（是不是越接近原色）

- B：亮度 （是不是接近亮度）

![image-20230605114517564](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605114517564.png)

## CIELAB SPACE （LAB）

![image-20230605114450250](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605114450250.png)

L:亮度，= HSV中的B

a:红绿互补色

b：蓝黄互补色



## 减色系统（CMYK）

![image-20230605115043991](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230605115043991.png)

- 前提：黑色墨水造价便宜



