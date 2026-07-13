module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.ConversionBetweenFloatingPointAndInteger.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::ConversionBetweenFloatingPointAndInteger
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+-+-+-----+-----+-+-----+------+------+--+--+
        // |sf|0|S|11110|ftype|1|rmode|opcode|000000|Rn|Rd|
        // +--+-+-+-----+-----+-+-----+------+------+--+--+

        unsigned short int sf, S, ftype, rmode, opcode, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        ftype = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        rmode = bits::extract<unsigned short int, std::uint32_t, 19, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 16, 18>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000000000, InstructionID::INSN_FCVTNS},
            {0b000000001, InstructionID::INSN_FCVTNU},
            {0b000000010, InstructionID::INSN_SCVTF},
            {0b000000011, InstructionID::INSN_UCVTF},
            {0b000000100, InstructionID::INSN_FCVTAS},
            {0b000000101, InstructionID::INSN_FCVTAU},
            {0b000000110, InstructionID::INSN_FMOV},
            {0b000000111, InstructionID::INSN_FMOV},
            {0b000001000, InstructionID::INSN_FCVTPS},
            {0b000001001, InstructionID::INSN_FCVTPU},
            {0b000010000, InstructionID::INSN_FCVTMS},
            {0b000010001, InstructionID::INSN_FCVTMU},
            {0b000011000, InstructionID::INSN_FCVTZS},
            {0b000011001, InstructionID::INSN_FCVTZU},
            {0b000100000, InstructionID::INSN_FCVTNS},
            {0b000100001, InstructionID::INSN_FCVTNU},
            {0b000100010, InstructionID::INSN_SCVTF},
            {0b000100011, InstructionID::INSN_UCVTF},
            {0b000100100, InstructionID::INSN_FCVTAS},
            {0b000100101, InstructionID::INSN_FCVTAU},
            {0b000101000, InstructionID::INSN_FCVTPS},
            {0b000101001, InstructionID::INSN_FCVTPU},
            {0b000101010, InstructionID::INSN_FCVTNS},
            {0b000101011, InstructionID::INSN_FCVTNU},
            {0b000110000, InstructionID::INSN_FCVTMS},
            {0b000110001, InstructionID::INSN_FCVTMU},
            {0b000110010, InstructionID::INSN_FCVTPS},
            {0b000110011, InstructionID::INSN_FCVTPU},
            {0b000110100, InstructionID::INSN_FCVTMS},
            {0b000110101, InstructionID::INSN_FCVTMU},
            {0b000110110, InstructionID::INSN_FCVTZS},
            {0b000110111, InstructionID::INSN_FCVTZU},
            {0b000111000, InstructionID::INSN_FCVTZS},
            {0b000111001, InstructionID::INSN_FCVTZU},
            {0b000111010, InstructionID::INSN_FCVTAS},
            {0b000111011, InstructionID::INSN_FCVTAU},
            {0b000111100, InstructionID::INSN_SCVTF},
            {0b000111101, InstructionID::INSN_UCVTF},
            {0b000111110, InstructionID::INSN_FJCVTZS},
            {0b001100000, InstructionID::INSN_FCVTNS},
            {0b001100001, InstructionID::INSN_FCVTNU},
            {0b001100010, InstructionID::INSN_SCVTF},
            {0b001100011, InstructionID::INSN_UCVTF},
            {0b001100100, InstructionID::INSN_FCVTAS},
            {0b001100101, InstructionID::INSN_FCVTAU},
            {0b001100110, InstructionID::INSN_FMOV},
            {0b001100111, InstructionID::INSN_FMOV},
            {0b001101000, InstructionID::INSN_FCVTPS},
            {0b001101001, InstructionID::INSN_FCVTPU},
            {0b001101010, InstructionID::INSN_FCVTNS},
            {0b001101011, InstructionID::INSN_FCVTNU},
            {0b001110000, InstructionID::INSN_FCVTMS},
            {0b001110001, InstructionID::INSN_FCVTMU},
            {0b001110010, InstructionID::INSN_FCVTPS},
            {0b001110011, InstructionID::INSN_FCVTPU},
            {0b001110100, InstructionID::INSN_FCVTMS},
            {0b001110101, InstructionID::INSN_FCVTMU},
            {0b001110110, InstructionID::INSN_FCVTZS},
            {0b001110111, InstructionID::INSN_FCVTZU},
            {0b001111000, InstructionID::INSN_FCVTZS},
            {0b001111001, InstructionID::INSN_FCVTZU},
            {0b001111010, InstructionID::INSN_FCVTAS},
            {0b001111011, InstructionID::INSN_FCVTAU},
            {0b001111100, InstructionID::INSN_SCVTF},
            {0b001111101, InstructionID::INSN_UCVTF},
            {0b100000000, InstructionID::INSN_FCVTNS},
            {0b100000001, InstructionID::INSN_FCVTNU},
            {0b100000010, InstructionID::INSN_SCVTF},
            {0b100000011, InstructionID::INSN_UCVTF},
            {0b100000100, InstructionID::INSN_FCVTAS},
            {0b100000101, InstructionID::INSN_FCVTAU},
            {0b100001000, InstructionID::INSN_FCVTPS},
            {0b100001001, InstructionID::INSN_FCVTPU},
            {0b100001010, InstructionID::INSN_FCVTNS},
            {0b100001011, InstructionID::INSN_FCVTNU},
            {0b100010000, InstructionID::INSN_FCVTMS},
            {0b100010001, InstructionID::INSN_FCVTMU},
            {0b100010010, InstructionID::INSN_FCVTPS},
            {0b100010011, InstructionID::INSN_FCVTPU},
            {0b100010100, InstructionID::INSN_FCVTMS},
            {0b100010101, InstructionID::INSN_FCVTMU},
            {0b100010110, InstructionID::INSN_FCVTZS},
            {0b100010111, InstructionID::INSN_FCVTZU},
            {0b100011000, InstructionID::INSN_FCVTZS},
            {0b100011001, InstructionID::INSN_FCVTZU},
            {0b100011010, InstructionID::INSN_FCVTAS},
            {0b100011011, InstructionID::INSN_FCVTAU},
            {0b100011100, InstructionID::INSN_SCVTF},
            {0b100011101, InstructionID::INSN_UCVTF},
            {0b100100000, InstructionID::INSN_FCVTNS},
            {0b100100001, InstructionID::INSN_FCVTNU},
            {0b100100010, InstructionID::INSN_SCVTF},
            {0b100100011, InstructionID::INSN_UCVTF},
            {0b100100100, InstructionID::INSN_FCVTAS},
            {0b100100101, InstructionID::INSN_FCVTAU},
            {0b100100110, InstructionID::INSN_FMOV},
            {0b100100111, InstructionID::INSN_FMOV},
            {0b100101000, InstructionID::INSN_FCVTPS},
            {0b100101001, InstructionID::INSN_FCVTPU},
            {0b100110000, InstructionID::INSN_FCVTMS},
            {0b100110001, InstructionID::INSN_FCVTMU},
            {0b100111000, InstructionID::INSN_FCVTZS},
            {0b100111001, InstructionID::INSN_FCVTZU},
            {0b101001110, InstructionID::INSN_FMOV},
            {0b101001111, InstructionID::INSN_FMOV},
            {0b101100000, InstructionID::INSN_FCVTNS},
            {0b101100001, InstructionID::INSN_FCVTNU},
            {0b101100010, InstructionID::INSN_SCVTF},
            {0b101100011, InstructionID::INSN_UCVTF},
            {0b101100100, InstructionID::INSN_FCVTAS},
            {0b101100101, InstructionID::INSN_FCVTAU},
            {0b101100110, InstructionID::INSN_FMOV},
            {0b101100111, InstructionID::INSN_FMOV},
            {0b101101000, InstructionID::INSN_FCVTPS},
            {0b101101001, InstructionID::INSN_FCVTPU},
            {0b101101010, InstructionID::INSN_FCVTNS},
            {0b101101011, InstructionID::INSN_FCVTNU},
            {0b101110000, InstructionID::INSN_FCVTMS},
            {0b101110001, InstructionID::INSN_FCVTMU},
            {0b101110010, InstructionID::INSN_FCVTPS},
            {0b101110011, InstructionID::INSN_FCVTPU},
            {0b101110100, InstructionID::INSN_FCVTMS},
            {0b101110101, InstructionID::INSN_FCVTMU},
            {0b101110110, InstructionID::INSN_FCVTZS},
            {0b101110111, InstructionID::INSN_FCVTZU},
            {0b101111000, InstructionID::INSN_FCVTZS},
            {0b101111001, InstructionID::INSN_FCVTZU},
            {0b101111010, InstructionID::INSN_FCVTAS},
            {0b101111011, InstructionID::INSN_FCVTAU},
            {0b101111100, InstructionID::INSN_SCVTF},
            {0b101111101, InstructionID::INSN_UCVTF}
        };

        const unsigned short int encoding = (sf << 8) | (S << 7) | (ftype << 5) | (rmode << 3) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        // Terrible piece of code
        if (((rmode == 0b10 || rmode == 0b11) && opcode >= 0b010) || (rmode == 0b01 && (opcode == 0b010 || opcode == 0b011)))
        {
            if (rmode == 0b11 && (opcode == 0b100 || opcode == 0b101))
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 64 >> (ftype - (ftype != 0b00 ? 1 : -1))));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, sf == 0b1 ? 64 : 32));
    
                return std::make_pair(it->second, std::move(this->m_Operands));
            }
            
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, sf == 0b1 ? 64 : 32));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 64 >> (ftype - (ftype != 0b00 ? 1 : -1))));
    
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
            
        if (opcode == 0b010 || opcode == 0b011)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 64 >> (ftype - (ftype != 0b00 ? 1 : -1))));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, sf == 0b1 ? 64 : 32));
            
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
                
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, sf == 0b1 ? 64 : 32));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 64 >> (ftype - (ftype != 0b00 ? 1 : -1))));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::ConversionBetweenFloatingPointAndInteger */
