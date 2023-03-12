#include "Core/Helpers.hpp"

namespace ogl
{
/*****************************************************************************/
bool String::endsWith(const std::string& inStr, std::string_view inSuffix)
{
	if (inStr.length() < inSuffix.length())
	{
		return false;
	}
	return std::equal(inSuffix.rbegin(), inSuffix.rend(), inSuffix.rbegin());
}

}
