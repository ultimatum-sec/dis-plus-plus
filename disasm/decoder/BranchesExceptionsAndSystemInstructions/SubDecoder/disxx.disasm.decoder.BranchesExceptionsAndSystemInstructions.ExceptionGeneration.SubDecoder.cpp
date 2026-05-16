module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.ExceptionGeneration.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::ExceptionGeneration
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
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--------+---+-----+---+--+
        // |11010100|opc|imm16|op2|LL|
        // +--------+---+-----+---+--+

        unsigned short int opc, imm16, op2, LL;
        opc = bits::extract<unsigned short int, std::uint32_t, 21, 23>(this->m_Insn);
        imm16 = bits::extract<unsigned short int, std::uint32_t, 5, 20>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 2, 4>(this->m_Insn);
        LL = bits::extract<unsigned short int, std::uint32_t, 0, 1>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000001, InstructionID::INSN_SVC},
            {0b00000010, InstructionID::INSN_HVC},
            {0b00000011, InstructionID::INSN_SMC},
            {0b00100000, InstructionID::INSN_BRK},
            {0100000000, InstructionID::INSN_HLT},
            {0b01100000, InstructionID::INSN_TCANCEL},
            {0b10100001, InstructionID::INSN_DCPS1},
            {0b10100010, InstructionID::INSN_DCPS2},
            {0b10100011, InstructionID::INSN_DCPS3}
        };

        const unsigned short int encoding = (opc << 5) | (op2 << 2) | LL;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        if (opc != 0b101 && imm16 != 0)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 16>>(imm16));
    
        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::ExceptionGeneration */
