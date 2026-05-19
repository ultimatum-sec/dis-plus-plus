module;

#include <utility>
#include <cstdint>
#include <memory>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Decoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.utility.bits;

import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.TestAndBranchImmediate.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.UnconditionalBranchRegister.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Barriers.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterMove.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.UnconditionalBranchImmediate.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ConditionalBranchImmediate.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegisterWithImmediateAndBranch.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterPairMove.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDs.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareAndBranchImmediate.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemWithResult.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemPairInstructionIDs.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDsWithRegisterArgument.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareBytesHalfwordsInRegisterAndBranch.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Hints.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.PSTATE.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegistersAndBranch.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.MiscellaneousBranchImmediate.SubDecoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ExceptionGeneration.SubDecoder;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions
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
		// +---+---+---+---+
		// |op0|101|op1|op2|
		// +---+---+---+---+

		unsigned short int op0, op1, op2;
		op0 = bits::extract<unsigned short int, std::uint32_t, 29, 31>(this->m_Insn);
		op1 = bits::extract<unsigned short int, std::uint32_t, 12, 25>(this->m_Insn);
		op2 = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

		if (op0 == 0b010 && (op1 & 0b11000000000000) == 0b00000000000000)
			return std::make_unique<ConditionalBranchImmediate::SubDecoder>(ConditionalBranchImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b010 && (op1 & 0b11000000000000) == 0b01000000000000)
			return std::make_unique<MiscellaneousBranchImmediate::SubDecoder>(MiscellaneousBranchImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b011 && (op1 & 0b11000000001000) == 0b00000000001000)
			return std::make_unique<CompareBytesHalfwordsInRegisterAndBranch::SubDecoder>(CompareBytesHalfwordsInRegisterAndBranch::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11000000000000) == 0b00000000000000)
			return std::make_unique<ExceptionGeneration::SubDecoder>(ExceptionGeneration::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && op1 == 0b01000000110001)
			return std::make_unique<SystemInstructionIDsWithRegisterArgument::SubDecoder>(SystemInstructionIDsWithRegisterArgument::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && op1 == 0b01000000110010 && op2 == 0b11111)
			return std::make_unique<Hints::SubDecoder>(Hints::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && op1 == 0b01000000110011)
			return std::make_unique<Barriers::SubDecoder>(Barriers::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11111110001111) == 0b01000000000100)
			return std::make_unique<PSTATE::SubDecoder>(PSTATE::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11111110000000) == 0b01001000000000)
			return std::make_unique<SystemWithResult::SubDecoder>(SystemWithResult::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11110110000000) == 0b01000010000000)
			return std::make_unique<SystemInstructionIDs::SubDecoder>(SystemInstructionIDs::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11110100000000) == 0b01000100000000)
			return std::make_unique<SystemRegisterMove::SubDecoder>(SystemRegisterMove::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11110110000000) == 0b01010010000000)
			return std::make_unique<SystemPairInstructionIDs::SubDecoder>(SystemPairInstructionIDs::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b11110100000000) == 0b01010100000000)
			return std::make_unique<SystemRegisterPairMove::SubDecoder>(SystemRegisterPairMove::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b110 && (op1 & 0b10000000000000) == 0b10000000000000)
			return std::make_unique<UnconditionalBranchRegister::SubDecoder>(UnconditionalBranchRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b011) == 0b000)
			return std::make_unique<UnconditionalBranchImmediate::SubDecoder>(UnconditionalBranchImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b011) == 0b001 && (op1 & 0b10000000000000) == 0b00000000000000)
			return std::make_unique<CompareAndBranchImmediate::SubDecoder>(CompareAndBranchImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b011) == 0b001 && (op1 & 0b10000000000000) == 0b10000000000000)
			return std::make_unique<TestAndBranchImmediate::SubDecoder>(TestAndBranchImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b011) == 0b011 && (op1 & 0b11000000001100) == 0b00000000000000)
			return std::make_unique<CompareRegistersAndBranch::SubDecoder>(CompareRegistersAndBranch::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b011) == 0b011 && (op1 & 0b11000000000100) == 0b01000000000000)
			return std::make_unique<CompareRegisterWithImmediateAndBranch::SubDecoder>(CompareRegisterWithImmediateAndBranch::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions */
