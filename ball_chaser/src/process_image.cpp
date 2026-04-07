#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "ball_chaser/DriveToTarget.h"

ros::ServiceClient client;


void drive_robot(float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (client.call(srv))
    {
        ROS_INFO("Send velocities - linear_x: %1.2f , angular_z: %1.2f",
                 srv.request.linear_x,
                 srv.request.angular_z);
    }
    else
    {
        ROS_ERROR("Failed to call service command_robot");
    }
}

void process_image_callback(const sensor_msgs::Image& img)
{
    ROS_INFO("width=%d height=%d step=%d encoding=%s", img.width, img.height, img.step, img.encoding.c_str());

    int step = 3;
    int width = img.width;
    int left_limit = width / 3;
    int right_limit = 2 * width / 3;

    for (int i = 0; i < img.data.size(); i += step)
    {
        if (img.data[i] > 200 &&
            img.data[i + 1] > 200 &&
            img.data[i + 2] > 200)
        {
            int pixel_index = i / step;
            int column = pixel_index % width;

            if (column < left_limit)
            {
                drive_robot(0.0, 0.5);
            }
            else if (column < right_limit)
            {
                drive_robot(0.5, 0.0);
            }
            else
            {
                drive_robot(0.0, -0.5);
            }

            return;
        }
    }

    drive_robot(0.0, 0.0);
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();
    return 0;
}

