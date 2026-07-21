module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneousFP16.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDTwoRegisterMiscellaneousFP16
{
	SubDecoder::SubDecoder(void) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{}
	{}

	SubDecoder::SubDecoder(std::uint32_t insn, std::uint64_t addr) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{insn, addr}
	{}

	SubDecoder::SubDecoder(const SubDecoder &other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{other}
	{}

	SubDecoder &SubDecoder::operator=(const SubDecoder &other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(other);
		return *this;
	}

	SubDecoder::SubDecoder(SubDecoder &&other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{std::forward<SubDecoder &&>(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(std::forward<SubDecoder &&>(other));
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +-+-+-----+-+------+------+--+--+--+
        // |0|Q|01110|a|111100|opcode|10|Rn|Rd|
        // +-+-+-----+-+------+------+--+--+--+

        unsigned short int Q, a, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        a = bits::extract<unsigned short int, std::uint32_t, 23, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 16>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0011000, InstructionID::INSN_FRINTN},
            {0b0011001, InstructionID::INSN_FRINTM},
            {0b0011010, InstructionID::INSN_FCVTNS},
            {0b0011011, InstructionID::INSN_FCVTMS},
            {0b0011100, InstructionID::INSN_FCVTAS},
            {0b0011101, InstructionID::INSN_SCVTF},
            {0b0101100, InstructionID::INSN_FCMGT},
            {0b0101101, InstructionID::INSN_FCMEQ},
            {0b0101110, InstructionID::INSN_FCMLT},
            {0b0101111, InstructionID::INSN_FABS},
            {0b0111000, InstructionID::INSN_FRINTP},
            {0b0111001, InstructionID::INSN_FRINTZ},
            {0b0111010, InstructionID::INSN_FCVTPS},
            {0b0111011, InstructionID::INSN_FCVTZS},
            {0b0111101, InstructionID::INSN_FRECPE},
            {0b1011000, InstructionID::INSN_FRINTA},
            {0b1011001, InstructionID::INSN_FRINTX},
            {0b1011010, InstructionID::INSN_FCVTNU},
            {0b1011011, InstructionID::INSN_FCVTMU},
            {0b1011100, InstructionID::INSN_FCVTAU},
            {0b1011101, InstructionID::INSN_UCVTF},
            {0b1101100, InstructionID::INSN_FCMGE},
            {0b1101101, InstructionID::INSN_FCMLE},
            {0b1101111, InstructionID::INSN_FNEG},
            {0b1111001, InstructionID::INSN_FRINTI},
            {0b1111010, InstructionID::INSN_FCVTPU},
            {0b1111011, InstructionID::INSN_FCVTZU},
            {0b1111101, InstructionID::INSN_FRSQRTE},
            {0b1111111, InstructionID::INSN_FSQRT}
        };

        const unsigned short int encoding = (Q << 6) | (a << 5) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const disxx::disasm::operand::VectorArrangementSpecifier specifier{static_cast<unsigned short int>(0b010 | Q)};
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rd
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
			->SetVectorArrangementSpecifier(specifier);
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rn
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
			->SetVectorArrangementSpecifier(specifier);
        if (opcode >= 0b01100 && opcode <= 0b01110)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<float, 1>>(0.f));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDTwoRegisterMiscellaneousFP16 */
