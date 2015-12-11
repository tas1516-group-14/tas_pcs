#include "ros/ros.h"
#include <iostream>

#ifndef goal_subscriber_class_hpp
#define goal_subscriber_class_hpp

class Goal_subscriber{

    public:
                std::vector<geometry_msgs::Pose> waypoints;


        geometry_msgs::Pose get_waypoint();
        void call_back_goal(const geometry_msgs::Pose& goal);

};

void Goal_subscriber::call_back_goal(const geometry_msgs::Pose& goal){

    waypoints.push_back(goal);
}

geometry_msgs::Pose Goal_subscriber::get_waypoint(){

    if (waypoints.size() > 0){
    geometry_msgs::Pose buffer= waypoints.front();
    waypoints.erase(waypoints.begin());
    return buffer;
    }
    else{
    geometry_msgs::Pose error;
    return error;
    }

}

#endif
