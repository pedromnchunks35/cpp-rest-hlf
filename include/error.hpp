#ifndef ERROR_HANDLING_HPP
#define ERROR_HANDLING_HPP

#include <boost/system/error_code.hpp>
#include <iostream>

inline void fail(boost::system::error_code ec, const char *what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}
#endif // ERROR_HANDLING_HPP
