//
// Created by alireza on 22/06/22.
//

#ifndef DISCOT_SRC_LOGGER_H_
#define DISCOT_SRC_LOGGER_H_

#include "spdlog/spdlog.h"
#include "memory"
#include "string"

namespace Scot {

class Logger {
 public:

  Logger();
  ~Logger() = default;

  void logCritical(std::string message, int rank);

  void logError(std::string message, int rank);

  void logWarning(std::string message, int rank);

  void logInfo(std::string message, int rank);

  void logDebug(std::string message, int rank);

 private:
  std::shared_ptr<spdlog::logger> logger_;
};

}

#endif //DISCOT_SRC_LOGGER_H_
