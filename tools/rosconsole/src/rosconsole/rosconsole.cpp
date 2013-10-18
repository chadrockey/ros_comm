/*
 * Copyright (c) 2008, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// Author: Josh Faust

#if defined(__APPLE__) && defined(__GNUC__) && defined(__llvm__) && !defined(__clang__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 2)
#error This code is known to provoke a compiler crash with llvm-gcc 4.2. You will have better luck with clang++. See code.ros.org/trac/ros/ticket/3626
#endif

#include "ros/console.h"
#include "ros/assert.h"
#include <ros/time.h>

#include <boost/thread.hpp>
#include <boost/shared_array.hpp>
#include <boost/regex.hpp>

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <cstring>
#include <stdexcept>

namespace ros
{
namespace console
{

bool g_initialized = false;
bool g_shutting_down = false;
boost::mutex g_init_mutex;

#ifdef WIN32
	#define COLOR_NORMAL ""
	#define COLOR_RED ""
	#define COLOR_GREEN ""
	#define COLOR_YELLOW ""
#else
	#define COLOR_NORMAL "\033[0m"
	#define COLOR_RED "\033[31m"
	#define COLOR_GREEN "\033[32m"
	#define COLOR_YELLOW "\033[33m"
#endif
const char* g_format_string = "[${severity}] [${time}]: ${message}";

void setFixedFilterToken(const std::string& key, const std::string& val)
{
}

/**
 * \brief For internal use only. Does the actual initialization of the rosconsole system.  Should only be called once.
 */
void do_initialize()
{
}

void initialize()
{
}

void vformatToBuffer(boost::shared_array<char>& buffer, size_t& buffer_size, const char* fmt, va_list args)
{
#ifdef _MSC_VER
  va_list arg_copy = args; // dangerous?
#else
  va_list arg_copy;
  va_copy(arg_copy, args);
#endif
#ifdef _MSC_VER
  size_t total = vsnprintf_s(buffer.get(), buffer_size, buffer_size, fmt, args);
#else
  size_t total = vsnprintf(buffer.get(), buffer_size, fmt, args);
#endif
  if (total >= buffer_size)
  {
    buffer_size = total + 1;
    buffer.reset(new char[buffer_size]);

#ifdef _MSC_VER
    vsnprintf_s(buffer.get(), buffer_size, buffer_size, fmt, arg_copy);
#else
    vsnprintf(buffer.get(), buffer_size, fmt, arg_copy);
#endif
  }
  va_end(arg_copy);
}

void formatToBuffer(boost::shared_array<char>& buffer, size_t& buffer_size, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  vformatToBuffer(buffer, buffer_size, fmt, args);

  va_end(args);
}

std::string formatToString(const char* fmt, ...)
{
  boost::shared_array<char> buffer;
  size_t size = 0;

  va_list args;
  va_start(args, fmt);

  vformatToBuffer(buffer, size, fmt, args);

  va_end(args);

  return std::string(buffer.get(), size);
}

#define INITIAL_BUFFER_SIZE 4096
static boost::mutex g_print_mutex;
static boost::shared_array<char> g_print_buffer(new char[INITIAL_BUFFER_SIZE]);
static size_t g_print_buffer_size = INITIAL_BUFFER_SIZE;
static boost::thread::id g_printing_thread_id;
void print(FilterBase* filter, log4cxx::Logger* logger, Level level, 
	   const char* file, int line, const char* function, const char* fmt, ...)
{
}

void print(FilterBase* filter, log4cxx::Logger* logger, Level level, 
	   const std::stringstream& ss, const char* file, int line, const char* function)
{
}

void notifyLoggerLevelsChanged()
{
}
void shutdown() 
{
  g_shutting_down = true;
}



} // namespace console
} // namespace ros
