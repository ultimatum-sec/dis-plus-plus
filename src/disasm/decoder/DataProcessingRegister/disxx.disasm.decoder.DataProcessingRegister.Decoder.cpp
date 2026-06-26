module disxx.disasm.decoder.DataProcessingRegister.Decoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.utility.bits;

import disxx.disasm.decoder.DataProcessingRegister.AddSubstractShiftedRegister.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.LogicalShiftedRegister.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.DataProcessing2Source.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.DataProcessing3Source.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.AddSubstractWithCarry.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.AddSubstractExtendedRegister.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.EvaluateIntoFlags.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.RotateRightIntoFlags.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.ConditionalSelect.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.DataProcessing1Source.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareRegister.SubDecoder;
import disxx.disasm.decoder.DataProcessingRegister.AddSubstractCheckedPointer.SubDecoder;

namespace disxx::disasm::decoder::DataProcessingRegister
{
	Decoder::Decoder(void) noexcept
		: disxx::disasm::decoder::abstract::Decoder{}
	{}

	Decoder::Decoder(std::uint32_t insn, std::uint64_t addr) noexcept
		: disxx::disasm::decoder::abstract::Decoder{insn, addr}
	{}

	Decoder::Decoder(const Decoder &other) noexcept
		: disxx::disasm::decoder::abstract::Decoder{other.m_Insn, other.m_ProgramCounter}
	{
		auto cloned{other.m_pSubDecoder->Clone()};
		this->m_pSubDecoder = cloned.release();
	}

	Decoder &Decoder::operator=(const Decoder &other) noexcept
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
		// +---+---+---+---+---+
		// |op0|op1|101|op2|op3|
		// +---+---+---+---+---+

		unsigned short int op0, op1, op2, op3;
		op0 = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
		op1 = bits::extract<unsigned short int, std::uint32_t, 28, 28>(this->m_Insn);
		op2 = bits::extract<unsigned short int, std::uint32_t, 21, 24>(this->m_Insn);
		op3 = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
	
		if (op0 == 0b0 && op1 == 0b1 && op2 == 0b0110)
			return std::make_unique<DataProcessing2Source::SubDecoder>(DataProcessing2Source::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b1 && op1 == 0b1 && op2 == 0b0110)
			return std::make_unique<DataProcessing1Source::SubDecoder>(DataProcessing1Source::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b0 && (op2 & 0b1000) == 0b0000)
			return std::make_unique<LogicalShiftedRegister::SubDecoder>(LogicalShiftedRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b0 && (op2 & 0b1001) == 0b1000)
			return std::make_unique<AddSubstractShiftedRegister::SubDecoder>(AddSubstractShiftedRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b0 && (op2 & 0b1001) == 0b1001)
			return std::make_unique<AddSubstractExtendedRegister::SubDecoder>(AddSubstractExtendedRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0000 && op3 == 0b000000)
			return std::make_unique<AddSubstractWithCarry::SubDecoder>(AddSubstractWithCarry::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0000 && (op3 & 0b111000) == 0b001000)
			return std::make_unique<AddSubstractCheckedPointer::SubDecoder>(AddSubstractCheckedPointer::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0000 && (op3 & 0b011111) == 0b000001)
			return std::make_unique<RotateRightIntoFlags::SubDecoder>(RotateRightIntoFlags::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0000 && (op3 & 0b001111) == 0b000010)
			return std::make_unique<EvaluateIntoFlags::SubDecoder>(EvaluateIntoFlags::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0010 && (op3 & 0b000010) == 0b000000)
			return std::make_unique<ConditionalCompareRegister::SubDecoder>(ConditionalCompareRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0010 && (op3 & 0b000010) == 0b000010)
			return std::make_unique<ConditionalCompareImmediate::SubDecoder>(ConditionalCompareImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && op2 == 0b0100)
			return std::make_unique<ConditionalSelect::SubDecoder>(ConditionalSelect::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op1 == 0b1 && (op2 & 0b1000) == 0b1000)
			return std::make_unique<DataProcessing3Source::SubDecoder>(DataProcessing3Source::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
	}
} /* disxx::disasm::decoder::DataProcessingRegister */
