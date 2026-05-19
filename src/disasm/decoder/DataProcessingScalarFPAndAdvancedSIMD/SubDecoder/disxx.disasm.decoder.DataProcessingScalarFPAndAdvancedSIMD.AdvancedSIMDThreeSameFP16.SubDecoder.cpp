module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeSameFP16.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeSameFP16
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
		if (this != &other)
			[[maybe_unused]] const auto &_{disxx::disasm::decoder::abstract::SubDecoder::operator=(other)};
		return *this;
	}

	SubDecoder::SubDecoder(SubDecoder &&other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{std::move(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		[[maybe_unused]] const auto &_{disxx::disasm::decoder::abstract::SubDecoder::operator=(std::move(other))};
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +-+-+-+-----+-+--+--+--+------+-+--+--+
        // |0|Q|U|01110|a|10|Rm|00|opcode|1|Rn|Rd|
        // +-+-+-+-----+-+--+--+--+------+-+--+--+

        unsigned short int Q, U, a, Rm, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        a = bits::extract<unsigned short int, std::uint32_t, 23, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 11, 13>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000, InstructionID::INSN_FMAXNM},
            {0b00001, InstructionID::INSN_FMLA},
            {0b00010, InstructionID::INSN_FADD},
            {0b00011, InstructionID::INSN_FMULX},
            {0b00100, InstructionID::INSN_FCMEQ},
            {0b00110, InstructionID::INSN_FMAX},
            {0b00111, InstructionID::INSN_FRECPS},
            {0b01000, InstructionID::INSN_FMINNM},
            {0b01001, InstructionID::INSN_FMLS},
            {0b01010, InstructionID::INSN_FSUB},
            {0b01011, InstructionID::INSN_FAMAX},
            {0b01110, InstructionID::INSN_FMIN},
            {0b01111, InstructionID::INSN_FRSQRTS},
            {0b10000, InstructionID::INSN_FMAXNMP},
            {0b10010, InstructionID::INSN_FADDP},
            {0b10011, InstructionID::INSN_FMUL},
            {0b10100, InstructionID::INSN_FCMGE},
            {0b10101, InstructionID::INSN_FACGE},
            {0b10110, InstructionID::INSN_FMAXP},
            {0b10111, InstructionID::INSN_FDIV},
            {0b11000, InstructionID::INSN_FMINNMP},
            {0b11010, InstructionID::INSN_FABD},
            {0b11011, InstructionID::INSN_FAMIN},
            {0b11100, InstructionID::INSN_FCMGT},
            {0b11101, InstructionID::INSN_FACGT},
            {0b11110, InstructionID::INSN_FMINP},
            {0b11111, InstructionID::INSN_FSCALE}
        };

        const unsigned short int encoding = (U << 4) | (a << 3) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
        for (const auto specifier{Q == 0b1 ? "8h" : "4h"}; auto &pReg : this->m_Operands)
            static_cast<disxx::disasm::operand::Register *>(pReg.get())->SetArrangementSpecifier(specifier);

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeSameFP16 */
