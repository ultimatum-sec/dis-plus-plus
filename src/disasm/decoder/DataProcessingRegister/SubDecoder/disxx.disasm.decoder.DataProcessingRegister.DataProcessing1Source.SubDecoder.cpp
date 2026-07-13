module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.DataProcessing1Source.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::DataProcessing1Source
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
        // +--+-+-+--------+-------+------+--+--+
        // |sf|1|S|11010110|opcode2|opcode|Rn|Rd|
        // +--+-+-+--------+-------+------+--+--+

        unsigned short int sf, S, opcode2, opcode, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        opcode2 = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000000000000, InstructionID::INSN_RBIT},
            {0b0000000000001, InstructionID::INSN_REV16},
            {0b0000000000010, InstructionID::INSN_REV},
            {0b0000000000100, InstructionID::INSN_CLZ},
            {0b0000000000101, InstructionID::INSN_CLS},
            {0b0000000000110, InstructionID::INSN_CTZ},
            {0b0000000000111, InstructionID::INSN_CNT},
            {0b0000000001000, InstructionID::INSN_ABS},
            {0b1000000000000, InstructionID::INSN_RBIT},
            {0b1000000000001, InstructionID::INSN_REV16},
            {0b1000000000010, InstructionID::INSN_REV32},
            {0b1000000000011, InstructionID::INSN_REV},
            {0b1000000000100, InstructionID::INSN_CLZ},
            {0b1000000000101, InstructionID::INSN_CLS},
            {0b1000000000110, InstructionID::INSN_CTZ},
            {0b1000000000111, InstructionID::INSN_CNT},
            {0b1000000001000, InstructionID::INSN_ABS},
            {0b1000001000000, InstructionID::INSN_PACIA},
            {0b1000001000001, InstructionID::INSN_PACIB},
            {0b1000001000010, InstructionID::INSN_PACDA},
            {0b1000001000011, InstructionID::INSN_PACDB},
            {0b1000001000100, InstructionID::INSN_AUTIA},
            {0b1000001000101, InstructionID::INSN_AUTIB},
            {0b1000001000110, InstructionID::INSN_AUTDA},
            {0b1000001000111, InstructionID::INSN_AUTDB}
        };

        const unsigned short int encoding = (sf << 12) | (S << 11) | (opcode2 << 6) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end())
        {
            static const std::unordered_map<unsigned int, InstructionID> insnTable2 = {
                {0b10000010010001111100000, InstructionID::INSN_PACIZA},
                {0b10000010010011111100000, InstructionID::INSN_PACIZB},
                {0b10000010010101111100000, InstructionID::INSN_PACDZA},
                {0b10000010010111111100000, InstructionID::INSN_PACDZB},
                {0b10000010011001111100000, InstructionID::INSN_AUTIZA},
                {0b10000010011011111100000, InstructionID::INSN_AUTIZB},
                {0b10000010011101111100000, InstructionID::INSN_AUTDZA},
                {0b10000010011111111100000, InstructionID::INSN_AUTDZB},
                {0b10000010100001111100000, InstructionID::INSN_XPACI},
                {0b10000010100011111100000, InstructionID::INSN_XPACD},
                {0b10000011000001111111110, InstructionID::INSN_PACNBIASPPC},
                {0b10000011000011111111110, InstructionID::INSN_PACNBIBSPPC},
                {0b10000011000101111111110, InstructionID::INSN_PACIA171615},
                {0b10000011000111111111110, InstructionID::INSN_PACIB171615},
                {0b10000011001000000011110, InstructionID::INSN_AUTIASPPCR},
                {0b10000011001010000011110, InstructionID::INSN_AUTIBSPPCR},
                {0b10000011010001111111110, InstructionID::INSN_PACIASPPC},
                {0b10000011010011111111110, InstructionID::INSN_PACIBSPPC},
                {0b10000011011101111111110, InstructionID::INSN_AUTIA171615},
                {0b10000011011111111111110, InstructionID::INSN_AUTIB171615}
            };

            const unsigned int encoding2 = (encoding << 5) | (((opcode >> 2) == 0b1001 ? 0b00000 : Rn) << 5) | ((opcode >> 4) <= 0b01 ? 0b00000 : Rd);
            const auto it2 = insnTable2.find(opcode2);
            if (it2 == insnTable2.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            if ((encoding2 & 0x3FF) == 0b1111111110 && (opcode >> 4) == 0b00)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64, true));
            else if ((encoding2 & 0x3FF) == 0b1111111110 && (opcode >> 4) == 0b01)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            else if ((encoding2 & 0x3FF) == 0b0000011110)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            else if ((opcode >> 1) == 0b10111)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64));
    
            return std::make_pair(it2->second, std::move(this->m_Operands));
        }
            
        const unsigned short int regSize = sf == 0b1 ? 64 : 32;
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize, opcode2 == 0b00001 ? true : false));
    
        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::DataProcessing1Source */
