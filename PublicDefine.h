#pragma once

#include <iostream>

// 在触发myAssert时候，会触发该断点，便于调试
inline void breakpointFunction() {
	int i = 0;
 	std::cout << i++ << std::endl;
}

#define DEBUG_OUTPUT
#define DEBUG_ASSERT

#ifdef DEBUG_ASSERT
#define myAssert(exp) \
	if (!(exp))  {  \
		std::cout << "Assertion failed: " << #exp << std::endl; \
        std::cout << "line: " << __LINE__ << std::endl; \
        std::cout << "file: " << __FILE__ << std::endl; \
		breakpointFunction(); \
		system("pause"); \
        exit(EXIT_FAILURE); \
    }
#define myAssertPlus(exp, message) \
	if (!(exp))  {  \
		std::cout << "Assertion failed: " << #exp << std::endl; \
        std::cout << "Message: " << message << std::endl; \
        std::cout << "line: " << __LINE__ << std::endl; \
        std::cout << "file: " << __FILE__ << std::endl; \
		breakpointFunction(); \
		system("pause"); \
        exit(EXIT_FAILURE); \
    }
#define myAssertWithFunction(exp, func) \
	if (!(exp))  {  \
		std::cout << "Assertion failed: " << #exp << std::endl; \
        std::cout << "line: " << __LINE__ << std::endl; \
        std::cout << "file: " << __FILE__ << std::endl; \
		func(); \
		breakpointFunction(); \
		system("pause"); \
        exit(EXIT_FAILURE); \
    }
#else
#define myAssert(exp)
#define myAssertPlus(exp, message)
#endif

#ifdef DEBUG_OUTPUT
#define debugLine std::cout << std::endl
#define dt(a)  << (#a) << "=" << (a) << " "
#define debugI(a) std::cout dt(a) << std::endl
#define debugII(a, b) std::cout dt(a) dt(b) << std::endl
#define debugIII(a, b, c) std::cout dt(a) dt(b) dt(c) << std::endl
#define debugIV(a, b, c, d) std::cout dt(a) dt(b) dt(c) dt(d) << std::endl
#define debugV(a, b, c, d, e) std::cout dt(a) dt(b) dt(c) dt(d) dt(e) << std::endl
#else
#define debugLine
#define debugI(v)
#define debugII(a, b)
#define debugIII(a, b, c)
#define debugIV(a, b, c, d)
#define debugV(a, b, c, d, e)
#endif