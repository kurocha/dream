//
//  Core/System.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 27/12/10.
//  Copyright (c) 2010 Samuel Williams. All rights reserved.
//
//

#include "System.hpp"

// For errno
#include <sys/errno.h>

// strerror_r
#include <string.h>

namespace Dream
{
	namespace Core
	{
		static std::string system_error_description(int error_number)
		{
			const std::size_t MAX_LENGTH = 1024;
			char buffer[MAX_LENGTH];

			if (strerror_r(error_number, buffer, MAX_LENGTH) == 0)
				return buffer;
			else
				return "Unknown failure";
		}

		SystemError::SystemError(StringT message) : _error_number(-1), _message(message) {
		}

		SystemError::SystemError(StringT domain, ErrorNumberT error_number, StringT error_description, StringT error_target) : _error_number(error_number) {
			StringStreamT f;

			f << domain << " Error #" << error_number << ": " << error_description << "(" << error_target << ")";

			_message = f.str();
		}

		StringT SystemError::what () const
		{
			return _message;
		}

		void SystemError::check (StringT what)
		{
			if (errno != 0) {
				// Get the system error message.
				auto error_description = system_error_description(errno);

				throw SystemError("System", errno, error_description, what);
			}
		}

		void SystemError::reset ()
		{
			errno = 0;
		}
	}
}
