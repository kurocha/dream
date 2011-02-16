/*
 *  Layers.h
 *  Dream
 *
 *  Created by Samuel Williams on 11/09/10.
 *  Copyright 2010 Orion Transfer Ltd. All rights reserved.
 *
 */

#ifndef _DREAM_CLIENT_GRAPHICS_LAYERS_H
#define _DREAM_CLIENT_GRAPHICS_LAYERS_H

#include "WireframeRenderer.h"

#include <boost/function.hpp>

namespace Dream {
	namespace Client {
		namespace Graphics {
			
			class TransformedGroup : public Group
			{
				EXPOSE_CLASS(TransformedGroup)
				
				class Class : public Group::Class
				{
					EXPOSE_CLASSTYPE
				};
				
				protected:
					Mat44 m_transform;
					
				public:
					TransformedGroup(const Mat44 & transform);
					
					void setTransform(const Mat44 & transform);
					
					Mat44 & transform() { return m_transform; }
					const Mat44 & transform() const { return m_transform; }					
					
					virtual void renderFrameForTime (IScene * scene, TimeT time);
			};
			
			class DelegateLayer : public Object, IMPLEMENTS(Layer) {
				EXPOSE_CLASS(DelegateLayer)
				
				class Class : public Group::Class
				{
					EXPOSE_CLASSTYPE
				};
				
				public:
					typedef boost::function<void (IScene * scene, TimeT time)> RenderFrameT;
					typedef boost::function<void (ISceneManager * manager, IScene * scene)> BecomeCurrentT;
					typedef boost::function<void (ISceneManager * manager, IScene * scene)> RevokeCurrentT;					
					
					RenderFrameT renderFrameForTimeCallback;
					BecomeCurrentT didBecomeCurrentCallback;
					RevokeCurrentT willRevokeCurrentCallback;
					
					virtual void renderFrameForTime (IScene * scene, TimeT time);
					
					virtual void didBecomeCurrent (ISceneManager * manager, IScene * scene);
					virtual void willRevokeCurrent (ISceneManager * manager, IScene * scene);
			};
			
			class BackgroundLayer : public Object, IMPLEMENTS(Layer) {
				EXPOSE_CLASS(BackgroundLayer)
				
				class Class : public Object::Class, IMPLEMENTS(Layer::Class) {
				public:
					EXPOSE_CLASSTYPE
				};
				
			protected:
				Vec4 m_color;
				
			public:
				BackgroundLayer(const Vec4 & color);
				
				virtual void renderFrameForTime (IScene * scene, TimeT time);
			};
			
			class AxisLayer : public Object, IMPLEMENTS(Layer) {
				EXPOSE_CLASS(AxisLayer)
				
				class Class : public Object::Class, IMPLEMENTS(Layer::Class) {
				public:
					EXPOSE_CLASSTYPE
				};
				
			public:
				virtual void renderFrameForTime (IScene * scene, TimeT time);
			};
		}
	}
}

#endif