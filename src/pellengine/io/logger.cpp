#include "logger.h"

namespace pellengine {

void Logger::print(LogSeverity severity, std::string string) {
  #ifdef ANDROID
    switch(severity) {
      case INFO:
        __android_log_write(ANDROID_LOG_INFO, "PellEngine", string.c_str());
        break;

      case ERROR:
        __android_log_write(ANDROID_LOG_ERROR, "PellEngine", string.c_str());
        break;
    }
  #endif
}

void Logger::println(LogSeverity severity, std::string string) {
  string += "\n";
  Logger::print(severity, string);
}

void Logger::vprint(LogSeverity severity, std::string string, ...) {
  va_list args;
  va_start(args, string);
  uint32_t msgSize = std::snprintf(NULL, 0, string.c_str(), args);
  std::string msg;
  msg.resize(msgSize);
  sprintf(msg.data(), string.c_str(), args); 
  Logger::print(severity, msg);
  va_end(args);
}

void Logger::vprintln(LogSeverity severity, std::string string, ...) {
  va_list args;
  va_start(args, string);
  uint32_t msgSize = std::snprintf(NULL, 0, string.c_str(), args);
  std::string msg;
  msg.resize(msgSize);
  sprintf(msg.data(), string.c_str(), args); 
  Logger::println(severity, msg);
  va_end(args);
}

}