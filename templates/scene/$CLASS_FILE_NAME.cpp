//
//  $CLASS_FILE_NAME.cpp
//  File file is part of the "$PROJECT_NAME" project and released under the $LICENSE.
//
//  Created by $AUTHOR_NAME on $DATE.
//  Copyright, $YEAR, by $AUTHOR_NAME. All rights reserved.
//

#include "$CLASS_NAME.h"

<NAMESPACE>
$CLASS_NAME::$CLASS_NAME()
{
}

$CLASS_NAME::~$CLASS_NAME()
{
}

void $CLASS_NAME::will_become_current (ISceneManager * manager)
{
	Ref<PointCamera> camera = new PointCamera;
	camera->set_direction({0.0, 0.0, -1.0});
	camera->set_up({0.0, 1.0, 0.0});
	camera->set_origin({0.0, 0.0, 100.0});

	AlignedBox3 box = AlignedBox3::from_center_and_size({0, 0, 0}, {1280, 720, 1});
	Ref<IProjection> projection = new ScaledOrthographicProjection(box);

	_viewport = new Viewport(camera, projection);
}

void $CLASS_NAME::will_revoke_current (ISceneManager * manager)
{
}

bool $CLASS_NAME::resize (const ResizeInput &input)
{
	_viewport->set_bounds(AlignedBox<2>(ZERO, input.new_size()));

	glViewport(0, 0, (GLsizei)input.new_size()[WIDTH], (GLsizei)input.new_size()[HEIGHT]);
	check_graphics_error();

	return Scene::resize(input);
}

void $CLASS_NAME::render_frame_for_time (TimeT time)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	manager()->process_pending_events(this);
	
	
}
</NAMESPACE>
