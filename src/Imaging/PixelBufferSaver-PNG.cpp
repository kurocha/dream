//
//  Imaging/PixelBufferSaver-PNG.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 15/05/09.
//  Copyright (c) 2009 Samuel Williams. All rights reserved.
//
//

#include "PixelBufferSaver.h"
#include "Image.h"

#include <stdexcept>

extern "C" {
#include <png.h>
#include <jpeglib.h>
}

namespace Dream
{
	namespace Imaging
	{
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

		int png_color_type (ImagePixelFormat pixel_format)
		{
			switch (pixel_format) {
			case RED:
			case GREEN:
			case BLUE:
			case LUMINANCE:
			case ALPHA:
				return PNG_COLOR_TYPE_GRAY;
			case LUMINANCE_ALPHA:
				return PNG_COLOR_TYPE_GRAY_ALPHA;
			case RGB:
				return PNG_COLOR_TYPE_RGB;
			case RGBA:
				return PNG_COLOR_TYPE_RGBA;
			}
			
			return -1;
		}
		
		Ref<Core::IData> save_pixel_buffer_as_png (IPixelBuffer * pixel_buffer)
		{
			Vec3u size = pixel_buffer->size();
		
			ensure(!pixel_buffer->is_packed_format());
			ensure(size[Z] == 1);
			
			Shared<DynamicBuffer> result_data(new DynamicBuffer);
			
			png_structp png_writer = NULL;
			png_infop png_info = NULL;
			std::vector<png_bytep> rows(size[HEIGHT]);
			
			for (IndexT y = 0; y < size[HEIGHT]; y += 1) {
				rows[y] = (png_bytep)pixel_buffer->pixel_dataAt(Vec3u(0, y, 0));
			}
			
			try {
				png_writer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, png_error, NULL);
				ensure(png_writer != NULL && "png_create_write_struct returned NULL!");
				
				png_info = png_create_info_struct(png_writer);
				ensure(png_info != NULL && "png_create_info_struct returned NULL!");
				
				png_set_write_fn(png_writer, (void *)(result_data.get()), png_write_to_buffer, png_flush_buffer);
				
				int bit_depth = (pixel_buffer->bytes_per_pixel() * 8) / pixel_buffer->channel_count();
				int color_type = png_color_type(pixel_buffer->pixel_format());

				png_set_IHDR(png_writer, png_info, size[WIDTH], size[HEIGHT], bit_depth, color_type, NULL, NULL, NULL);
				
				png_write_info(png_writer, png_info);
				
				png_write_image(png_writer, (png_bytepp)&rows[0]);
				
				// After you are finished writing the image, you should finish writing the file.
				png_write_end(png_writer, NULL);
			} catch (std::exception & e) {
				std::cerr << "PNG write error: " << e.what() << std::endl;
			
				if (png_writer)
					png_destroy_write_struct(&png_writer, &png_info);
				
				throw;
			}
			
			png_destroy_write_struct(&png_writer, &png_info);
			
			return new BufferedData(result_data);
		}
		
		
#pragma mark -
#pragma mark Unit Tests
		
#ifdef ENABLE_TESTING

		UNIT_TEST(PixelBufferSaver) {
			Ref<IPixelBuffer> pixel_buffer = new Image(vec(100.0, 100.0, 1.0), RGB, UBYTE);
			testing("PNG");
			
			Ref<IData> png_data = save_pixel_buffer_as_png(pixel_buffer.get());
			
			png_data->buffer()->write_to_file("UnitTest.png");
		}
		
#endif
		
	}
}
