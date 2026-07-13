module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointDataProcessing2Source.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointDataProcessing2Source
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
        // +-+-+-+-----+-----+-+--+------+--+--+--+
        // |M|0|S|11110|ftype|1|Rm|opcode|10|Rn|Rd|
        // +-+-+-+-----+-----+-+--+------+--+--+--+

        unsigned short int M, S, ftype, Rm, opcode, Rn, Rd;
        M = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        ftype = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000000, InstructionID::INSN_FMUL},
            {0b00000001, InstructionID::INSN_FDIV},
            {0b00000010, InstructionID::INSN_FADD},
            {0b00000011, InstructionID::INSN_FSUB},
            {0b00000100, InstructionID::INSN_FMAX},
            {0b00000101, InstructionID::INSN_FMIN},
            {0b00000110, InstructionID::INSN_FMAXNM},
            {0b00000111, InstructionID::INSN_FMINNM},
            {0b00001000, InstructionID::INSN_FNMUL},
            {0b00010000, InstructionID::INSN_FMUL},
            {0b00010001, InstructionID::INSN_FDIV},
            {0b00010010, InstructionID::INSN_FADD},
            {0b00010011, InstructionID::INSN_FSUB},
            {0b00010100, InstructionID::INSN_FMAX},
            {0b00010101, InstructionID::INSN_FMIN},
            {0b00010110, InstructionID::INSN_FMAXNM},
            {0b00010111, InstructionID::INSN_FMINNM},
            {0b00011000, InstructionID::INSN_FNMUL},
            {0b00110000, InstructionID::INSN_FMUL},
            {0b00110001, InstructionID::INSN_FDIV},
            {0b00110010, InstructionID::INSN_FADD},
            {0b00110011, InstructionID::INSN_FSUB},
            {0b00110100, InstructionID::INSN_FMAX},
            {0b00110101, InstructionID::INSN_FMIN},
            {0b00110110, InstructionID::INSN_FMAXNM},
            {0b00110111, InstructionID::INSN_FMINNM},
            {0b00111000, InstructionID::INSN_FNMUL}
        };

        const unsigned short int encoding = (M << 7) | (S << 6) | (ftype << 4) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const unsigned short int regSize = 64 >> (ftype - (ftype != 0b00 ? 1 : -1));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, regSize));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointDataProcessing2Source */
