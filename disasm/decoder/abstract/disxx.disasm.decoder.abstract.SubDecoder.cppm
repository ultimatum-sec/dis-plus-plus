module;

#include <disconf.hpp>

export module disxx.disasm.decoder.abstract.SubDecoder;

export import <functional>;
export import <expected>;
export import <optional>;
export import <memory>;
export import <vector>;

export import disxx.utility.error.DisassemblyError;

export import disxx.disasm.InstructionID;
export import disxx.disasm.operand.AbstractOperand;

export using DisassemblyResult = std::expected
<
	std::pair
	<
		InstructionID,
		std::vector
		<
			std::unique_ptr
			<disxx::disasm::operand::AbstractOperand>
		>
	>,
	disxx::utility::error::DisassemblyError
>;

export namespace disxx::disasm::decoder::abstract
{
	class __DISXX_PRIVATE__ [[nodiscard]] SubDecoder
	{
	  protected:
		// m_Operands will be moved from this class after Decode call!
		[[maybe_unused]] mutable std::vector
		<
			// These pointers CAN'T be zero - 
			// - there is no opportunity somewhere in the code to
			// create a nullptr for this vector
			std::unique_ptr
			<disxx::disasm::operand::AbstractOperand>
		> m_Operands;

		// Possible reference to one of immediate operands
		mutable std::optional<std::reference_wrapper<disxx::disasm::operand::AbstractOperand>> m_ProgramCounterRelevantAddress;
	
		// Program counter address	
		[[maybe_unused]] std::uint64_t m_ProgramCounter;
	
		// Bytes of the instruction (4 bytes for AArch64)
		std::uint32_t m_Insn;
		
		// explicit padding
		[[maybe_unused]] std::uint32_t m_Padding;

	  public:
		explicit SubDecoder(void) noexcept;
		explicit SubDecoder(std::uint32_t, std::uint64_t) noexcept;

		virtual ~SubDecoder(void) noexcept = default;

		explicit SubDecoder(const SubDecoder &) noexcept;
		SubDecoder &operator=(const SubDecoder &) noexcept;

		explicit SubDecoder(SubDecoder &&) noexcept;
		SubDecoder &operator=(SubDecoder &&) noexcept;

		const std::optional<std::reference_wrapper<disxx::disasm::operand::AbstractOperand>> &
		GetProgramCounterRelevantAddress(void) const noexcept;

		virtual std::unique_ptr<SubDecoder> Clone(void) const noexcept = 0;
		virtual DisassemblyResult Decode(void) const = 0;
	};
} /* disxx::disasm::analyzer::abstract */
