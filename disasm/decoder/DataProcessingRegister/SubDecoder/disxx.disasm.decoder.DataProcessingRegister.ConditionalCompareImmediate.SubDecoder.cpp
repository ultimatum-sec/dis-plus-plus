module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.ConditionalCompareImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Condition;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::ConditionalCompareImmediate
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
        // +--+--+-+--------+----+----+-+--+--+--+----+
        // |sf|op|S|11010010|imm5|cond|1|o2|Rn|o3|nzcv|
        // +--+--+-+--------+----+----+-+--+--+--+----+

        unsigned short int sf, op, S, imm5, cond, o2, Rn, o3, nzcv;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        imm5 = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        cond = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        o2 = bits::extract<unsigned short int, std::uint32_t, 10, 10>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        o3 = bits::extract<unsigned short int, std::uint32_t, 4, 4>(this->m_Insn);
        nzcv = bits::extract<unsigned short int, std::uint32_t, 0, 3>(this->m_Insn);

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00100, InstructionID::INSN_CCMN},
            {0b01100, InstructionID::INSN_CCMP},
            {0b10100, InstructionID::INSN_CCMN},
            {0b11100, InstructionID::INSN_CCMP}
        };
    
        const unsigned short int encoding = (sf << 4) | (op << 3) | (S << 2) | (o2 << 1) | o3;
           const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, sf == 0b1 ? 64 : 32));
           if (sf == 0b1)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned long long int, 64>>(imm5, disxx::disasm::operand::Immediate<unsigned long long int, 64>::Option::OPT_ZEROEXTEND));
        else
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned int, 32>>(imm5, disxx::disasm::operand::Immediate<unsigned int, 32>::Option::OPT_ZEROEXTEND));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(nzcv));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Condition>(cond));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::ConditionalCompareImmediate */
