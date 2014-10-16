//
//  Class.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 10/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "Class.hpp"

#include "Core/Strings.hpp"
#include "Core/Timer.hpp"

#include <map>

namespace Dream
{
// MARK: -
// MARK: class Object

	IObject::~IObject ()
	{
	}

	IClassType::~IClassType()
	{
	}

	Object::Object() : _finalizers(nullptr) {
	}

	Object::~Object () {
		FinalizerReference * current = _finalizers;

		while (current) {
			if (current->finalizer)
				current->finalizer->finalize(this);

			FinalizerReference * next = current->next;

			delete current;
			current = next;
		}
	}

	void Object::insert_finalizer(IFinalizer * finalizer) {
		FinalizerReference * next = new FinalizerReference;

		next->finalizer = finalizer;
		next->next = _finalizers;

		_finalizers = next;
	}

	bool Object::erase_finalizer(IFinalizer * finalizer) {
		FinalizerReference ** previous = &_finalizers;
		FinalizerReference * current = _finalizers;

		// It may be possible to improve upon this approach since it is a linear scan.
		while (current) {
			if (current->finalizer == finalizer) {
				*previous = current->next;

				delete current;

				return true;
			}

			previous = &(current->next);
			current = current->next;
		}

		return false;
	}
}
