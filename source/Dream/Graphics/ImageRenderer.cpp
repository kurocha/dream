//
//  Graphics/ImageRenderer.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 22/04/09.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
//

#include "ImageRenderer.h"

namespace Dream
{
	namespace Graphics
	{
		using namespace Euclid::Numerics::Constants;

		ImageRenderer::ImageRenderer(Ptr<TextureManager> texture_manager) : _texture_manager(texture_manager) {
			DREAM_ASSERT(texture_manager);

			_texture_parameters.target = GL_TEXTURE_2D;
			_texture_parameters.wrap = GL_CLAMP_TO_EDGE;
			_texture_parameters.min_filter = GL_NEAREST;
			_texture_parameters.mag_filter = GL_NEAREST;
			_texture_parameters.generate_mip_maps = false;

			// Setup the vertex associations:
			auto binding = _vertex_array.binding();
			auto attributes = binding.attach(_vertex_buffer);
			attributes[0] = &Vertex::position;
			attributes[1] = &Vertex::mapping;
		}

		ImageRenderer::~ImageRenderer() {
		}

		Ref<Texture> ImageRenderer::fetch(Ptr<Image> image, bool invalidate) {
			DREAM_ASSERT(image);

			// We assume that the buffer doesn't need to be changed unless the pointers are different or invalidate is true.
			// For mutable pixel buffers, this isn't such a good option - perhaps implementing a mutating count, or a running checksum?

			TextureCacheT::iterator cache = _texture_cache.find(image);

			if (cache != _texture_cache.end()) {
				if (invalidate) {
					// Update the texture data:
					auto & binding = _texture_manager->bind(cache->second);
					binding.update(image);
				}

				// Return the cached texture:
				return cache->second;
			} else {
				//logger()->log(LOG_DEBUG, LogBuffer() << "Fetch " << image << ": allocating new texture");
				Ref<Texture> texture;

				if (_available_textures.size() > 0) {
					texture = _available_textures.back();
					_available_textures.pop_back();
					
					auto & binding = _texture_manager->bind(texture);
					binding.update(_texture_parameters, image);
				} else {
					// Create a new texture with the pixel buffer:
					texture = _texture_manager->allocate(_texture_parameters, image);
				}

				image->insert_finalizer(this);
				_texture_cache[image] = texture;

				return texture;
			}
		}

		void ImageRenderer::finalize(Object * object)
		{
			this->invalidate(ptr(object).as<Image>());
		}

		void ImageRenderer::render(const AlignedBox2 & box, Ptr<Image> image) {
			render(box, image, Vec2b(false, true), 0);
		}

		void ImageRenderer::render(const AlignedBox2 & box, Ptr<Image> image, Vec2b flip, RotationT rotation) {
			const Vec2b CORNERS[] = {
				Vec2b(false, false),
				Vec2b(true, false),
				Vec2b(false, true),
				Vec2b(true, true)
			};

			std::vector<Vertex> vertices;

			Ref<Texture> texture = fetch(image);
			AlignedBox2 mapping_box(ZERO, image->size() / texture->size().reduce());

			for (std::size_t i = 0; i < 4; i += 1) {
				Vertex vertex = {
					.position = box.corner(CORNERS[i]),
					.mapping = mapping_box.corner(CORNERS[(i+rotation) % 4])
				};

				vertices.push_back(vertex);
			}

			if (flip[X]) {
				std::swap(vertices[0].mapping, vertices[1].mapping);
				std::swap(vertices[2].mapping, vertices[3].mapping);
			}

			if (flip[Y]) {
				std::swap(vertices[0].mapping, vertices[2].mapping);
				std::swap(vertices[1].mapping, vertices[3].mapping);
			}

			_texture_manager->bind(0, texture);

			{
				auto binding = _vertex_array.binding();
				auto buffer_binding = _vertex_buffer.binding();

				check_graphics_error();

				buffer_binding.set_data(vertices);
				binding.draw_arrays(GL_TRIANGLE_STRIP, 0, vertices.size());
			}
		}

		void ImageRenderer::render(const AlignedBox2 & box, Ptr<Image> image, const AlignedBox2 & inner)
		{
			// If the inner alignment box is bigger than the box we are rendering, we have no choice but to scale the inner box:
			
			Vec2 border_size = image->size() - inner.size();
			
			if (border_size.greater_than(box.size())) {
				render(box, image);

				return;
			}

			std::vector<Vertex> vertices;

			Ref<Texture> texture = fetch(image);

			// Calculate the box coordinates:
			const Vec2 box_diagonal[] = {
				box.min(),
				box.min() + inner.min(),
				box.max() - (image->size() - inner.max()),
				box.max(),
			};

			// Calculate the image coordinates:
			const Vec2 image_diagonal[] = {
				0.0,
				inner.min() / image->size(),
				inner.max() / image->size(),
				1.0,
			};

			for (std::size_t j = 0; j < 3; j += 1) {
				RealT position_y[] = {box_diagonal[j][Y], box_diagonal[j+1][Y]};
				RealT mapping_y[] = {image_diagonal[j][Y], image_diagonal[j+1][Y]};

				for (std::size_t i = 0; i < 4; i += 1) {
					RealT position_x = box_diagonal[i][X];
					RealT mapping_x = image_diagonal[i][X];

					// Lower vertex
					{
						Vertex vertex = {
							.position = Vec2(position_x, position_y[0]),
							.mapping = Vec2(mapping_x, mapping_y[0]),
						};

						// Add a 2nd vertex at the start of the row for the degenerate triangle between the end of the previous row and this one.
						if (j > 0 && i == 0)
							vertices.push_back(vertex);

						vertices.push_back(vertex);
					}

					// Upper vertex
					{
						Vertex vertex = {
							.position = Vec2(position_x, position_y[1]),
							.mapping = Vec2(mapping_x, mapping_y[1]),
						};

						vertices.push_back(vertex);
					}
				}

				// Add a vertex at the end of the row for the degenerate triangle between this and the next row, except at the last row.
				if (j < 2)
					vertices.push_back(vertices.back());
			}

			_texture_manager->bind(0, texture);

			{
				auto binding = _vertex_array.binding();
				auto buffer_binding = _vertex_buffer.binding();

				check_graphics_error();

				buffer_binding.set_data(vertices);
				binding.draw_arrays(GL_TRIANGLE_STRIP, 0, vertices.size());
			}
		}

		void ImageRenderer::invalidate(Ptr<Image> image) {
			auto iterator = _texture_cache.find(image);

			if (iterator != _texture_cache.end()) {
				_available_textures.push_back(iterator->second);
				_texture_cache.erase(iterator);
			}
		}
	}
}
