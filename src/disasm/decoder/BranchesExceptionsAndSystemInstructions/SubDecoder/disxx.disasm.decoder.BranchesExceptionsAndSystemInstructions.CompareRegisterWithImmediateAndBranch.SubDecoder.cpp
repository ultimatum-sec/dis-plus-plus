module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.CompareRegisterWithImmediateAndBranch.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::CompareRegisterWithImmediateAndBranch
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
        // +--+-------+--+----+-+----+--+
        // |sf|1110101|cc|imm6|0|imm9|Rt|
        // +--+-------+--+----+-+----+--+

        unsigned short int sf, cc, imm6, Rt;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        cc = bits::extract<unsigned short int, std::uint32_t, 21, 23>(this->m_Insn);
        imm6 = bits::extract<unsigned short int, std::uint32_t, 15, 20>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
        const auto imm
        {
            disxx::disasm::operand::Immediate<signed long long int, 64>
            {
                bits::extract<signed long long int, std::uint32_t, 5, 13>(this->m_Insn) << 2,
                disxx::disasm::operand::Immediate<signed long long int, 64>::Option::OPT_SIGNEXTEND
            }
        };

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000, InstructionID::INSN_CBGT},
            {0b0001, InstructionID::INSN_CBLT},
            {0b0010, InstructionID::INSN_CBHI},
            {0b0011, InstructionID::INSN_CBLO},
            {0b0110, InstructionID::INSN_CBEQ},
            {0b0111, InstructionID::INSN_CBNE},
            {0b1000, InstructionID::INSN_CBGT},
            {0b1001, InstructionID::INSN_CBLT},
            {0b1010, InstructionID::INSN_CBHI},
            {0b1011, InstructionID::INSN_CBLO},
            {0b1110, InstructionID::INSN_CBEQ},
            {0b1111, InstructionID::INSN_CBNE}
        };

        const unsigned short int encoding = (sf << 3) | cc;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, sf == 0b1 ? 64 : 32));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<signed short int, 6>>(imm6));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<signed long long int, 64>>(imm + this->m_ProgramCounter));
        this->m_ProgramCounterRelevantAddress = std::ref(**this->m_Operands.rbegin());
        
        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::CompareRegisterWithImmediateAndBranch */
