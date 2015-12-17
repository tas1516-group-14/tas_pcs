

#ifndef car_class_h
#define car_class_h

#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <iostream>
#include "/home/tas/catkin_ws/src/tas_pcs/simple_navigation_goals/include/config.h"

class Car{

    private:
                std::vector<geometry_msgs::Pose> m_waypoints;
                ros::Subscriber m_subPosition;
                ros::Subscriber m_subGoal;
                ros::Publisher  m_pubGoal;

                float m_posX;
                float m_posY;
                float m_oriZ;

    public:


        Car(ros::NodeHandle& node);
        geometry_msgs::Pose get_waypoint();
        void callbackGoal(const geometry_msgs::Pose& goal);
        void callbackPosition(const geometry_msgs::PoseWithCovarianceStamped& position);
        void publishGoal(geometry_msgs::Pose& goal, bool notComplete, bool feedback);


};



#endif
