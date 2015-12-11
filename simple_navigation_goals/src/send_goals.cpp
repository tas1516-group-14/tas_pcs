
#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "goal_subscriber_class.hpp"
#include <string>
#include <sstream>

int main(int argc, char** argv){

ros::init(argc, argv, "send_goals");
ros::NodeHandle n;

float x_pos;
geometry_msgs::Pose goal_msg;

ros::Publisher goal= n.advertise<geometry_msgs::Pose>("my_goals/goal",100);

ros::Rate r(1);

if(argc == 2){
ROS_INFO("%s", *(argv+1));
std::stringstream sstr(*(argv+1));
sstr >> x_pos;

ROS_INFO("bla  bala %f", x_pos);
goal_msg.position.x= x_pos;
goal_msg.position.y = 19.75;
goal_msg.position.z = 0.000;
goal_msg.orientation.x = 0.000;
goal_msg.orientation.y = 0.000;
goal_msg.orientation.z = 1;
goal_msg.orientation.w = 0;
goal.publish(goal_msg);
r.sleep();
return 0;
}



//insert your waypoints here
int i=1;
while(i>0){
    goal_msg.position.x= i;
    goal_msg.position.y = 19.75;
    goal_msg.position.z = 0.000;
    goal_msg.orientation.x = 0.000;
    goal_msg.orientation.y = 0.000;
    goal_msg.orientation.z = 1;
    goal_msg.orientation.w = 0;
    goal.publish(goal_msg);
    i--;
    ROS_INFO("goal [%f]",goal_msg.position.x);
    ros::spinOnce();
    r.sleep();
}

return 0;
}

/*
#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "goal_subscriber_class.hpp"



#include "geometry_msgs/PointStamped.h" //für die KlickPunkt Ausgabe

float x,y,z;

void klickCallback(const geometry_msgs::PointStamped& klickpoint)  // wurde mit "rostopic type /clicked_point " entnommen und die Messagebezeichnung ist frei wählbar (myname.msgname dabei wird msgname mit rostopic "type /clicked_point | rosmsg show" angezeigt)

{

  x=klickpoint.point.x;
  y=klickpoint.point.y;
  z=klickpoint.point.z;
}

int main(int argc, char** argv){

ros::init(argc, argv, "send_goals");
ros::NodeHandle n;


ros::Publisher goal= n.advertise<geometry_msgs::Pose>("my_goals/goal",100);
ros::Subscriber sub = n.subscribe("/clicked_point", 1000, klickCallback);

ros::spin();
ros::Rate r(10);



geometry_msgs::Pose goal_msg;



    goal_msg.position.x= x;
    goal_msg.position.y = y;
    goal_msg.position.z = z;
    goal_msg.orientation.x = 0.000;
    goal_msg.orientation.y = 0.000;
    goal_msg.orientation.z = 1;
    goal_msg.orientation.w = 0;
    goal.publish(goal_msg);
    ROS_INFO("goal [%f]",goal_msg.position.x);
    ros::spinOnce();
    r.sleep();


return 0;
}
*/
