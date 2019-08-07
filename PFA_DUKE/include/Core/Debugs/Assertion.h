#ifndef _ASSERTION_H_
#define _ASSERTION_H_

#include <string>
#include <exception>

namespace Core::Debug
{
	/* static assert break program */
	void 	StaticAssert(bool condition, std::string file, int line);
	/* assert send an alert */
	void 	Assert(bool condition, std::string file, int line);
}

/* Macro to deactivate asserts */
#ifdef NDEBUG
	#define ASSERT(condition) ((void)0)
	#define STATICASSERT(condition) throw std::string("fail")
/* Macro to call asserts */
#else
	#define ASSERT(condition)\
	Core::Debug::Assert(condition, __FILE__, __LINE__)
	#define STATICASSERT(condition)\
	Core::Debug::StaticAssert(condition, __FILE__, __LINE__)
#endif

#endif //_ASSERTION_H_
