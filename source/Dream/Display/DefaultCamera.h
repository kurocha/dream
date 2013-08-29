//
//  DefaultCamera.h
//  File file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/7/2013.
//  Copyright, 2013, by Samuel Williams. All rights reserved.
//

#ifndef DREAM_CLIENT_DISPLAY_DEFAULTCAMERA_H
#define DREAM_CLIENT_DISPLAY_DEFAULTCAMERA_H

#include "Input.h"
#include "../Renderer/BirdsEyeCamera.h"

namespace Dream
{
	namespace Display
	{
		// The default camera type, often used in very basic scenes for debugging, etc.
		typedef Renderer::BirdsEyeCamera DefaultCameraT;
		
		// Update the camera based on motion input:
		bool default_camera_motion_handler(DefaultCameraT & camera, const MotionInput & input);
	}
}

#endif
