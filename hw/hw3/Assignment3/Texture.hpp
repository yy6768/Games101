//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        if (u < 0.0f) {
            u = 0.0f;
        }
        else if (abs(u - 1.0f) < 1e-6) {
            u = 1.0f - 1e-5;
        } 
        if (v < 0.0f) {
            v = 0;
        }
        else if (abs(v - 1.0f) < 1e-6) {
            v = 1.0f - 1e-5;
        }
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v) {
        float u0 = int(u * width), v0 = int(v * height);
        float du[4] = { 0, 0, 1, 1 }, dv[4] = { 0, 1, 0, 1 };
        Eigen::Vector3f temp_color[4];
        for (int i = 0; i < 4; i++) {
            float un = u0 + du[i], vn = v0 + dv[i];
            temp_color[i] = getColor(un / width, vn / height);
        }
        auto bottom_color = temp_color[0] + (temp_color[1] - temp_color[0]) * (u * width - u0);
        auto top_color = temp_color[2] + (temp_color[3] - temp_color[2]) * (u * width - u0);
        return bottom_color + (top_color - bottom_color) * (v * height - v0);
    }
};
#endif //RASTERIZER_TEXTURE_H
