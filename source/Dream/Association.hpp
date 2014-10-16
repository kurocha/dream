//
//  Association.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 9/01/13.
//  Copyright (c) 2013 Samuel Williams. All rights reserved.
//

#ifndef _DREAM_ASSOCIATION_H
#define _DREAM_ASSOCIATION_H

#include "Class.hpp"

namespace Dream
{
	// An association maintains a weak reference with a given object.
	template <typename ObjectT>
	class Association : public Pointer<ObjectT>, virtual public IFinalizer
	{
	private:
		void associate ()
		{
			// Calling this function more than once may cause memory related crashes.
			if (this->_object)
				this->_object->insert_finalizer(this);
		}

	public:
		virtual void finalize(Object * object)
		{
			this->_object = NULL;
		}
		
		void clear ()
		{
			if (this->_object) {
				this->_object->erase_finalizer(this);
				this->_object = NULL;
			}
		}

		Association& set (ObjectT * object)
		{
			clear();

			if (object) {
				object->insert_finalizer(this);
				this->_object = object;
			}

			return *this;
		}

		Association ()
		{
		}

		Association (ObjectT * object) : Pointer<ObjectT>(object)
		{
			associate();
		}

		Association (const Association& other) : Pointer<ObjectT>(other.get())
		{
			associate();
		}

		template <typename OtherObjectT>
		Association (const Pointer<OtherObjectT> & other) : Pointer<ObjectT>(other.get())
		{
			associate();
		}

		Association& operator= (const Association & other)
		{
			return set(other.get());
		}

		template <typename OtherObjectT>
		Association& operator= (const Pointer<OtherObjectT> & other)
		{
			return set(other.get());
		}

		Association& operator= (ObjectT * object)
		{
			return set(object);
		}

		template <typename OtherObjectT>
		Association& operator= (OtherObjectT * object)
		{
			return set(dynamic_cast<ObjectT *>(object));
		}

		~Association ()
		{
			clear();
		}
	};
}

#endif
