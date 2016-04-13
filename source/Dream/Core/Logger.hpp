//
//  Logger.h
//  Dream
//
//  Created by Samuel Williams on 24/01/12.
//  Copyright (c) 2012 Orion Transfer Ltd. All rights reserved.
//

#pragma once

#include "Timer.hpp"

#include <thread>
#include <mutex>
#include <unordered_map>
#include <sstream>

namespace Dream
{
	namespace Core
	{
		/// Use this namespace for global logging facilities.
		namespace Logging {
			enum LogLevel {
				LOG_ERROR = 1,
				LOG_WARN = 2,
				LOG_INFO = 4,
				LOG_DEBUG = 8,

				LOG_ALL = (1 | 2 | 4 | 8)
			};

			typedef std::stringstream LogBuffer;
		}

		using namespace Logging;

		class Log;

		class Logger : public Object {
		private:
			template <typename HeadT, typename... TailT>
			static void write(LogBuffer & output, HeadT head, TailT... tail) {
				output << head << " ";

				write(output, tail...);
			}

			static void write(LogBuffer & output) {}

		protected:
			Timer _log_time;

			std::mutex _lock;

			LogLevel _log_level;
			FileDescriptor _output;

			typedef std::unordered_map<std::thread::id, std::string> ThreadNamesT;
			ThreadNamesT _thread_names;

			static const char * level_name(LogLevel level);

			void header(LogLevel level);

			/// Print out a logging header
			void start_session();

			std::string thread_name() const;
		public:
			Logger();
			virtual ~Logger();

			void log(LogLevel level, const std::string & message);
			void log(LogLevel level, const std::ostream & buffer);

			template <typename... TailT>
			void print(LogLevel level, TailT... tail)
			{
				if (level & _log_level) {
					LogBuffer buffer;
					
					write(buffer, tail...);

					log(level, buffer);
				}
			}

			void enable(LogLevel level);
			void disable(LogLevel level);

			void set_thread_name(std::string name);
		};

		namespace Logging {
			Logger * console();

			template <typename... TailT>
			void log_error(TailT... tail)
			{
				console()->print(LOG_ERROR, tail...);
			}

			template <typename... TailT>
			void log_warning(TailT... tail)
			{
				console()->print(LOG_WARN, tail...);
			}

			template <typename... TailT>
			void log(TailT... tail)
			{
				console()->print(LOG_INFO, tail...);
			}
			
			template <typename... TailT>
			void log_debug(TailT... tail)
			{
				console()->print(LOG_DEBUG, tail...);
			}
		};
	}
}
