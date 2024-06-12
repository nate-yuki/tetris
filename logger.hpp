/**
 * @file  logger.hpp
 * @brief Include file for Logger class and its helper functions.
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP


#include "constants.hpp"

#include <string>
#include <fstream>
#include <sstream>


/**
 * @brief Logging singleton class.
 * @example
 * 
 *     Logger::get()->init("log.txt");
 *     log("Log message", __FILE__, __LINE__);
 *     log("Another log message", __FILE__, __LINE__);
 *     Logger::get()->flush(); // Flush before closing
 *     Logger::get()->free();
 */
class Logger
{
public:
    /// Get the singleton instance.
    static Logger *get();

    /// Max characters stored in the buffer before flushing.
    static constexpr int MAX_BUFFER_LEN = 1024;

    /**
     * @brief Open `path` for logging.
     * @param path Output file path.
     * @throws `ExceptionFile` thrown if `path` does not exist or is not readable.
     */
    void init(const std::string &path);

    /**
     * @brief Write `msg` to the buffer.
     * @param msg The message.
     * @param flush If true, flush after writing.
     */
    void write_to_buffer(const std::string &msg, bool flush=FLUSH_ALL);

    /**
     * @brief Flush the buffer.
     * @throws `ExceptionFile` thrown if buffer contents could not be written to path.
     */
    void flush();

    /// Close the output file.
    void free();

private:
    static Logger logger;
    Logger();

    std::ofstream fout;
    std::stringstream buffer;
    int currLen;
};


/**
 * @brief Make a message in logger format.
 * @note Should only be called by `log`.
 * @see log
 */
const std::string make_log_msg(const char *file, int line, const std::string &msg);

/**
 * @brief Write `msg` to `Logger` buffer.
 * @param msg Log message.
 * @param file `__FILE__` initialized at the calling line.
 * @param line `__LINE__` initialized at the calling line.
 */
void log(const std::string &msg, const char *file, int line);


#endif
