// Importing require libraries and ROS messages
#include <actionlib/client/simple_action_client.h>          //for moving robot to reach goal
#include <move_base_msgs/MoveBaseAction.h>
#include <fiducial_msgs/FiducialTransformArray.h>           //for getting fiducial transforms
#include <fiducial_msgs/FiducialTransform.h>
#include <geometry_msgs/Twist.h>                            //for geometry_msgs::Twist
#include <tf2/LinearMath/Quaternion.h>                      //builtin tools to get transform matrix b/w frames
#include <tf2_ros/transform_broadcaster.h>
#include <tf2_ros/transform_listener.h>
#include <ros/ros.h>                                    
#include <std_srvs/SetBool.h>


typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

/**
 * @brief Explorer class that takes care of the first part of the problem
 * 
 */
class Explorer {
public:
  Explorer(ros::NodeHandle& nh) : _nh(nh), explorer_client("/explorer/move_base", true) {
  while (!explorer_client.waitForServer(ros::Duration(5.0))) {                                    // wait for the action server to come up
    ROS_INFO("Waiting for the move_base action server to come up for explorer");
  }                                                                                               //Build goal for explorer
  explorer_goal.target_pose.header.frame_id = "map";
  explorer_goal.target_pose.pose.orientation.w = 1.0;
  tf2_ros::TransformListener tfListener(tfBuffer);

  }
  ~Explorer() {}
  void start(fiducial_msgs::FiducialTransformArray& markerPoseArray) {                            //Explorer will inititate fiducial transforms
for(uint8_t i{1};i<5;++i){
    paramName="/aruco_lookup_locations/target_"+std::to_string(i);                                //creating a parameter to look for (in proper format)
    ROS_INFO("param name %s", paramName.c_str());
    fetchParam(_nh,paramName,x,y);                                                                

  explorer_goal.target_pose.header.stamp = ros::Time::now();                                      //getting pose of the first location
  explorer_goal.target_pose.pose.position.x = x;
  explorer_goal.target_pose.pose.position.y = y;
  ROS_INFO("Sending Goal");
  explorer_client.sendGoal(explorer_goal);                                                        //ask explorer to go there
  explorer_client.waitForResult();                                                                //wait for amcl to confirm if explorer reach there
  rotateExplorer(_nh,markerPose,tfBuffer);                                                        //rotate bot and look for marker get the marker pose and decode marker
  markerPoseArray.transforms.push_back(markerPose);                                               //store pose of the marker
  ROS_INFO("MARKER POSE %f", markerPoseArray.transforms[0].transform.translation.x);                   

  }

  explorer_goal.target_pose.header.stamp = ros::Time::now();                                      //after all four positions are done, send the explorer to start position
  explorer_goal.target_pose.pose.position.x = -4.0;
  explorer_goal.target_pose.pose.position.y = 2.5;
  ROS_INFO("Sending Explorer to Start Position");
  explorer_client.sendGoal(explorer_goal);
  explorer_client.waitForResult();


  }
private:
  /**
   * @brief This function fetch x & y location for a given goal number
   * 
   * @param _nh nodehandle
   * @param paramName number of goal or target
   * @param x x position
   * @param y y position
   */
  void fetchParam(const ros::NodeHandle& _nh, std::string& paramName, double& x, double& y) {
    XmlRpc::XmlRpcValue value;
    if(!_nh.getParam(paramName,value)){
      ROS_ERROR("failed to fetch the param");
      exit(1);
    }
    x=static_cast<double>(value[0]);
    y=static_cast<double>(value[1]);
  }

  /**
   * @brief Get the Fiducal Marker object
   * 
   * @param _nh nodehandle
   * @param pose pose of detected fiducial
   * @return true if we decoded it
   * @return false if we did not
   */
  bool getFiducalMarker(ros::NodeHandle& _nh, fiducial_msgs::FiducialTransform& pose,tf2_ros::Buffer& tfBuffer) {
    fiducial_msgs::FiducialTransformArrayConstPtr fiducialTransformArrayMsg=ros::topic::waitForMessage<fiducial_msgs::FiducialTransformArray>("/fiducial_transforms",_nh);
    if(fiducialTransformArrayMsg->transforms.size()>0){
      for(auto& it : fiducialTransformArrayMsg->transforms){
        ROS_INFO("FOUND THE MARKER %d",it.fiducial_id); 
        pose.fiducial_id = it.fiducial_id;
        geometry_msgs::TransformStamped transformStamped;
        float saftyDistance=0.4;                                                                    //adding safety distance so followe does not slam into wall
        try {
          transformStamped = tfBuffer.lookupTransform("map", "my_frame", ros::Time(0));
          pose.transform.translation.x = transformStamped.transform.translation.x - saftyDistance;
          pose.transform.translation.y = transformStamped.transform.translation.y - saftyDistance;
          pose.transform.rotation.w = 1.0;
        }
        catch (tf2::TransformException& ex) {
          ROS_WARN("%s", ex.what());
          ros::Duration(1.0).sleep();
        }

    }
        return true;
    }
    return false;
  }

