module;

#include <unordered_map>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarShiftByImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarShiftByImmediate
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
        // +--+-+------+----+----+------+-+--+--+
        // |01|U|111110|immh|immb|opcode|1|Rn|Rd|
        // +--+-+------+----+----+------+-+--+--+

        unsigned short int U, immh, immb, opcode, Rn, Rd;
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        immh = bits::extract<unsigned short int, std::uint32_t, 19, 22>(this->m_Insn);
        immb = bits::extract<unsigned short int, std::uint32_t, 16, 18>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 11, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, std::pair<InstructionID, unsigned short int>> insnTable = {
            {0b000000, {InstructionID::INSN_SSHR, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b000010, {InstructionID::INSN_SSRA, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b000100, {InstructionID::INSN_SRSHR, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b000110, {InstructionID::INSN_SRSRA, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b001010, {InstructionID::INSN_SHL, ((immh << 3) | immb) - (8 << 3)}},
            {0b001110, {InstructionID::INSN_SQSHL, ((immh << 3) | immb) - (8 << 3)}},
            {0b010010, {InstructionID::INSN_SQSHRN, ((immh << 3) | immb) - (8 << bits::HighestSetBitNZ<unsigned short int, 4>(immh))}},
            {0b010011, {InstructionID::INSN_SQRSHRN, ((8 << bits::HighestSetBitNZ<unsigned short int, 2>(bits::extract<unsigned short int, unsigned short int, 0, 2>(immh))) * 2) - ((immh << 3) | immb)}},
            {0b011100, {InstructionID::INSN_SCVTF, (std::array<unsigned short int, 4>{16, 16, 32, 64}.at(bits::HighestSetBit<unsigned short int, 4>(immh)) * 2) - ((immh << 3) | immb)}},
            {0b011111, {InstructionID::INSN_FCVTZS, (std::array<unsigned short int, 4>{16, 16, 32, 64}.at(bits::HighestSetBit<unsigned short int, 4>(immh)) * 2) - ((immh << 3) | immb)}},
            {0b100000, {InstructionID::INSN_USHR, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b100010, {InstructionID::INSN_USRA, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b100100, {InstructionID::INSN_URSHR, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b100110, {InstructionID::INSN_URSRA, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b101000, {InstructionID::INSN_SRI, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b101010, {InstructionID::INSN_SLI, ((8 << 3) * 2) - ((immh << 3) | immb)}},
            {0b101100, {InstructionID::INSN_SQSHLU, ((immh << 3) | immb) - (8 << bits::HighestSetBitNZ<unsigned short int, 4>(immh))}},
            {0b101110, {InstructionID::INSN_UQSHL, ((immh << 3) | immb) - (8 << bits::HighestSetBitNZ<unsigned short int, 4>(immh))}},
            {0b110000, {InstructionID::INSN_SQSHRUN, ((8 << bits::HighestSetBitNZ<unsigned short int, 2>(bits::extract<unsigned short int, unsigned short int, 0, 2>(immh))) * 2) - ((immh << 3) | immb)}},
            {0b110001, {InstructionID::INSN_SQRSHRUN, ((8 << bits::HighestSetBitNZ<unsigned short int, 2>(bits::extract<unsigned short int, unsigned short int, 0, 2>(immh))) * 2) - ((immh << 3) | immb)}},
            {0b110010, {InstructionID::INSN_UQSHRN, ((8 << bits::HighestSetBitNZ<unsigned short int, 2>(bits::extract<unsigned short int, unsigned short int, 0, 2>(immh))) * 2) - ((immh << 3) | immb)}},
            {0b110011, {InstructionID::INSN_UQRSHRN, ((8 << bits::HighestSetBitNZ<unsigned short int, 2>(bits::extract<unsigned short int, unsigned short int, 0, 2>(immh))) * 2) - ((immh << 3) | immb)}},
            {0b111100, {InstructionID::INSN_UCVTF, (std::array<unsigned short int, 4>{16, 16, 32, 64}.at(bits::HighestSetBit<unsigned short int, 4>(immh)) * 2) - ((immh << 3) | immb)}},
            {0b111111, {InstructionID::INSN_FCVTZU, (std::array<unsigned short int, 4>{16, 16, 32, 64}.at(bits::HighestSetBit<unsigned short int, 4>(immh)) * 2) - ((immh << 3) | immb)}}
        };

        if (immh == 0b0000) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const unsigned short int encoding = (U << 5) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, shift]{it->second};

        // Some opcodes requires highest bit (in the size field) is active
        const auto isActive
        {
            [](const unsigned short int val) -> bool
            {
                std::array<unsigned short int, 6> vals
                {
                    0b00000,
                    0b00010,
                    0b00100,
                    0b00110,
                    0b10000,
                    0b01010
                };
    
                return std::ranges::find(vals, val) != vals.end();
            }(opcode)
        };

        const auto index{bits::HighestSetBitNZ<unsigned short int, 4>(immh)};
        if (!isActive)
        {
            if (opcode >= 0b10010 && opcode <= 0b10011)
            {
                // Reserved...
                if (index == 4) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                
                const auto VbSize{std::array<unsigned short int, 3>{8, 16, 32}.at(index)};
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, VbSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, VbSize << 1));
            }
            else
            {
                const auto regSize{std::array<unsigned short int, 4>{8, 16, 32, 64}.at(index)};
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
            }
        }
        else
        {
            if (immh < 0b1000) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            
            const auto regSize{std::array<unsigned short int, 4>{8, 16, 32, 64}.at(index)};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
        }

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 16>>(shift));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarShiftByImmediate */
