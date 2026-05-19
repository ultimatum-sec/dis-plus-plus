module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.Barriers.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.MemoryBarrier;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::Barriers
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
        // +--------------------+---+---+--+
        // |11010101000000110011|CRm|op2|Rt|
        // +--------------------+---+---+--+

        unsigned short int CRm, op2, Rt;
        CRm = bits::extract<unsigned short int, std::uint32_t, 8, 11>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 5, 7>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b01011111, InstructionID::INSN_CLREX},
            {0b10011111, InstructionID::INSN_DSB},
            {0b10111111, InstructionID::INSN_DMB},
            {0b11011111, InstructionID::INSN_ISB},
            {0b11111111, InstructionID::INSN_SB}
        };

        // It has no any instruction yet
        InstructionID insn{};

        const unsigned short int encoding = (op2 << 5) | Rt;
        if (const auto it{insnTable.find(encoding)}; it != insnTable.end()) [[likely]]
            insn = it->second;
        else if ((CRm & 0b0011) == 0b0010 && encoding == 0b00111111) [[likely]]
            insn = InstructionID::INSN_DSB;
        else if (CRm == 0b0000 && encoding == 0b01111111) [[likely]]
            insn = InstructionID::INSN_TCOMMIT;
        else [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        if (op2 == 0b111 || op2 == 0b011)
            return std::make_pair(insn, std::move(this->m_Operands));
        else if (op2 == 0b010)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(CRm));
            return std::make_pair(insn, std::move(this->m_Operands));
        }

        this->m_Operands.emplace_back
        (
            std::make_unique<disxx::disasm::operand::MemoryBarrier>
            (
                CRm,
                op2 != 0b010 && op2 != 0b110 && (CRm & 0b0011) == 0b0010 && encoding == 0b00111111
            )
        );

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::Barriers */
