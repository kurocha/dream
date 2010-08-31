/*
 *  Geometry/AlignedBox.cpp
 *  This file is part of the "Dream" project, and is licensed under the GNU GPLv3.
 *
 *  Created by Samuel Williams on 12/07/08.
 *  Copyright 2008 Samuel Williams. All rights reserved.
 *
 */

#include "AlignedBox.h"

namespace Dream {
	namespace Geometry {

#pragma mark -
#pragma mark Unit Tests
		
#ifdef ENABLE_TESTING
		
		UNIT_TEST(AlignedBox) {
			testing("Construction and Assignment");
			
			AlignedBox<2> a(ZERO), b(IDENTITY), c(ZERO);
			check(a.min() == Vec2(0.0) << 0.0), "Zero box";
			check(a.max() == Vec2(0.0) << 0.0), "Zero box";

			check(b.min() == Vec2(0.0) << 0.0), "Identity box";
			
			check(a.isDegenerate()) << "Zero box is degenerate";
			check(b.isNormal()) << "Identity box is normal";
			
			testing("Intersections: Contains point");
			
			AlignedBox<3> box1(vec(0.0, 0.0, 0.0), vec(1.0, 1.0, 1.0)), 
				box2(vec(0.0, 0.0, 0.0), vec(10.0, 10.0, 10.0)), 
				box3(vec(5.0, 5.0, 5.0), vec(10.0, 10.0, 10.0)),
				box4(vec(9.0, 9.0, 9.0), vec(10.0, 10.0, 10.0)),
				box5(vec(-5.0, 0.0, 0.0), vec(5.0, 10.0, 10.0)),
				box5a(vec(0.0, 0.0, 0.0), vec(5.0, 10.0, 10.0)),
				box6(vec(5.0, 0.0, 0.0), vec(10.0, 10.0, 10.0));
			
			check(box1.containsPoint(vec(0.0, 0.0, 0.0), true )) << "Contains point was correct";
			check(!box1.containsPoint(vec(0.0, 0.0, 0.0), false)) << "Contains point was correct";
			check(box1.containsPoint(vec(1.0, 1.0, 0.5), true )) << "Contains point was correct";
			check(!box1.containsPoint(vec(1.0, 1.0, 0.5), false)) << "Contains point was correct";
			check(box1.containsPoint(vec(1.0, 1.0, 1.0), true )) << "Contains point was correct";
			check(!box1.containsPoint(vec(1.0, 1.0, 1.0), false)) << "Contains point was correct";
			check(!box1.containsPoint(vec(1.0, 2.0, 1.0), true )) << "Contains point was correct";
			check(!box1.containsPoint(vec(1.0, 1.0, 2.0), false)) << "Contains point was correct";
			check(!box1.containsPoint(vec(3.0, 1.0, 0.0), false)) << "Contains point was correct";
			
			testing("Intersections: Other boxes");
			
			check(box1.intersectsWith(box2)) << "Intersection result was correct";
			check(!box1.intersectsWith(box3)) << "Intersection result was correct";
						
			testing("Box Orientations");
			
			AlignedBox<3> box1a(box1);
			
			check(box2.orientationOf(box3) == vec(1.0, 1.0, 1.0)) << "Box orientation was correct";
			box1a.alignWithinSuperBox(box2, vec(1.0, 1.0, 1.0));
			check(box1a == box4) << "Box was aligned correctly";
			box1a.alignWithinSuperBox(box2, vec(0.0, 0.0, 0.0));
			check(box1 == box1a) << "Box was aligned correctly";
			
			testing ("Subtraction");
			
			AlignedBox<3> box2a(box2);
			// Subtract left half
			box2a.subtractInOrder(box5, vec<unsigned>(0, 1, 2));
			
			// We should have the right half
			check(box2a == box6) << "Box was subtracted correctly";
			
			AlignedBox<3> box2b(box2);
			// Subtract right half
			box2b.subtractInOrder(box6, vec<unsigned>(0, 1, 2));
			
			 // We should have the left half
			check(box2b == box5a) << "Box was subtracted correctly";
			
			AlignedBox<3> c1(Vec3(0, 0, 0), Vec3(50, 10, 10)),
				c2(Vec3(5, 5, 0), Vec3(15, 15, 15)),
				c3(Vec3(5, 10, 0), Vec3(15, 15, 15));
			
			AlignedBox<3> c2a(c2);
			
			c2a.subtractInOrder(c1, vec<unsigned>(0, 1, 2));
			check(c2a == c3) << "Box was subtracted correctly";
		}
		
#endif
		
	}
}