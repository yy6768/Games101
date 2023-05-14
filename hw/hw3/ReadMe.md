# Games101 作业3

## 前言

- 源码在：[yy6768/Games101: 学习Games101的笔记和自己的作业解决 (github.com)](https://github.com/yy6768/Games101)

## 目标

- 参数插值: 正确插值颜色、法向量、纹理坐标、位置
- Blinn-phong 反射模型
- Texture mapping
- Bump mapping 和Displacement mapping

加分项

- 尝试更多模型

-  双线性纹理插值



## 框架变化

- 相较于上节课，这节课使用了第三方.obj 文件加载库来加载小奶牛

  - 库函数会返回TriangleList（三角形，包含对应的法向量和纹理坐标）

- 新的Texure类

  ```c++
  Eigen::Vector3f getColor(float u, float v)
      {
          auto u_img = u * width;
          auto v_img = (1 - v) * height;
          auto color = image_data.at<cv::Vec3b>(v_img, u_img);
          return Eigen::Vector3f(color[0], color[1], color[2]);
      }
  ```

  - 专门获取纹理颜色的函数

- Shader.hpp 

  - 定义了`fragment_shader_payload`

  - main.cpp中已实现法向量上色的样例Shader

    ```c++
    Eigen::Vector3f normal_fragment_shader(const fragment_shader_payload& payload)
    {
        Eigen::Vector3f return_color = (payload.normal.head<3>().normalized() + Eigen::Vector3f(1.0f, 1.0f, 1.0f)) / 2.f;
        Eigen::Vector3f result;
        result << return_color.x() * 255, return_color.y() * 255, return_color.z() * 255;
        return result;
    }
    ```

  - 需要实现其他两个Shader

- `rasterize_triangle` 函数与你在作业 2 中实现的内容相似。不同之处在于被 设定的数值将不再是常数，而是按照 `Barycentric Coordinates `对法向量、颜 色、纹理颜色与底纹颜色 (Shading Colors) 进行插值。



## 实现

### normal（法向量）

- 复制hw2的代码

- 在深度测试部分，hw3不像hw2直接设置frame_buf,而是需要对多个属性进行插值：

  ```c++
  if (z_interpolated < depth_buf[get_index(x, y)]) {
                      depth_buf[get_index(x, y)] = z_interpolated;//更新z值
                      // hw2:setColor hw3:X 不能这样做
                      // TODO: Interpolate the attributes:
                      // auto interpolated_color
                      // auto interpolated_normal
                      // auto interpolated_texcoords
                      // auto interpolated_shadingcoords
  }
  ```

- 通过计算出插值后然后对各个值通过`interpolate`函数得到获得的值



### phong 模型

- 复习：

![image-20230430154833920](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430154833920.png)

![image-20230430161444411](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430161444411.png)

![image-20230430161501584](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430161501584.png)

![image-20230430161513576](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430161513576.png)

- 只需按照第一页PPT计算出$I,n,v$，然后计算出$r = \sqrt{(I*I)}$
- 全部向量`normalize`
- 计算三项*ambient*, *diffuse*, and *specular* 
- 累加

![image-20230430164210044](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430164210044.png)

### texture

- 本以为很简单，直接在`texture_fragment_shader`中添加getColor函数即可，
- 但是后来经常出现getColor越界的问题，最终通过奇奇怪怪的边界判定解决了（在getColor中添加边界判断）

![image-20230430171601389](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430171601389.png)

### bump

- bump是描述物体凹凸的模型，主要影响法向量

  ![image-20230430172912678](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430172912678.png)

  ![image-20230430172944758](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430172944758.png)

- bump和displacement在课程中没有特别详细，

- 附：

  - [（超详细！）计算机图形学 补充篇 2.关于阴影与法线贴图 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/517241213)
  - [计算机图形学八：纹理映射的应用(法线贴图，凹凸贴图与阴影贴图等相关应用的原理详解) - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/144357517)



#### 凹凸贴图（bump）

- 一维黑白贴图

- $H_g, H_a,H_r$(表示纹理处高度，纹理上方高度以及纹理右侧高度)

- 可以计算发现$normal = \frac {<H_g-H_r, H_g-H_a,1>}{\sqrt{(H_g-H_a)^2 + (H_g- H_r)^2 + 1}}$

#### 切向空间

- **切向空间是由切线** $t_\perp$ **、副切线** $b_\perp$ **、顶点法线** $\hat n$ 以模型顶点为中心的坐标空间

- TBN矩阵： 由Tangent切线、Bitangent副切线（有些地方也叫副法线）、Normal法线，组成的3x3的矩阵（**这里的切线和副切线都是正交后的）**



回到源代码上，实际上现在我们已知发现n（这里的n应该是近似顶点法线）

计算t：[作业3 bump mapping中TBN的t 公式怎么推导的 – 计算机图形学与混合现实在线平台 (games-cn.org)](https://games-cn.org/forums/topic/zuoye3-bump-mappingzhongtbndet-gongshizenmetuidaode/)

> 这里说是计算切线，其实更准确一点是在找一条切线然后跟n和它俩的叉乘构成一个Orthonormal basis。
>
> n是由它在x-z平面上的投影和它的y-component构成的。你把这个投影旋转到与y轴重合，n的y-component就到了一个平行于x-z平面的平面上，那么它的x，z坐标，就是t的x，z坐标。然后很显然，**t的y坐标就是n在x-z平面上的投影的模长**

计算t（有点抽象）

![dabf0aaa15a8b7ca1fd4a2831f9626e](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/dabf0aaa15a8b7ca1fd4a2831f9626e.jpg)

得到t后用叉乘可以计算出b

构建出TBN矩阵可以完成世界空间到切向空间的变换

u，v对应纹理图的坐标，w,h对应方向右和上的方向



![image-20230430205614130](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430205614130.png)

### displacement

- 有点类似于bump和phone模型的混合
- 使用$p = p + kn * n * h(u,v)$对点进行位移，也就是说通过法线方向向量对点进行拔高，最后得到凹凸的表面

![image-20230430234725377](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430234725377.png)

![image-20230430234631559](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230430234631559.png)

## 提高

### bilinear

![image-20230501101545733](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230501101545733.png)

代码实现非常简单，这里就不展示了，只需要对上下左右4个像素点调用getColor最后线性加权平均即可

![image-20230501101320756](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230501101320756.png)

- 左边二插值，右边普通渲染，可以看到在颜色边缘处的像素会做更丰富的过渡处理



### 其他模型

感觉很怪

很糊很糊

![image-20230501104536496](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230501104536496.png)

crate更是重量级

![image-20230501105321965](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230501105321965.png)

感觉必须在代码中修改才能出现一定效果



bunny好小

![image-20230501105722904](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230501105722904.png)