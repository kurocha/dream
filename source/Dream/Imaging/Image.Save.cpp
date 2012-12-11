//
//  Imaging/Image.Save.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/05/09.
//  Copyright (c) 2009 Samuel Williams. All rights reserved.
//
//

#include "Image.h"
#include "../Events/Logger.h"

#include <stdexcept>

extern "C" {
#include <png.h>
#include <jpeglib.h>
}

namespace Dream
{
	namespace Imaging
	{
		using namespace Events::Logging;

		static void png_write_to_buffer (png_structp png_writer, png_bytep data, png_size_t length)
		{
			DynamicBuffer * buffer = (DynamicBuffer*)png_get_io_ptr(png_writer);

			buffer->append(length, data);
		}

		static void png_flush_buffer (png_structp read_ptr)
		{
		}

		static void png_error(png_structp png_ptr, png_const_charp msg) {
			throw std::runtime_error(msg);
		}

		static int png_color_type (PixelFormat pixel_format)
		{
			switch (pixel_format) {
				case PixelFormat::R:
				case PixelFormat::G:
				case PixelFormat::B:
				case PixelFormat::A:
				case PixelFormat::L:
					return PNG_COLOR_TYPE_GRAY;
				case PixelFormat::LA:
					return PNG_COLOR_TYPE_GRAY_ALPHA;
				case PixelFormat::RGB:
					return PNG_COLOR_TYPE_RGB;
				case PixelFormat::RGBA:
				case PixelFormat::BGRA:
					return PNG_COLOR_TYPE_RGBA;
			}

			return -1;
		}

		Ref<IData> Image::save_to_data (const Ptr<Image> image)
		{
			// Information about the image:
			auto image_reader = reader(*image);
			auto size = image->size();
			auto layout = image->layout();

			// Structures to process the image:
			png_structp png_writer = NULL;
			png_infop png_info = NULL;

			// Data pointers, one per row:
			std::vector<png_bytep> rows(size[HEIGHT]);

			// The buffer to contain the results:
			Shared<DynamicBuffer> result_data = new DynamicBuffer;

			for (std::size_t y = 0; y < size[HEIGHT]; y += 1) {
				rows[y] = (png_bytep)image_reader[Vec2u(0, y)];
			}

			try {
				png_writer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_error, NULL);
				DREAM_ASSERT(png_writer != NULL && "png_create_write_struct returned NULL!");

				png_info = png_create_info_struct(png_writer);
				DREAM_ASSERT(png_info != NULL && "png_create_info_struct returned NULL!");

				png_set_write_fn(png_writer, (void *)(result_data.get()), png_write_to_buffer, png_flush_buffer);

				int bit_depth = (layout.bytes_per_pixel() * 8) / layout.channel_count();
				int color_type = png_color_type(layout.format);

				png_set_IHDR(png_writer, png_info, size[WIDTH], size[HEIGHT], bit_depth, color_type, 0, 0, 0);

				png_write_info(png_writer, png_info);

				png_write_image(png_writer, (png_bytepp)&rows[0]);

				// After you are finished writing the image, you should finish writing the file.
				png_write_end(png_writer, NULL);
			} catch (std::exception & e) {
				logger()->log(LOG_ERROR, LogBuffer() << "PNG write error: " << e.what());

				if (png_writer)
					png_destroy_write_struct(&png_writer, &png_info);

				throw;
			}

			png_destroy_write_struct(&png_writer, &png_info);

			return new BufferedData(result_data);
		}


// MARK: -
// MARK: Unit Tests

#ifdef ENABLE_TESTING

		UNIT_TEST(PixelBufferSaver) {
			Ref<IPixelBuffer> pixel_buffer = new Image(vec(100.0, 100.0, 1.0), PixelFormat::RGB, DataType::BYTE);
			testing("PNG");

			Ref<IData> png_data = save_pixel_buffer_as_png(pixel_buffer.get());

			png_data->buffer()->write_to_file("UnitTest.png");
		}

#endif
	}
}
