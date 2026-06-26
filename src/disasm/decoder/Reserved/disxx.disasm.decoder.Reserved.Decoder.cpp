module;

#include <functional>
#include <utility>
#include <cstdint>
#include <memory>
#include <limits>
#include <format>
#include <string>

module disxx.disasm.decoder.Reserved.Decoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.utility.bits;

import disxx.disasm.decoder.Reserved.UDF.SubDecoder;

namespace disxx::disasm::decoder::Reserved
{
	Decoder::Decoder(void) noexcept
		: disxx::disasm::decoder::abstract::Decoder{}
	{}

	Decoder::Decoder(std::uint32_t insn, std::uint64_t addr) noexcept
		: disxx::disasm::decoder::abstract::Decoder{insn, addr}
	{}

	Decoder::Decoder(const Decoder &other) noexcept(false)
		: disxx::disasm::decoder::abstract::Decoder{other.m_Insn, other.m_ProgramCounter}
	{
		auto cloned{other.m_pSubDecoder->Clone()};
		this->m_pSubDecoder = cloned.release();
	}

	Decoder &Decoder::operator=(const Decoder &other) noexcept(false)
	{
		if (this != &other) [[likely]]
		{
			if (this->m_pSubDecoder) [[likely]]
				this->m_pSubDecoder.Delete();
			auto cloned{other.m_pSubDecoder->Clone()};
			this->m_pSubDecoder = cloned.release();
			this->m_ProgramCounter = other.m_ProgramCounter;
			this->m_Insn = other.m_Insn;
		}

		return *this;
	}

	Decoder::Decoder(Decoder &&other) noexcept
		: disxx::disasm::decoder::abstract::Decoder{std::forward<Decoder &&>(other)}
	{}

	Decoder &Decoder::operator=(Decoder &&other) noexcept
	{
		disxx::disasm::decoder::abstract::Decoder::operator=(std::forward<Decoder &&>(other));
		return *this;
	}

	std::expected
	<
		std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder>,
		disxx::utility::error::DisassemblyError
	> Decoder::__GetDecoder(void) const noexcept
	{
		// +-+---+----+---+
		// |0|op0|0000|op1|
		// +-+---+----+---+

		unsigned short int op0, op1;
		op0 = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
		op1 = bits::extract<unsigned short int, std::uint32_t, 16, 24>(this->m_Insn);

		if (op0 == 0b00 && op1 == 0b000000000) [[likely]]
			return std::make_unique<UDF::SubDecoder>(UDF::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
	}
} /* disxx::disasm::decoder::Reserved */
