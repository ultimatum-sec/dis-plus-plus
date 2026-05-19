module;

#include <disconf.hpp>

#include <functional>
#include <utility>
#include <cstdint>
#include <memory>
#include <string>

export module disxx.disasm.decoder.DecoderFactory:PseudoDecoder;

import disxx.disasm.decoder.abstract.Decoder;
import disxx.utility.error.DisassemblyError;

namespace disxx::disasm::decoder::PseudoInstruction
{
	// This decoder serves as a stub for undecoded instructions.
	// It always returns an error.
	class __DISXX_PRIVATE__ [[nodiscard]] Decoder final : public disxx::disasm::decoder::abstract::Decoder
	{
	  protected:
		std::expected
		<
			std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder>,
			disxx::utility::error::DisassemblyError
		> __GetDecoder(void) const noexcept override;

	  public:
		explicit Decoder(void) noexcept;
		explicit Decoder(std::uint32_t, std::uint64_t) noexcept;
	
		explicit Decoder(const Decoder &other) noexcept(false);
		Decoder &operator=(const Decoder &other) noexcept(false);

		explicit Decoder(Decoder &&other) noexcept;
		Decoder &operator=(Decoder &&other) noexcept;
	};

	Decoder::Decoder(void) noexcept
		: disxx::disasm::decoder::abstract::Decoder{}
	{}

	Decoder::Decoder(std::uint32_t insn, std::uint64_t addr) noexcept
		: disxx::disasm::decoder::abstract::Decoder{insn, addr}
	{}

	Decoder::Decoder(const Decoder &other) noexcept(false)
		: disxx::disasm::decoder::abstract::Decoder{other.m_Insn, other.m_ProgramCounter}
	{
		if (this->m_pSubDecoder)
			this->m_pSubDecoder.Get().reset();
		auto cloned{other.m_pSubDecoder->Clone()};
		this->m_pSubDecoder.Get()= std::move(cloned);
	}

	Decoder &Decoder::operator=(const Decoder &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pSubDecoder)
				this->m_pSubDecoder.Get().reset();
			auto cloned{other.m_pSubDecoder->Clone()};
			this->m_pSubDecoder.Get() = std::move(cloned);
		
			this->m_ProgramCounter = other.m_ProgramCounter;
			this->m_Insn = other.m_Insn;
		}

		return *this;
	}

	Decoder::Decoder(Decoder &&other) noexcept
		: disxx::disasm::decoder::abstract::Decoder{std::move(other)}
	{}

	Decoder &Decoder::operator=(Decoder &&other) noexcept
	{
		disxx::disasm::decoder::abstract::Decoder::operator=(std::move(other));
		
		return *this;
	}

	std::expected
	<
		std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder>,
		disxx::utility::error::DisassemblyError
	> Decoder::__GetDecoder(void) const noexcept
	{ return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}}; }
} /* disxx::disasm::decoder::PseudoInstruction */
