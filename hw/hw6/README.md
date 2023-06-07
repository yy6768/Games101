# Games101 HW6

## 前言

- hw6主题：物体划分算法 Bounding Volume Hierarchy (BVH)

- 任务：

  - 包围盒求交
  - BVH查找

- 提高：SAH：查找

- 源码在：[yy6768/Games101: 学习Games101的笔记和自己的作业解决 (github.com)](https://github.com/yy6768/Games101)

  

## 代码框架

主要比对和hw5的区别

- 材质`Material.hpp`: hw5存放在global.hpp中

  ```c++
  class Material{
  public:
      MaterialType m_type;
      Vector3f m_color;
      Vector3f m_emission;
      float ior;
      float Kd, Ks;
      float specularExponent;
      //Texture tex;
  
      inline Material(MaterialType t=DIFFUSE_AND_GLOSSY, Vector3f c=Vector3f(1,1,1), Vector3f e=Vector3f(0,0,0));
      inline MaterialType getType();
      inline Vector3f getColor();
      inline Vector3f getColorAt(double u, double v);
      inline Vector3f getEmission();
  
  
  };
  ```

  - 增加了物理渲染得相关量

- `Intersection.hpp`:数据结构包含了相交相关的信息。

- `Ray.hpp`: 光线类

- `Bounds3.hpp`：3维包围盒

  - `Vector3f pMin, pMax; // two points to specify the bounding box`

    通过两个维向量描述左下和右上得坐标

  - 有包括`Intersect`、 `Overlaps` 、 `Inside`等辅助判断函数

  - 也有Union合并函数

- `BVH.cpp`：BVH 加速类。场景 scene 拥有一个 BVHAccel 实例。从根节点开 始，我们可以递归地从物体列表构造场景的 BVH.

## 包围盒求交

目标：实现`IntersectP(const Ray& ray, const Vector3f& invDir,const std::array<int, 3>& dirIsNeg)`函数

- 求出每一维得进入时间tMin和最大时间tMax

![image-20230519130827674](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519130827674.png)

- 真正的进入时间为tmin最大的一维，出射时间为tmax最小的一维

![image-20230519133319264](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519133319264.png)

- ![image-20230519133455533](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519133455533.png)

```c++
inline bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic

    // 计算x,y,z方向得tmin和tmax
    Vector3f tMin = (pMin - ray.origin) * invDir, tMax = (pMax - ray.origin) * invDir;
    for (int i = 0; i < 3; i++) {
        if (!dirIsNeg[i]) {
            std::swap(tMin[i], tMax[i]);
        }
    }
    float tEnter = std::max(tMin[0], std::max(tMin[1], tMin[2]));
    float tExit = std::min(tMax[0], std::min(tMax[1], tMax[2]));
    return tEnter < tExit&& tExit >= 0.0f;
    // TODO test if ray bound intersects
    
}
```



## BVH

主体思想：

![image-20230519134743514](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519134743514.png)

算法流程：

![image-20230519134758094](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519134758094.png)

```c++

    std::array<int, 3> dirIsNeg{ int(ray.direction[0] > 0), int(ray.direction[1] > 0), int(ray.direction[2] > 0) };
    if (node == nullptr || !node->bounds.IntersectP(ray,ray.direction_inv,dirIsNeg)) {
        return Intersection();
    }

    if (node->left == nullptr && node->right == nullptr) { // 叶子节点
        return node->object->getIntersection(ray);
    }
    // 内部节点
    auto hit1 = getIntersection(node->left, ray);
    auto hit2 = getIntersection(node->right, ray);
    if (hit1.distance < hit2.distance) {
        return hit1;
    }

    return hit2;
```



## Render重构

- 大致与之前一次的代码一致
- 不同点在于castRay移到了Scene类中，Ray变成了一个类



## getIntersection 重构

- 这个感觉和上次作业实现的完全无关，就是根据已经判断好的相交情况计算

- 如果$t\_tmp <0$说名没有相交，就需要计算intersection的信息



```c++
 // TODO find ray triangle intersection
    if (t_tmp < 0) {
        return inter;
    }

    inter.happened = true;
    inter.coords = ray(t_tmp);
    inter.normal = normal;
    inter.distance = t_tmp;
    inter.obj = this;
    inter.m = m;

    return inter;
```

在这里不知道是不是我的代码问题

需要将Vector.hpp中的`[]`重载修改才行

```c++
float       operator[](int index) const;
float&      operator[](int index);
```



之后就完成了基本的渲染(小兔兔还是很可爱滴！)

![image-20230519160308453](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519160308453.png)



- 时间大概在12s左右

  ![image-20230519160347149](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519160347149.png)

##  SAH 查找

- 参考:
  - [Slide View : Computer Graphics : 15-462/662 Fall 2015 (cmu.edu)](http://15462.courses.cs.cmu.edu/fall2015/lecture/acceleration/slide_024)
  - [10_accelstructures.key (cmu.edu)](http://15462.courses.cs.cmu.edu/fall2015content/lectures/10_acceleration/10_acceleration_slides.pdf)
  - [SAH加速结构 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/501370100)
  - [BVH with SAH (Bounding Volume Hierarchy with Surface Area Heuristic) - lookof - 博客园 (cnblogs.com)](https://www.cnblogs.com/lookof/p/3546320.html)

- SAH(（Surface Area Heuristic):表面积启发式算法
  - SAH是一种划分策略（BVH的物体如何划分如子节点中）
  - BVH通常有`SPLIT_MIDDLE`,`SPLIT_EQUAL_COUNT`,`SPLIT_SAH`划分，我们采取的就是SPLIT_EQUAL_COUNT的方式
- SAH是一种概率论的思想，因为划分会将总的物体集合S划分为集合A和集合B，那么我们需要判断的总时间期望就是$E(T) =p(A)\sum T(A) + p(B)\sum T(B) +T_{tran}$
  - $T_{tran}$是遍历代价

![image-20230519162022447](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519162022447.png)

- $p(A)$和$p(B)$  无法量化计算，但是SAH方法提出物体的表面积越大，与光线相交的可能性越高，所以可以用表面积估算

- 引用参考文章中的例子

  **![image-20230519153920173](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519153920173.png)**

- 但是有一个需要注意的点：

  > **注意S(A)/S(C)+S(B)/S(C) 是有可能大于S(C)的**，因为S(A)与S(B)可能会发生”重叠“，这当然是我们不喜欢的情况，原因上面已经讲过。所以S(A)/S(C)+S(B)/S(C)越小（越接近S(C)）我们越喜欢。
  >
  >  
  >
  > 在划分一个节点代表的空间区域时，可以通过不同的切法将该空间划分成两个子区域。切法可以平均等距地一刀一刀地切，也可以耍点小手段带点”智能“使其”自适应“。总之每次划分，都会得到两个子区域A和B。那么相应也会算出一个cost(A,B)来。比较所有划分方案下所得的cost(A,B)，取值最小的方案就是成本最低的划分方案，也作为划分该节点的最终/优方案。如下图所示，scheme1和scheme2就是两种不同的划分方案。

  ![image-20230519154738869](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519154738869.png)

- 上图通过按照每一个轴划分为固定数目的桶来实现

![image-20230519161151580](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519161151580.png)



### 实现

- 

![image-20230519161822701](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519161822701.png)

Bound3是给出了表面积的

- 关于$T_{tran}$和$T_{A}、T_B$  的计算

  没有查阅资料前真的难住我了，但是在参考了众多博客后：

  [(128条消息) Games101：作业6解析（含提高部分SAH）_games101作业6_Q_pril的博客-CSDN博客](https://blog.csdn.net/Q_pril/article/details/124054123)

  [（深度解析）GAMES 101 作业6：BVH与SAH(Surface Area Heuristic) - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/475966001)。。。

  发现网上广为流传的都是认为

  > 每个物体求交的代价是相同的（设为1），并设遍历当前所有包围盒的代价为0.125（因为遍历代价小于求交代价）

  这貌似也比较“启发性”，暂时没找到相关的数学证明和经验来源

- 之后我们只需要对于每个桶的位置计算损失并且选出最小的损失即可

  ```C++
  float cost = travCost + (leftshapes.size() * leftSa + rightshapes.size() * rightSa) / sa;
  ```

- 完整代码

  ```c++
  if (splitMethod == SplitMethod::SAH) {
              const int bucket_num = 10;
              const float travCost = 0.125f;
              const float interCost = 1.0f;
              int minBucketIndex = 0;
              float minCost = std::numeric_limits<float>::infinity();
              float sa = centroidBounds.SurfaceArea();
              for (int i = 0; i < bucket_num; ++ i) {
                  auto beginning = objects.begin();
                  auto middling = objects.begin() + (objects.size()  * i / bucket_num);
                  auto ending = objects.end();
                  auto leftshapes = std::vector<Object*>(beginning, middling);
                  auto rightshapes = std::vector<Object*>(middling, ending);
  
                  // leftshapes rightshapes 对应的包围盒
                  Bounds3 leftBounds, rightBounds;
                  for (int k = 0; k < leftshapes.size(); ++k)
                      leftBounds = Union(leftBounds, leftshapes[k]->getBounds().Centroid());
                  for (int k = 0; k < rightshapes.size(); ++k)
                      rightBounds = Union(rightBounds, rightshapes[k]->getBounds().Centroid());
                  float leftSa = leftBounds.SurfaceArea(), rightSa = rightBounds.SurfaceArea();
                  float cost = travCost + (leftshapes.size() * leftSa + rightshapes.size() * rightSa) / sa;
                  if (cost < minCost) {
                      minBucketIndex = i;
                      minCost = cost;
                  }
              }
              auto beginning = objects.begin();
              auto middling = objects.begin() + (objects.size() * minBucketIndex/  bucket_num);
              auto ending = objects.end();
              auto leftshapes = std::vector<Object*>(beginning, middling);
              auto rightshapes = std::vector<Object*>(middling, ending);
  
              assert(objects.size() == (leftshapes.size() + rightshapes.size()));
  
              node->left = recursiveBuild(leftshapes);
              node->right = recursiveBuild(rightshapes);
  
              node->bounds = Union(node->left->bounds, node->right->bounds);
          }
  ```

最终效果：

![image-20230519164435319](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519164435319.png)

（快了1s，网上大多都是快了1~2s)

![image-20230519164227416](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230519164227416.png)
