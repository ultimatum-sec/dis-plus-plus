module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.DataProcessing2Source.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::DataProcessing2Source
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
		: disxx::disasm::decoder::abstract::SubDecoder{std::move(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(std::forward<SubDecoder &&>(other));
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<std::decay_t<decltype(*this)>>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+-+-+--------+--+------+--+--+
        // |sf|0|S|11010110|Rm|opcode|Rn|Rd|
        // +--+-+-+--------+--+------+--+--+

        unsigned short int sf, S, Rm, opcode, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000010, InstructionID::INSN_UDIV},
            {0b00000011, InstructionID::INSN_SDIV},
            {0b00001000, InstructionID::INSN_LSLV},
            {0b00001001, InstructionID::INSN_LSRV},
            {0b00001010, InstructionID::INSN_ASRV},
            {0b00001011, InstructionID::INSN_RORV},
            {0b00010000, InstructionID::INSN_CRC32B},
            {0b00010001, InstructionID::INSN_CRC32H},
            {0b00010010, InstructionID::INSN_CRC32W},
            {0b00010100, InstructionID::INSN_CRC32CB},
            {0b00010101, InstructionID::INSN_CRC32CH},
            {0b00010110, InstructionID::INSN_CRC32CW},
            {0b00011000, InstructionID::INSN_SMAX},
            {0b00011001, InstructionID::INSN_UMAX},
            {0b00011010, InstructionID::INSN_SMIN},
            {0b00011011, InstructionID::INSN_UMIN},
            {0b10000000, InstructionID::INSN_SUBP},
            {0b10000010, InstructionID::INSN_UDIV},
            {0b10000011, InstructionID::INSN_SDIV},
            {0b10000100, InstructionID::INSN_IRG},
            {0b10000101, InstructionID::INSN_GMI},
            {0b10001000, InstructionID::INSN_LSLV},
            {0b10001001, InstructionID::INSN_LSRV},
            {0b10001010, InstructionID::INSN_ASRV},
            {0b10001011, InstructionID::INSN_RORV},
            {0b10001100, InstructionID::INSN_PACGA},
            {0b10010011, InstructionID::INSN_CRC32X},
            {0b10010111, InstructionID::INSN_CRC32CX},
            {0b10011000, InstructionID::INSN_SMAX},
            {0b10011001, InstructionID::INSN_UMAX},
            {0b10011010, InstructionID::INSN_SMIN},
            {0b10011011, InstructionID::INSN_UMIN},
            {0b11000000, InstructionID::INSN_SUBPS}
        };

        const unsigned short int encoding = (sf << 7) | (S << 6) | opcode;
        const auto it = insnTable.find(encoding);
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        if (opcode == 0b000100)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64, true));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64));
        
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (opcode == 0b001100)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
    
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (opcode == 0b000101)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64));
        
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (opcode == 0b000000)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64, true));
        
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
            
        const unsigned short int regSize = sf == 0b1 ? 64 : 32;
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
        this->m_Operands.emplace_back
        (
            std::make_unique<disxx::disasm::operand::Register>
            (
                disxx::disasm::operand::Register::Type::TYPE_GPR,
                Rm,
                encoding == 0b1001001 || encoding == 0b10010111
                    ? 64
                    : regSize
            )
        );

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::DataProcessing2Source */
