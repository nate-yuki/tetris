/**
 * @file  exceptions.hpp
 * @brief Include file for exception classes and their helper functions.
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP


#include <string>


/**
 * @brief Virtual class for exceptions.
 * @example
 * Throwing a file I/O exception:
 * 
 *     if (fout.fail())
 *     {
 *         throw ExceptionFile(__FILE__, __LINE__, "Could not write to file");
 *     }
 */
class Exception
{
public:
    /// Get error message.
    virtual const std::string &what() const = 0;

    /// Get exit code.
    virtual int get_exit_code() const = 0;

    virtual ~Exception(){};
    
    enum ExitCode {
        EXCEPTION_SUCCESS, // No exception.
        EXCEPTION_SDL, // SDL exception.
        EXCEPTION_FILE, // File I/O exception.
    };
};

/// SDL exception class.
class ExceptionSDL: public Exception
{
public:
    ExceptionSDL(const char *file, int line, const char *msg);

    const std::string &what() const;
    int get_exit_code() const;

private:
    const std::string msg;
};


/// File I/O exception class.
class ExceptionFile: public Exception
{
public:
    ExceptionFile(const char *file, int line, const char *msg);

    const std::string &what() const;
    int get_exit_code() const;

private:
    const std::string msg;
};


/**
 * @brief Make a message in exception format.
 * @note Should only be called by error classes initializers.
 * @param file `__FILE__` initialized at the calling line.
 * @param line `__LINE__` initialized at the calling line.
 * @param msg Exception message.
 * @param type Exception type.
 * @return Message in exception format.
 */
const std::string make_err_msg(
    const char *file,
    int line,
    const char *msg,
    const std::string &type
);


#endif
