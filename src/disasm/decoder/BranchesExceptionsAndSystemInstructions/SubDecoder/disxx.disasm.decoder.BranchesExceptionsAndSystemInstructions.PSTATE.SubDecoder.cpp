module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.PSTATE.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.PState;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::PSTATE
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
        // +-------------+---+----+---+---+--+
        // |1101010100000|op1|0100|CRm|op2|Rt|
        // +-------------+---+----+---+---+--+

        unsigned short int op1, CRm, op2, Rt;
        op1 = bits::extract<unsigned short int, std::uint32_t, 16, 18>(this->m_Insn);
        CRm = bits::extract<unsigned short int, std::uint32_t, 8, 11>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 5, 7>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000000, InstructionID::INSN_CFINV},
            {0b000001, InstructionID::INSN_XAFLAG},
            {0b000010, InstructionID::INSN_AXFLAG}
        };

        if (Rt != 0b1111) [[unlikely]]
           return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const unsigned short int encoding = (op1 << 3) | op2;
        if (const auto it{insnTable.find(encoding)}; it != insnTable.end())
            return std::make_pair(it->second, std::move(this->m_Operands));
        else if (op1 == 0b011 && (CRm >> 3) == 0b0 && op2 == 0b011)
        {
            const auto option{bits::extract<unsigned short int, unsigned short int, 1, 2>(CRm)};
            if (option != 0b01 && option != 0b10) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::PState>(option));

            return std::make_pair
            (
                (CRm & 0b1) == 0b1
                    ? InstructionID::INSN_SMSTART
                    : InstructionID::INSN_SMSTOP,
                std::move(this->m_Operands)
            );
        }

        const auto pstate{disxx::disasm::operand::PState{encoding}};
        auto imm{CRm};
        try
        {
            const auto _{pstate.GetMnemonic()};
            // All good, can decode PSTATE
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::PState>(pstate));
        }
        catch (...)
        {
            // Ok, should try to decode it with another encoding!
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::PState>((CRm << 6) | encoding));
            imm &= 1;
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(imm));
    
        return std::make_pair(InstructionID::INSN_MSR, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::PSTATE */
