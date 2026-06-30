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

template <typename T>
concept BytesRange = std::ranges::range<T> && std::same_as
<
	typename T::value_type,
	typename disxx::disasm::Bytes
>;

export namespace disxx::disasm
{
	class [[nodiscard]] __DISXX_EXPORT__ Disassembler
	{
	  public:
		explicit Disassembler(void) noexcept = default;

		explicit Disassembler(const Disassembler &) noexcept = default;
		Disassembler &operator=(const Disassembler &) noexcept = default;

		~Disassembler(void) noexcept = default;

		Instruction DisassembleSingle(const Bytes, const Address) const noexcept(false);
		template <BytesRange T>
		std::vector<Instruction> DisassembleAll(std::ranges::ref_view<T>, Address) const noexcept(false);
	};

	template <BytesRange T>
	std::vector<Instruction> Disassembler::DisassembleAll(std::ranges::ref_view<T> ref, Address addr) const noexcept(false)
	{
		std::vector<Instruction> vec{};
		for (const auto &insn : ref)
			vec.emplace_back(this->DisassembleSingle(insn, addr++));
		return vec;
	}
} /* disxx::disasm */
