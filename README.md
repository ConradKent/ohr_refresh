## Required Software:

### OpenGL/GLEW/SDL2:

OpenGL: https://www.opengl.org//
GLEW: http://glew.sourceforge.net/
SDL: https://www.libsdl.org/

Some more work needs to be done on paring down what exact things from OpenGL/GLEW/SDL need to be installed and used. For now, it's a safe bet to just install GLEW and SDL2, which can be done using

		$sudo apt install libglew-dev
	
as well as

		$sudo apt install libsdl2-dev

### OpenVR

https://github.com/ValveSoftware/openvr
Must have SDL2 and GLEW dev packages already installed.
Install by using:

		$git clone https://github.com/ValveSoftware/openvr
		$cd openvr
		$mkdir build && cd build
		$cmake ..
		$make
		$sudo make install
		
### ROS/Gazebo

Install ROS Noetic and Gazebo using this tutorial: http://wiki.ros.org/noetic/Installation/Ubuntu
Set up a Catkin Workspace (~/catkin_ws) using this tutorial: http://wiki.ros.org/catkin/Tutorials/create_a_workspace. The rest of this document assumes that your workspace is called "catkin_ws" and is located in your home folder.

### Add model path to .bashrc

We need to tell gazebo where some of our models are. First do this:
		
		$cd ~/catkin_ws
		$gedit ~/.bashrc

Now paste the following two lines at the end of your .bashrc file (replace with the path to your model folder):

		$export GAZEBO_MODEL_PATH=~/catkin_dev/src/openvr_headset_ros/models:${GAZEBO_MODEL_PATH}
		
		$export GAZEBO_RESOURCE_PATH=~/catkin_dev/src/openvr_headset_ros/models:${GAZEBO_RESOURCE_PATH}

### How to run

Make sure SteamVR is already running (you just need the headset to be active, it doesn't matter if the controllers are attached)
then run:

cd catkin_dev
source /opt/ros/noetic/setup.bash
source devel/setup.bash
export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:/home/conrad/catkin_dev/devel/lib
roslaunch ohr_refresh main.launch

(change paths to your computer's)
