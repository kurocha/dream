//
//  Imaging/PixelBuffer.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 30/04/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _DREAM_IMAGING_PIXELBUFFER_H
#define _DREAM_IMAGING_PIXELBUFFER_H

#include "../Framework.h"
#include "../Core/Algorithm.h"
#include "../Core/Buffer.h"

#include <Euclid/Numerics/Vector.h>

namespace Dream {
	namespace Imaging {
		using namespace Euclid::Numerics::Constants;

		/// Pixel component data type. All data types are assumed to be unsigned.
		enum class DataType : unsigned {
			BYTE = 0x0101,
			SHORT = 0x0202,
			INTEGER = 0x0304,
			FLOAT = 0x0404,
		};

		/// Image pixel formats.
		enum class PixelFormat : unsigned {
			// Single channel formats:
			R = 0x0101,
			G = 0x0201,
			B = 0x0301,
			A = 0x0401,

			// Full colour formats:
			RGB = 0x0503,
			RGBA = 0x0604,
			BGRA = 0x0704,

			// Luminance formats:
			L = 0x0801,
			LA = 0x0902,
		};

		std::uint8_t byte_size(DataType type);
		std::uint8_t channel_count(PixelFormat type);

		using Dimensions = std::vector<std::size_t>;

		// This type is guaranteed to be big enough to hold even RGBA16.
		// This is useful when you want a generic representation of a pixel
		typedef uint64_t PixelT;

		struct PixelLayout {
			PixelFormat format;
			DataType data_type;

			// The size in pixels for each dimension.
			Dimensions dimensions;
			
			std::size_t data_size() const;

			std::uint8_t channel_count() const { return Imaging::channel_count(format); }
			std::uint8_t bytes_per_pixel() const { return channel_count() * byte_size(data_type); }

			/// @returns the maximum value of an individual pixel.
			PixelT maximum_pixel () const {
				return ((PixelT)1 << (bytes_per_pixel() * 8)) - 1;
			}
		};

		// A pixel buffer is a container for pixel data along with a layout that allows for interpretation of the buffer contents. This typically includes a pixel format which describes the layout of colour channels within an individual pixel element, a data type which describes the storage for each pixel component and a size which describes the organisation of pixel data into rows and columns and potentially other dimensions.
		class IPixelBuffer : implements IObject {
		public:
			virtual ~IPixelBuffer();

			virtual const PixelLayout & layout () const = 0;
			virtual const ByteT * data () const = 0;
		};

		using Core::Buffer;

		class PixelBuffer : implements IPixelBuffer {
		protected:
			PixelLayout _pixel_layout;
			Shared<Buffer> _buffer;

		public:
			PixelBuffer (const PixelLayout & pixel_layout, Shared<Buffer> buffer);
			virtual ~PixelBuffer();

			virtual const PixelLayout & layout () const;
			virtual const ByteT * data () const;
		};
	}
}

#endif
