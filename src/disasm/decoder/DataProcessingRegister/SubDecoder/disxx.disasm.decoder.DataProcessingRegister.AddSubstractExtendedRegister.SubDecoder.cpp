module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.AddSubstractExtendedRegister.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Extension;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::AddSubstractExtendedRegister
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
        // +--+--+-+-----+---+-+--+------+----+--+--+
        // |sf|op|S|01011|opt|1|Rm|option|imm3|Rn|Rd|
        // +--+--+-+-----+---+-+--+------+----+--+--+

        unsigned short int sf, op, S, opt, Rm, option, imm3, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        opt = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        option = bits::extract<unsigned short int, std::uint32_t, 13, 15>(this->m_Insn);
        imm3 = bits::extract<unsigned short int, std::uint32_t, 10, 12>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b00000, {InstructionID::INSN_ADD, std::nullopt}},
            {0b00100, {InstructionID::INSN_ADDS, InstructionID::INSN_CMN}},
            {0b01000, {InstructionID::INSN_SUB, std::nullopt}},
            {0b01100, {InstructionID::INSN_SUBS, InstructionID::INSN_CMP}},
            {0b10000, {InstructionID::INSN_ADD, std::nullopt}},
            {0b10100, {InstructionID::INSN_ADDS, InstructionID::INSN_CMN}},
            {0b11000, {InstructionID::INSN_SUB, std::nullopt}},
            {0b11100, {InstructionID::INSN_SUBS, InstructionID::INSN_CMP}}
        };

        const unsigned short int encoding = (sf << 4) | (op << 3) | (S << 2) | opt;
        auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};
        const unsigned short int regSize = 32 << sf;

        // TODO: Check all the extensions
        if (alias && Rd == 0b11111)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize, true));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
            if (imm3 && (sf == 0b0 && option < 0b1000))
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Extension>(option, imm3));

            return std::make_pair(alias.value(), std::move(this->m_Operands));
        }
            
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize, true));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize, true));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, (sf == 0b1 && (option & ~(0b1 << 2)) == 0b11) ? 64 : 32));
        if (imm3)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Extension>(option, imm3));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::AddSubstractExtendedRegister */
