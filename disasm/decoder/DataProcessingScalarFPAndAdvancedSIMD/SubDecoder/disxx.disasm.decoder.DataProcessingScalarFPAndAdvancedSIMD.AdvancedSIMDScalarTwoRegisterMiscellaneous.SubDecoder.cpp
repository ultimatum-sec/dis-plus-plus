module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarTwoRegisterMiscellaneous.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarTwoRegisterMiscellaneous
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
        // +--+-+-----+----+-----+------+--+--+--+
        // |01|U|11110|size|10000|opcode|10|Rn|Rd|
        // +--+-+-----+----+-----+------+--+--+--+

        unsigned short int U, size, opcode, Rn, Rd;
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 16>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000011, InstructionID::INSN_SUQADD},
            {0b000111, InstructionID::INSN_SQABS},
            {0b110100, InstructionID::INSN_SQXTN},
            {0b100011, InstructionID::INSN_USQADD},
            {0b100111, InstructionID::INSN_SQNEG},
            {0b110010, InstructionID::INSN_SQXTUN},
            {0b110100, InstructionID::INSN_UQXTN}
        };
        
        // InsnTable with size field in encoding
        std::unordered_map<unsigned short int, InstructionID> insnTableWithSize = {
            {0b00011010, InstructionID::INSN_FCVTNS},
            {0b00111010, InstructionID::INSN_FCVTNS},
            {0b00011011, InstructionID::INSN_FCVTMS},
            {0b00111011, InstructionID::INSN_FCVTMS},
            {0b00011100, InstructionID::INSN_FCVTAS},
            {0b00111100, InstructionID::INSN_FCVTAS},
            {0b00011101, InstructionID::INSN_SCVTF},
            {0b00111101, InstructionID::INSN_SCVTF},
            {0b01001100, InstructionID::INSN_FCMGT},
            {0b01101100, InstructionID::INSN_FCMGT},
            {0b01001101, InstructionID::INSN_FCMEQ},
            {0b01101101, InstructionID::INSN_FCMEQ},
            {0b01001110, InstructionID::INSN_FCMLT},
            {0b01101110, InstructionID::INSN_FCMLT},
            {0b01011010, InstructionID::INSN_FCVTPS},
            {0b01111010, InstructionID::INSN_FCVTPS},
            {0b01011011, InstructionID::INSN_FCVTZS},
            {0b01111011, InstructionID::INSN_FCVTZS},
            {0b01011101, InstructionID::INSN_FRECPE},
            {0b01111101, InstructionID::INSN_FRECPE},
            {0b01011111, InstructionID::INSN_FRECPX},
            {0b01111111, InstructionID::INSN_FRECPX},
            {0b01101000, InstructionID::INSN_CMGT},
            {0b01101001, InstructionID::INSN_CMEQ},
            {0b01101010, InstructionID::INSN_CMLT},
            {0b01101011, InstructionID::INSN_ABS},
            {0b10011010, InstructionID::INSN_FCVTNU},
            {0b10111010, InstructionID::INSN_FCVTNU},
            {0b10011011, InstructionID::INSN_FCVTMU},
            {0b10111011, InstructionID::INSN_FCVTMU},
            {0b10011100, InstructionID::INSN_FCVTAU},
            {0b10111100, InstructionID::INSN_FCVTAU},
            {0b10011101, InstructionID::INSN_UCVTF},
            {0b10111101, InstructionID::INSN_UCVTF},
            {0b10110110, InstructionID::INSN_FCVTXN},
            {0b11001100, InstructionID::INSN_FCMGE},
            {0b11101100, InstructionID::INSN_FCMGE},
            {0b11001101, InstructionID::INSN_FCMLE},
            {0b11101101, InstructionID::INSN_FCMLE},
            {0b11011010, InstructionID::INSN_FCVTPU},
            {0b11111010, InstructionID::INSN_FCVTPU},
            {0b11011011, InstructionID::INSN_FCVTZU},
            {0b11111011, InstructionID::INSN_FCVTZU},
            {0b11011101, InstructionID::INSN_FRSQRTE},
            {0b11111101, InstructionID::INSN_FRSQRTE},
            {0b11101000, InstructionID::INSN_CMGE},
            {0b11101001, InstructionID::INSN_CMLE},
            {0b11101011, InstructionID::INSN_NEG}
        };

        auto encoding = (U << 5) | opcode;
        if (auto it{insnTable.find(encoding)}; it != insnTable.end())
        {
            if (opcode == 0b00011 || opcode == 0b00111)
            {
                const auto regSize{std::array<unsigned short int, 4>{8, 16, 32, 64}.at(size)};
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
            }
            else
            {
                // Reserved...
                if (size == 4) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

                unsigned short int VbSize{}, VaSize{};
                VbSize = std::array<unsigned short int, 3>{8, 16, 32}.at(size);
                VaSize = VbSize << 1;

                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, VbSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, VaSize));
            }

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else
        {
            encoding = (U << 7) | (size << 5) | opcode;
            it = insnTableWithSize.find(encoding);
            if (it == insnTableWithSize.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
            if (encoding != 0b10110110)
            {
                const auto regSize{bits::extract<unsigned short int, unsigned short int, 0, 0>(size) == 0b0 ? 32 : 64};
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
                if (opcode != 0b01011 && bits::extract<unsigned short int, unsigned short int, 4, 4>(opcode) == 0b0)
                {
                    if (opcode <= 0b01110 && opcode >= 0b01100)
                        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<float, 1>>(0));
                    else
                        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 1>>(0));
                }

                return std::make_pair(it->second, std::move(this->m_Operands));
            }
            else
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 32));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 64));

                return std::make_pair(it->second, std::move(this->m_Operands));
            }
        }
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarTwoRegisterMiscellaneous */
