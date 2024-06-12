/**
 * @file  exceptions.cpp
 * @brief Implementation of the exception classes and their helper functions.
 */

#include "exceptions.hpp"

#include <sstream>


ExceptionSDL::ExceptionSDL (const char *file, int line, const char *msg)
    : msg(make_err_msg(file, line, msg, "SDL exception"))
{}

const std::string &ExceptionSDL::what () const
{
    return msg;
}

int ExceptionSDL::get_exit_code () const
{
    return EXCEPTION_SDL;
}


ExceptionFile::ExceptionFile (const char *file, int line, const char *msg)
    : msg(make_err_msg(file, line, msg, "File I/O exception"))
{}

const std::string &ExceptionFile::what () const
{
    return msg;
}

int ExceptionFile::get_exit_code () const
{
    return EXCEPTION_FILE;
}


const std::string make_err_msg (
    const char *file,
    int line,
    const char *msg,
    const std::string &type
)
{
    std::stringstream _msg;
    _msg << type << " at " << file << "(" << line << "): " << msg;
    return _msg.str();
}
