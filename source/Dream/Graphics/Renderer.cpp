//
//  Graphics/Renderer.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 31/01/12.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
//

#include "Renderer.h"
#include "ShaderFactory.h"

namespace Dream
{
	namespace Graphics
	{
		INode::~INode() {
		}

		void INode::accept(IRenderer * renderer) {
			renderer->render(this);
		}

		void INode::traverse(IRenderer * renderer) {
			// An implementation would look something like this:
			//for (auto child : this->children()) {
			//	child->accept(renderer);
			//}
		}

// MARK: -

		IRenderer::~IRenderer() {
		}

		void IRenderer::render(INode * node) {
			node->traverse(this);
		}

		void IRenderer::traverse(INode * node) {
			node->accept(this);
		}

// MARK: -

		RendererState::~RendererState() {
		}

		Ref<Program> RendererState::load_program(const Path & name, const ShaderParser::DefinesMapT * defines) {
			Ref<ShaderFactory> factory = resource_loader->load<ShaderFactory>(name);

			Ref<Program> program = new Program;
			factory->attach(shader_manager, program, defines);
			program->link();

			program->bind_fragment_location("fragment_color");

			return program;
		}

		Ref<Texture> RendererState::load_texture(const TextureParameters & parameters, const Path & name) {
			Ref<IPixelBuffer> image = resource_loader->load<IPixelBuffer>(name);

			if (image) {
				return texture_manager->allocate(parameters, image);
			} else {
				return nullptr;
			}
		}
	}
}
