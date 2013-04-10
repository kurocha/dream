//
//  Imaging/Image.h
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 30/04/07.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//
//

#ifndef _DREAM_IMAGING_IMAGE_H
#define _DREAM_IMAGING_IMAGE_H

#include "../Framework.h"
#include "PixelBuffer.h"
#include "../Resources/Loader.h"

#include <Euclid/Numerics/Vector.IO.h>

namespace Dream {
	namespace Imaging {
		using namespace Resources;
		using namespace Euclid::Numerics;

		// This class represents your typical 2-dimentional uncompressed image.
		class Image : public Object, implements IPixelBuffer {
		public:
			class Loader : public Object, implements ILoadable {
			public:
				virtual void register_loader_types (ILoader * loader);
				virtual Ref<Object> load_from_data (const Ptr<IData> data, const ILoader * loader);
			};

		protected:
			PixelLayout _layout;

			Vec2u _size;
			DynamicBuffer _buffer;

			void allocate();

		public:
			Image(const Vec2u & size, PixelFormat pixel_format, DataType data_type);
			virtual ~Image ();

			virtual const PixelLayout & layout () const;
			virtual const ByteT * data () const;

			const Buffer & buffer () const { return _buffer; }
			ByteT * data () { return _buffer.begin(); }

			const Vec2u & size() const { return _size; }

			void resize(const Vec2u & size, PixelFormat pixel_format, DataType data_type);

			void fill(ByteT value = 0);

			static Ref<Image> load_from_data (const Ptr<IData> data);
			static Ref<IData> save_to_data (const Ptr<Image> image);
		};

		template <typename DataT, dimension N>
		class Reader {
		protected:
			typedef Vector<N, std::size_t> CoordinateT;
			typedef typename std::remove_const<DataT>::type PixelElementT;

			DataT * _data;
			CoordinateT _size;
			std::uint8_t _bytes_per_pixel;

			std::size_t offset (const CoordinateT & coordinate) const
			{
				std::size_t pixel_offset = 0, stride = 1;

				for (dimension n = 0; n < N; n += 1) {
					pixel_offset += coordinate[n] * stride;
					stride *= _size[n];
				}

				return (pixel_offset * _bytes_per_pixel);
			}

		public:
			Reader(DataT * data, const CoordinateT & size, std::uint8_t bytes_per_pixel) : _data(data), _size(size), _bytes_per_pixel(bytes_per_pixel)
			{
			}

			DataT * operator[] (const CoordinateT & coordinate) const
			{
				return _data + offset(coordinate);
			}

			template <dimension E>
			void get(const CoordinateT & coordinate, Vector<E, PixelElementT> & pixel) const {
				std::copy_n((*this)[coordinate], E, pixel.begin());
			}

			const CoordinateT & size () const { return _size; }
		};

		inline Reader<const ByteT, 2> reader (const Image & image)
		{
			return {image.data(), image.size(), image.layout().bytes_per_pixel()};
		}

		template <typename DataT, dimension N>
		inline Reader<DataT, N> reader (DataT * data, Vector<N, std::size_t> size, uint8_t bytes_per_pixel) {
			return {data, size, bytes_per_pixel};
		}

		template <typename DataT, dimension N>
		struct Writer : public Reader<DataT, N> {
		protected:
			typedef Vector<N, std::size_t> CoordinateT;
			typedef typename std::remove_const<DataT>::type PixelElementT;

			template <typename ReaderT>
			inline void copy(dimension d, CoordinateT & offset, const ReaderT & reader, const CoordinateT & from, const CoordinateT & to, const CoordinateT & size) {
				
				if (d == 0) {
					std::copy_n(reader[from + offset], size[0], (*this)[to + offset]);
				} else {
					for (std::size_t n = 0; n < size[d]; n += 1) {
						offset[d] = n;
						copy(d-1, offset, reader, from, to, size);
					}
				}
			}

		public:
			Writer(DataT * data, const CoordinateT & size, std::uint8_t bytes_per_pixel) : Reader<DataT, N>(data, size, bytes_per_pixel)
			{
			}

			template <typename ReaderT>
			void copy(const ReaderT & reader, const CoordinateT & from, const CoordinateT & to, const CoordinateT size) {
				CoordinateT offset = 0;

				copy(N-1, offset, reader, from, to, size);
			}

			template <dimension E>
			void set(const CoordinateT & coordinate, const Vector<E, PixelElementT> & pixel) {
				std::copy_n(pixel.begin(), E, (*this)[coordinate]);
			}
		};

		inline Writer<ByteT, 2> writer (Image & image)
		{
			return {image.data(), image.size(), image.layout().bytes_per_pixel()};
		}
	}
}

#endif
