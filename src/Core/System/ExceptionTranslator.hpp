#pragma once

namespace ogl
{
#if defined(OGL_MSVC)
struct SEHException : public std::exception
{
	SEHException() = delete;
	SEHException(const u32 inValue);

	u32 value() const noexcept;

private:
	const u32 m_value;
};
#endif

/**
 *  This class is used specifically to catch floating point exceptions on Windows
 */
struct ExceptionTranslator
{
	ExceptionTranslator();
	OGL_DISALLOW_COPY_MOVE(ExceptionTranslator);
	~ExceptionTranslator();

private:
#if defined(OGL_MSVC)
	void* m_oldFunction = nullptr;
#endif
};
}
