module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndFixedPoint.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace
{
	inline disxx::disasm::operand::Register::Type mktp(unsigned short int ftype) noexcept
	{
		switch (ftype)
		{
		  case 0b11:
			return disxx::disasm::operand::Register::Type::TYPE_H;

		  case 0b10:
			[[fallthrough]];
		  case 0b00:
			return disxx::disasm::operand::Register::Type::TYPE_S;
		
		  default:
			return disxx::disasm::operand::Register::Type::TYPE_D;
		}
	}
} /* */

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::ConversionBetweenFloatingPointAndFixedPoint
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
		: disxx::disasm::decoder::abstract::SubDecoder{std::forward<SubDecoder &&>(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(std::forward<SubDecoder &&>(other));
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +--+-+-+-----+-----+-+-----+------+-----+--+--+
        // |sf|0|S|11110|ftype|0|rmode|opcode|scale|Rn|Rd|
        // +--+-+-+-----+-----+-+-----+------+-----+--+--+

        unsigned short int sf, S, ftype, rmode, opcode, scale, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        ftype = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        rmode = bits::extract<unsigned short int, std::uint32_t, 10, 19>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 16, 18>(this->m_Insn);
        scale = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000000010, InstructionID::INSN_SCVTF},
            {0b000000011, InstructionID::INSN_UCVTF},
            {0b000011000, InstructionID::INSN_FCVTZS},
            {0b000011001, InstructionID::INSN_FCVTZU},
            {0b000100010, InstructionID::INSN_SCVTF},
            {0b000100011, InstructionID::INSN_UCVTF},
            {0b000111000, InstructionID::INSN_FCVTZS},
            {0b000111001, InstructionID::INSN_FCVTZU},
            {0b001100010, InstructionID::INSN_SCVTF},
            {0b001100011, InstructionID::INSN_UCVTF},
            {0b001111000, InstructionID::INSN_FCVTZS},
            {0b001111001, InstructionID::INSN_FCVTZU},
            {0b100000010, InstructionID::INSN_SCVTF},
            {0b100000011, InstructionID::INSN_UCVTF},
            {0b100011000, InstructionID::INSN_FCVTZS},
            {0b110011001, InstructionID::INSN_FCVTZU},
            {0b100100010, InstructionID::INSN_SCVTF},
            {0b100100011, InstructionID::INSN_UCVTF},
            {0b100111000, InstructionID::INSN_FCVTZS},
            {0b100111001, InstructionID::INSN_FCVTZU},
            {0b101100010, InstructionID::INSN_SCVTF},
            {0b101100011, InstructionID::INSN_UCVTF},
            {0b101111000, InstructionID::INSN_FCVTZS},
            {0b101111001, InstructionID::INSN_UCVTZS}
        };

        const unsigned short int encoding = (sf << 8) | (S << 7) | (ftype << 5) | (rmode << 3) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
        if (opcode <= 0b001)
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					sf
						? disxx::disasm::operand::Register::Type::TYPE_X
						: disxx::disasm::operand::Register::Type::TYPE_W,
					Rd
				)
			);
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(mktp(ftype), Rn));
        }
        else
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(mktp(ftype), Rd));
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					sf
						? disxx::disasm::operand::Register::Type::TYPE_X
						: disxx::disasm::operand::Register::Type::TYPE_W,
					Rn
				)
			);
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<double, 64>>(64.f - scale));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::ConversionBetweenFloatingPointAndFixedPoint */
