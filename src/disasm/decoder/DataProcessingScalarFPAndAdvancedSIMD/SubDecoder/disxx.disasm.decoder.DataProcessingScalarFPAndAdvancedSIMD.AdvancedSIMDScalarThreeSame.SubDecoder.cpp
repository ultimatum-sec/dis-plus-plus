module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarThreeSame.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarThreeSame
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+-+-----+----+-+--+------+-+--+--+
        // |01|U|11110|size|1|Rm|opcode|1|Rn|Rd|
        // +--+-+-----+----+-+--+------+-+--+--+
    
        unsigned short int U, size, Rm, opcode, Rn, Rd;
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 11, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000001, InstructionID::INSN_SQADD},
            {0b000101, InstructionID::INSN_SQSUB},
            {0b001001, InstructionID::INSN_SQSHL},
            {0b001011, InstructionID::INSN_SQRSHL},
            {0b010110, InstructionID::INSN_SQDMULH},
            {0b100001, InstructionID::INSN_UQADD},
            {0b100101, InstructionID::INSN_UQSUB},
            {0b101001, InstructionID::INSN_UQSHL},
            {0b101011, InstructionID::INSN_UQRSHL},
            {0b110110, InstructionID::INSN_SQRDMULH}
        };

        std::unordered_map<unsigned short int, InstructionID> insnTableWithSize = {
            {0b00011011, InstructionID::INSN_FMULX},
            {0b00111011, InstructionID::INSN_FMULX},
            {0b00011100, InstructionID::INSN_FCMEQ},
            {0b00111100, InstructionID::INSN_FCMEQ},
            {0b00011111, InstructionID::INSN_FRECPS},
            {0b00111111, InstructionID::INSN_FRECPS},
            {0b01011111, InstructionID::INSN_FRSQRTS},
            {0b01111111, InstructionID::INSN_FRSQRTS},
            {0b01100110, InstructionID::INSN_CMGT},
            {0b01100111, InstructionID::INSN_CMGE},
            {0b01101000, InstructionID::INSN_SSHL},
            {0b01101010, InstructionID::INSN_SRSHL},
            {0b01110000, InstructionID::INSN_ADD},
            {0b01110001, InstructionID::INSN_CMTST},
            {0b10011100, InstructionID::INSN_FCMGE},
            {0b10111100, InstructionID::INSN_FCMGE},
            {0b10011101, InstructionID::INSN_FACGE},
            {0b10111101, InstructionID::INSN_FACGE},
            {0b11011010, InstructionID::INSN_FABD},
            {0b11111010, InstructionID::INSN_FABD},
            {0b11011100, InstructionID::INSN_FCMGT},
            {0b11111100, InstructionID::INSN_FCMGT},
            {0b11011101, InstructionID::INSN_FACGT},
            {0b11111101, InstructionID::INSN_FACGT},
            {0b11100110, InstructionID::INSN_CMHI},
            {0b11100111, InstructionID::INSN_CMHS},
            {0b11101000, InstructionID::INSN_USHL},
            {0b11101010, InstructionID::INSN_URSHL},
            {0b11110000, InstructionID::INSN_SUB},
            {0b11110001, InstructionID::INSN_CMEQ}
        };

        unsigned short int regSize, encoding = (U << 5) | opcode;
        auto it{insnTable.find(encoding)};
        if (it != insnTable.end()) [[unlikely]]
        {
            // Extra check if it's sqdmulh
            if (opcode == 0b10110 && size != 0b01 && size != 0b10) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            
            regSize = std::array<unsigned short int, 4>{8, 16, 32, 64}.at(size);
        }
        else
        {
            encoding = (U << 7) | (size << 5) | opcode;
            it = insnTableWithSize.find(encoding);
            if (it == insnTable.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            regSize = std::array<unsigned short int, 4>{8, 16, 32, 64}.at(size | 0b10);
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, regSize));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarThreeSame */
