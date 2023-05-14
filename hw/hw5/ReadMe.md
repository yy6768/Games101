# Assignment 5

## 前言

- hw5主题：光线追踪
- 任务：
  - 光线生成
  - 光线与三角的相交

- 源码在：[yy6768/Games101: 学习Games101的笔记和自己的作业解决 (github.com)](https://github.com/yy6768/Games101)

## Task#1：光线的生成

> - main函数定义场景参数，添加物体（球体或三角形）到场景中，并设置其材质，然后将光源添加到场景中	
> - 调用 Render(scene) 函数。在遍历所有像素的循环里，生成对应的光线并将 返回的颜色保存在帧缓冲区（framebuffer）中。在渲染过程结束后，帧缓冲 区中的信息将被保存为图像。

现在我们需要修改Render函数：

- 为每个像素生成一条对应的光线
- 调用函数 castRay() 来得到颜色
- 将颜色存储在帧缓冲区的相应像素中。

### 生成光线

我们现在已有eye point 的位置，castRay也已经实现好，具体应该计算x和y的具体位置

![image-20230514132511391](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230514132511391.png)

我们的ij的坐标和xy坐标关系如下,需要注意：

- j和y的方向相反
- 中心坐标迁移到中心
- x和i还有`imageAspectRatio`的拉伸

![85233aa151aad952ae272b1a884ce31](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/85233aa151aad952ae272b1a884ce31.jpg)

最后$$x = (\frac {2 * (i + 0.5)}{width}  -1) * imageAspectRatio *scale \\ y = 1 - \frac {2 *(j+0.5)}{height} * scale$$

得到

![image-20230514140413109](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230514140413109.png)

```c++
void Renderer::Render(const Scene& scene)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = std::tan(deg2rad(scene.fov * 0.5f));
    float imageAspectRatio = scene.width / (float)scene.height;

    // Use this variable as the eye position to start your rays.
    Vector3f eye_pos(0);
    int m = 0;
    for (int j = 0; j < scene.height; ++j)
    {
        for (int i = 0; i < scene.width; ++i)
        {
            // generate primary ray direction
            // TODO: Find the x and y positions of the current pixel to get the direction
            // vector that passes through it.
            float x = ((i + 0.5f) * 2.0f / scene.width - 1.0f) * scale * imageAspectRatio;
            float y = (1.0f - 2.0f * ((j + 0.5f) / scene.height)) * scale;
            // Also, don't forget to multiply both of them with the variable *scale*, and
            // x (horizontal) variable with the *imageAspectRatio*            

            Vector3f dir = normalize( Vector3f(x, y, -1)); // Don't forget to normalize this direction!
            framebuffer[m++] = castRay(eye_pos, dir, scene, 0);
        }
        UpdateProgress(j / (float)scene.height);
    }

    // save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (char)(255 * clamp(0, 1, framebuffer[i].x));
        color[1] = (char)(255 * clamp(0, 1, framebuffer[i].y));
        color[2] = (char)(255 * clamp(0, 1, framebuffer[i].z));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}

```



## Task#2：光线与三角形相交

> Triangle.hpp 中的 rayTriangleIntersect(): v0, v1, v2 是三角形的三个 顶点，orig 是光线的起点，dir 是光线单位化的方向向量。tnear, u, v 是你需 要使用我们课上推导的 **Moller-Trumbore** 算法来更新的参数。

![image-20230514143412934](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230514143412934.png)

结果：

![image-20230514145642662](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230514145642662.png)



```c++
bool rayTriangleIntersect(const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, const Vector3f& orig,
                          const Vector3f& dir, float& tnear, float& u, float& v)
{
    // TODO: Implement this function that tests whether the triangle
    // that's specified bt v0, v1 and v2 intersects with the ray (whose
    // origin is *orig* and direction is *dir*)
    // Also don't forget to update tnear, u and v.
    auto e1 = v1 - v0;
    auto e2 = v2 - v0;
    auto s = orig - v0;
    auto s1 = crossProduct(dir, e2);
    auto s2 = crossProduct(s, e1);
    float prod = dotProduct(s1, e1);
    tnear = dotProduct(s2, e2) / prod;
    u = dotProduct(s1, s) / prod;
    v = dotProduct(s2, dir) / prod;
    return tnear >= 0 && u >= 0 && v >= 0 && (1.0f - u - v) >= 0;
}
```

