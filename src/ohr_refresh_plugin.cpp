//roslaunch ohr_refresh main.launch
//built on top of https://www.theconstructsim.com/create-a-ros-sensor-plugin-for-gazebo/


#include <gazebo/common/Plugin.hh>
#include "ohr_refresh/ohr_refresh_plugin.h"

#include <string>

#include <gazebo/sensors/Sensor.hh>
#include <gazebo/sensors/CameraSensor.hh>
#include <gazebo/sensors/SensorTypes.hh>

#include <sensor_msgs/Illuminance.h>

// OpenGL stuff
#include <SDL2/SDL.h>
#include <iostream> // not sure if I need this
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glut.h>
#include <openvr/openvr.h>


	//define the framebuffers
    GLuint FramebufferLeft = 0;
    GLuint FramebufferRight = 0;

    //define the textures
    GLuint tex_left;
    GLuint tex_right;


namespace Gazebo_OpenVR_Hack
{
	
	vr::IVRSystem* vr_rendering_system;
	
	void CheckVrStatus(uint32_t pnWidth, uint32_t pnHeight)
	{
			// Check whether there is an HMD plugged-in and the SteamVR runtime is installed
	if (vr::VR_IsHmdPresent())
	{
			std::cout << "An HMD was successfully found in the system" << std::endl;

			if (vr::VR_IsRuntimeInstalled()) {
					std::cout << "Runtime correctly installed" << std::endl;
					std::cout << "Width: " << pnWidth << std::endl;
					std::cout << "Height: " << pnHeight << std::endl;
			}
			else
			{
					std::cout << "Runtime was not found, quitting app" << std::endl;
			}
	}
	else
	{
			std::cout << "No HMD was found in the system, quitting app" << std::endl;
	}
	
	
	}

	
	
	// GLEW initialization, the args for glutInit aren't amazing but it's the only solution I can come up with without accessing gazebo's main().
	void GLEW_initializer()
	{

		int i = 0;
		glutInit(&i, NULL);
		glutCreateWindow("GLEW Test");
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}
		fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}

}
namespace gazebo
{
  // Register this plugin with the simulator
  GZ_REGISTER_SENSOR_PLUGIN(OpenVRCameraPlugin)

  ////////////////////////////////////////////////////////////////////////////////
  // Constructor
  OpenVRCameraPlugin::OpenVRCameraPlugin()
  {
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Destructor
  OpenVRCameraPlugin::~OpenVRCameraPlugin()
  {
    std::cout << "Camera unloaded" << std::endl;
  }

  void OpenVRCameraPlugin::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
  {
	    CameraPlugin::Load(_parent, _sdf);  


	uint32_t pnWidth;
    uint32_t pnHeight;
    vr::HmdError err;
	Gazebo_OpenVR_Hack::vr_rendering_system =  vr::VR_Init(&err,vr::EVRApplicationType::VRApplication_Scene);
	Gazebo_OpenVR_Hack::vr_rendering_system->GetRecommendedRenderTargetSize(&pnWidth, &pnHeight ); // in case I figure out a way to do this later.
	Gazebo_OpenVR_Hack::CheckVrStatus(pnWidth,pnHeight);
	

    
    // Initialize OpenGL tools through GLEW
	Gazebo_OpenVR_Hack::GLEW_initializer();

    glGenFramebuffers(1, &FramebufferLeft);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferLeft);

    glGenTextures(1,&tex_left);

    glBindTexture(GL_TEXTURE_2D, tex_left);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA,pnWidth,pnHeight,0,GL_BGR,GL_UNSIGNED_BYTE,this->parentSensor->sensors::CameraSensor::ImageData());

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_left, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferLeft);
    glViewport(0,0,pnHeight,pnWidth); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    //Right eye texture and framebuffer binding

    glGenFramebuffers(1, &FramebufferRight);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferRight);

    glGenTextures(1,&tex_right);

    glBindTexture(GL_TEXTURE_2D, tex_right);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA,pnWidth,pnHeight,0,GL_BGR,GL_UNSIGNED_BYTE,this->parentSensor->sensors::CameraSensor::ImageData());


    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_right, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferRight);
    glViewport(0,0,pnWidth,pnHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
    
    fprintf(stdout, "OpenGL texture and framebuffer objects created\n");

    }

  ////////////////////////////////////////////////////////////////////////////////
  // Update the controller
  void OpenVRCameraPlugin::OnNewFrame(const unsigned char *_image,
    unsigned int _width, unsigned int _height, unsigned int _depth,
    const std::string &_format)
  { 
	  
	  vr::TrackedDevicePose_t pose[vr::k_unMaxTrackedDeviceCount];
	  vr::VRCompositor()->WaitGetPoses(pose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	  ///////// Send to headset
		glBindTexture(GL_TEXTURE_2D, tex_left);
		glTexSubImage2D(GL_TEXTURE_2D, 0,0,0,_width,_height,GL_BGR,GL_UNSIGNED_BYTE,this->parentSensor->sensors::CameraSensor::ImageData());
		vr::Texture_t leftEyeTexture = {(void*)(uintptr_t)tex_left, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture );

		glBindTexture(GL_TEXTURE_2D, tex_right);
		glTexSubImage2D(GL_TEXTURE_2D, 0,0,0,_width,_height,GL_BGR,GL_UNSIGNED_BYTE,this->parentSensor->sensors::CameraSensor::ImageData());
		vr::Texture_t rightEyeTexture = {(void*)(uintptr_t)tex_right, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture );
		
		glFinish();
		
 }
}
