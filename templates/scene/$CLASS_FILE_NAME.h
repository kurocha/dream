//
//  $CLASS_FILE_NAME.cpp
//  File file is part of the "$PROJECT_NAME" project and released under the $LICENSE.
//
//  Created by $AUTHOR_NAME on $DATE.
//  Copyright, $YEAR, by $AUTHOR_NAME. All rights reserved.
//

#ifndef $GUARD_NAME
#define $GUARD_NAME

#include <Dream/Client/Display/Scene.h>

<NAMESPACE>
using namespace Dream::Renderer;
using namespace Dream::Client::Display;

class $CLASS_NAME : public Dream::Client::Scene
{
public:
	$CLASS_NAME();
	virtual ~$CLASS_NAME();
	
	bool resize (const ResizeInput & input);
	
	void will_become_current (ISceneManager * manager);
	void will_revoke_current (ISceneManager * manager);
	void render_frame_for_time (TimeT time);
	
private:
	Ref<Viewport> _viewport;
};
</NAMESPACE>

#endif
