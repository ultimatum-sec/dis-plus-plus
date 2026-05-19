module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.DataProcessing3Source.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::DataProcessing3Source
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
        // +--+----+-----+----+--+--+--+--+--+
        // |sf|op54|11011|op31|Rm|o0|Ra|Rn|Rd|
        // +--+----+-----+----+--+--+--+--+--+

        unsigned short int sf, op54, op31, Rm, o0, Ra, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op54 = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
        op31 = bits::extract<unsigned short int, std::uint32_t, 21, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        o0 = bits::extract<unsigned short int, std::uint32_t, 15, 15>(this->m_Insn);
        Ra = bits::extract<unsigned short int, std::uint32_t, 10, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
    
        static const std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b0000000, {InstructionID::INSN_MADD, InstructionID::INSN_MUL}},
            {0b0000001, {InstructionID::INSN_MSUB, InstructionID::INSN_MNEG}},
            {0b1000000, {InstructionID::INSN_MADD, InstructionID::INSN_MUL}},
            {0b1000001, {InstructionID::INSN_MSUB, InstructionID::INSN_MNEG}},
            {0b1000010, {InstructionID::INSN_SMADDL, InstructionID::INSN_SMULL}},
            {0b1000011, {InstructionID::INSN_SMSUBL, InstructionID::INSN_SMNEGL}},
            {0b1000100, {InstructionID::INSN_SMULH, std::nullopt}},
            {0b1000110, {InstructionID::INSN_SMADDPT, std::nullopt}},
            {0b1000111, {InstructionID::INSN_SMSUBPT, std::nullopt}},
            {0b1001010, {InstructionID::INSN_UMADDL, InstructionID::INSN_UMULL}},
            {0b1001011, {InstructionID::INSN_UMSUBL, InstructionID::INSN_UMNEGL}},
            {0b1001100, {InstructionID::INSN_UMULH, std::nullopt}}
        };

        const unsigned short int encoding = (sf << 6) | (op54 << 4) | (op31 << 1) | o0;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};

        // Alias here
        if (alias && Ra == 0b11111)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 32));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 32));
        

            return std::make_pair(alias.value(), std::move(this->m_Operands));
        }
        else if (op31 == 0b001 || op31 == 0b101)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 32));
               this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 32));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Ra, 64));

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (op31 == 0b010 || op31 == 0b110)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64));

            return std::make_pair(insn, std::move(this->m_Operands));
        }
            
        unsigned short int regSize = sf == 0b1 ? 64 : 32;
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Ra, regSize));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::DataProcessing3Source */
