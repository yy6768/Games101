#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Eigen>
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // ��z����ת
    rotation_angle = rotation_angle * MY_PI / 180.f;
    Eigen::Matrix4f translate;
    translate << std::cos(rotation_angle), -std::sin(rotation_angle), 0, 0,
        std::sin(rotation_angle), std::cos(rotation_angle), 0, 0,
        0, 0, 1.0f, 0,
        0, 0, 0, 1.0f;
    model = translate * model;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    float angle = eye_fov * MY_PI / 180.f /2.0f;
    float t = zNear * std::tan(angle);
    float b = -1.0f * t;
    float r = t * aspect_ratio;
    float l = -1.0f * r;

    Eigen::Matrix4f ortho1;
    Eigen::Matrix4f ortho2;
    Eigen::Matrix4f proToOr;

    proToOr << zNear, 0, 0, 0,
        0, zNear, 0, 0,
        0, 0, zNear + zFar, -zNear * zFar,
        0, 0, 1.0f, 0;

    ortho1 << 2.0f / (r - l), 0, 0, 0,
        0, 2.0f / (t - b), 0, 0,
        0, 0, 2.0f / (zNear - zFar), 0,
        0, 0, 0, 1.0f;

    ortho2 << 1.0f, 0, 0, -1.0 * (r + l) / 2.0f,
        0, 1.0f, 0, -1.0f * (b + t) / 2.0f,
        0, 0, 1.0f, -1.0f * (zNear + zFar) / 2.0f,
        0, 0, 0, 1.0f;

    projection = ortho1 * ortho2 * proToOr * projection;
    

    return projection;
}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle) {
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix3f temp = Eigen::Matrix3f::Identity();
    float ag = angle / 180 * MY_PI;
    Eigen::Matrix3f tr;
    Eigen::Matrix3f mul;
    mul << 0, -axis[2], axis[1],
        axis[2], 0, -axis[0],
        -axis[1], axis[0], 0;
    tr = cos(ag) * temp + (1 - cos(ag)) * axis * axis.adjoint() + mul * sin(ag);
    model << tr(0, 0), tr(0, 1), tr(0, 2), 0,
        tr(1, 0), tr(1, 1), tr(1, 2), 0,
        tr(2, 0), tr(2, 1), tr(2, 2), 0,
        0, 0, 0, 1;
    return model;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        Vector3f axis;
        axis << 0, 10.0f, 0;
        //r.set_model(get_model_matrix(angle));
        r.set_model(get_rotation(axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);
        Vector3f axis;
        axis << 0, 1.0f, 1.0f;
        r.set_model(get_rotation(axis, angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
