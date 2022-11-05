- The `script` folder contains install.bash to install required packages.
# Final Project #

## Introduction 
  	This file includes C++ program solution with launch files for 
  	final project UMD ENPM 809Y Spring 2021 batch. The folders consists 
  	of C++ program along with simulation worlds to test the code.
	
	These files are executable:
		main.cpp

  
## Requirements
       ***Important*************************************************************
       *It is required that final_project  module is in the catkin workspace   *
       *To test it on your worlds, update yaml file in param folder accordingly*
       *************************************************************************
     
	
### Running code in ubuntu
After making sure that the path of the final_project module is compiled using catkin_make command
Make sure that current working derectory is same as the directory of program
You can change the working derectory by using **cd** command
* Run the following command which will compile the package
````
cd ~/catkin_ws/src/final_project/script
./install.bash
````
````
catkin_make
````
* Run folloing line to launch explorer and follower in simulation world
````
roslaunch final_project multiple_robots.launch
````
* Run following command initiate the node
````
rosrun final_project final_project_node
````
* Call service to start the tasks
````
rosservice call /start "data: true"
````

### Troubleshooting ###
	Most commonly the issue will be with the Gazebo simulation Close all the terminals of ROS and restart them to solve majority of issues.
	For issues that you may encounter which can't be solved with simple restart, create an issue on GitHub.
  
### Maintainers ###
	Mrugesh Shah (mrugesh.shah92@gmail.com)
	Jeet Patel   (jeetpatel242@gmail.com)
	Dhyey Patel  (dhyeypate1825@gmail.com)
