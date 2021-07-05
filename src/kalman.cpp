//
// Created by huangyujun on 4/16/21.
//

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseStamped.h>
#include "../include/kalman.h"
#include <std_msgs/UInt32.h>
#include "/home/up/catkin_ws/devel/include/nlink_parser/SwarmInfoStamped.h"
#include <nav_msgs/Odometry.h>


//#include "TimeConverter.h"

ros::Publisher kalman_pose_pub;
geometry_msgs::PoseStamped g_latest_pose;

Kalman kalman;


//camera_frame_rate 不准
double camera_frame_rate = 61;//rostopic hz name 读出来的
double imu_rate = 50;//rostopic hz name 读出来的
double pose_pub_rate = 50;

Eigen::Vector3d my_pose(0,0,0);


//imu回调函数
void imu_callback(const sensor_msgs::Imu::ConstPtr& msg)
{
    std::cout<<"goto imu"<<std::endl;
    //线加速度，只用到这个了
    //geometry_msgs::Vector3 imu_linear_acceleration;
    Eigen::Vector3d imu_linear_acceleration;
    imu_linear_acceleration(0)=msg->linear_acceleration.x+0.46134;
    imu_linear_acceleration(1)=msg->linear_acceleration.y+0.17305;
    imu_linear_acceleration(2)=msg->linear_acceleration.z-9.795587;

    std::cout<<"imu"<<std::endl;

    kalman.predict(imu_linear_acceleration,msg->header);
    std::cout<<"imu,predict"<<std::endl;
}

//二维码回调函数,放在一个3x1的矩阵里
void aruco_callback(const nlink_parser::SwarmInfoStamped::ConstPtr& msg)
{
    bool flag_aruco= false;
//    geometry_msgs::Pose aruco_pose;
//    aruco_pose.position.x=msg->pose.position.x;
//    aruco_pose.position.y=msg->pose.position.y;
//    aruco_pose.position.z=msg->pose.position.z;
    Eigen::Vector3d aruco_pose;
    aruco_pose(0)=msg->poses[1].x+my_pose.x();
    aruco_pose(1)=msg->poses[1].y+my_pose.y();
    aruco_pose(2)=msg->poses[1].z+my_pose.z();

    if ((msg->poses[1].x == 0)&&(msg->poses[1].y == 0)&&(msg->poses[1].z == 0))
    {
        flag_aruco= true;
    }

    std::cout<<"aruco"<<std::endl;
    if (flag_aruco== false)
    {
        kalman.update(aruco_pose,msg->header);
    }

    std::cout<<"aruco,update"<<std::endl;

}

//t265回调函数,放在一个3x1的矩阵里
void t265_callback(const nav_msgs::Odometry::ConstPtr& msg)
{

    std::cout<<"goto T265"<<std::endl;
    my_pose(0)=msg->pose.pose.position.x;
    my_pose(1)=msg->pose.pose.position.y;
    my_pose(2)=msg->pose.pose.position.z;
    std::cout<<"GET T265 NUM"<<std::endl;

}

//定时器回调函数,放在一个2x1的矩阵里
void pose_pub_timer_callback(const ros::TimerEvent&)
{
    //实时跑的时候改这里
    g_latest_pose.header=kalman.getheader();
    g_latest_pose.pose=kalman.getpose();
//    g_latest_pose.pose.position.x=g_latest_pose.pose.position.x+my_pose.x();
//    g_latest_pose.pose.position.y=g_latest_pose.pose.position.y+my_pose.y();
//    g_latest_pose.pose.position.z=g_latest_pose.pose.position.z+my_pose.z();


    std::cout<<"time getheader()"<<std::endl;
    //实时跑的时候改这里

    kalman_pose_pub.publish(g_latest_pose);


}

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "kalman_swarm_node");

    // 创建节点句柄
    ros::NodeHandle nh;

    kalman_pose_pub = nh.advertise<geometry_msgs::PoseStamped>("/kalman_swarm/pose", 10);

    //t265的Subscriber
    ros::Subscriber t265_sub = nh.subscribe<nav_msgs::Odometry>("/uav1/camera/odom/sample",1,t265_callback);

    //imu的Subscriber
    ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/uav2/mavros/imu/data",1,imu_callback);

    //二维码的Subscriber
    ros::Subscriber aruco_sub = nh.subscribe<nlink_parser::SwarmInfoStamped>("/uav1/uwb_recv_detected_aruco_pose",1,aruco_callback);

    ros::spinOnce();
    ros::Timer pose_pub_timer = nh.createTimer(ros::Duration(1/pose_pub_rate), pose_pub_timer_callback);

    // 循环等待回调函数
    ros::spin();

    return 0;
}