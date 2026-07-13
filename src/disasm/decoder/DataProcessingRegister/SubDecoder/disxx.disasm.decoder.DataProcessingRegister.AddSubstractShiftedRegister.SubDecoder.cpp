module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.AddSubstractShiftedRegister.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.Shift;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::AddSubstractShiftedRegister
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
	{ return std::make_unique<std::decay_t<std::decay_t<std::decay_t<decltype(*this)>>>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+--+-+-----+-----+-+--+----+--+--+
        // |sf|op|S|01011|shift|0|Rm|imm6|Rn|Rd|
        // +--+--+-+-----+-----+-+--+----+--+--+

        unsigned short int sf, op, S, shift, Rm, imm6, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        shift = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        imm6 = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b000, {InstructionID::INSN_ADD, std::nullopt}},
            {0b001, {InstructionID::INSN_ADDS, InstructionID::INSN_CMN}},
            {0b010, {InstructionID::INSN_SUB, InstructionID::INSN_NEG}},
            {0b011, {InstructionID::INSN_SUBS, std::nullopt}}, // 2 aliases: processing in other way
            {0b100, {InstructionID::INSN_ADD, std::nullopt}},
            {0b101, {InstructionID::INSN_ADDS, InstructionID::INSN_CMN}},
            {0b110, {InstructionID::INSN_SUB, InstructionID::INSN_NEG}},
            {0b111, {InstructionID::INSN_SUBS, std::nullopt}} // 2 aliases: processing in other way
        };

        const unsigned short int encoding = (sf << 2) | (op << 1) | S;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};
        const unsigned short int regSize = 32 << sf;

        if (alias && Rd == 0b11111)
        {
            if (shift == 0b11) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
            if (imm6)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));

            return std::make_pair(alias.value(), std::move(this->m_Operands));
        }
        else if (alias && Rn == 0b11111)
        {
            if (shift == 0b11) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
            if (imm6)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));
    
            return std::make_pair(alias.value(), std::move(this->m_Operands));
        }
        else if (op == 0b1 && S == 0b1 && (Rd == 0b11111 || (Rn == 0b11111 && Rd != 0b11111)))
        {
            if (shift == 0b11) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            if (Rd == 0b11111)
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
            }
            else
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
            }

            if (imm6)
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));

            return std::make_pair
            (
                Rd == 0b11111
                    ? InstructionID::INSN_CMP
                    : InstructionID::INSN_NEGS,
                std::move(this->m_Operands)
            );
        }
            
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
        if (imm6)
        	this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(static_cast<disxx::disasm::operand::Shift::Type>(shift), imm6));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::AddSubstractShiftedRegister */
