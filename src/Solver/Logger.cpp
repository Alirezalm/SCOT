//
// Created by alireza on 22/06/22.
//

#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
namespace Scot {
//todo: adjust logger format
Logger::Logger() {
  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
  console_sink->set_level(spdlog::level::debug);

//  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>("../logs/logs.txt", true);
//  file_sink->set_level(spdlog::level::debug);

//  spdlog::sinks_init_list sinks = {file_sink, console_sink};
  logger_ = std::make_shared<spdlog::logger>("console logger", console_sink);
//  logger_->set_pattern(">>>%^%l%$ %v<<<");
  logger_->set_level(spdlog::level::critical);

}
void Logger::logCritical(std::string message, int rank) {
  logger_->critical("node: {}: message: {}", rank, message);
}
void Logger::logError(std::string message, int rank) {
  logger_->error("node: {}: message: {}", rank, message);
}
void Logger::logWarning(std::string message, int rank) {
  logger_->warn("node: {}: message: {}", rank, message);
}
void Logger::logInfo(std::string message, int rank) {
  logger_->info("node: {}: message: {}", rank, message);
}
void Logger::logDebug(std::string message, int rank) {
  logger_->debug("node: {}: message: {}", rank, message);
}

}