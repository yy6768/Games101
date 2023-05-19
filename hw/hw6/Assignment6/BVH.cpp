#include <algorithm>
#include <cassert>
#include <vector>
#include "BVH.hpp"

BVHAccel::BVHAccel(std::vector<Object*> p, int maxPrimsInNode,
                   SplitMethod splitMethod)
    : maxPrimsInNode(std::min(255, maxPrimsInNode)), splitMethod(splitMethod),
      primitives(std::move(p))
{
    time_t start, stop;
    time(&start);
    if (primitives.empty())
        return;

    root = recursiveBuild(primitives);

    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    printf(
        "\rBVH Generation complete: \nTime Taken: %i hrs, %i mins, %i secs\n\n",
        hrs, mins, secs);
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<Object*> objects)
{
    BVHBuildNode* node = new BVHBuildNode();

    // Compute bounds of all primitives in BVH node
    Bounds3 bounds;
    for (int i = 0; i < objects.size(); ++i)
        bounds = Union(bounds, objects[i]->getBounds());
    if (objects.size() == 1) {
        // Create leaf _BVHBuildNode_
        node->bounds = objects[0]->getBounds();
        node->object = objects[0];
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }
    else if (objects.size() == 2) {
        node->left = recursiveBuild(std::vector{objects[0]});
        node->right = recursiveBuild(std::vector{objects[1]});

        node->bounds = Union(node->left->bounds, node->right->bounds);
        return node;
    }
    else {
        Bounds3 centroidBounds;
        for (int i = 0; i < objects.size(); ++i)
            centroidBounds =
                Union(centroidBounds, objects[i]->getBounds().Centroid());
        int dim = centroidBounds.maxExtent();
        switch (dim) {
        case 0:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().x <
                    f2->getBounds().Centroid().x;
                });
            break;
        case 1:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().y <
                    f2->getBounds().Centroid().y;
                });
            break;
        case 2:
            std::sort(objects.begin(), objects.end(), [](auto f1, auto f2) {
                return f1->getBounds().Centroid().z <
                    f2->getBounds().Centroid().z;
                });
            break;
        }
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
        else  {
            auto beginning = objects.begin();
            auto middling = objects.begin() + (objects.size() / 2);
            auto ending = objects.end();
            auto leftshapes = std::vector<Object*>(beginning, middling);
            auto rightshapes = std::vector<Object*>(middling, ending);

            assert(objects.size() == (leftshapes.size() + rightshapes.size()));

            node->left = recursiveBuild(leftshapes);
            node->right = recursiveBuild(rightshapes);

            node->bounds = Union(node->left->bounds, node->right->bounds);
        }
    }

    return node;
}

Intersection BVHAccel::Intersect(const Ray& ray) const
{
    Intersection isect;
    if (!root)
        return isect;
    isect = BVHAccel::getIntersection(root, ray);
    return isect;
}

Intersection BVHAccel::getIntersection(BVHBuildNode* node, const Ray& ray) const
{
    // TODO Traverse the BVH to find intersection
    
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

}