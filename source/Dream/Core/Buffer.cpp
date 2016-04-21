//
//  Core/Buffer.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 26/10/08.
//  Copyright (c) 2008 Samuel Williams. All rights reserved.
//
//

#include "Buffer.hpp"
#include "System.hpp"
#include "Logger.hpp"

// File and memory manipulation
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <unistd.h>

// For testing
#include <string>
#include <iomanip>
#include <algorithm>

// memcpy
#include <string.h>

namespace Dream
{
	namespace Core
	{
// MARK: -
// MARK: class Buffer

		using namespace Logging;

		Buffer::~Buffer ()
		{
		}

		bool Buffer::empty () const
		{
			return size() == 0;
		}

		const Byte * Buffer::at (std::size_t loc) const
		{
			DREAM_ASSERT(loc <= size());
			return begin() + loc;
		}

		void Buffer::read (std::size_t offset, std::size_t size, Byte * value) const
		{
			memcpy(value, at(offset), size);
		}

		const Byte * Buffer::end () const
		{
			return begin() + size();
		}

		const Byte & Buffer::operator[] (std::size_t idx) const
		{
			return begin()[idx];
		}

		bool Buffer::operator== (const Buffer & other) const
		{
			if (size() != other.size())
				return false;

			// If size is the same, check data is the same
			return bcmp(begin(), other.begin(), size()) == 0;
		}

		bool Buffer::operator!= (const Buffer & other) const
		{
			return !((*this) == other);
		}
		
		// >04000000 4B657931 84050000 00040000< ....Key1........ 00000000
		// >004B6579 32200600 00004170 706C6573< .Key2 ....Apples 00000010
		void Buffer::hexdump (std::ostream & out)
		{
			// http://stahlforce.com/dev/index.php?tool=csc01
			const Byte * current = begin();
			std::size_t remaining = size();

			while (true) {
				StringStream buffer;

				buffer << "0x";

				buffer.fill('0');
				buffer.width(sizeof(long) * 2);
				buffer.setf(std::ios::hex, std::ios::basefield);

				buffer << (current - begin()) << " >";

				std::size_t count = std::min(remaining, (std::size_t)4*4);

				for (std::size_t i = 0; i < (4*4); i += 1) {
					if (i > 0 && i % 4 == 0)
						buffer << ' ';

					if (i < count) {
						buffer.width(2);
						buffer << (int)(*(current + i));
					} else
						buffer << "  ";
				}

				buffer << "< ";

				out << buffer.str();

				for (std::size_t i = 0; i < count; i += 1) {
					Byte character = *(current + i);
					if (character >= 32 && character <= 128)
						out << character;
					else
						out << ".";
				}

				out << std::endl;

				remaining -= count;
				if (remaining == 0)
					break;

				current += count;
			}
		}

		// http://www.flounder.com/checksum.htm
		uint32_t Buffer::checksum () const
		{
			uint32_t sum = 0;
			uint32_t r = 55665;
			const uint32_t C1 = 52845;
			const uint32_t C2 = 22719;

			std::size_t s = size();
			const Byte * b = begin();

			for (unsigned i = 0; i < s; i += 1) {
				Byte cipher = (b[i] ^ (r >> 8));
				r = (cipher + r) * C1 + C2;
				sum += cipher;
			}

			return sum;
		}

