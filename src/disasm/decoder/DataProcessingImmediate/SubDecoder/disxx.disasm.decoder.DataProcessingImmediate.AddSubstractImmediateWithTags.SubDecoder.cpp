module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingImmediate.AddSubstractImmediateWithTags.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingImmediate::AddSubstractImmediateWithTags
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
        // +--+--+-+-------+----+---+----+--+--+
        // |sf|op|S|1000110|imm6|op3|imm4|Rn|Rd|
        // +--+--+-+-------+----+---+----+--+--+

        unsigned short int sf, op, S, imm6, imm4, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        imm6 = bits::extract<unsigned short int, std::uint32_t, 16, 21>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 14, 15>(this->m_Insn);
        imm4 = bits::extract<unsigned short int, std::uint32_t, 10, 13>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b100, InstructionID::INSN_ADDG},
            {0b110, InstructionID::INSN_SUBG}
        };

        const unsigned short int encoding = (sf << 2) | (op << 1) | S;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, 64, true));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imm6));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(imm4));
    
        return std::make_pair(it->second, std::move(this->m_Operands));    
	}
} /* disxx::disasm::decoder::DataProcessingImmediate::AddSubstractImmediateWithTags */
