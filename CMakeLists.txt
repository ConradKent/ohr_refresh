cmake_minimum_required(VERSION 3.0.2)
project(ohr_refresh)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  gazebo_plugins
  gazebo_ros
  roscpp
)

find_library(OPENVR_LIBRARY openvr_api HINTS /usr/local/lib/)

find_package (gazebo REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS gazebo_plugins gazebo_ros roscpp
)

###########
## Build ##
###########

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(include)
include_directories( ${catkin_INCLUDE_DIRS} 
                     ${Boost_INCLUDE_DIR} 
                     ${GAZEBO_INCLUDE_DIRS}
                     ${GLEW_LIBRARIES} 
                     ${GLUT_LIBRARIES} 
                     ${SDL2_LIBRARIES}
                     ${OPENVR_INCLUDE_DIR}
)

add_library(ohr_refresh_plugin src/ohr_refresh_plugin.cpp)
target_link_libraries( ohr_refresh_plugin 
			${catkin_LIBRARIES} 
			${GAZEBO_LIBRARIES} 
	             	${GLEW_LIBRARIES} 
	             	${GLUT_LIBRARIES} 
	             	${SDL2_LIBRARIES}
	             	${OPENVR_LIBRARY}
			CameraPlugin 
)
