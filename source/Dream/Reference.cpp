//
//  Ref.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 3/10/09.
//  Copyright (c) 2009 Samuel Williams. All rights reserved.
//
//

#include "Reference.hpp"

namespace Dream {
	SharedObject::SharedObject () : _count(0) 
	{
	}

	SharedObject::SharedObject (const SharedObject & other) : _count(0) {
	}

	SharedObject & SharedObject::operator= (const SharedObject & other) {
		// Don't touch reference count.

		return *this;
	}

	SharedObject::~SharedObject () {
	}

	void SharedObject::retain () const {
		_count.fetch_add(1);
	}

	bool SharedObject::release () const {
		// Returns the value before subtracting 1:
		NumberT count = _count.fetch_sub(1);

		if (count == 1) {
			deallocate();
			return true;
		}

		return false;
	}

	void SharedObject::deallocate () const {
		delete this;
	}

	SharedObject::NumberT SharedObject::reference_count () const {
		return _count.load();
	}
}
