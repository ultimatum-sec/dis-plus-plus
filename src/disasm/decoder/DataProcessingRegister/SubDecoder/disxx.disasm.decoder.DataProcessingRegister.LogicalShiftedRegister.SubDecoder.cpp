module;

#include <unordered_map>
#include <optional>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.LogicalShiftedRegister.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.Shift;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::LogicalShiftedRegister
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
        // +--+---+-----+-----+-+--+----+--+--+
        // |sf|opc|01010|shift|N|Rm|imm6|Rn|Rd|
        // +--+---+-----+-----+-+--+----+--+--+

        unsigned short int sf, opc, shift, N, Rm, imm6, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
        shift = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        N = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        imm6 = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b0000, {InstructionID::INSN_AND, std::nullopt}},
            {0b0001, {InstructionID::INSN_BIC, std::nullopt}},
            {0b0010, {InstructionID::INSN_ORR, InstructionID::INSN_MOV}},
            {0b0011, {InstructionID::INSN_ORN, InstructionID::INSN_MVN}},
            {0b0100, {InstructionID::INSN_EOR, std::nullopt}},
            {0b0101, {InstructionID::INSN_EON, std::nullopt}},
            {0b0110, {InstructionID::INSN_ANDS, InstructionID::INSN_TST}},
            {0b0111, {InstructionID::INSN_BICS, std::nullopt}},
            {0b1000, {InstructionID::INSN_AND, std::nullopt}},
            {0b1001, {InstructionID::INSN_BIC, std::nullopt}},
            {0b1010, {InstructionID::INSN_ORR, InstructionID::INSN_MOV}},
            {0b1011, {InstructionID::INSN_ORN, InstructionID::INSN_MVN}},
            {0b1100, {InstructionID::INSN_EOR, std::nullopt}},
            {0b1101, {InstructionID::INSN_EON, std::nullopt}},
            {0b1110, {InstructionID::INSN_ANDS, InstructionID::INSN_TST}},
            {0b1111, {InstructionID::INSN_BICS, std::nullopt}}
        };

        const unsigned short int encoding = (sf << 3) | (opc << 1) | N;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};
        const unsigned short int regSize = 32 << sf;

        if (alias && Rn == 0b11111)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
            if (imm6)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));
        
            return std::make_pair(alias.value(), std::move(this->m_Operands));
        }
        else if (alias && Rd == 0b11111)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
            if (imm6)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));
        
            return std::make_pair(alias.value(), std::move(this->m_Operands));
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
        if (imm6)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::LogicalShiftedRegister */
