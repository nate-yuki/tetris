/**
 * @file  logger.cpp
 * @brief Implementation of the class Logger and its helper functions.
 */
#include "logger.hpp"
#include "exceptions.hpp"


Logger Logger::logger;


Logger::Logger () {}

Logger *Logger::get ()
{
    return &logger;
}

void Logger::init (const std::string &path)
{
    fout.open(path);
    if (fout.fail())
    {
        std::string msg = "Could not open \"" + path + "\"";
        throw ExceptionFile(__FILE__, __LINE__, msg.c_str());
    }
    
    currLen = 0;
}

void Logger::write_to_buffer (const std::string &msg, bool flush)
{
    buffer << msg;

    if (flush || (currLen += msg.size() > MAX_BUFFER_LEN))
    {
        this->flush();
    }
}

void Logger::flush ()
{
    fout << buffer.str() << std::flush;
    if (fout.fail())
    {
        throw ExceptionFile(__FILE__, __LINE__, "Could not write to log");
    }

    buffer.str("");
    currLen = 0;
}

void Logger::free ()
{
    fout.close();
}


const std::string make_log_msg (const char *file, int line, const std::string &msg)
{
    std::stringstream _msg;
    _msg << file << "\t" << line << ": \t" << msg << "\n";
    return _msg.str();
}

void log (const std::string &msg, const char *file, int line, bool flush)
{
    Logger::get()->write_to_buffer(make_log_msg(file, line, msg), flush);
}
