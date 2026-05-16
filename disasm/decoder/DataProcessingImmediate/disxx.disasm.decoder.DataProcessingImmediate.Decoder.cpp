module;

#include <cstdint>
#include <memory>

module disxx.disasm.decoder.DataProcessingImmediate.Decoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.utility.bits;

import disxx.disasm.decoder.DataProcessingImmediate.MoveWideImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.Bitfield.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.Extract.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.DataProcessing1SourceImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.MinMaxImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediateWithTags.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.PCRelAddressing.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingImmediate.LogicalImmediate.SubDecoder;

namespace disxx::disasm::decoder::DataProcessingImmediate
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
	{
		// +---+---+---+
		// |op0|100|op1|
		// +---+---+---+

		unsigned short int op0, op1;
		op0 = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
		op1 = bits::extract<unsigned short int, std::uint32_t, 22, 25>(this->m_Insn);

		if (op0 == 0b11 && (op1 >> 1) == 0b111)
			return std::make_unique<DataProcessing1SourceImmediate::SubDecoder>(DataProcessing1SourceImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op1 >> 2) == 0b00)
			return std::make_unique<PCRelAddressing::SubDecoder>(PCRelAddressing::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op1 >> 1) == 0b010)
			return std::make_unique<AddSubstractImmediate::SubDecoder>(AddSubstractImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b0110)
			return std::make_unique<AddSubstractImmediateWithTags::SubDecoder>(AddSubstractImmediateWithTags::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b0111)
			return std::make_unique<MinMaxImmediate::SubDecoder>(MinMaxImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op1 >> 1) == 0b100)
			return std::make_unique<LogicalImmediate::SubDecoder>(LogicalImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op1 >> 1) == 0b101)
			return std::make_unique<MoveWideImmediate::SubDecoder>(MoveWideImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op1 >> 1) == 0b110)
			return std::make_unique<Bitfield::SubDecoder>(Bitfield::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 != 0b11 && (op1 >> 1) == 0b111)
			return std::make_unique<Extract::SubDecoder>(Extract::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
	}
} /* DataProcessingImmediate */