		void Buffer::write_to_file (const Path & p)
		{
			SystemError::reset();
			
			// Open and create the output file
			mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

			FileDescriptor fd = open(p.to_local_path().c_str(), O_RDWR | O_CREAT | O_TRUNC, mode);
			SystemError::check(__PRETTY_FUNCTION__);

			// Seek to the end
			//result = lseek(fd, size() - 1, SEEK_SET);
			//DREAM_ASSERT(result != -1);

			// Write a byte to give the file appropriate size
			//result = write(fd, "", 1);
			//DREAM_ASSERT(result != -1);
			
			int result = posix_fallocate(fd, 0, size());
			SystemError::check(__PRETTY_FUNCTION__, result);
			
			// mmap the file
			Byte * dst = (Byte *)mmap(0, size(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			SystemError::check(__PRETTY_FUNCTION__);

			madvise(dst, size(), MADV_SEQUENTIAL);
			SystemError::check(__PRETTY_FUNCTION__);

			// Copy the data
			memcpy(dst, begin(), size());
			SystemError::check(__PRETTY_FUNCTION__);

			// Clean up
			munmap(dst, size());
			close(fd);
			SystemError::check(__PRETTY_FUNCTION__);
		}

// MARK: -
// MARK: class MutableBuffer

		MutableBuffer::~MutableBuffer ()
		{
		}

		Byte * MutableBuffer::at (std::size_t loc)
		{
			DREAM_ASSERT(loc < size());
			return begin() + loc;
		}

		Byte * MutableBuffer::end ()
		{
			return begin() + size();
		}

		Byte & MutableBuffer::operator[] (std::size_t idx)
		{
			return begin()[idx];
		}

		void MutableBuffer::assign (std::size_t count, const Byte & value, std::size_t offset)
		{
			DREAM_ASSERT((count + offset) <= size());

			memset(begin() + offset, value, count);
		}

		void MutableBuffer::assign (const Byte * other_begin, const Byte * other_end, std::size_t offset)
		{
			DREAM_ASSERT((other_end - other_begin) + offset <= size());

			memcpy(begin() + offset, other_begin, other_end - other_begin);
		}

		void MutableBuffer::assign (const Buffer & other, std::size_t offset)
		{
			assign(other.begin(), other.end(), offset);
		}

		void MutableBuffer::assign (const Buffer & other, std::size_t other_offset, std::size_t other_size, std::size_t offset)
		{
			assign(other.begin() + other_offset, other.begin() + other_offset + other_size, offset);
		}

		void MutableBuffer::assign (const char * string, std::size_t offset)
		{
			std::size_t len = strlen(string);
			assign((const Byte *)string, (const Byte *)string + len, offset);
		}

// MARK: -
// MARK: class Resizable

		ResizableBuffer::~ResizableBuffer ()
		{
		}

		void ResizableBuffer::expand (std::size_t amount)
		{
			resize(size() + amount);
		}

		void ResizableBuffer::append (std::size_t size, const Byte * data)
		{
			expand(size);

			memcpy(this->end() - size, data, size);
		}

// MARK: -
// MARK: class StaticBuffer

		StaticBuffer StaticBuffer::for_cstring (const char * str, bool include_null_byte)
		{
			return StaticBuffer((const Byte*)str, strlen(str) + (int)include_null_byte);
		}

		StaticBuffer::StaticBuffer (const Byte * buf, const std::size_t & size) : _size(size), _buf(buf)
		{
		}

		StaticBuffer::~StaticBuffer ()
		{
		}

		std::size_t StaticBuffer::size () const
		{
			return _size;
		}

		const Byte * StaticBuffer::begin () const
		{
			return _buf;
		}

// MARK: -
// MARK: class FileBuffer

		FileBuffer::FileBuffer (const Path & file_path)
		{
			FileDescriptor input = open(file_path.to_local_path().c_str(), O_RDONLY);

			if (input == -1) {
				log("Error loading path", file_path);
				perror(__PRETTY_FUNCTION__);
			}
			
			DREAM_ASSERT(input != -1);

			_size = lseek(input, 0, SEEK_END);

			_buf = mmap(0, _size, PROT_READ, MAP_SHARED, input, 0);
			DREAM_ASSERT(_buf != (Byte *)-1);
		}

		FileBuffer::~FileBuffer ()
		{
			munmap(_buf, _size);
		}

		std::size_t FileBuffer::size () const
		{
			return _size;
		}

		const Byte * FileBuffer::begin () const
		{
			return (const Byte *)_buf;
		}

// MARK: -
// MARK: class DynamicBuffer

		void DynamicBuffer::allocate (std::size_t size)
		{
			if (size != _capacity) {
				_buf = (Byte*)realloc(_buf, size);
				DREAM_ASSERT(_buf != NULL);

				_capacity = size;
			}
		}

		void DynamicBuffer::deallocate ()
		{
			if (_buf) {
				free(_buf);
				_buf = NULL;
				_size = 0;
				_capacity = 0;
			}
		}

		DynamicBuffer::DynamicBuffer () : _capacity (0), _size (0), _buf (NULL)
		{
		}

		DynamicBuffer::DynamicBuffer (std::size_t size, bool reserved) : _buf (NULL)
		{
			allocate(size);

			if (reserved == false)
				_size = size;
			else
				_size = 0;
		}

		DynamicBuffer::~DynamicBuffer ()
		{
			deallocate();
		}

		std::size_t DynamicBuffer::capacity () const
		{
			return _capacity;
		}

		std::size_t DynamicBuffer::size () const
		{
			return _size;
		}

		void DynamicBuffer::clear ()
		{
			deallocate();
		}

		void DynamicBuffer::reserve (std::size_t size)
		{
			allocate(size);
		}

		void DynamicBuffer::resize (std::size_t size)
		{
			if (size > _capacity) {
				allocate(size);
			}

			_size = size;
		}

		Byte * DynamicBuffer::begin ()
		{
			return _buf;
		}

		const Byte * DynamicBuffer::begin () const
		{
			return _buf;
		}

// MARK: -
// MARK: class PackedData

		PackedBuffer::PackedBuffer (std::size_t size) : _size (size)
		{
		}

		PackedBuffer::~PackedBuffer ()
		{
		}

		// Data is packed at the end of the class.
		Byte * PackedBuffer::data ()
		{
			return (Byte*)this + sizeof(*this);
		}

		const Byte * PackedBuffer::data () const
		{
			return (const Byte*)this + sizeof(*this);
		}

		PackedBuffer * PackedBuffer::new_buffer (std::size_t size)
		{
			std::size_t total_size = sizeof(PackedBuffer) + size;
			void * data = ::operator new (total_size);
			PackedBuffer * buffer = new(data) PackedBuffer(size);

			return buffer;
		}

		std::size_t PackedBuffer::size () const
		{
			return _size;
		}

		Byte * PackedBuffer::begin ()
		{
			return data();
		}

		const Byte * PackedBuffer::begin () const
		{
			return data();
		}

// MARK: -
// MARK: class BufferStream

		BufferStream::BufferStream (const Buffer & buf) : std::streambuf (), std::istream (this)
		{
			std::streambuf::setg ((char *) buf.begin (), (char *) buf.begin (), (char *) buf.end ());
		}
	}
}
