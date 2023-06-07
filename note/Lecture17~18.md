# Lecture 17

## 课程内容

- 材质和外观



## 材质和外观

- BRDF由材质决定 Material == BRDF

- 能量守恒（入射和出射能量相同）

  - 等于$f_r = \frac \rho \pi (单位是albedo)$

  ![image-20230522221745080](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522221745080.png)



### 不同材质

- 散射（表面粗糙，不是纯的镜面） 类铜镜 glossy

  ![image-20230522222516732](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522222516732.png)

  

- 折射 （镜子）

  ![image-20230522222440779](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522222440779.png)

  ![image-20230522222749351](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522222749351.png)

  



### 反射定律

两个视角：

- 平行四边形法则：投影方程移项得出
- 投射到从上往下看的视角

![image-20230522223615423](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522223615423.png)



### 折射

![image-20230522224046208](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522224046208.png)



#### Snell‘s 定律

- 入射角* 入射介质折射率 = 反射角 * 反射角折射率

![image-20230522224235259](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522224235259.png)

![image-20230522224800177](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522224800177.png)

- 有可能在实数上无解（入射率大于折射率，在特定角度上）
  - 这种情况叫全反射
- BTDF（双向折射分布函数）
- BSDF(双向散射分布函数 = BRDF + BTDF)



### Fresnel 反射/项

![image-20230522225222581](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522225222581.png)

![image-20230522225311489](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522225311489.png)

- 极化（波动光学相关）
- 本门课只关注红线，法线角度越大，反射率越高（$\eta = 1.5$)



金属的菲涅尔项

- 大量的反射

![image-20230522225547725](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522225547725.png)



菲涅尔项的计算

![image-20230522225625135](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522225625135.png)

简化方法：Schlick’s  近似

![image-20230522225706950](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522225706950.png)



## Microfacet Material（微表面模型）

- 从远处看看不到凹凸细节，只能看到平面，有高光

![image-20230522230320043](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522230320043.png)



- 宏观层面看：（微表面）看到是平面且粗糙
- 近处看：材质和几何

![image-20230522230603874](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522230603874.png)



镜面和漫反射的微表面法线

![image-20230522230720087](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522230720087.png)

- F.G.D
  - Freshel 项
  - G: shadowing- masking term(几何项，有些微表面会相互遮挡)
  - D(h) 法线分布
- half vector （四边形法则得到的近似法线）

![image-20230522230853941](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522230853941.png)

- 微表面效果很好
- 有不同微表面模型的细分，但是都基于物理



### Isotropic/ Anisotropic Material(各向同/异性）

![image-20230522231551868](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522231551868.png)

- 各项异性：$f_r(\theta_i,\phi_i;\theta_r, \phi_r) \ne f_r(\theta_i, \theta_r,\phi_r - \phi_i)$不满足旋转相同

![image-20230522231655763](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522231655763.png)



## BRDF的性质

- 非负性
- 线性：可以累加

![image-20230522232132697](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522232132697.png)

- 可逆性（光路可逆）
- 能量守恒

![image-20230522232315196](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522232315196.png)

- 各向同性和各向异性（各项同性只有3维）

![image-20230522232420718](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522232420718.png)



## 测量BRDF

使用仪器沿着物体转一周，给定入射和出射方向并且测量BRDF

![image-20230522232654353](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522232654353.png)

![image-20230522232729505](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522232729505.png)

- 各向同性可以降成3维
- 对称性可以只枚举一半的输入



## 表示BRDF

- Tabular Representation

  ![image-20230522232926356](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230522232926356.png)

- MRTL库测了很多BRDF的数据





# Lecture 18

- 无偏光线传播方法
  - 双向光线路径追踪
  - MLT（马尔科夫链传播）
- 有偏的光线传播方法
  - 光子映射
  - VCM（集合方法）

- 实时辐射度算法



## 无偏vs有偏

![image-20230530224053993](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530224053993.png)

无偏表示期望的值不变

## 双向路径传播

- 生成两个子路径（从光源和摄像机）

  ![image-20230530224204801](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530224204801.png)

实现效果

![image-20230530224234346](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530224234346.png)

- 光源只朝上方打，那么整个场景其实是通过间接光（上方墙壁的弹射）



## MLT

- 马尔科夫链采样
  - 可以生成任意的PDF类型采样的样本
  - 根据光线当前状态产生新的样本
  - 特别适合做局部的复杂光线追踪
  - 无偏

