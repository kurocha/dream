//
//  Display/Application.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 14/09/11.
//  Copyright (c) 2011 Samuel Williams. All rights reserved.
//

#include "Application.h"

#include <Dream/Events/Thread.h>

#include <Dream/Imaging/Image.h>
#include <Dream/Text/Font.h>
#include <Dream/Audio/Sound.h>
#include <Dream/Audio/OggResource.h>
#include <Dream/Graphics/ShaderFactory.h>


namespace Dream
{
	namespace Display
	{
		static void _show_startup_message_if_needed()
		{
			static bool _startup_messages_displayed = false;

			if (_startup_messages_displayed == false) {
				_startup_messages_displayed = true;

				std::cout << "Dream Framework. Copyright © 2006-2013 Samuel Williams. All Rights Reserved." << std::endl;
				std::cout << "For more information visit http://www.oriontransfer.co.nz/research/dream" << std::endl;
			}
		}

		IApplication::IApplication()
		{
			logger()->set_thread_name("Application");

			_show_startup_message_if_needed();
		}

		IApplicationDelegate::~IApplicationDelegate ()
		{
		}

		void IApplicationDelegate::application_did_finish_launching (IApplication * application)
		{
		}

		void IApplicationDelegate::application_will_terminate (IApplication * application)
		{
		}

		void IApplicationDelegate::application_will_enter_background (IApplication * application)
		{
		}

		void IApplicationDelegate::application_did_enter_foreground (IApplication * application)
		{
		}

		IApplication::~IApplication ()
		{
		}
	}
}
