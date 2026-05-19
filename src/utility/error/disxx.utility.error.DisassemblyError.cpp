module;

#include <format>
#include <string>

module disxx.utility.error.DisassemblyError;

namespace disxx::utility::error
{
	DisassemblyError::DisassemblyError(void) noexcept
		: m_Error{}
	{}

	DisassemblyError::DisassemblyError(std::uint32_t insn) noexcept
		: m_Error{std::format(".long {:#08x}", insn)}
	{}

	bool DisassemblyError::operator==(const DisassemblyError &other) noexcept
	{ return this->m_Error == other.m_Error; }

	bool DisassemblyError::operator!=(const DisassemblyError &other) noexcept
	{ return this->m_Error != other.m_Error; }

	const char *DisassemblyError::what(void) const noexcept
	{ return this->m_Error.c_str(); }
} /* disxx::util::error */
