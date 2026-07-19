module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarPairwise.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarPairwise
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
        // +--+-+-----+----+-----+------+--+--+--+
        // |01|U|11110|size|11000|opcode|10|Rn|Rd|
        // +--+-+-----+----+-----+------+--+--+--+

        unsigned short int U, size, opcode, Rn, Rd;
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 16>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00001100, InstructionID::INSN_FMAXNMP},
            {0b00001101, InstructionID::INSN_FADDP},
            {0b00001111, InstructionID::INSN_FMAXP},
            {0b01001100, InstructionID::INSN_FMINNMP},
            {0b01001111, InstructionID::INSN_FMINP},
            {0b01111011, InstructionID::INSN_ADDP},
            {0b10001100, InstructionID::INSN_FMAXNMP},
            {0b10001101, InstructionID::INSN_FADDP},
            {0b10001111, InstructionID::INSN_FMAXP},
            {0b11001100, InstructionID::INSN_FMINNMP},
            {0b11001111, InstructionID::INSN_FMINP}
        };

        unsigned short int encoding = (U << 7) | (size << 5) | opcode;
        if (U == 0b1) encoding &= ~(1 << 5);

        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto type
        {
            (U == 0b1 || opcode == 0b11011)
                ? (
                    bits::extract<unsigned short int, unsigned short int, 0, 0>(size)
                        ? disxx::disasm::operand::Register::Type::TYPE_D
                        : disxx::disasm::operand::Register::Type::TYPE_S
                ) : disxx::disasm::operand::Register::Type::TYPE_H
        };

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rd));
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rn
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetVectorArrangementSpecifier
        (
            type != disxx::disasm::operand::Register::Type::TYPE_H
                ? (
					type == disxx::disasm::operand::Register::Type::TYPE_S
						? disxx::disasm::operand::VectorArrangementSpecifier{0b010}
						: disxx::disasm::operand::VectorArrangementSpecifier{0b111}
				)
                : disxx::disasm::operand::VectorArrangementSpecifier{0b1110}
        );

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarPairwise */
