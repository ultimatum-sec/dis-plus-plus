module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing1Source.SubDecoder;

import disxx.utility.error.DisassemblyError;
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

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointDataProcessing1Source
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
        // +-+-+-+-----+-----+-+------+-----+--+--+
        // |M|0|S|11110|ftype|1|opcode|10000|Rn|Rd|
        // +-+-+-+-----+-----+-+------+-----+--+--+

        unsigned short int M, S, ftype, opcode, Rn, Rd;
        M = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        ftype = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 15, 20>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000000000, InstructionID::INSN_FMOV},
            {0b0000000001, InstructionID::INSN_FABS},
            {0b0000000010, InstructionID::INSN_FNEG},
            {0b0000000011, InstructionID::INSN_FSQRT},
            {0b0000000101, InstructionID::INSN_FCVT},
            {0b0000000111, InstructionID::INSN_FCVT},
            {0b0000001000, InstructionID::INSN_FRINTN},
            {0b0000001001, InstructionID::INSN_FRINTP},
            {0b0000001010, InstructionID::INSN_FRINTM},
            {0b0000001011, InstructionID::INSN_FRINTZ},
            {0b0000001100, InstructionID::INSN_FRINTA},
            {0b0000001110, InstructionID::INSN_FRINTX},
            {0b0000001111, InstructionID::INSN_FRINTI},
            {0b0000010000, InstructionID::INSN_FRINT32Z},
            {0b0000010001, InstructionID::INSN_FRINT32X},
            {0b0000010010, InstructionID::INSN_FRINT64Z},
            {0b0000010011, InstructionID::INSN_FRINT64X},
            {0b0001000000, InstructionID::INSN_FMOV},
            {0b0001000001, InstructionID::INSN_FABS},
            {0b0001000010, InstructionID::INSN_FNEG},
            {0b0001000011, InstructionID::INSN_FSQRT},
            {0b0001000100, InstructionID::INSN_FCVT},
            {0b0001000110, InstructionID::INSN_BFCVT},
            {0b0001000111, InstructionID::INSN_FCVT},
            {0b0001001000, InstructionID::INSN_FRINTN},
            {0b0001001001, InstructionID::INSN_FRINTP},
            {0b0001001010, InstructionID::INSN_FRINTM},
            {0b0001001011, InstructionID::INSN_FRINTZ},
            {0b0001001100, InstructionID::INSN_FRINTA},
            {0b0001001110, InstructionID::INSN_FRINTX},
            {0b0001001111, InstructionID::INSN_FRINTI},
            {0b0001010000, InstructionID::INSN_FRINT32Z},
            {0b0001010001, InstructionID::INSN_FRINT32X},
            {0b0001010010, InstructionID::INSN_FRINT64Z},
            {0b0001010011, InstructionID::INSN_FRINT64X},
            {0b0011000000, InstructionID::INSN_FMOV},
            {0b0011000001, InstructionID::INSN_FABS},
            {0b0011000010, InstructionID::INSN_FNEG},
            {0b0011000011, InstructionID::INSN_FSQRT},
            {0b0011000100, InstructionID::INSN_FCVT},
            {0b0011000101, InstructionID::INSN_FCVT},
            {0b0011001000, InstructionID::INSN_FRINTN},
            {0b0011001001, InstructionID::INSN_FRINTP},
            {0b0011001010, InstructionID::INSN_FRINTM},
            {0b0011001011, InstructionID::INSN_FRINTZ},
            {0b0011001100, InstructionID::INSN_FRINTA},
            {0b0011001110, InstructionID::INSN_FRINTX},
            {0b0011001111, InstructionID::INSN_FRINTI}
        };

        const unsigned short int encoding = (M << 9) | (S << 8) | (ftype << 6) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        if (opcode <= 0b000111 && opcode >= 0b000100)
        {
            switch ((ftype << 2) | (opcode & 0b11))
            {
              case 0b1100:
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_S,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_H,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));

              case 0b1101:
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_D,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_H,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));

              case 0b0011:
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_H,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_S,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));

              case 0b0001:
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_D,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_S,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));

              case 0b0111:
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_H,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_D,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));

              case 0b0100:
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_S,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_D,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));

              default:
                if (encoding != 0b0001000110) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_H,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_S,
						Rn
					)
				);
                
                return std::make_pair(it->second, std::move(this->m_Operands));
            }
        }

		const auto type{mktp(ftype)};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rd));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rn));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointDataProcessing1Source */
