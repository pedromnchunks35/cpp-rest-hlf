#ifndef DEBUG_FUNC
#include <iostream>
#define DEBUG_FUNC
#ifdef PR_DEBUG //! THIS IS PASSED IN THE COMPILER
#define LOG_W(x) std::cout << "[DEBUG]>>> " << x << std::endl
#else
#define LOG_W(X)
#endif
#endif