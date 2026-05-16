module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.BranchesExceptionsAndSystemInstructions.SystemInstructionIDs.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.SystemOperand;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::SystemInstructionIDs
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
        // +----------+-+--+---+---+---+---+--+
        // |1101010100|L|01|op1|CRn|CRm|op2|Rt|
        // +----------+-+--+---+---+---+---+--+

        unsigned short int L, op1, CRn, CRm, op2, Rt;
        L = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        op1 = bits::extract<unsigned short int, std::uint32_t, 16, 18>(this->m_Insn);
        CRn = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        CRm = bits::extract<unsigned short int, std::uint32_t, 8, 11>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 5, 7>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const unsigned short int encoding = (op1 << 11) | (CRn << 7) | (CRm << 3) | op2;
        if (L == 0b1)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));

            const std::unordered_map<unsigned short int, InstructionID> aliasTable = {
                {0b01101110111001, InstructionID::INSN_GCSPOPM},
                {0b01101110111011, InstructionID::INSN_GCSSS2}
            };

            if (auto it{aliasTable.find(encoding)}; it != aliasTable.end())
                return std::make_pair(it->second, std::move(this->m_Operands));
            
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 3>>(op1));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(CRn));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(CRm));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 3>>(op2));

            return std::make_pair(InstructionID::INSN_SYSL, std::move(this->m_Operands));
        }
        
        const std::unordered_map<unsigned short int, InstructionID> aliasTable = {
            {0b11001110000000, InstructionID::INSN_APAS},
            {0b01101110011100, InstructionID::INSN_CFP},
            {0b01101110011110, InstructionID::INSN_COSP},
            {0b01101110011111, InstructionID::INSN_CPP},
            {0b01101110011101, InstructionID::INSN_DVP},
            {0b00001110111101, InstructionID::INSN_GCSPOPCX},
            {0b00001110111110, InstructionID::INSN_GCSPOPX},
            {0b01101110111000, InstructionID::INSN_GCSPUSHM},
            {0b00001110111100, InstructionID::INSN_GCSPUSHX},
            {0b01101110111010, InstructionID::INSN_GCSSS1},
            {0b01101110010111, InstructionID::INSN_TRCIT}
        };

        if (auto it{aliasTable.find(encoding)}; it != aliasTable.end())
        {
            if (op2 == 0b000 || op2 == 0b010 || (CRm == 0b0010 && op2 == 0b111))
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
            else if (CRm == 0b0011)
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::SystemOperand>(CRm));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
            }

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (CRn == 0b0111 && (CRm & ~1) == 0b1000 && bits::SysOp(op1, 0b0111, CRm, op2) == 1)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::SystemOperand>(encoding));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
        
            return std::make_pair(InstructionID::INSN_AT, std::move(this->m_Operands));
        }
        else if (op1 == 0b001 && CRn == 0b0111 && CRm == 0b0010 && bits::SysOp(0b001, 0b0111, 0b0010, op2) == 2)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::SystemOperand>(op2));
        
            return std::make_pair(InstructionID::INSN_BRB, std::move(this->m_Operands));
        }
        else if (CRn == 0b0111 && bits::SysOp(op1, 0b0111, CRm, op2) == 3)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::SystemOperand>(encoding));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));

            return std::make_pair(InstructionID::INSN_DC, std::move(this->m_Operands));
        }
        else if (CRn == 0b0111 && bits::SysOp(op1, 0b0111, CRm, op2) == 4)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::SystemOperand>(encoding));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
        
            return std::make_pair(InstructionID::INSN_IC, std::move(this->m_Operands));
        }
        else if ((CRn & ~1) == 0b1000 && bits::SysOp(op1, CRn, CRm, op2) == 5)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::SystemOperand>(encoding));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
        
            return std::make_pair(InstructionID::INSN_TLBI, std::move(this->m_Operands));
        }
        
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 3>>(op1));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(CRn));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(CRm));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 3>>(op2));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));

        return std::make_pair(InstructionID::INSN_SYSL, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::BranchesExceptionsAndSystemInstructions::SystemInstructionIDs */
