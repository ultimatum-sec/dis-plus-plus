module;

#include <disconf.hpp>

export module disxx.disasm.decoder.abstract.Decoder;

export import disxx.utility.error.DisassemblyError;
import disxx.utility.wrapper.Pointer;

export import <expected>;

export import disxx.disasm.InstructionID;
export import disxx.disasm.operand.AbstractOperand;

export import disxx.disasm.decoder.abstract.SubDecoder;

export namespace disxx::disasm::decoder::abstract
{
	class __DISXX_PRIVATE__ [[nodiscard]] Decoder
	{
      protected:
		// Subdecoder (in some cases may become nullptr)
		disxx::utility::wrapper::Pointer<std::unique_ptr<SubDecoder>> m_pSubDecoder;
		
		// Instruction's address
		std::uint64_t m_ProgramCounter;
		
		// Instruction's bytes
		std::uint32_t m_Insn;

		// Explicit padding
		std::uint32_t m_Pad;

	  protected:
		virtual std::expected
		<
			std::unique_ptr<SubDecoder>,
			disxx::utility::error::DisassemblyError
		> __GetDecoder(void) const noexcept = 0;

  	  public:
		explicit Decoder(void) noexcept;
		explicit Decoder(std::uint32_t, std::uint64_t) noexcept;
		explicit Decoder(std::unique_ptr<SubDecoder> &&) noexcept;

		virtual ~Decoder(void) noexcept = default;

		//explicit Decoder(const Decoder &) noexcept(false);
		//Decoder &operator=(const Decoder &) noexcept(false);
	
		explicit Decoder(Decoder &&) noexcept;
		Decoder &operator=(Decoder &&) noexcept;

		bool IsProgramCounterRelevant(void) const noexcept(false);

		DisassemblyResult Decode(void) noexcept(false);
	};
} /* disxx::disasm::decoder::abstract */
