module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingImmediate.MoveWideImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.Shift;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingImmediate::MoveWideImmediate
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +--+---+------+--+-----+--+
        // |sf|opc|100101|hw|imm16|Rd|
        // +--+---+------+--+-----+--+

        unsigned short int sf, opc, hw, imm16, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
        hw = bits::extract<unsigned short int, std::uint32_t, 21, 22>(this->m_Insn);
        imm16 = bits::extract<unsigned short int, std::uint32_t, 5, 20>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b00000, {InstructionID::INSN_MOVN, InstructionID::INSN_MOV}},
            {0b01000, {InstructionID::INSN_MOVZ, InstructionID::INSN_MOV}},
            {0b01100, {InstructionID::INSN_MOVK, std::nullopt}},
            {0b10000, {InstructionID::INSN_MOVN, InstructionID::INSN_MOV}},
            {0b11000, {InstructionID::INSN_MOVZ, InstructionID::INSN_MOV}},
            {0b11100, {InstructionID::INSN_MOVK, std::nullopt}}
        };

        const unsigned short int encoding = (sf << 4) | (opc << 2) | (sf == 0b1 ? 0b00 : (hw & ~1));
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};
    
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sf
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rd
			)
		);
        if (alias && !(imm16 == 0x0000 && hw != 0b00) && encoding != 0b00000)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 16>>(imm16 | hw));
            
            return std::make_pair(*alias, std::move(this->m_Operands));
        }
        else if (alias && !(imm16 == 0x00 && hw != 0b00) && bits::IsOnes<unsigned short int, 16>(imm16))
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 16>>(imm16 | hw));

            return std::make_pair(*alias, std::move(this->m_Operands));
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 16>>(imm16));
        if (hw) this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(0b000, hw << 4));
    
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingImmediate::MoveWideImmediate */