![image-20230530224659712](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530224659712.png)

![image-20230530224713509](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530224713509.png)

缺点：

- 难以预测何时收敛
- 不能确保每个像素都能收敛
- 生成的结果可能很脏

![image-20230530224910952](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530224910952.png)

## 光子映射

- 有偏估计
- 非常擅长计算caustics（光线的聚焦产生的复杂的图案)

![image-20230530225042091](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530225042091.png)

### 光子的实现的方法

![image-20230530225301708](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530225301708.png)

- 从光线出发，将光子进行弹射，到diffuse的表面停止
- 光子收敛（从物体打出sub-path，弹射直到遇到diffuse surface）
- 计算着色点附近最近的N个光子，其组成的密度，最终得到光子的密度（N除以光子组成的面积）

![image-20230530225441789](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530225441789.png)

- N小噪声比较大，N大会比较模糊
- 有偏但是一致

![image-20230530225723211](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530225723211.png)

![image-20230530225757188](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530225757188.png)



### Vertex Connection and Merging

相当于双向路径追踪和光子映射结合

光子在超参数r内就进行合并

![image-20230530225924610](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530225924610.png)

### 实时辐射度

![image-20230530230433547](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530230433547.png)

- 已经被照亮的面认为它们是光源（图2绿色的点）
- 根据新生成的简介光源来渲染

![image-20230530230630426](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530230630426.png)

缺陷：

- 许多地方莫名其妙会发光（平方项分子）
- VPL不能做镜面的物体



## 高级外观建模

![image-20230530233114502](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530233114502.png)

- 散射介质
- 表面模型
- 程序化生成的模型



### 非表面模型

- 雾、云、……

![image-20230530233216214](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530233216214.png)

几种可能:

- 吸收（absorption）
- 本身内部有光源
- 外部散射
- 向内散射

#### 参与介质

- 由相位函数定义（类似BRDF）

![image-20230530233316303](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530233316303.png)

![image-20230530233426148](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230530233426148.png)



### 头发表面模型

![image-20230531092919150](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531092919150.png)

#### Kajiya -kay 模型

- 有色高光和无色高光

![image-20230531093242946](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531093242946.png)

- 发丝看作圆柱体，散射成圆柱

![image-20230531094411166](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531094411166.png)

#### Marschner 模型

![image-20230531094454987](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531094454987.png)

- TT直接穿透
- R发生反射
- TRT穿透后在内部反射



#### 动物毛发模型

- 动物毛发的髓质非常大

![image-20230531095046625](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531095046625.png)

- yan老师提出了双层模型

![image-20230531095212741](C:\Users\12587\AppData\Roaming\Typora\typora-user-images\image-20230531095212741.png)

![image-20230531095410992](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531095410992.png)





### Granular Material(颗粒模型)

![image-20230531095743631](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531095743631.png)



### Translucent Material 

- 半透光模型（透射材质）

![image-20230531095928331](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531095928331.png)

#### 次表面散射

![image-20230531100103680](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531100103680.png)

![image-20230531100136649](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531100136649.png)

- BSSRDF：BRDF的反射，可以从任意地方进入，从任意地方出去



近似次表面散射效果

![image-20230531100400506](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531100400506.png)



![image-20230531100458844](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531100458844.png)



![image-20230531100605389](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531100605389.png)



### 布料材质

![image-20230531100954191](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531100954191.png)

- 缠绕的纤维形成股（ply)
- 缠绕的股形成线(Yarn)



表面模型非常复杂，不能用BRDF进行渲染

- 当成物体表面

![image-20230531101200538](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101200538.png)



- 将布料上的毛立体化（变成云烟雾类似的Participating Media）

![image-20230531101257151](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101257151.png)

- 渲染每一股

![image-20230531101344353](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101344353.png)



### 复杂材质

- 渲染结果过于完美（划痕）

![image-20230531101504643](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101504643.png)

- Yan（狗头）的论文添加了细节

![image-20230531101616455](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101616455.png)



#### 渲染效率问题

![image-20230531101852024](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101852024.png)

解决方案

给出一个光线分布，只计算这些分布中的光线

![image-20230531101950309](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531101950309.png)



#### 波优化

![image-20230531102228746](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531102228746.png)

## 程序化生成的外观

![image-20230531102501763](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531102501763.png)



- 定义程序式的函数可以进行纹理的动态查询
- noise function

![image-20230531102541055](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230531102541055.png)



