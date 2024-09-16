#ifndef LOGGER_HH
#define LOGGER_HH

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <spdlog/fmt/ostr.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include "constants.h"

// Singleton Logger class that will log to both the console and a log file

/**
 * @class Logger
 * @brief Singleton class to log messages to the console and a log file.
 * 
 * This class is a singleton class that logs messages to the console and a log file. It provides functions to log messages with different log levels and formats.
 * The log file is stored in the `LOGS_DIR` with a timestamped name.
 */
class Logger
{
public:

    /**
     * @brief Log a message with the visibility of info.
     */
    static void info(const std::string &message)
    {
        getInstance().logger_->info(message);
    }

    /**
     * @brief Log a precise double value with the visibility of info.
     * 
     * Format: "label: value"
     */
    static void info_double(const std::string &label, double value)
    {
        Logger::log_double(spdlog::level::info, label, value);
    }

    /**
     * @brief Log a message with the visibility of debug.
     */
    static void debug(const std::string &message)
    {
        getInstance().logger_->debug(message);
    }

    /**
     * @brief Log a precise double value with the visibility of debug.
     * 
     * Format: "label: value"
     */
    static void debug_double(const std::string &label, double value)
    {
        Logger::log_double(spdlog::level::debug, label, value);
    }

    /**
     * @brief Log a message with the visibility of warn.
     */
    static void warn(const std::string &message)
    {
        getInstance().logger_->warn(message);
    }

    /**
     * @brief Log a message with the visibility of error.
     */
    static void error(const std::string &message)
    {
        getInstance().logger_->error(message);
    }

    /**
     * @brief Log a message with the visibility of critical.
     */
    static void critical(const std::string &message)
    {
        getInstance().logger_->critical(message);
    }

    /**
     * @brief Log a message with a specified log level.
     * @param level Log level of the message.
     * @param message Message to log.
     */
    static void log(spdlog::level::level_enum level, const std::string &message)
    {
        getInstance().logger_->log(level, message);
    }

    /**
     * @brief Log a message displaying the granularity values for a component.
     */
    static void log_granularity(int component, double granularity_offset, double granularity_slope)
    {
        Logger::log_double_2(spdlog::level::info, "Granularities for harmonic B" + std::to_string(component), "Offset", "Slope", granularity_offset, granularity_slope);
    }

    /**
     * @brief Log a message displaying the extrapolated values for a component.
     */
    static void log_extrapolated_values(int component, double extrapolated_offset, double extrapolated_slope)
    {
        Logger::log_double_2(spdlog::level::info, "Extrapolated values for harmonic B" + std::to_string(component), "Offset", "Slope", extrapolated_offset, extrapolated_slope);
    }

    /**
     * @brief Log a message displaying the new parameter ranges for a component.
     */
    static void log_parameter_ranges(int component, double new_offset_min, double new_offset_max, double new_slope_min, double new_slope_max)
    {
        getInstance().logger_->info("New parameter ranges for harmonic B" + std::to_string(component) + ": Offset: [" + format_message_scientific(new_offset_min) + ", " + format_message_scientific(new_offset_max) + "], Slope: [" + format_message_scientific(new_slope_min) + ", " + format_message_scientific(new_slope_max) + "].");
    }

    /**
     * @brief Log a message displaying the reverted configuration for a component.
     */
    static void log_reverted_config(int component, double offset, double slope)
    {
        Logger::log_double_2(spdlog::level::warn, "Reverted to the previous configuration", "Offset", "Slope", offset, slope);
    }

    /**
     * @brief Log the current timestamp with a label.
     * 
     * Format: "label: timestamp"
     */
    static void log_timestamp(std::string label)
    {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << now_ms.count();

        std::string timestamp = ss.str();
        getInstance().logger_->trace("{}: {}", label, timestamp);
    }


    /**
     * @brief Enable trace level logging.
     * 
     * This function enables trace level logging, which logs everything including trace messages.
     */
    static void enable_trace()
    {
        getInstance().logger_->set_level(spdlog::level::trace);
        getInstance().logger_->flush_on(spdlog::level::trace);
    }

    /**
     * @brief Disable all logging.
     * 
     * This function disables all logging for this Logger instance.
     */
    static void deactivate()
    {
        getInstance().logger_->sinks().clear();
    }


private:

    /**
     * @brief Constructor for the Logger class.
     * 
     * The constructor creates a log directory if it doesn't exist and generates a timestamped log file name.
     * It sets up a multi-sink logger with a console sink and a file sink.
     */
    Logger()
    {
        // Create log directory if it doesn't exist
        std::filesystem::create_directories(LOGS_DIR);

        // Generate timestamped log file name
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d_%H-%M-%S");
        std::string timestamp = ss.str();
        std::string log_filename = LOGS_DIR + "log_" + timestamp + ".txt";

        // Create a multi-sink logger (console + file)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename, true);

        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

        logger_ = std::make_shared<spdlog::logger>("multi_sink", begin(sinks), end(sinks));
        spdlog::register_logger(logger_);

        logger_->set_level(spdlog::level::debug); // Set to debug to log everything but trace
        logger_->flush_on(spdlog::level::debug);  // Flush on every log
    }

    ~Logger()
    {
        spdlog::drop_all(); // Cleanup the logger
    }

    std::shared_ptr<spdlog::logger> logger_;

    static Logger &getInstance()
    {
        static Logger instance;
        return instance;
    }

    /**
     * @brief Log a double value with a label and high precision.
     * 
     * Format: "label: value"
     */
    static void log_double(spdlog::level::level_enum level, const std::string &label, double value)
    {
        getInstance().logger_->log(level, "{}: {}", label, format_message_scientific(value));
    }

    /**
     * @brief Log two double values with labels and high precision.
     * 
     * Format: "prefix: label_1: value_1, label_2: value_2"
     */
    static void log_double_2(spdlog::level::level_enum level, const std::string &prefix, const std::string &label_1, const std::string &label_2, double value_1, double value_2)
    {
        getInstance().logger_->log(level, "{}: {}: {}, {}: {}", prefix, label_1, format_message_scientific(value_1), label_2, format_message_scientific(value_2));
    }

    /**
     * @brief Format a double value to a string with high precision.
     * @param value Double value to format.
     * @return Formatted string of the double value.
     * 
     * If the value is between 1e-4 and 1e4, it will be formatted normally.
     * Otherwise, it will be formatted in scientific notation.
     */
    static std::string format_message_scientific(double value)
    {
        // only do the formatting for small or big numbers
        if (std::abs(value) > 1e-4 && std::abs(value) < 1e4)
        {
            return std::to_string(value);
        }

        std::stringstream ss;
        ss << std::scientific << value;
        return ss.str();
    }

    // Delete copy constructor and assignment operator
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

#endif // LOGGER_HH
