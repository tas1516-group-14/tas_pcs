#include "/home/tas/catkin_ws/src/tas_pcs/simple_navigation_goals/include/car_class.h"


Car::Car(ros::NodeHandle& node){

    m_subGoal= node.subscribe(GOALSUBSCRIBER, SUB_BUF_SIZE_GOAL, &Car::callbackGoal, this);
    m_pubGoal= node.advertise<GOALPUBTYPE>(GOALPUBLISHER, PUB_BUF_SIZE_GOAL);

    m_subPosition= node.subscribe(POSITIONSUBSCRIBER, SUB_BUF_SIZE_POSITION, &Car::callbackPosition, this);
    m_actionClient = new MoveBaseClient("move_base", true);
}

Car::~Car(){

    delete m_actionClient;
    m_actionClient= 0;
}

void Car::callbackGoal(const geometry_msgs::Pose& goal){

    m_waypoints.push_back(goal);

    if(DEBUG){

        ROS_INFO("I heard: x[%f], y[%f], theta[%f].", goal.position.x, goal.position.y, goal.orientation.z);
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

bool Car::sendGoal(){

    ros::Rate r(SENDGOALRATE);
    move_base_msgs::MoveBaseGoal goal;

    if(m_waypoints.size() > 0){

        goal.target_pose.header.stamp = ros::Time::now(); // set current time
        goal.target_pose.header.frame_id= "map";
        goal.target_pose.pose = get_waypoint();
        ROS_INFO("Sending goal");
        m_actionClient->sendGoal(goal);
        //m_actionClient->sendGoal(goal, &Car::acCallbackDone(), this, &Car::acCallbackActive(), this, &Car::acCallbackFeedback(), this); // send goal and register callback handler
        m_actionClient->waitForResult(); // wait for goal result

        if(m_actionClient->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {

            ROS_INFO("The base moved to goal.");
            return true;
        }
        else {

            ROS_INFO("The base failed to move to goal for some reason.");
            return false;
        }
    }


    r.sleep();
    ros::spinOnce();

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

//actionClient Callbacks
void Car::acCallbackDone(const actionlib::SimpleClientGoalState& state, const move_base_msgs::MoveBaseResultConstPtr& result) {
    ROS_INFO("Finished in state [%s]", state.toString().c_str());
}


void Car::acCallbackActive() {
    ROS_INFO("Goal just went active");
}


void Car::acCallbackFeedback(const move_base_msgs::MoveBaseFeedbackConstPtr& feedback) {
    ROS_INFO("[X]:%f [Y]:%f [W]: %f [Z]: %f", feedback->base_position.pose.position.x,feedback->base_position.pose.position.y,feedback->base_position.pose.orientation.w, feedback->base_position.pose.orientation.z);
}
