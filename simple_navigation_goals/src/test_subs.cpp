

#include "/home/tas/catkin_ws/src/tas_pcs/simple_navigation_goals/include/car_class.h"

int main(int argc, char** argv){

    ros::init(argc, argv,"test_subs");
    ros::NodeHandle node;
    ros::Rate rate(1);

    geometry_msgs::Pose goal;

    Car my_sub(node);

        while(ros::ok){

            my_sub.publishGoal(goal, true, false);
            rate.sleep();
            ros::spinOnce();
        }


    return 0;
}
