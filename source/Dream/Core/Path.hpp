//
//  Core/Path.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/09/10.
//  Copyright (c) 2010 Samuel Williams. All rights reserved.
//
//

#ifndef _DREAM_CORE_PATH_H
#define _DREAM_CORE_PATH_H

#include "Strings.hpp"
#include <vector>

namespace Dream
{
	namespace Core
	{
		/**
		    A simple class for managing abstract paths.
		 */
		class Path {
		public:
			typedef std::vector<StringT> ComponentsT;
			struct NameComponents {
				StringT basename;
				StringT extension;
			};

			static const char SEPARATOR = '/';

		protected:
			ComponentsT _components;

		public:
			Path() {}
			Path(const ComponentsT & components);

			Path(const char * string_rep);
			Path(const StringT & string_rep);
			Path(const StringT & string_rep, const char separator);

			bool empty () const { return _components.size() == 0; }
			explicit operator bool() const { return !empty(); }

			StringT to_local_path() const;

			/// Does the path components begin with ""
			bool is_absolute() const;
			/// Returns an absolute path
			Path to_absolute();

			/// Does the path components end with ""
			bool is_directory() const;
			/// Returns a path representing a directory (i.e. trailing slash)
			Path to_directory();

			const ComponentsT & components() const { return _components; }

			// Remove "." and ".."
			Path simplify() const;

			NameComponents last_name_components() const;

			/// Pop n components off the path
			Path parent_path (std::size_t n = 1) const;

			Path operator+(const Path & other) const;
			Path operator+(const NameComponents & other) const;

			bool operator<(const Path & other) const;
			bool operator==(const Path & other) const;

			Path with_extension(const StringT & extension) const;
		};

		std::ostream & operator<<(std::ostream & output, const Path & path);
	}
}

#endif
