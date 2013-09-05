//
//  ShaderParser.h
//  File file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2013.
//  Copyright, 2013, by Samuel Williams. All rights reserved.
//

#ifndef DREAM_CLIENT_GRAPHICS_SHADERPARSER_H
#define DREAM_CLIENT_GRAPHICS_SHADERPARSER_H

#include <map>
#include <sstream>
#include <iosfwd>

#include "Graphics.h"

namespace Dream
{
	namespace Graphics
	{
		class ShaderParser
		{
		public:
			class ParseError : std::runtime_error
			{
			public:
				ParseError(const std::string & what) : std::runtime_error(what) {}
			};

			enum class SourceType {
				HEADER = 1, GEOMETRY = GL_GEOMETRY_SHADER, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER,
			};
			
			typedef std::map<std::string, std::string> DefinesMapT;

		protected:
			// All source code buffers currently encountered:
			std::map<SourceType, std::stringstream> _source_buffers;

			// The currently selected source code buffer:
			std::stringstream * _header;
			std::stringstream * _current_source = nullptr;

			std::size_t _line = 0;

			std::stringstream * set_current_source(SourceType source_type, bool append_line_number = true);
			void append_line(const std::string & line);

			// Parse the shader header token.
			void parse_header(std::istream & input_stream);

		public:
			ShaderParser(const Buffer & buffer);
			
			// Add #define statements to the header.
			void add_definitions(const DefinesMapT & defines);

			// Parse shader source code.
			void parse(std::istream & input_stream);

			// Get a buffer for the corresponding source type with an optional map of defines as per `add_definitions`.
			void full_buffer(std::stringstream & buffer, SourceType source_type, const DefinesMapT * defines = nullptr) const;

			// To get a full source buffer, you should call `full_buffer(pair.first)`.
			const std::map<SourceType, std::stringstream> & source_buffers() const { return _source_buffers; }
		};
	}
}

#endif
