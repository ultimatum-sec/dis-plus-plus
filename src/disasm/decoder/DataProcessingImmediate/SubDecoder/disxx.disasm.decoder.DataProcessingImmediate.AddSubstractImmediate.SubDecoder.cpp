module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.Shift;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingImmediate::AddSubstractImmediate
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
	{ return std::make_unique<std::decay_t<std::decay_t<decltype(*this)>>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+--+-+------+--+-----+--+--+
        // |sf|op|S|100010|sh|imm12|Rn|Rd|
        // +--+--+-+------+--+-----+--+--+

        unsigned short int sf, op, S, sh, imm12, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        sh = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        imm12 = bits::extract<unsigned short int, std::uint32_t, 10, 21>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b000, {InstructionID::INSN_ADD, InstructionID::INSN_MOV}},
            {0b001, {InstructionID::INSN_ADDS, InstructionID::INSN_CMN}},
            {0b010, {InstructionID::INSN_SUB, std::nullopt}},
            {0b011, {InstructionID::INSN_SUBS, InstructionID::INSN_CMP}},
            {0b100, {InstructionID::INSN_ADD, InstructionID::INSN_MOV}},
            {0b101, {InstructionID::INSN_ADDS, InstructionID::INSN_CMN}},
            {0b110, {InstructionID::INSN_SUB, std::nullopt}},
            {0b111, {InstructionID::INSN_SUBS, InstructionID::INSN_CMP}}
        };

        const unsigned short int encoding = (sf << 2) | (op << 1) | S;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};

        const unsigned short int regSize = sf == 0b1 ? 64 : 32;
        if (alias)
        {
            if (const auto &value{alias.value()}; Rd == 0b11111 && (value == InstructionID::INSN_CMN || value == InstructionID::INSN_CMP))
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize, true));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned int, 28>>(imm12 /* << (sh == 0b1 ? 12 : 0) */));
                if (sh) this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(disxx::disasm::operand::Shift::Type::SHIFT_LSL, 12));
            
                return std::make_pair(alias.value(), std::move(this->m_Operands));
            }
            else if (value == InstructionID::INSN_MOV && (sh == 0b0 && imm12 == 0x000 && (Rd == 0b11111 || Rn == 0b11111)))
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize, true));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize, true));
                
                return std::make_pair(alias.value(), std::move(this->m_Operands));
            }
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize, S != 0b1));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize, true));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned int, 28>>(imm12 /* << (sh == 0b1 ? 12 : 0) */));
        if (sh) this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(disxx::disasm::operand::Shift::Type::SHIFT_LSL, 12));
    
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingImmediate::AddSubstractImmediate */
