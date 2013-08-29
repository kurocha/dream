//
//  ShaderFactory.h
//  File file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2013.
//  Copyright, 2013, by Samuel Williams. All rights reserved.
//

#ifndef DREAM_CLIENT_GRAPHICS_SHADERFACTORY_H
#define DREAM_CLIENT_GRAPHICS_SHADERFACTORY_H

#include "ShaderParser.h"
#include "ShaderManager.h"

namespace Dream
{
	namespace Graphics
	{
		class ShaderFactory : public Object
		{
		public:
			class Loader : public Object, public virtual Resources::ILoadable {
			public:
				virtual void register_loader_types (ILoader * loader);
				virtual Ref<Object> load_from_data (const Ptr<IData> data, const ILoader * loader);
			};

			ShaderFactory(Ptr<IData> data, const ILoader * loader);
			virtual ~ShaderFactory();

			// Compile and attach all shaders to the given program:
			void attach(Ptr<ShaderManager> shader_manager, Ptr<Program> program, const ShaderParser::DefinesMapT * defines = nullptr);

		protected:
			ShaderParser _shader_parser;
		};
	}
}

#endif
