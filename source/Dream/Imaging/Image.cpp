//
//  Imaging/Image.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 1/05/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#include "Image.h"
#include <cassert>

namespace Dream {
	namespace Imaging {
		void Image::Loader::register_loader_types (ILoader * loader)
		{
			loader->set_loader_for_extension(this, "jpg");
			loader->set_loader_for_extension(this, "png");
		}

		Ref<Object> Image::Loader::load_from_data(const Ptr<IData> data, const ILoader * loader)
		{
			return Image::load_from_data(data);
		}

		Image::Image(const Vec2u & size, PixelFormat pixel_format, DataType data_type)
		{
			resize(size, pixel_format, data_type);
		}

		Image::~Image ()
		{
		}

		const PixelLayout & Image::layout () const
		{
			return _layout;
		}

		const ByteT * Image::data () const
		{
			return _buffer.begin();
		}

		void Image::allocate ()
		{
			_buffer.resize(_size.product() * _layout.bytes_per_pixel());
		}

		void Image::resize(const Vec2u & size, PixelFormat pixel_format, DataType data_type)
		{
			_layout.format = pixel_format;
			_layout.data_type = data_type;
			_layout.dimensions = {size[WIDTH], size[HEIGHT]};

			_size = size;

			if (_size.product() > 0)
				allocate();
		}

		void Image::fill(ByteT value)
		{
			std::fill(_buffer.begin(), _buffer.end(), value);
		}
	}
}
