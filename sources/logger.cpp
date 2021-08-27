#include "logger.h"

#include <config.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Logger::logger() {
  static std::shared_ptr<spdlog::logger> _logger;
  if (!_logger) {
    auto consoleLogger = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleLogger->set_level(LOG_LEVEL_CONSOLE);

    time_t rawtime = time(nullptr);
    struct tm* tm = localtime(&rawtime);
    char logsFilePath[4096];
    sprintf(logsFilePath, "%s/auto-sdr %04d-%02d-%02d %02d:%02d:%02d.txt", LOG_DIR.c_str(), tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    auto fileLogger = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logsFilePath, true);
    fileLogger->set_level(LOG_LEVEL_FILE);

    std::initializer_list<spdlog::sink_ptr> loggers{consoleLogger, fileLogger};
    _logger = std::make_shared<spdlog::logger>("auto-sdr", loggers);
    _logger->set_level(spdlog::level::trace);
  }
  return _logger;
}