# Games101 HW7

## 前言

- hw7主题： 完整的光线追踪

- 任务：

  - Path Tracing

- 提高：
  - 多线程

  - Microfacet

- 源码在：[yy6768/Games101: 学习Games101的笔记和自己的作业解决 (github.com)](https://github.com/yy6768/Games101)



## 搬运

![image-20230523233021775](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230523233021775.png)



## Ray-Tracing

- 给出的伪代码：

  ![伪代码](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524091907976.png)

- 首先明白过程是我们在Render.cpp中最初通过Scene发射光线

  ![光线投射1](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524133838677.png)

  - 通过hw6的光线加速结构BVH来判断投射出的物体是否相交，无相交就返回0的光照

  - 如果相交的物体中是光源，那么我们可以注意到material中有emission相关的量：

    ![material.hpp](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524134354359.png)

    利用这类量判断是否是光源，如果是，则返回直接光照L_dir = getEmission 

    ![是光源的情况](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/435de4c95db1fc85956ba788934ca9f.jpg)

  - 如果不是光源则需要进行之后的操作

- 采样光线（光线抵达的不是光源）

  - 首先我们需要明白一些给出的API：

    - `sampleLight(Intersection pos, float pdf)`函数已经实现好了,这个函数可以根据光源面积均匀采样1个点

  - 当前的情况：

    ![traceRay击打到普通的物体到p点](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524135126105.png)

  - 此时我们应该获得这个光线的相交的信息

    ![image-20230524140503810](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524140503810.png)

  - 对光源采样：利用api对光源进行采样

    ![image-20230524140746426](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524140746426.png)

- 投射出入射光

  - 得到了新的光线的相交的信息之后，可以投射出新的光纤（假设从P点投出到Q点，实际的光路就是从Q点射向P点）

    ![Ray构造函数构造新的光线](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524141018151.png)

  - 投射出新的光线应该判断中间是否有物体遮挡

    ![物体遮挡的情况](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/7b8cfa3c5ca1d88925772f1c94554c4.jpg)

    可以通过距离判断是否有物体遮挡

    ![判断是否有物体遮挡](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524141519378.png)

- 俄罗斯轮盘赌+反射方程计算L_indir

  - 给出的api：

    ![api](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524142335421.png)

  - 俄罗斯轮盘赌

  ![俄罗斯轮盘赌的原理-Lecture16](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524142148658.png)

  - 保证期望一致可以有概率结束递归

  - 代码

    ![image-20230524144247483](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524144247483.png)

## 结果&Debug

![image-20230524141114731](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524141114731.png)

- 犯下了多个低级错误：

  - 包括没有搞清楚ws和wo的意义：ws应该是光源入射角，wo是初涉角，此时计算两个夹角$\theta$和$\theta'$都使用ws和N还有NN计算，并且，由于ws是从x到p所以与N的夹角大于90°

    ![示意图](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524143431086.png)

  - 间接光照判断错误，应该是没有emission的物体产生的光线是物理光照

最终结果：

![时间](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524151337463.png)

![效果](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524151402894.png)

- 有白色噪点，说明没有处理好pdf

  ```c++
  //渲染方程公式
  //为保证期望一致除以轮盘赌概率
  //除以pdf是蒙特卡罗积分
    auto p = m->pdf(wo, wi, N);
  
    if (p > eps) {
        L_indir = castRay(trace_ray, depth + 1) * m->eval(wo, wi, N) * dotProduct(wi, N) / m->pdf(wo, wi, N) / RussianRoulette;
    }
  ```


最后发现不是pdf的问题，应该是SPP太小了

## 多线程

- C++11引入的标准线程库
  - 思路就是把任务编写成函数，然后让每个线程都操纵不同的部分
  - 最后使用join把多个线程合并

代码：

```c++
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);
    int m = 0;

    // change the spp value to change sample ammount
    int spp = 16;
    std::cout << "SPP: " << spp << "\n";
    int process = 0;
    int thread_num = 24;
    std::thread thread_group[24];
    std::mutex mtx;
    auto RenderPerThread = [&](uint32_t rs, uint32_t re) {
        for (uint32_t j = rs; j < re; ++j) {
            int m = j * scene.width;
            for (uint32_t i = 0; i < scene.width; ++i) {
                // generate primary ray direction
                float x = (2 * (i + 0.5) / (float)scene.width - 1) *
                    imageAspectRatio * scale;
                float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

                Vector3f dir = normalize(Vector3f(-x, y, 1));
                for (int k = 0; k < spp; k++) {
                    framebuffer[m] += scene.castRay(Ray(eye_pos, dir), 0) / spp;
                }
                m++;
            }
            process++;
            std::lock_guard<std::mutex> guard(mtx);
            UpdateProgress(process / (float)scene.height);
        }
    };
    for (int i = 0; i < thread_num; i++) {
        thread_group[i] = std::thread(RenderPerThread, i * scene.height / thread_num, (i + 1) * scene.height / thread_num);
    }
    for (int i = 0; i < thread_num; i++) {
        thread_group[i].join();
    }
    UpdateProgress(1.f);
    //for (uint32_t j = 0; j < scene.height; ++j) {
    //    for (uint32_t i = 0; i < scene.width; ++i) {
    //        // generate primary ray direction
    //        float x = (2 * (i + 0.5) / (float)scene.width - 1) *
    //                  imageAspectRatio * scale;
    //        float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

    //        Vector3f dir = normalize(Vector3f(-x, y, 1));
    //        for (int k = 0; k < spp; k++){
    //            framebuffer[m] += scene.castRay(Ray(eye_pos, dir), 0) / spp;  
    //        }
    //        m++;
    //    }
    //    UpdateProgress(j / (float)scene.height);
    //}
    //UpdateProgress(1.f);

    // save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}

```

大致需要渲染5min左右（SPP=16）

渲染SPP=256需要1小时

![spp256时间](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524191213620.png)

![结果](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524191203514.png)

## Microfacet 

![Microfacet原理](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524192044115.png)

微表面模型参考：

- [(130条消息) Games101,作业7（微表面模型）_Elsa的迷弟的博客-CSDN博客](https://blog.csdn.net/weixin_44518102/article/details/122698851?spm=1001.2101.3001.6650.9&utm_medium=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromBaidu~Rate-9.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromBaidu~Rate-9.pc_relevant_default&utm_relevant_index=12)
- [(130条消息) GAMES101-现代计算机图形学学习笔记（作业07）_games101作业7_CCCCCCros____的博客-CSDN博客](https://blog.csdn.net/qq_36242312/article/details/116307626?spm=1001.2014.3001.5502)

- 业界主流的法线分布函数GGX:![image-20230524195615255](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524195615255.png)
- 业界主流的几何函数（shadowing-masking)是分离遮蔽阴影（Separable Masking and Shadowing Function）。
  - 该形式将几何项G分为两个独立的部分：光线方向（light） 和 视线方向（view），并对两者用相同的分布函数来描述。
  - 其中UE4的方案是Schlick-GGX，即基于Schlick近似，将k映射为 $k = \frac {\alpha}{2}$,去匹配GGX Smith方程：
    $\alpha = (\frac{roughness +1}{ 2})^2\\k = \alpha / 2\\ G_{direction} = \frac {n \cdot v}{n\cdot v (1-k)+ k}\\ G(n,v,l,k) = G_{view}(n,v,k) G_{light}(n,l,k)$
  - 粗糙度$\alpha$越大表面越粗糙 取值在[0,1]
- fresnel已经提供好了



### 实现

显然我们需要先添加MICROFACET这种材质：

```c++
enum MaterialType { DIFFUSE, MICROFACET};
```



对于sample和pdf不需要做大的修改，只需要添加MICROFACET的情况

```c++
case DIFFUSE:
case MICROFACET:
{
	...
}
```



对于eval是主要不同的地方，因为Microfacet的BRDF不一样

![参考](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524202634447.png)

- 漫反射+镜面来源于Bling-phong模型

- 镜面反射项就是为表面的BRDF的表达式

- 漫反射项：

  ![image-20230524205817443](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524205817443.png)

代码可以参考参考中所说的

- 具体要注意的几个点：第一个只考虑半球面（cos < 0(接触不到光源) ), BRDF  = 0

```c++
 float cosalpha = dotProduct(N, wo);
 float eps = 1e-3;
 if (cosalpha > 0.0f) {
 	// 计算为表面的BRDF
 } else {
 	return 0.0f
 }
```

- 第二个需要重新更改 getIntersection函数，参考引用中的说法，球面相交的精度不够，所以需要添加`t> 0.5`的判定

```
	Intersection getIntersection(Ray ray){
        Intersection result;
        result.happened = false;
        Vector3f L = ray.origin - center;
        float a = dotProduct(ray.direction, ray.direction);
        float b = 2 * dotProduct(ray.direction, L);
        float c = dotProduct(L, L) - radius2;
        float t0, t1;
        if (!solveQuadratic(a, b, c, t0, t1)) return result;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return result;
        if (t0 > 0.5) {
            result.happened = true;

            result.coords = Vector3f(ray.origin + ray.direction * t0);
            result.normal = normalize(Vector3f(result.coords - center));
            result.m = this->m;
            result.obj = this;
            result.distance = t0;
            
        }
        return result;

    }
```

### 结果

需要修改main.cpp产生球体

SPP = 16

![球体](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230524214620790.png)

## 参考

- [(129条消息) Games101：作业7（含提高部分）_games101作业7-CSDN博客](https://blog.csdn.net/Q_pril/article/details/124206795?ydreferer=aHR0cHM6Ly9jbi5iaW5nLmNvbS8%3D)（主要是多线程）

- [作业7发布公告 – 计算机图形学与混合现实在线平台 (games-cn.org)](http://games-cn.org/forums/topic/graphics-intro-hw7/) （关键的3条错误信息）

微表面模型参考：

- [(130条消息) Games101,作业7（微表面模型）_Elsa的迷弟的博客-CSDN博客](https://blog.csdn.net/weixin_44518102/article/details/122698851?spm=1001.2101.3001.6650.9&utm_medium=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromBaidu~Rate-9.pc_relevant_default&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2~default~BlogCommendFromBaidu~Rate-9.pc_relevant_default&utm_relevant_index=12)
- [(130条消息) GAMES101-现代计算机图形学学习笔记（作业07）_games101作业7_CCCCCCros____的博客-CSDN博客](https://blog.csdn.net/qq_36242312/article/details/116307626?spm=1001.2014.3001.5502)
- [基于物理的渲染：微平面理论(Cook-Torrance BRDF推导) - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/152226698)（解释漫反射项）
