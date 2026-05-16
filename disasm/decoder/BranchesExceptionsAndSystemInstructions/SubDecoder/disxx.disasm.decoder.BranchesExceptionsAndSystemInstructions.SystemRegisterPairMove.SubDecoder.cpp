module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemRegisterPairMove.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::SystemRegisterPairMove
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
        // +----------+-+-+--+---+---+---+---+--+
        // |1101010101|L|1|o0|op1|CRn|CRm|op2|Rt|
        // +----------+-+-+--+---+---+---+---+--+

        unsigned short int L, o0, op1, CRn, CRm, op2, Rt;
        L = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        o0 = bits::extract<unsigned short int, std::uint32_t, 19, 19>(this->m_Insn);
        op1 = bits::extract<unsigned short int, std::uint32_t, 16, 18>(this->m_Insn);
        CRn = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        CRm = bits::extract<unsigned short int, std::uint32_t, 8, 11>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 5, 7>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const unsigned short int systemreg = (o0 << 14) | (op1 << 11) | (CRn << 7) | (CRm << 3) | op2;
        if (L == 0b0)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_SYSREG, systemreg, 0));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt + 1, 64));

            return std::make_pair(InstructionID::INSN_MSRR, std::move(this->m_Operands));
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt + 1, 64));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_SYSREG, systemreg, 0));

        return std::make_pair(InstructionID::INSN_MRRS, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::SystemRegisterPairMove */
