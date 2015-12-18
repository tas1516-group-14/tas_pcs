

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
                MoveBaseClient  *m_actionClient;

                float m_posX;
                float m_posY;
                float m_oriZ;


    public:

        //Constructur
        Car(ros::NodeHandle& node);
        ~Car();
        //methods
        geometry_msgs::Pose get_waypoint();
        bool sendGoal();
        //callback
        void callbackGoal(const geometry_msgs::Pose& goal);
        void callbackPosition(const geometry_msgs::PoseWithCovarianceStamped& position);
        //callback ac
        void acCallbackFeedback(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback);
        void acCallbackDone(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result);
        void acCallbackActive();
        //publish
        void publishGoal(geometry_msgs::Pose& goal, bool notComplete, bool feedback);

};



#endif
