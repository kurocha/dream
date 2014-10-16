//
//  Core/Dictionary.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 22/12/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Dictionary.hpp"

namespace Dream
{
	namespace Core
	{
		inline std::size_t size_of_type_identifier (TypeIdentifierT tid)
		{
			return tid & 0x0F;
		}

		/// Returns whether the key has a value in the dictionary.
		bool Dictionary::key (const KeyT & key)
		{
			return _values.find(key) != _values.end();
		}

		void Dictionary::set_value (const KeyT & key, const Value & value)
		{
			_values[key] = value;
		}

		const Value Dictionary::get_value (const KeyT & key) const
		{
			ValuesT::const_iterator i = _values.find(key);

			if (i != _values.end())
				return i->second;
			else
				return Value();
		}

		void Dictionary::update (const Ptr<Dictionary> other)
		{
			ValuesT values = other->_values;
			values.insert(_values.begin(), _values.end());
			_values.swap(values);
		}

		void Dictionary::insert (const Ptr<Dictionary> other)
		{
			_values.insert(other->_values.begin(), other->_values.end());
		}

		Ref<IData> Dictionary::serialize () const
		{
			Shared<DynamicBuffer> buffer(new DynamicBuffer);

			for (ValuesT::const_iterator i = _values.begin(); i != _values.end(); i++) {
				const KeyT & key = i->first;
				const Value & value = i->second;

				TypeSerialization<TI_STRING>::append_to_buffer(*buffer, key);
				value.append_to_buffer(*buffer);
			}

			buffer->hexdump(std::cout);

			return new BufferedData(buffer);
		}

		void Dictionary::deserialize (Ref<IData> data)
		{
			Shared<Buffer> buffer = data->buffer();
			std::size_t offset = 0;

			while (offset < buffer->size()) {
				KeyT key = TypeSerialization<TI_STRING>::read_from_buffer(*buffer, offset);
				_values[key] = Value::read_from_buffer(*buffer, offset);
			}
		}

		void Dictionary::debug (std::ostream & out) const
		{
			for (ValuesT::const_iterator i = _values.begin(); i != _values.end(); i++) {
				const KeyT & key = i->first;
				const Value & value = i->second;

				out << key << ": " << value << std::endl;
			}
		}
	}
}
