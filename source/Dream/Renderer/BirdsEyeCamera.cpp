//
//  Renderer/BirdsEyeCamera.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 16/10/06.
//  Copyright (c) 2006 Samuel Williams. All rights reserved.
//
//

#include "BirdsEyeCamera.h"
#include "../Events/Logger.h"

#include <Euclid/Numerics/Vector.IO.h>

namespace Dream {
	namespace Renderer {
		using namespace Events::Logging;
		using namespace Euclid::Numerics;

		BirdsEyeCamera::BirdsEyeCamera() : _up(0.0, 0.0, 1.0), _right(1.0, 0.0, 0.0), _center(ZERO), _twist(0), _azimuth(R45), _incidence(R45), _multiplier(1), _reverse(false), _invalid(true) {
			_distance = 100;
			_azimuth = R45;
			_incidence = R45;
			_twist = 0;

			regenerate();
		}

		BirdsEyeCamera::~BirdsEyeCamera () {
		}

		void BirdsEyeCamera::regenerate () {
			_forward = cross_product(_up, _right);

			logger()->log(LOG_DEBUG, LogBuffer() << "Forward: " << _forward);

			_right = -cross_product(_up, _forward);

			logger()->log(LOG_DEBUG, LogBuffer() << "Right: " << _right);
			logger()->log(LOG_DEBUG, LogBuffer() << "Up: " << _up);

			_invalid = true;
		}

		Mat44 BirdsEyeCamera::view_matrix () const
		{
			if (_invalid) {
				Vec3 world_up(0.0, 1.0, 0.0), world_forward(0.0, 0.0, -1.0);
				Vec3 far = _forward * _distance;

				_view_matrix_cache = rotate(_up, world_up, _forward) << translate(far) << rotate(_incidence, _right) << rotate(_azimuth, _up) << translate(-_center) << rotate(_twist, _up);

				/*			
				 m = m.rotating_matrix(_up, world_up, _forward);
				 m = m.translated_matrix(far);
				 m = m.rotated_matrix(_incidence, _right);
				 m = m.rotated_matrix(_azimuth, _up);
				 m = m.translated_matrix(-_center);
				 m = m.rotated_matrix(_twist, _up);
				 */

				_invalid = false;
			}

			return _view_matrix_cache;
		}

		void BirdsEyeCamera::set_multiplier (const Vec3 &m) {
			_multiplier = m;
			
			_invalid = true;
		}

		const Vec3 & BirdsEyeCamera::multiplier () {
			return _multiplier;
		}

		void BirdsEyeCamera::set_center (const Vec3 &new_center) {
			_center = new_center;
			
			_invalid = true;
		}

		void BirdsEyeCamera::set_up (const Vec3 &up) {
			if (_up != up) {
				_up = up;
				regenerate();
			}
		}

		void BirdsEyeCamera::set_right (const Vec3 &right) {
			if (_right != right) {
				_right = right;
				regenerate();
			}
		}

		void BirdsEyeCamera::set_distance (const RealT& amount, bool relative) {
			if (relative)
				_distance += amount;
			else
				_distance = amount;
			
			_invalid = true;
		}

		void BirdsEyeCamera::set_twist (const RealT& amount, bool relative) {
			if (relative)
				_twist += amount;
			else
				_twist = amount;
			
			_invalid = true;
		}

		void BirdsEyeCamera::set_azimuth (const RealT& amount, bool relative) {
			if (relative)
				_azimuth += amount;
			else
				_azimuth = amount;
			
			_invalid = true;
		}

		void BirdsEyeCamera::set_incidence (const RealT &amount, bool relative) {
			if (relative)
				_incidence += amount;
			else
				_incidence = amount;
			
			_invalid = true;
		}
	}
}
