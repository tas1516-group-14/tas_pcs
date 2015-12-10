#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include "tf/tf.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "tf/transform_listener.h"


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class Car{

	public:

	float pose_x;
	float pose_y;
	float pose_z;

	float orient_x;
	float orient_y;
	float orient_z;
	float orient_w;

	bool initialized;

	void callBackGetPosition(const geometry_msgs::PoseWithCovarianceStamped& positon);
};

void Car::callBackGetPosition(const geometry_msgs::PoseWithCovarianceStamped& position){
    pose_x= position.pose.pose.position.x;
    pose_y= position.pose.pose.position.y;
    pose_z= position.pose.pose.position.z;

    orient_x= position.pose.pose.orientation.x;
    orient_y= position.pose.pose.orientation.y;
    orient_z= position.pose.pose.orientation.z;
    orient_w= position.pose.pose.orientation.w;
    initialized= true;

    ROS_INFO("x [%f], frame_id[%std::msgs_stromg]", pose_x, position.header.frame_id);
}

int main(int argc, char** argv){

    ros::init(argc, argv, "move_tas_car");
    ros::NodeHandle n;
    ros::Rate(100);

    MoveBaseClient ac("move_base", true);

    Car my_car;
    geometry_msgs::PoseStamped goal;

    ros::Publisher  move_car= n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 1);
    ros::Subscriber pose_car= n.subscribe("amcl_pose", 1, &Car::callBackGetPosition, &my_car);


    goal.pose.position.x= 1;
    goal.pose.orientation.z= my_car.orient_z+1;
    goal.header.frame_id= ("base_link");
    goal.header.stamp= ros::Time::now();
    ros::spinOnce();


    ros::spin();
    return 0;
}
