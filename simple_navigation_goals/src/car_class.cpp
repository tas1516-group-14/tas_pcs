#include "/home/tas/catkin_ws/src/tas_pcs/simple_navigation_goals/include/car_class.h"


Car::Car(ros::NodeHandle& node){

    m_subGoal= node.subscribe(GOALSUBSCRIBER, SUB_BUF_SIZE_GOAL, &Car::callbackGoal, this);
    m_pubGoal= node.advertise<GOALPUBTYPE>(GOALPUBLISHER, PUB_BUF_SIZE_GOAL);

    m_subPosition= node.subscribe(POSITIONSUBSCRIBER, SUB_BUF_SIZE_POSITION, &Car::callbackPosition, this);
}


void Car::callbackGoal(const geometry_msgs::Pose& goal){

    m_waypoints.push_back(goal);

    if(DEBUG){

        ROS_INFO("I heard: x[%f], y[%f], theta[%f] was sent.", goal.position.x, goal.position.y, goal.orientation.z);
    }
}

geometry_msgs::Pose Car::get_waypoint(){

    if (m_waypoints.size() > 0){
    geometry_msgs::Pose buffer= m_waypoints.front();
    m_waypoints.erase(m_waypoints.begin());
    return buffer;
    }
    else{
    geometry_msgs::Pose error;
    return error;
    }

}

void Car::callbackPosition(const geometry_msgs::PoseWithCovarianceStamped& position){

    m_posX = position.pose.pose.position.x;
    m_posY = position.pose.pose.position.y;
    m_oriZ = position.pose.pose.orientation.z;

    if(DEBUG){
        ROS_INFO("Car position: x[%f], y[%f], theta[%f]", m_posX, m_posY, m_oriZ);
    }
}

void Car::publishGoal(geometry_msgs::Pose &goal, bool notComplete= false, bool feedback= false){


    if(notComplete == true){



        goal.position.x= 20;
        goal.position.y = 19.75;
        goal.position.z = 0.000;
        goal.orientation.x = 0.000;
        goal.orientation.y = 0.000;
        goal.orientation.z = 1;
        goal.orientation.w = 0;

    }

    m_pubGoal.publish(goal);

    if(feedback == true){

        ROS_INFO("Goal with Position x[%f], y[%f], theta[%f] was sent.", goal.position.x, goal.position.y, goal.orientation.z);
    }
}
