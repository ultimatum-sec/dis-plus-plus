module;

#include <exception>
#include <string>

module disxx.utility.error.ParserError;

namespace disxx::utility::error
{
	ParserError::ParserError(void) noexcept = default;
	
	ParserError::ParserError(std::string err) noexcept
		: std::exception{}
		, m_Error{err}
	{}
	
	const char *ParserError::what(void) const noexcept
	{ return this->m_Error.c_str(); }
} /* disxx::utility::error */
