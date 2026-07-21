module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointConditionalSelect.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Condition;
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

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointConditionalSelect
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
        // +-+-+-+-----+-----+-+--+----+--+--+--+
        // |M|0|S|11110|ftype|1|Rm|cond|11|Rn|Rd|
        // +-+-+-+-----+-----+-+--+----+--+--+--+

        unsigned short int M, S, ftype, Rm, cond, Rn, Rd;
        M = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        ftype = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        cond = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        if (M == 0b1 || S == 0b1 || ftype == 0b10) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

		const auto type{mktp(ftype)};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rd));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rn));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rm));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Condition>(cond));

        return std::make_pair(InstructionID::INSN_FCSEL, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointConditionalSelect */
