//
//  Client/Display/X11/Context.cpp
//  This file is part of the "Dream" project, and is released under the MIT license.
//
//  Created by Samuel Williams on 06/11/2012.
//  Copyright (c) 2007 Samuel Williams. All rights reserved.
//

#include "Context.h"

namespace Dream
{
	namespace Client
	{
		namespace Display
		{
			namespace X11
			{
				
				using namespace Events::Logging;
				using namespace Euclid::Numerics::Constants;

// MARK: -

				static void check_glx_version(XDisplay * display)
				{
					int glx_major, glx_minor;
					
					// FBConfigs were added in GLX version 1.3.
					if (!glXQueryVersion(display, &glx_major, &glx_minor))
						throw ContextInitializationError("Could not fetch GLX version!");
					
					if ((glx_major == 1 && glx_minor < 3) || (glx_major < 1))
						throw ContextInitializationError("Unsupported GLX version!");
				}

				static void find_best_framebuffer_configuration(XDisplay * display, int screen_number, GLXFBConfig & best_configuration, XVisualInfo *& visual_info)
				{
					int frame_buffer_attributes[] = {
						GLX_X_RENDERABLE, True,
						GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
						GLX_RENDER_TYPE, GLX_RGBA_BIT,
						GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
						GLX_RED_SIZE, 8,
						GLX_GREEN_SIZE, 8,
						GLX_BLUE_SIZE, 8,
						GLX_ALPHA_SIZE, 8,
						GLX_DEPTH_SIZE, 24,
						GLX_STENCIL_SIZE, 8,
						GLX_DOUBLEBUFFER, True,
						//GLX_SAMPLE_BUFFERS, 1,
						//GLX_SAMPLES, 4,
						None
					};
					
					int count = 0;
					GLXFBConfig * configurations = glXChooseFBConfig(display, screen_number, frame_buffer_attributes, &count);
					
					if (!configurations) {
						throw ContextInitializationError("No valid frame buffer configurations found!");
					}
					
					log_debug("Found", count, "valid frame buffer configurations for requested attributes");

					assert(count > 0);

					// Get the first frame buffer configuration:
					best_configuration = configurations[0];
					XFree(configurations);
					
					visual_info = glXGetVisualFromFBConfig(display, best_configuration);
					
					if (!visual_info)
						throw ContextInitializationError("Couldn't get a visual buffer.");
				}

				static GLXContext create_glx_context(XDisplay * display, const GLXFBConfig & configuration)
				{
					int context_attributes[] = {
						GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
						GLX_CONTEXT_MINOR_VERSION_ARB, 2,
						GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
						None
					};
					
					PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
					return glXCreateContextAttribsARB(display, configuration, NULL, True, context_attributes);
				}

				// A callback that waits until the window is displayed on the screen:
				static Bool wait_for_map_notify(XDisplay * display, XEvent * event, XPointer arg) {
					return (event->type == MapNotify) && (event->xmap.window == (XWindow)arg);
				}

				static void map_window(XDisplay * display, XWindow window)
				{
					// This function is probably non-reentrant.
					XMapWindow(display, window);
				
					XEvent event;
					XIfEvent(display, &event, wait_for_map_notify, (XPointer)window);
				}

				static void set_vertical_sync(XDisplay * display, GLXWindow window, bool enabled)
				{
					PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte *)"glXSwapIntervalEXT");

					if (glXSwapIntervalEXT)
						glXSwapIntervalEXT(display, window, enabled ? 1 : 0);
				}
				
				void WindowContext::setup_graphics_context(Ptr<Dictionary> config, Vec2u size)
				{
					DREAM_ASSERT(_display != nullptr);

					// Check that we have a supported version of GLX:
					check_glx_version(_display);

					int screen_number = DefaultScreen(_display);

					XVisualInfo * visual_info = nullptr;
					GLXFBConfig best_configuration;

					find_best_framebuffer_configuration(_display, screen_number, best_configuration, visual_info);
					log_debug("Chosen visual info id =", visual_info->visualid);

					// Now we know where to create the window:
					XWindow root_window = RootWindow(_display, visual_info->screen);

					XSetWindowAttributes window_attributes;
					window_attributes.background_pixel = 0;
					window_attributes.border_pixel = 0;
					window_attributes.colormap = XCreateColormap(_display, root_window, visual_info->visual, AllocNone);
					window_attributes.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
					unsigned long mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

					// Create a new X11 window for OpenGL rendering:
					_window = XCreateWindow(_display, root_window, 0, 0, size[WIDTH], size[HEIGHT], 0, visual_info->depth, InputOutput, visual_info->visual, mask, &window_attributes);

					if (!_window)
						throw ContextInitializationError("Couldn't create X11 window!");

					// Create the GLX context:
					_glx_context = create_glx_context(_display, best_configuration);

					if (!_glx_context)
						throw ContextInitializationError("Couldn't create GLX context!");

					// Create the GLX window:
					_glx_window = glXCreateWindow(_display, best_configuration, _window, NULL);

					if (!_glx_window)
						throw ContextInitializationError("Couldn't create GLX window!");

					map_window(_display, _window);

					make_current_context();

					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
					glPixelStorei(GL_PACK_ALIGNMENT, 1);

					set_vertical_sync(_display, _glx_window, true);

					LogBuffer buffer;
					buffer << "OpenGL Context Initialized..." << std::endl;
					buffer << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
					buffer << "OpenGL Renderer: " << glGetString(GL_RENDERER) << " " << glGetString(GL_VERSION) << std::endl;
					buffer << "OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
					logger()->log(LOG_INFO, buffer);

					glXMakeContextCurrent(_display, None, None, nullptr);

					XSync(_display, False);
				}

				void WindowContext::make_current_context()
				{
					glXMakeContextCurrent(_display, _glx_window, _glx_window, _glx_context);
				}
				
				void WindowContext::flush_buffers() {
					glXSwapBuffers(_display, _glx_window);
				}

				WindowContext::WindowContext(Ptr<Dictionary> config) : _display(nullptr), _glx_context(nullptr)
				{
					Vec2u initial_size {1024, 768};

					config->get("Context.Size", initial_size);
					config->get("Context.XDisplay", _display);

					if (!_display) {
						_display = XOpenDisplay(nullptr);
						
						if (!_display)
							throw ContextInitializationError("Could not connect to X11 display!");
					}

					setup_graphics_context(config, initial_size);
				}

				WindowContext::~WindowContext() {
					stop();

					if (_renderer_thread) {
						_renderer_thread->stop();
					}

					if (_glx_context) {
						glXDestroyContext(_display, _glx_context);
						glXDestroyWindow(_display, _glx_window);
					}
					
					if (_window) {
						XDestroyWindow(_display, _window);
					}

					if (_display) {
						XCloseDisplay(_display);
					}
				}

				void WindowContext::render_frame()
				{
					if (!_initialized)
					{
						logger()->set_thread_name("Renderer");
						
						_initialized = true;
					}
					
					glXMakeContextCurrent(_display, _glx_window, _glx_window, _glx_context);

					_context_delegate->render_frame_for_time(this, system_time());
					flush_buffers();

					glXMakeContextCurrent(_display, None, None, nullptr);
				}

				void WindowContext::start() {
					logger()->log(LOG_DEBUG, "Starting context...");

					if (!_renderer_thread) {
						_renderer_thread = new Events::Thread;
						_renderer_timer = new Events::TimerSource(std::bind(&WindowContext::render_frame, this), 1.0/60.0, true, true);
						_renderer_thread->loop()->schedule_timer(_renderer_timer);
					}

					Vec2u new_size(1024, 768);
					ResizeInput resize_input(new_size);
					process(resize_input);

					_initialized = false;
					_renderer_thread->start();
				}

				void WindowContext::stop() {
					logger()->log(LOG_DEBUG, "Stopping context...");

					_renderer_thread->stop();
				}

				Vec2u WindowContext::size() {
					return Vec2u(1024, 768);
				}

				void WindowContext::set_cursor_mode(CursorMode mode) {

				}
			}
		}
	}
}
