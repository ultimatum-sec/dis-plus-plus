module;

#include <utility>
#include <cstdint>
#include <format>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDScalarCopy.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarCopy
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
        // +--+--+--------+----+-+----+-+--+--+
        // |01|op|11110000|imm5|0|imm4|1|Rn|Rd|
        // +--+--+--------+----+-+----+-+--+--+

        unsigned short int op, imm5, imm4, Rn, Rd;
        op = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        imm5 = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        imm4 = bits::extract<unsigned short int, std::uint32_t, 11, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        if (op != 0b0 && imm4 != 0b0000) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto index{bits::HighestSetBit<unsigned short int, 5>(imm5)};
        if (index >= 4) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				std::array<disxx::disasm::operand::Register::Type, 4>
				{
					disxx::disasm::operand::Register::Type::TYPE_B,
					disxx::disasm::operand::Register::Type::TYPE_H,
					disxx::disasm::operand::Register::Type::TYPE_S,
					disxx::disasm::operand::Register::Type::TYPE_D
				}[index],
				Rd
			)
		);
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
			disxx::disasm::operand::VectorArrangementSpecifier
			{
				static_cast<unsigned short int>(0b1000 + index),
                static_cast<unsigned short int>(imm5 >> (index + 1))
        	}
		);

        return std::make_pair(InstructionID::INSN_MOV, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDScalarCopy */
