//
//  Graphics/ShaderManager.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 5/12/11.
//  Copyright (c) 2011 Samuel Williams. All rights reserved.
//

#include "ShaderManager.h"

#include <exception>

namespace Dream
{
	namespace Graphics
	{
		ShaderError::ShaderError(StringT message) : _message(message)
		{
		}

		const char * ShaderError::what () const noexcept
		{
			return _message.c_str();
		}

// MARK: -

		Program::Program()
		{
			_handle = glCreateProgram();
		}

		Program::~Program()
		{
			glDeleteProgram(_handle);
		}

		void Program::attach(GLenum shader)
		{
			glAttachShader(_handle, shader);
		}

		static Shared<Buffer> program_info_log(GLuint program)
		{
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			if (length > 0) {
				Shared<MutableBuffer> buffer = PackedBuffer::new_buffer(length);

				glGetProgramInfoLog(program, length, NULL, (GLchar *)buffer->begin());

				return buffer;
			} else {
				return NULL;
			}
		}

		bool Program::link()
		{
			glLinkProgram(_handle);

			GLint link_status;
			property(GL_LINK_STATUS, &link_status);
			glGetProgramiv(_handle, GL_LINK_STATUS, &link_status);

			if (!link_status) {
				LogBuffer buffer;
				buffer << "Error linking program:" << std::endl;
				buffer << program_info_log(_handle)->begin() << std::endl;
				logger()->log(LOG_ERROR, buffer);
			}

			return link_status != 0;
		}

		GLint Program::attribute_location(const char * name)
		{
			return glGetAttribLocation(_handle, name);
		}

		GLint Program::uniform_location(const char * name)
		{
			return glGetUniformLocation(_handle, name);
		}

#ifndef DREAM_OPENGLES2
		GLint Program::uniform_block_index(const char * name) {
			return glGetUniformBlockIndex(_handle, name);
		}
#endif

		void Program::bind_fragment_location(const char * name, GLuint output)
		{
//#ifdef DREAM_OPENGL32
			glBindFragDataLocation(_handle, output, name);
//#endif
		}

		void Program::enable()
		{
			glUseProgram(_handle);
		}

		void Program::disable()
		{
			glUseProgram(0);
		}

// MARK: -

		ShaderManager::ShaderManager()
		{
		}

		ShaderManager::~ShaderManager()
		{
			for (auto i : _shaders) {
				glDeleteShader(i);
			}
		}

		static Shared<Buffer> shader_info_log(GLuint shader)
		{
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			if (length > 0) {
				Shared<MutableBuffer> buffer = PackedBuffer::new_buffer(length);

				glGetShaderInfoLog(shader, length, NULL, (GLchar *)buffer->begin());

				return buffer;
			} else {
				return NULL;
			}
		}

		GLenum ShaderManager::compile(GLenum type, const Buffer & buffer)
		{
			GLuint shader = glCreateShader(type);

			const GLchar * source = (GLchar*)buffer.begin();
			GLint length = (GLint)buffer.size();
			glShaderSource(shader, 1, &source, &length);
			check_graphics_error();

			glCompileShader(shader);
			
			GLint compile_status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

			if (!compile_status) {
				LogBuffer log_buffer;
				log_buffer << "Error compiling shader:" << std::endl;
				log_buffer << shader_info_log(shader)->begin() << std::endl;
				logger()->log(LOG_ERROR, log_buffer);
				
				glDeleteShader(shader);
				
				return 0;
			}

			return shader;
		}
	}
}
