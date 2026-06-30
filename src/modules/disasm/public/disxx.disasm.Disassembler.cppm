module;

#include <disconf.hpp>

#include <unordered_map>
#include <string_view>
#include <sstream>
#include <regex>

export module disxx.disasm.Disassembler;

export import <expected>;
export import <concepts>;
export import <ranges>;
export import <format>;
export import <vector>;

export import disxx.utility.error.DisassemblyError;
export import disxx.disasm.Instruction;
export import disxx.disasm.Address;
export import disxx.disasm.Bytes;

export namespace disxx::disasm
{
	class [[nodiscard]] __DISXX_EXPORT__ Disassembler
	{
	  public:
		using Result = std::expected
		<
			Instruction,
			disxx::utility::error::DisassemblyError
		>;

	  public:
		explicit Disassembler(void) noexcept = default;

		explicit Disassembler(const Disassembler &) noexcept = default;
		Disassembler &operator=(const Disassembler &) noexcept = default;

		~Disassembler(void) noexcept = default;

		Result Disassemble(const Bytes, const Address) const noexcept(false);
	};
} /* disxx::disasm */
