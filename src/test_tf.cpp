#include "ros/ros.h"
#include <tf/transform_datatypes.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>


int main(int argc, char **argv)
{
	ROS_INFO("TF_node : ON");
	
	ros::init(argc, argv, "TF_node"); 
	
	ros::NodeHandle n;

      tf::TransformBroadcaster odom_broadcaster,o2;
//     first, we'll publish the transform over tf
      geometry_msgs::TransformStamped odom_trans,o;
      tf::TransformListener listener;
         geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(0,0,0);
	 geometry_msgs::Quaternion odom_quat2 = tf::createQuaternionMsgFromRollPitchYaw(0,0,0);
	 
	     geometry_msgs::PointStamped base_point,map_point;

	while(ros::ok())
	{
	  ros::spinOnce();
	  ros::Time current_time = ros::Time::now();
	  base_point.header.stamp = ros::Time();
	  base_point.point.x = 2;
	  base_point.point.y = 3;
	  base_point.point.z = 4;
    
    
	  odom_trans.header.stamp = current_time;
	  odom_trans.header.frame_id = "odom";
	  odom_trans.child_frame_id = "base_link";
	  odom_trans.transform.translation.x = 10;
	  odom_trans.transform.translation.y = 10;
	  odom_trans.transform.translation.z = 10;
	  odom_trans.transform.rotation = odom_quat;
	  odom_broadcaster.sendTransform(odom_trans);
    
   
	  // MAP-->odom
	  o.header.stamp = current_time;
	  o.header.frame_id = "map";
	  o.child_frame_id = "odom";

	  o.transform.translation.x = 5;
	  o.transform.translation.y = -5;
	  o.transform.translation.z = 23;
	  o.transform.rotation = odom_quat2;
//     send the transform
	o2.sendTransform(o);
      
	try{
	    listener.transformPoint("base_link",ros::Time::now(), base_point,"map", map_point); 
	    ROS_INFO("map x-->%f, map y---> %f, map z--> %f",map_point.point.x,map_point.point.y,map_point.point.z); 
	    ROS_INFO("base_link x-->%f, base_link y---> %f, base_link z--> %f",base_point.point.x,base_point.point.y,base_point.point.z); 
	    }
	    catch (tf::TransformException& ex){
		
	    }
    }
	return 0;
}
