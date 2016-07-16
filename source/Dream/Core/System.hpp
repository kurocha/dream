//
//  Core/System.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 27/12/10.
//  Copyright (c) 2010 Samuel Williams. All rights reserved.
//
//

#pragma once

#include "../Framework.hpp"
#include "Strings.hpp"

namespace Dream
{
	namespace Core
	{
		typedef int ErrorNumber;

		class SystemError : public std::exception {
		protected:
			ErrorNumber _error_number;
			StringT _message;
			StringT _formatted_message;

		public:
			SystemError(StringT domain, ErrorNumber error_number, StringT error_description, StringT error_target);
			SystemError(StringT message);

			const char * what () const noexcept;

			static void check (StringT what);
			static void check (StringT what, ErrorNumber error_number);
			static void reset ();
		};
	}
}
