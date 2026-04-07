#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

void drive_robot(float lin_x, float ang_z)
{
    geometry_msgs::Twist motor_command;
    motor_command.linear.x = lin_x;
    motor_command.angular.z = ang_z;
    motor_command_publisher.publish(motor_command);
}

bool command_robot(ball_chaser::DriveToTarget::Request &req,
                   ball_chaser::DriveToTarget::Response &res)
{
    drive_robot(req.linear_x, req.angular_z);
    res.msg_feedback = "Command received";
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "drive_bot");
    ros::NodeHandle n;

    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", command_robot);

    ros::spin();
    return 0;
}

