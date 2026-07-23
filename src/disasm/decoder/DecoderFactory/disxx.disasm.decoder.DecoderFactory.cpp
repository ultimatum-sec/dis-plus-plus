module;

#include <type_traits>
#include <cstdint>
#include <memory>

module disxx.disasm.decoder.DecoderFactory;

import :PseudoDecoder;

import disxx.disasm.utility.bits;
import disxx.disasm.decoder.abstract.Decoder;
import disxx.disasm.decoder.Reserved.Decoder;
import disxx.disasm.decoder.DataProcessingImmediate.Decoder;
import disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Decoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.Decoder;
import disxx.disasm.decoder.LoadsAndStores.Decoder;

namespace disxx::disasm::decoder
{
	std::unique_ptr<abstract::Decoder> DecoderFactory::GetDecoder(std::uint32_t insn, std::uint64_t PC) noexcept
	{
	    // +---+---+
	    // |op0|op1|
	    // +---+---+

	    unsigned short int op0, op1;
	    op0 = bits::extract<unsigned short int, std::uint32_t, 29, 31>(insn);
	    op1 = bits::extract<unsigned short int, std::uint32_t, 25, 28>(insn);
	
	    if (op0 == 0b00 && op1 == 0b0000)
	        return std::make_unique<Reserved::Decoder>(insn, PC);
	    else if ((op1 >> 1) == 0b100)
	        return std::make_unique<DataProcessingImmediate::Decoder>(insn, PC);
	    else if ((op1 >> 1) == 0b101)
	        return std::make_unique<BranchesExceptionsAndSystemInstructions::Decoder>(insn, PC);
	    else if ((op1 & ~(1 << 3)) == 0b0101)
	        return std::make_unique<DataProcessingRegister::Decoder>(insn, PC);
	    else if ((op1 & ~(1 << 3)) == 0b0111)
	        return std::make_unique<DataProcessingScalarFPAndAdvancedSIMD::Decoder>(insn, PC);
	    else if ((op1 & 0b0101) == 0b0100)
	        return std::make_unique<LoadsAndStores::Decoder>(insn, PC);
		else
			return std::make_unique<PseudoInstruction::Decoder>(insn, PC);
	}
} /* disxx::disasm */
