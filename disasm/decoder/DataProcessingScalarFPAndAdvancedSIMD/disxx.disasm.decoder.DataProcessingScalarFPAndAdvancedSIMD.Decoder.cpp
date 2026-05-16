module;

#include <cstdint>
#include <memory>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.Decoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.utility.bits;

import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointCompare.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing3Source.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSameFP16.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndFixedPoint.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSameExtra.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneousFP16.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalSelect.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTableLookup.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDAcrossLanes.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarPairwise.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing2Source.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneous.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicAES.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSame.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDModifiedImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA512.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeDifferent.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDExtract.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDPermute.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeDifferent.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneous.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarXIndexedElement.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarCopy.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterSHA.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterImm2.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDVectorXIndexedElement.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicFourRegister.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSameFP16.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneousFP16.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalCompare.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSame.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndInteger.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.XAR.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing1Source.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDShiftByImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeRegisterExtension.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarShiftByImmediate.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDCopy.SubDecoder;
import disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterSHA512.SubDecoder;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD
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
		// +---+---+---+---+---+
		// |op0|111|op1|op2|op3|
		// +---+---+---+---+---+

		unsigned short int op0, op1, op2, op3;
		op0 = bits::extract<unsigned short int, std::uint32_t, 28, 31>(this->m_Insn);
		op1 = bits::extract<unsigned short int, std::uint32_t, 23, 24>(this->m_Insn);
		op2 = bits::extract<unsigned short int, std::uint32_t, 19, 22>(this->m_Insn);
		op3 = bits::extract<unsigned short int, std::uint32_t, 10, 18>(this->m_Insn);
		
		if ((op0 & 0b1111) == 0b0100 && (op1 & 0b10) == 0b00 && (op2 & 0b0111) == 0b0101 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<CryptographicAES::SubDecoder>(CryptographicAES::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000 && (op3 & 0b000100011) == 0b000000000)
			return std::make_unique<CryptographicThreeRegisterSHA::SubDecoder>(CryptographicThreeRegisterSHA::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0111) == 0b0101 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<CryptographicTwoRegisterSHA::SubDecoder>(CryptographicTwoRegisterSHA::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b11) == 0b00 && (op2 & 0b1100) == 0b0000 && (op3 & 0b000100001) == 0b000000001)
			return std::make_unique<AdvancedSIMDScalarCopy::SubDecoder>(AdvancedSIMDScalarCopy::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b1100) == 0b1000 && (op3 & 0b000110001) == 0b000000001)
			return std::make_unique<AdvancedSIMDThreeSameFP16::SubDecoder>(AdvancedSIMDThreeSameFP16::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b1111) == 0b1111 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::SubDecoder>(AdvancedSIMDScalarTwoRegisterMiscellaneousFP16::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000 && (op3 & 0b000100001) == 0b000100001)
			return std::make_unique<AdvancedSIMDScalarThreeSameExtra::SubDecoder>(AdvancedSIMDScalarThreeSameExtra::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0111) == 0b0100 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<AdvancedSIMDScalarTwoRegisterMiscellaneous::SubDecoder>(AdvancedSIMDScalarTwoRegisterMiscellaneous::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0111) == 0b0110 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<AdvancedSIMDScalarPairwise::SubDecoder>(AdvancedSIMDScalarPairwise::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000011) == 0b000000000)
			return std::make_unique<AdvancedSIMDScalarThreeDifferent::SubDecoder>(AdvancedSIMDScalarThreeDifferent::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000001) == 0b000000001)
			return std::make_unique<AdvancedSIMDScalarThreeSame::SubDecoder>(AdvancedSIMDScalarThreeSame::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b11) == 0b10 && (op3 & 0b000000001) == 0b000000001)
			return std::make_unique<AdvancedSIMDScalarShiftByImmediate::SubDecoder>(AdvancedSIMDScalarShiftByImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1101) == 0b0101 && (op1 & 0b10) == 0b10 && (op3 & 0b000000001) == 0b000000000)
			return std::make_unique<AdvancedSIMDScalarXIndexedElement::SubDecoder>(AdvancedSIMDScalarXIndexedElement::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000 && (op3 & 0b000100011) == 0b000000000)
			return std::make_unique<AdvancedSIMDTableLookup::SubDecoder>(AdvancedSIMDTableLookup::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000 && (op3 & 0b000100011) == 0b000000010)
			return std::make_unique<AdvancedSIMDPermute::SubDecoder>(AdvancedSIMDPermute::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1011) == 0b0010 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000 && (op3 & 0b000100001) == 0b000000000)
			return std::make_unique<AdvancedSIMDExtract::SubDecoder>(AdvancedSIMDExtract::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b11) == 0b00 && (op2 & 0b1100) == 0b0000 && (op3 & 0b000100001) == 0b000000001)
			return std::make_unique<AdvancedSIMDCopy::SubDecoder>(AdvancedSIMDCopy::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b1100) == 0b1000 && (op3 & 0b000110001) == 0b000000001)
			return std::make_unique<AdvancedSIMDThreeSameFP16::SubDecoder>(AdvancedSIMDThreeSameFP16::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b1111) == 0b1111 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<AdvancedSIMDTwoRegisterMiscellaneousFP16::SubDecoder>(AdvancedSIMDTwoRegisterMiscellaneousFP16::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000 && (op3 & 0b000100001) == 0b000100001)
			return std::make_unique<AdvancedSIMDThreeRegisterExtension::SubDecoder>(AdvancedSIMDThreeRegisterExtension::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0111) == 0b0100 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<AdvancedSIMDTwoRegisterMiscellaneous::SubDecoder>(AdvancedSIMDTwoRegisterMiscellaneous::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0111) == 0b0110 && (op3 & 0b110000011) == 0b000000010)
			return std::make_unique<AdvancedSIMDAcrossLanes::SubDecoder>(AdvancedSIMDAcrossLanes::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000011) == 0b000000000)
			return std::make_unique<AdvancedSIMDThreeDifferent::SubDecoder>(AdvancedSIMDThreeDifferent::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000001) == 0b000000001)
			return std::make_unique<AdvancedSIMDThreeSame::SubDecoder>(AdvancedSIMDThreeSame::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b11) == 0b10 && (op2 & 0b1111) == 0b0000 && (op3 & 0b000000001) == 0b000000001)
			return std::make_unique<AdvancedSIMDModifiedImmediate::SubDecoder>(AdvancedSIMDModifiedImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b11) == 0b10 && op2 != 0b0000 && (op3 & 0b000000001) == 0b000000001)
			return std::make_unique<AdvancedSIMDShiftByImmediate::SubDecoder>(AdvancedSIMDShiftByImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1001) == 0b0000 && (op1 & 0b10) == 0b10 && (op3 & 0b000000001) == 0b000000000)
			return std::make_unique<AdvancedSIMDVectorXIndexedElement::SubDecoder>(AdvancedSIMDVectorXIndexedElement::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b1100 && (op1 & 0b11) == 0b00 && (op2 & 0b1100) == 0b1000 && (op3 & 0b000110000) == 0b000100000)
			return std::make_unique<CryptographicThreeRegisterImm2::SubDecoder>(CryptographicThreeRegisterImm2::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b1100 && (op1 & 0b11) == 0b00 && (op2 & 0b1100) == 0b1100 && (op3 & 0b000101100) == 0b000100000)
			return std::make_unique<CryptographicThreeRegisterSHA512::SubDecoder>(CryptographicThreeRegisterSHA512::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b1100 && (op1 & 0b11) == 0b00 && (op3 & 0b000100000) == 0b000000000)
			return std::make_unique<CryptographicFourRegister::SubDecoder>(CryptographicFourRegister::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b1100 && (op1 & 0b11) == 0b01 && (op2 & 0b1100) == 0b0000)
			return std::make_unique<XAR::SubDecoder>(XAR::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b1111) == 0b1100 && (op1 & 0b11) == 0b01 && (op2 & 0b1111) == 0b1000 && (op3 & 0b111111100) == 0b000100000)
			return std::make_unique<CryptographicTwoRegisterSHA512::SubDecoder>(CryptographicTwoRegisterSHA512::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0000)
			return std::make_unique<ConversionBetweenFloatingPointAndFixedPoint::SubDecoder>(ConversionBetweenFloatingPointAndFixedPoint::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000111111) == 0b000000000)
			return std::make_unique<ConversionBetweenFloatingPointAndInteger::SubDecoder>(ConversionBetweenFloatingPointAndInteger::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000011111) == 0b000010000)
			return std::make_unique<FloatingPointDataProcessing1Source::SubDecoder>(FloatingPointDataProcessing1Source::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000001111) == 0b000001000)
			return std::make_unique<FloatingPointCompare::SubDecoder>(FloatingPointCompare::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000111) == 0b000000100)
			return std::make_unique<FloatingPointImmediate::SubDecoder>(FloatingPointImmediate::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000011) == 0b000000001)
			return std::make_unique<FloatingPointConditionalCompare::SubDecoder>(FloatingPointConditionalCompare::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000011) == 0b000000010)
			return std::make_unique<FloatingPointDataProcessing2Source::SubDecoder>(FloatingPointDataProcessing2Source::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b00 && (op2 & 0b0100) == 0b0100 && (op3 & 0b000000011) == 0b000000011)
			return std::make_unique<FloatingPointConditionalSelect::SubDecoder>(FloatingPointConditionalSelect::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		else if ((op0 & 0b0101) == 0b0001 && (op1 & 0b10) == 0b10)
			return std::make_unique<FloatingPointDataProcessing3Source::SubDecoder>(FloatingPointDataProcessing3Source::SubDecoder{this->m_Insn, this->m_ProgramCounter});
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
	}
} /* DataProcessingScalarFPAndAdvancedSIMD */
