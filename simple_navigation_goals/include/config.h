#ifndef _BSP_CONFIG_H
#define _BSP_CONFIG_H

#include "ros/ros.h"
#include <move_base_msgs/MoveBaseAction.h>
#include <move_base_msgs/MoveBaseActionResult.h>
#include <actionlib/client/simple_action_client.h>


const bool DEBUG = true;
//Parameters of the goal subscriber (subscribes goals and save it in GOAL.m_waypoints)
const std::string GOALSUBSCRIBER = "my_goals/goal";
const uint32_t  SUB_BUF_SIZE_GOAL     = 10;
//Parameters of the goal publisher
typedef geometry_msgs::Pose GOALPUBTYPE;
const std::string GOALPUBLISHER = "my_goals/goal";
const uint32_t PUB_BUF_SIZE_GOAL = 10;
//Parameters of the position subscriber (gets the position of the car)
const std::string POSITIONSUBSCRIBER = "/amcl_pose";
const uint32_t  SUB_BUF_SIZE_POSITION     = 10;
//action Client
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
const uint32_t SENDGOALRATE = 10;

#endif
