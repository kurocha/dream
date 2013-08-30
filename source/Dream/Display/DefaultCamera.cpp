//
//  DefaultCamera.cpp
//  File file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 14/7/2013.
//  Copyright, 2013, by Samuel Williams. All rights reserved.
//

#include "DefaultCamera.h"

namespace Dream
{
	namespace Display
	{
		bool default_camera_motion_handler(DefaultCameraT & camera, const MotionInput & input) {
			using namespace Renderer;
			using namespace Euclid::Numerics;
			
			const Vec3 & d = input.motion();

			if (input.button_pressed_or_dragged(MouseLeftButton)) {
				RealT k = -1.0, i = number(camera.incidence().value).modulo(R360);

				if (i < 0) i += R360;

				// Reverse motion if we are upside down:
				if (camera.reverse() && i > R180 && i < R360)
					k *= -1.0;

				// Find the relative position of the mouse, if it is in the lower half,
				// reverse the rotation.
				Vec2 relative = input.bounds().relative_offset_of(input.current_position().reduce());

				//logger()->log(LOG_DEBUG, LogBuffer() << "Motion: " << d);

				// If mouse button is in lower half of view:
				if (relative[Y] <= 0.5)
					k *= -1.0;

				camera.set_azimuth(camera.azimuth() + degrees(k * d[X] * camera.multiplier()[X]));
				camera.set_incidence(camera.incidence() + degrees(d[Y] * camera.multiplier()[Y]));

				return true;
			} else if (input.key().button() == MouseScroll) {
				camera.set_distance(camera.distance() + (d[Y] * camera.multiplier()[Z]));
					
				return true;
			} else {
				return false;
			}
		}
	}
}
