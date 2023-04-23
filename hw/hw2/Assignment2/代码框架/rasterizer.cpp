// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


static bool insideTriangle(float x, float y, const Vector3f* _v)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Vector3f Q = { x,y,0 };
    Vector3f edge0 = _v[1] - _v[0];
    Vector3f edge1 = _v[2] - _v[1];
    Vector3f edge2 = _v[0] - _v[2];

    Vector3f v0Q = Q - _v[0];
    Vector3f v1Q = Q - _v[1];
    Vector3f v2Q = Q - _v[2];

    return edge0.cross(v0Q).z() > 0 && edge1.cross(v1Q).z() > 0 && edge2.cross(v2Q).z()> 0;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = - vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {

    // super-sampling weighted
    std::vector<Eigen::Vector2f> weight{
        {0.25f, 0.25f},
        {0.75f, 0.25f},
        {0.25f, 0.75f},
        {0.75f, 0.75f}
    };
    

    auto v = t.toVector4();
    
    // TODO : Find out the bounding box of current triangle.
    int left = v[0].x(), right = v[0].x(),  bottom = v[0].y(), top = v[0].y();
    left = std::min((int)v[1].x(), left);
    left = std::min((int)v[2].x(), left);
    right = std::max((int)v[1].x(), right);
    right = std::max((int)v[2].x(), right);
    bottom = std::min((int)v[1].y(), bottom);
    bottom = std::min((int)v[2].y(), bottom);
    top = std::max((int)v[1].y(), top);
    top = std::max((int)v[2].y(), top);
    

    // iterate through the pixel and find if the current pixel is inside the triangle
    for (int x = left; x <= right; x++) {
        for (int y = bottom; y <= top; y++ ) {
            int cnt = 0;
            for (int i = 0; i < 4; i++) {
                if (insideTriangle(x + weight[i].x(), y + weight[i].y(), t.v)) {
                    // If so, use the following code to get the interpolated z value.
                    auto [alpha, beta, gamma] = computeBarycentric2D(x + weight[i].x(), y+ weight[i].y(), t.v);
                    float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                    z_interpolated *= w_reciprocal;
                    // TODO : set the current pixel (use the set_pixel function) 
                    // to the color of the triangle (use getColor function) if it should be painted.
                    if (z_interpolated < super_depth_buf[get_super_index(x, y, i)]) {
                        cnt++;
                        super_depth_buf[get_super_index(x, y, i)] = z_interpolated;
                        Eigen::Vector3f color = t.getColor();
                        set_super_pixel({ (float)x, (float)y, z_interpolated }, i, color);
                    }
                }
           }
            if (cnt > 0) {
                Eigen::Vector3f color{ 0.0f, 0.0f, 0.0f };
                for (int i = 0; i < 4; i++) {
                    color += super_frame_buf[get_super_index(x, y, i)];
                }
                color /= 4;
                set_pixel({ (float)x, (float)y, 0 }, color);
            }
        }
    }
   
 
}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
        std::fill(super_frame_buf.begin(), super_frame_buf.end(), Eigen::Vector3f{ 0, 0, 0 });
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
        std::fill(super_depth_buf.begin(), super_depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h); 
    super_frame_buf.resize(w * h * 4);
    depth_buf.resize(w * h);
    super_depth_buf.resize(w * h * 4);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

int rst::rasterizer::get_super_index(int x, int y, int i) {
    return ((height - 1 - y) * width + x) * 4 + i;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

void rst::rasterizer::set_super_pixel(const Eigen::Vector3f& point, int i, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = ((height - 1 - point.y()) * width + point.x()) * 4 + i;
    super_frame_buf[ind] = color;

}

// clang-format on