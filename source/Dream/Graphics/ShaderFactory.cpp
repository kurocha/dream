//
//  ShaderFactory.cpp
//  File file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2013.
//  Copyright, 2013, by Samuel Williams. All rights reserved.
//

#include "ShaderFactory.h"

namespace Dream
{
	namespace Graphics
	{
		ShaderFactory::ShaderFactory(Ptr<IData> data, const ILoader * loader) : _shader_parser(*data->buffer())
		{
		}
		
		ShaderFactory::~ShaderFactory()
		{
		}
		
		void ShaderFactory::attach(Ptr<ShaderManager> shader_manager, Ptr<Program> program, const ShaderParser::DefinesMapT * defines)
		{
			for (auto & pair : _shader_parser.source_buffers())
			{
				if (pair.first == ShaderParser::SourceType::HEADER) continue;
				
				StringStreamT source_buffer;
				_shader_parser.full_buffer(source_buffer, pair.first, defines);
			
				source_buffer.seekg(0);
				BufferedData data(source_buffer);
				
				GLuint shader = shader_manager->compile((GLenum)pair.first, *data.buffer());
				
				if (shader)
					program->attach(shader);
				else
					throw std::runtime_error("Could not load shader!");
			}
		}
		
		void ShaderFactory::Loader::register_loader_types (ILoader * loader)
		{
			loader->set_loader_for_extension(this, "shader");
		}
		
		Ref<Object> ShaderFactory::Loader::load_from_data (const Ptr<IData> data, const ILoader * loader)
		{
			return new ShaderFactory(data, loader);
		}
	}
}
