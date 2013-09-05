//
//  ShaderParser.cpp
//  File file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2013.
//  Copyright, 2013, by Samuel Williams. All rights reserved.
//

#include "ShaderParser.h"

namespace Dream
{
	namespace Graphics
	{
		ShaderParser::ShaderParser(const Buffer & buffer)
		{
			BufferStream input_stream(buffer);

			parse(input_stream);
		}
		
		void ShaderParser::parse_header(std::istream & input_stream)
		{
			std::string line_buffer;
			std::getline(input_stream, line_buffer);

			if (line_buffer.find("@shader") != 0)
				throw ParseError("Invalid start sequence, missing @shader.");

			_header = set_current_source(SourceType::HEADER, false);
		}

		std::stringstream * ShaderParser::set_current_source(SourceType source_type, bool append_line_number)
		{
			_current_source = &_source_buffers[source_type];

			if (append_line_number)
				(*_current_source) << "#line " << _line << std::endl;

			return _current_source;
		}

		void ShaderParser::append_line(const std::string & line)
		{
			if (_current_source)
				(*_current_source) << line << std::endl;
		}

		void ShaderParser::add_definitions(const DefinesMapT & defines)
		{
			for (auto & pair : defines)
				(*_header) << "#define " << pair.first << " " << pair.second << std::endl;
		}

		//, const std::map<std::string, std::string> & definitions
		void ShaderParser::parse(std::istream & input_stream)
		{
			parse_header(input_stream);
			
			std::string line_buffer;

			while (input_stream.good()) {
				// Read one line:
				std::getline(input_stream, line_buffer);

				_line += 1;

				if (line_buffer.find("@geometry") == 0) {
					set_current_source(SourceType::GEOMETRY);
				} else if (line_buffer.find("@vertex") == 0) {
					set_current_source(SourceType::VERTEX);
				} else if (line_buffer.find("@fragment") == 0) {
					set_current_source(SourceType::FRAGMENT);
				} else {
					append_line(line_buffer);
				}
			}
		}

		void ShaderParser::full_buffer(std::stringstream & buffer, SourceType source_type, const DefinesMapT * defines) const
		{
			buffer << _header->str();

			if (defines)
			{
				for (auto & pair : *defines)
					buffer << "#define " << pair.first << " " << pair.second << std::endl;
			}

			if (source_type != SourceType::HEADER)
			{
				auto source_buffer = _source_buffers.find(source_type);

				if (source_buffer != _source_buffers.end())
					buffer << source_buffer->second.str();
			}
		}
	}
}
