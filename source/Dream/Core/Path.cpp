//
//  Core/Path.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 2/09/10.
//  Copyright (c) 2010 Samuel Williams. All rights reserved.
//
//

#include "Path.hpp"
#include <iterator>

#include <cstddef>

namespace Dream
{
	namespace Core
	{
		Path::Path(const ComponentsT & components) : _components(components) {
		}

		Path::Path(const char * string_rep) {
			split(string_rep, Path::SEPARATOR, std::inserter(_components, _components.begin()));
		}

		Path::Path(const StringT & string_rep) {
			split(string_rep, Path::SEPARATOR, std::inserter(_components, _components.begin()));
		}

		Path::Path(const StringT & string_rep, const char separator) {
			split(string_rep, separator, std::inserter(_components, _components.begin()));
		}

		StringT Path::to_local_path() const {
			StringStream buffer;

			for (std::size_t i = 0; i < _components.size(); i++) {
				if (i > 0) buffer << Path::SEPARATOR;

				buffer << _components[i];
			}

			return buffer.str();
		}

		bool Path::is_absolute() const {
			if (_components.size() > 0)
				return _components.front() == "";
			else
				return false;
		}

		Path Path::to_absolute() {
			ComponentsT result;
			result.push_back("");
			result.insert(result.end(), _components.begin(), _components.end());

			return Path(result);
		}

		bool Path::is_directory() const {
			return _components.back() == "";
		}

		Path Path::to_directory() {
			ComponentsT result(_components);
			result.push_back("");

			return Path(result);
		}

		Path Path::simplify() const {
			ComponentsT result;

			if (is_absolute())
				result.push_back("");

			for (std::size_t i = 0; i < _components.size(); i++) {
				const StringT & bit = _components[i];

				if (bit == "..") {
					result.pop_back();
				} else if (bit != "." && bit != "") {
					result.push_back(bit);
				}
			}

			return Path(result);
		}

		Path::NameComponents Path::last_name_components () const {
			NameComponents name;
			const StringT & last_component = _components.back();

			if (!last_component.empty()) {
				std::size_t pos = last_component.find('.');

				if (pos != StringT::npos) {
					name.basename = StringT(&last_component[0], pos);

					pos++; // Skip over the dot
					name.extension = StringT(&last_component[pos], last_component.size() - pos);
				} else {
					name.basename = last_component;
				}
			}

			return name;
		}

		Path Path::parent_path (std::size_t n) const {
			ComponentsT result;

			if (n < _components.size()) {
				result.insert(result.end(), _components.begin(), _components.end() - n);
			} else if (is_absolute()) {
				result.push_back("");
			}

			return Path(result);
		}

		Path Path::with_extension(const StringT & extension) const {
			Path copy = this->parent_path();

			NameComponents name = this->last_name_components();
			name.extension = extension;

			return copy + name;
		}

		Path Path::operator+(const Path & other) const {
			ComponentsT result(_components);
			result.insert(result.end(), other._components.begin(), other._components.end());

			return Path(result);
		}

		Path Path::operator+(const NameComponents & other) const {
			ComponentsT result(_components);
			result.push_back(other.basename + '.' + other.extension);

			return Path(result);
		}

		bool Path::operator<(const Path & other) const {
			std::size_t i = 0;

			for (; i < _components.size() && i < other._components.size(); i++) {
				if (_components[i] < other._components[i])
					return true;

				if (_components[i] > other._components[i])
					return false;
			}

			std::ptrdiff_t diff = _components.size() - other._components.size();

			// diff is less than zero if |this| < |other|
			return diff < 0;
		}

		bool Path::operator==(const Path & other) const {
			return _components == other._components;
		}

		std::ostream & operator<<(std::ostream & output, const Path & path) {
			return (output << path.to_local_path());
		}
	}
}
