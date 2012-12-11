//
//  Imaging/PixelBuffer.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "PixelBuffer.h"

#include <iostream>
#include <unistd.h>

namespace Dream {
	namespace Imaging {
		std::uint8_t byte_size(DataType data_type) {
			return 0xFF & (unsigned)data_type;
		}

		std::uint8_t channel_count (PixelFormat pixel_format) {
			return 0xFF & (unsigned)pixel_format;
		}

		std::size_t PixelLayout::data_size() const {
			std::size_t pixel_count = 1;

			for (std::size_t dimension : dimensions) {
				pixel_count *= dimension;
			}

			return pixel_count * bytes_per_pixel();
		}

		IPixelBuffer::~IPixelBuffer() {

		}

		PixelBuffer::PixelBuffer (const PixelLayout & pixel_layout, Shared<Buffer> buffer) : _pixel_layout(pixel_layout), _buffer(buffer)
		{
		}

		PixelBuffer::~PixelBuffer()
		{
		}

		const PixelLayout & PixelBuffer::layout () const
		{
			return _pixel_layout;
		}

		const ByteT * PixelBuffer::data () const
		{
			return _buffer->begin();
		}
	}
}
