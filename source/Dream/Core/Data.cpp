//
//  Core/Data.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 5/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "Data.hpp"

#include <streambuf>
#include <sstream>
#include <stdexcept>
#include <iomanip>

namespace Dream
{
	namespace Core
	{
// MARK: -
// MARK: LocalFileData

		LocalFileData::LocalFileData (const Path & path) : _path(path)
		{
		}

		LocalFileData::~LocalFileData ()
		{
		}

		Shared<Buffer> LocalFileData::buffer () const
		{
			if (!_buffer) {
				_buffer = new FileBuffer(_path);
			}

			return _buffer;
		}

		Shared<std::istream> LocalFileData::input_stream () const
		{
			std::ifstream * file_input_stream = new std::ifstream(_path.to_local_path().c_str(), std::ios::binary);

			return Shared<std::istream>(file_input_stream);
		}

		std::size_t LocalFileData::size () const
		{
			return buffer()->size();
		}


// MARK: -
// MARK: BufferedData

		BufferedData::BufferedData (Shared<Buffer> buffer) : _buffer(buffer)
		{
		}

		/// Create a buffer from a given input stream
		BufferedData::BufferedData (std::istream & stream)
		{
			Shared<DynamicBuffer> buffer = new DynamicBuffer;

			buffer->append(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

			_buffer = buffer;
		}

		BufferedData::~BufferedData ()
		{
		}

		Shared<Buffer> BufferedData::buffer () const
		{
			return _buffer;
		}

		Shared<std::istream> BufferedData::input_stream () const
		{
			return new BufferStream(*_buffer);
		}

		std::size_t BufferedData::size () const
		{
			return _buffer->size();
		}

		std::string format_data_size(std::size_t byte_count)
		{
			static const char * postfix[] = {"B", "KB", "MB", "GB", "TB"};
			
			double total = byte_count;
			std::size_t i = 0;
			
			while (total > 1024 && i < 4) {
				i += 1;
				total = total / 1024.0;
			}
			
			std::stringstream output;
			
			output << std::setprecision(4) << total << postfix[i];
			
			return output.str();
		}
	}
}
