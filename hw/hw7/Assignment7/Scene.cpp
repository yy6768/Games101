//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f L_dir; // 直接光照
    Intersection inter = intersect(ray);
    if (!inter.happened) {
        return L_dir;
    }
    // 打到光源
    if (inter.m->hasEmission()) {
        return inter.m->getEmission();
    }
    
    // 交点
    Vector3f p = inter.coords;
    // p 点的材质
    Material* m = inter.m;
    // 法线
    Vector3f N = inter.normal.normalized();
    // 从投射光到物体的方向 向里
    Vector3f wo = ray.direction; 

    Intersection light_inter;
    float light_pdf;
    // 对光源点采样
    sampleLight(light_inter, light_pdf); 

    Vector3f x = light_inter.coords; // 发出光线的交点（可能是另一个物体）

    Vector3f ws = (x - p).normalized(); // 发射点x 入射点p 光线方向x - p
    Vector3f NN = light_inter.normal; // x处的法线
    Vector3f emit = light_inter.emit; // x处的能量

    Ray in_ray(p, ws);
    
    float true_dis = intersect(in_ray).distance; // 真正光线相交的距离

    float eps = 1e-3;

    float dis = (x - p).norm(); // 和光源的距离
    if (true_dis - dis > -eps) { // 没有阻挡 
        Vector3f f_r = m->eval(wo, ws, N); // eval 是计算材料的BRDF
        float cos_theta = dotProduct(N, -ws); // Lecture15 22 页ppt
        float cos_theta_x = dotProduct(NN, ws); 
        L_dir = emit * f_r * cos_theta * cos_theta_x / std::pow(dis, 2) / light_pdf;
    }

    Vector3f L_indir;
    
    if (get_random_float() < RussianRoulette) { // 俄罗斯轮盘赌
        Vector3f wi = m->sample(wo, N).normalized(); //其他物体的初射光
        Ray trace_ray(p, wi); // 入射光线
        Intersection trace_inter = intersect(trace_ray);
        if (trace_inter.happened && !trace_inter.m->hasEmission()) { // 不是光源才是间接光照
            //渲染方程公式
            //为保证期望一致除以轮盘赌概率
            //除以pdf是蒙特卡罗积分

            float p = m->pdf(wo, wi, N);
            if (p > eps) {
                L_indir = castRay(trace_ray, depth + 1) * m->eval(wo, wi, N) * dotProduct(wi, N) / p / RussianRoulette;
            }
        }
    }
    return L_dir + L_indir;
}