 /**
  * @brief This function will rotate bot, look for marker, get the marker pose with transforms
  * 
  * @param _nh nodehandle
  * @param pose pose of the marker
  */
  void rotateExplorer(ros::NodeHandle& _nh, fiducial_msgs::FiducialTransform& pose, tf2_ros::Buffer& tfBuffer) {
    bool markerFound=false;
    ros::Publisher cmdPub=_nh.advertise<geometry_msgs::Twist>("/explorer/cmd_vel",10);
    geometry_msgs::Twist twistMsg;                                                                    //twist messages to rotate bot
    twistMsg.angular.z=0.1;
    
    while (!markerFound)
    {
      if(getFiducalMarker(_nh, pose, tfBuffer)){
        markerFound=true;                                                                             //return true after gettting encoded data of marker
        break;
      }
      cmdPub.publish(twistMsg);
      ros::Duration(1.0).sleep();
    }
    twistMsg.angular.z=0.1;
    cmdPub.publish(twistMsg);
    ros::Duration(1.0).sleep();
    
  }
ros::NodeHandle& _nh;                                       //another nodehandle for explorer
MoveBaseClient explorer_client;                             //movebase clien initialisation
move_base_msgs::MoveBaseGoal explorer_goal;                 //movebase message to send gole info
std::string paramName;
fiducial_msgs::FiducialTransform markerPose;                
tf2_ros::Buffer tfBuffer;
double x, y;
};

/**
 * @brief Follower calls to take care of the second half of project
 * 
 */
class Follower {
public:
  Follower(ros::NodeHandle& _nh) : nh(_nh), follower_client("/follower/move_base", true) {
    while (!follower_client.waitForServer(ros::Duration(5.0))) {
    ROS_INFO("Waiting for the move_base action server to come up for follower");
  }
  follower_goal.target_pose.header.frame_id = "map";                                          //Build goal for follower
  follower_goal.target_pose.pose.orientation.w = 1.0;

  }
  ~Follower() {}

  void start(fiducial_msgs::FiducialTransformArray& markerPoseArray) {
    for(uint8_t i{0};i<4;++i){                                                                //fetch all four goals for the follower created by explorer
    for (auto& it: markerPoseArray.transforms){
      if(it.fiducial_id==i){                                                                  //if the fiducial id fall in order, go for that location

      follower_goal.target_pose.pose.position.x = it.transform.translation.x;
      follower_goal.target_pose.pose.position.y = it.transform.translation.y;
      follower_goal.target_pose.header.stamp = ros::Time::now();
      ROS_INFO("Sending Goal for Marker %d", i);
      follower_client.sendGoal(follower_goal);                                                //ask movebase client for follower to go there
      follower_client.waitForResult();                                                        //wait for the confirmation
      }
    }  
  }
  follower_goal.target_pose.pose.position.x = -4.0;                                           //after going through all goals, send follower to the start location
  follower_goal.target_pose.pose.position.y = 3.5;
  follower_goal.target_pose.header.stamp = ros::Time::now();
  ROS_INFO("Sending Follower to Start Position");
  follower_client.sendGoal(follower_goal);
  follower_client.waitForResult();  
  }
private:
  ros::NodeHandle& nh;
  MoveBaseClient follower_client;
  move_base_msgs::MoveBaseGoal follower_goal;
};


void fiducialClbk(const fiducial_msgs::FiducialTransformArrayConstPtr& msg){                    //if you find the fiducial, do the frame transform to get pose wrt the map
  if (!msg->transforms.empty()) {
    static tf2_ros::TransformBroadcaster br;
    geometry_msgs::TransformStamped transformStamped;
    transformStamped.header.stamp = ros::Time::now();
    transformStamped.header.frame_id = "explorer_tf/camera_rgb_optical_frame";                  //tf transform b/w aruco/*camera/explorer*/map
    transformStamped.child_frame_id = "my_frame";
    transformStamped.transform.translation = msg->transforms[0].transform.translation;
    transformStamped.transform.rotation.w = 1.0;
    // transformStamped.transform.rotation = msg->transforms[0].transform.rotation;
    br.sendTransform(transformStamped);
    // ROS_INFO("broadcasting");
  }

}


/**
 * @brief Initiate the service
 * 
 */
class StartService {
  public:
  StartService(ros::NodeHandle& _nh): nh(_nh){
    service = nh.advertiseService("start", &StartService::startClbk, this);           // Initiate service with defined call back function
    
  }
  ~StartService(){}
  
  private:
  ros::NodeHandle& nh;
  ros::ServiceServer service;

  fiducial_msgs::FiducialTransformArray markerPoseArray;
  bool startClbk(std_srvs::SetBool::Request& req, std_srvs::SetBool::Response& res) {
    if(req.data) {                                                                     // Wait for the request from another terminal
      Explorer explorer(nh);                                                           // Create object for explorer
      Follower follower(nh);                                                           // Create object for follower 
      explorer.start(markerPoseArray);                                                 // Initiate the explorer
      follower.start(markerPoseArray);                                                 // Initiate the follower
      res.message="task completed";
      res.success=true;
    }
    else {
      res.message="task not completed";
      res.success=false;
    }
    return true;
  }

  

};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "simple_navigation_goals");
  ros::NodeHandle nh;

  StartService startService(nh);
  
  ros::Subscriber fiducialSubscriber = nh.subscribe("/fiducial_transforms",10, fiducialClbk);

  ros::spin();
  
  ros::shutdown();

  return 0;


}