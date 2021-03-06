//
//  Assertion.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 26/10/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Assertion.hpp"

#include <sstream>
#include <iostream>

static void assertion_failure() {
	return;
}

namespace Dream
{
	AssertionError::AssertionError (const char * expression, const char * file, unsigned line) throw () : _expression (expression), _file (file), _line (line)
	{
		using namespace std;

		try {
			stringstream s;
			s << _file << ":" << _line << " assertion failed: " << _expression;
			_what = s.str ();
		} catch (...)   {
			_what = "Could not format assertion text!";
		}
	}

	AssertionError::~AssertionError () throw ()
	{
	}

	const char * AssertionError::what () const throw ()
	{
		return _what.c_str();
	}

	void AssertionError::assert_handler (bool condition, const char * expression, const char * file, unsigned line)
	{
		if (!condition) {
			assertion_failure();

			AssertionError assertion_error(expression, file, line);

			//std::cerr << assertion_error.what();

			throw assertion_error;
		}
	}
}
