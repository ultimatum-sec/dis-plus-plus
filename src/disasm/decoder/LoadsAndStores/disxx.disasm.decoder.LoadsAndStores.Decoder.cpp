module disxx.disasm.decoder.LoadsAndStores.Decoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.utility.bits;

import disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRWriteback.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.MemoryCopyAndMemorySet.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.CompareAndSwapUnprivileged.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.CompareAndSwapPairUnprivileged.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.CompareAndSwap.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.OneHundredAndTwentyEightBitAtomicMemoryOperations.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.GCS.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperationsUnprivileged.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePostIndexed.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.LDIAPPSTILP.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterPairPreIndexed.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePreIndexed.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.NoAllocatePairOffset.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructurePostIndexed.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructure.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRSIMDAndFP.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructures.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwapPair.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterPairOffset.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterPairPostIndexed.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperations.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.Ordered.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.CompareAndSwapPair.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterLiteral.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructuresPostIndexed.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwap.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.ExclusiveRegisterUnprivileged.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterUnprivileged.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterPAC.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterUnsignedImmediate.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRUnscaledImmediate.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.ExclusivePair.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.ExclusiveRegister.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.MemoryTags.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterOffset.SubDecoder;
import disxx.disasm.decoder.LoadsAndStores.RegisterUnscaledImmediate.SubDecoder;

namespace disxx::disasm::decoder::LoadsAndStores
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
		unsigned short int op0, op1, op2;
        op0 = bits::extract<unsigned short int, std::uint32_t, 28, 31>(this->m_Insn);
        op1 = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 10, 24>(this->m_Insn);

		if ((op0 & 0b1011) == 0b0000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b000100000000000)
			return std::make_unique<CompareAndSwapPair::SubDecoder>(CompareAndSwapPair::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b110000000000000)
			return std::make_unique<CompareAndSwapPairUnprivileged::SubDecoder>(CompareAndSwapPairUnprivileged::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && op1 == 0b1 && (op2 & 0b110111111000000) == 0b000000000000000)
			return std::make_unique<SIMDMultipleStructures::SubDecoder>(SIMDMultipleStructures::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && op1 == 0b1 && (op2 & 0b110100000000000) == 0b010000000000000)
			return std::make_unique<SIMDMultipleStructuresPostIndexed::SubDecoder>(SIMDMultipleStructuresPostIndexed::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && op1 == 0b1 && (op2 & 0b110011110000000) == 0b100000000000000)
			return std::make_unique<SIMDSingleStructure::SubDecoder>(SIMDSingleStructure::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && op1 == 0b1 && (op2 & 0b110000000000000) == 0b110000000000000)
			return std::make_unique<SIMDSingleStructurePostIndexed::SubDecoder>(SIMDSingleStructurePostIndexed::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0001 && op1 == 0b0 && (op2 & 0b100100000111111) == 0b100100000000010)
			return std::make_unique<RCWCompareAndSwap::SubDecoder>(RCWCompareAndSwap::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0001 && op1 == 0b0 && (op2 & 0b100100000111111) == 0b100100000000011)
			return std::make_unique<RCWCompareAndSwapPair::SubDecoder>(RCWCompareAndSwapPair::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0001 && op1 == 0b0 && (op2 & 0b100100000000011) == 0b100100000000000)
			return std::make_unique<OneHundredAndTwentyEightBitAtomicMemoryOperations::SubDecoder>(OneHundredAndTwentyEightBitAtomicMemoryOperations::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0001 && op1 == 0b0 && (op2 & 0b100100000000011) == 0b100100000000001)
			return std::make_unique<AtomicMemoryOperationsUnprivileged::SubDecoder>(AtomicMemoryOperationsUnprivileged::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b1101 && op1 == 0b0 && (op2 & 0b111111111100011) == 0b100011111000011)
			return std::make_unique<GCS::SubDecoder>(GCS::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if (op0 == 0b1101 && op1 == 0b0 && (op2 & 0b100100000000000) == 0b100100000000000)
			return std::make_unique<MemoryTags::SubDecoder>(MemoryTags::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b1000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b000100000000000)
			return std::make_unique<ExclusivePair::SubDecoder>(ExclusivePair::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b1000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b100000000000000)
			return std::make_unique<ExclusiveRegisterUnprivileged::SubDecoder>(ExclusiveRegisterUnprivileged::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b1000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b110000000000000)
			return std::make_unique<CompareAndSwapUnprivileged::SubDecoder>(CompareAndSwapUnprivileged::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b000000000000000)
			return std::make_unique<ExclusiveRegister::SubDecoder>(ExclusiveRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b010000000000000)
			return std::make_unique<Ordered::SubDecoder>(Ordered::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0000 && op1 == 0b0 && (op2 & 0b110100000000000) == 0b010100000000000)
			return std::make_unique<CompareAndSwap::SubDecoder>(CompareAndSwap::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0001 && op1 == 0b0 && (op2 & 0b110100000000011) == 0b100000000000010)
			return std::make_unique<LDIAPPSTILP::SubDecoder>(LDIAPPSTILP::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0001 && op1 == 0b0 && (op2 & 0b110111111111111) == 0b110000000000010)
			return std::make_unique<LDAPRSTLRWriteback::SubDecoder>(LDAPRSTLRWriteback::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0001 && op1 == 0b0 && (op2 & 0b100100000000011) == 0b100000000000000)
			return std::make_unique<LDAPRSTLRUnscaledImmediate::SubDecoder>(LDAPRSTLRUnscaledImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0001 && op1 == 0b1 && (op2 & 0b100100000000011) == 0b100000000000010)
			return std::make_unique<LDAPRSTLRSIMDAndFP::SubDecoder>(LDAPRSTLRSIMDAndFP::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0001 && (op2 & 0b100000000000000) == 0b000000000000000)
			return std::make_unique<RegisterLiteral::SubDecoder>(RegisterLiteral::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0001 && (op2 & 0b100100000000011) == 0b100000000000001)
			return std::make_unique<MemoryCopyAndMemorySet::SubDecoder>(MemoryCopyAndMemorySet::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0010 && (op2 & 0b110000000000000) == 0b000000000000000)
			return std::make_unique<NoAllocatePairOffset::SubDecoder>(NoAllocatePairOffset::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0010 && (op2 & 0b110000000000000) == 0b010000000000000)
			return std::make_unique<RegisterPairPostIndexed::SubDecoder>(RegisterPairPostIndexed::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0010 && (op2 & 0b110000000000000) == 0b100000000000000)
			return std::make_unique<RegisterPairOffset::SubDecoder>(RegisterPairOffset::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0010 && (op2 & 0b110000000000000) == 0b110000000000000)
			return std::make_unique<RegisterPairPreIndexed::SubDecoder>(RegisterPairPreIndexed::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000011) == 0b000000000000000)
			return std::make_unique<RegisterUnscaledImmediate::SubDecoder>(RegisterUnscaledImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000011) == 0b000000000000001)
			return std::make_unique<RegisterImmediatePostIndexed::SubDecoder>(RegisterImmediatePostIndexed::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000011) == 0b000000000000010)
			return std::make_unique<RegisterUnprivileged::SubDecoder>(RegisterUnprivileged::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000011) == 0b000000000000011)
			return std::make_unique<RegisterImmediatePreIndexed::SubDecoder>(RegisterImmediatePreIndexed::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000011) == 0b000100000000000)
			return std::make_unique<AtomicMemoryOperations::SubDecoder>(AtomicMemoryOperations::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000011) == 0b000100000000010)
			return std::make_unique<RegisterOffset::SubDecoder>(RegisterOffset::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100100000000001) == 0b000100000000001)
			return std::make_unique<RegisterPAC::SubDecoder>(RegisterPAC::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0011) == 0b0011 && (op2 & 0b100000000000000) == 0b100000000000000)
			return std::make_unique<RegisterUnsignedImmediate::SubDecoder>(RegisterUnsignedImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
	}
} /* disxx::disasm::decoder::LoadsAndStores */
