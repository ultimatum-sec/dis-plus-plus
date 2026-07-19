module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDAcrossLanes.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDAcrossLanes
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
        // +-+-+-+-----+----+-----+------+--+--+--+
        // |0|Q|U|01110|size|11000|opcode|10|Rn|Rd|
        // +-+-+-+-----+----+-----+------+--+--+--+
    
        unsigned short int Q, U, size, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 16>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000000011, InstructionID::INSN_SADDLV},
            {0b000001010, InstructionID::INSN_SMAXV},
            {0b000011010, InstructionID::INSN_SMINV},
            {0b000011011, InstructionID::INSN_ADDV},
            {0b000001100, InstructionID::INSN_FMAXNMV},
            {0b000001111, InstructionID::INSN_FMAXV},
            {0b001001100, InstructionID::INSN_FMINNV},
            {0b001001111, InstructionID::INSN_FMINV},
            {0b010000011, InstructionID::INSN_UADDLV},
            {0b010001010, InstructionID::INSN_UMAXV},
            {0b010011010, InstructionID::INSN_UMINV},
            {0b110001100, InstructionID::INSN_FMAXNMV},
            {0b110001111, InstructionID::INSN_FMAXV},
            {0b111001100, InstructionID::INSN_FMINNMV},
            {0b111001111, InstructionID::INSN_FMINV}
        };

        unsigned short int encoding = (Q << 8) | (U << 7) | (size << 5) | opcode;
        if (opcode == 0b00011 || opcode == 0b01010 || opcode == 0b11010 || opcode == 0b11011)
            encoding &= (~(1 << 8)) & (~(0b11 << 5)); // Turn off Q and size bits
        else if (U == 0b0)
            encoding &= ~(0b1 << 8); // Turn off Q bit

        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end() || size >= 0b10 || (size == 0b10 && Q == 0b0 && U != 0b0)) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        disxx::disasm::operand::VectorArrangementSpecifier T{};
        if (opcode == 0b00011 || opcode == 0b01010 || opcode == 0b11010 || opcode == 0b11011)
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					[size, opcode] mutable -> disxx::disasm::operand::Register::Type
					{
						if (opcode == 0b00011)
							++size;
						switch (opcode)
						{
						  case 0b00:
							return disxx::disasm::operand::Register::Type::TYPE_B;

						  case 0b01:
							return disxx::disasm::operand::Register::Type::TYPE_H;
		
						  case 0b10:
							return disxx::disasm::operand::Register::Type::TYPE_S;

						  case 0b11:
							return disxx::disasm::operand::Register::Type::TYPE_D;

						  default:
							return disxx::disasm::operand::Register::Type::TYPE_Q;
						}
					}(),
					Rd
				)
			);
 
            T = disxx::disasm::operand::VectorArrangementSpecifier((Q << 2) | size);
        }
        else
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					U
						? disxx::disasm::operand::Register::Type::TYPE_S
						: disxx::disasm::operand::Register::Type::TYPE_H,
					Rd
				)
			);

            if (U == 0b0)
				T = disxx::disasm::operand::VectorArrangementSpecifier(0b010 | Q);
            else
				T = disxx::disasm::operand::VectorArrangementSpecifier(0b101);
        }

        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rn
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetVectorArrangementSpecifier(T);

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDAcrossLanes */
