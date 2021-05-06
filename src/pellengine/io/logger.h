#ifndef _PELLENGINE_GRAPHICS_LOGGER_H_
#define _PELLENGINE_GRAPHICS_LOGGER_H_

#include <string>
#include <iostream>

#ifdef ANDROID
  #include <android/log.h>
#endif

namespace pellengine {

enum LogSeverity {
  INFO,
  ERROR
};

class Logger {
 public:
  static void print(LogSeverity severity, std::string string);
  static void println(LogSeverity severity, std::string string);
  static void vprint(LogSeverity severity, std::string string, ...);
  static void vprintln(LogSeverity severity, std::string string, ...);

};

}

#endif