#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>

nav_msgs::Odometry odometry;
sensor_msgs::Imu Imu;



void imu_callback(const sensor_msgs::Imu::ConstPtr msg)
{
	  Imu.orientation.x = msg->orientation.x;
	  Imu.orientation.y = msg->orientation.y;
	  Imu.orientation.z = msg->orientation.z;
	  Imu.orientation.w = msg->orientation.w;
	  Imu.header.frame_id = "red_blue/odom";
	  Imu.header.stamp = ros::Time::now();
}

void odom_callback(const nav_msgs::Odometry::ConstPtr msg)
{
	  odometry.child_frame_id = "red_blue/base_link";
	  odometry.header.frame_id= "red_blue/odom";
	  odometry.pose.pose.position.x = msg->pose.pose.position.x;
	  odometry.pose.pose.position.y = msg->pose.pose.position.y;
	  odometry.pose.pose.position.z = msg->pose.pose.position.z;
	  odometry.pose.pose.orientation.x = msg->pose.pose.orientation.x;
	  odometry.pose.pose.orientation.y = msg->pose.pose.orientation.y;
	  odometry.pose.pose.orientation.z = msg->pose.pose.orientation.z;
	  odometry.header.stamp = ros::Time::now();
}
int main(int argc, char **argv)
{
	ROS_INFO("Publishing...");
	ros::init(argc, argv,"publisher_node");
	ros::NodeHandle publisher_node;
	ros::Publisher odom,imu,gps;
	ros::Subscriber imu_sim,odom_sim;
	//Subscriber
	imu_sim = publisher_node.subscribe<sensor_msgs::Imu>("/red_blue/imu_data",1000,&imu_callback);
	odom_sim = publisher_node.subscribe<nav_msgs::Odometry>("/red_blue/odom",1000,&odom_callback);
	// Publisher
	odom = publisher_node.advertise<nav_msgs::Odometry>("/test_odom",1000);
	imu = publisher_node.advertise<sensor_msgs::Imu>("/test_imu",10000);
	gps = publisher_node.advertise<sensor_msgs::NavSatFix>("/test_gps",10);
	int time = 0;
	
	while (ros::ok())
	{ 
	  time = time+1;
	  sensor_msgs::NavSatFix GPS;
	  GPS.position_covariance_type = 0;
	  GPS.latitude = 1e-7*cos(time);
	  GPS.longitude = 1e-7*(-sin(time));
	  GPS.altitude = 0;
	  GPS.status.status = 0;
	  GPS.status.service = 1;
	  GPS.header.frame_id = "red_blue/odom";
	  GPS.header.stamp = ros::Time::now();
	  odom.publish<nav_msgs::Odometry>(odometry);
	  imu.publish<sensor_msgs::Imu>(Imu);
	  gps.publish<sensor_msgs::NavSatFix>(GPS);
	  ros::spinOnce();
	}
	return 0;
}
