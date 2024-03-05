#include "Core/System/ExceptionTranslator.hpp"

#if defined(OGL_MSVC)
	#include "Libraries/WindowsApi.hpp"
	#include "Core/System/SignalHandler.hpp"

	#include <eh.h>
#endif

namespace ogl
{
#if defined(OGL_MSVC)
/*****************************************************************************/
SEHException::SEHException(const u32 inValue) :
	m_value(inValue)
{
}

u32 SEHException::value() const noexcept
{
	return m_value;
}
void translatorFunction(u32 inValue, EXCEPTION_POINTERS*)
{
	if (inValue == 3221225620)
	{
		::raise(SIGFPE);
	}
	else if (inValue == 3221225477)
	{
		::raise(SIGSEGV);
	}
	else
	{
		log_warning("unknown / unhandled exception:", inValue);
	}

	throw SEHException(inValue);
}
#endif

/*****************************************************************************/
ExceptionTranslator::ExceptionTranslator()
{
#if defined(OGL_MSVC)
	m_oldFunction = _set_se_translator(translatorFunction);
#endif
}

/*****************************************************************************/
ExceptionTranslator::~ExceptionTranslator()
{
#if defined(OGL_MSVC)
	_set_se_translator(static_cast<_se_translator_function>(m_oldFunction));
#endif
}
}
