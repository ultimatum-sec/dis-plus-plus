module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <format>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicThreeRegisterImm2.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::CryptographicThreeRegisterImm2
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
        // +-----------+--+--+----+------+--+--+
        // |11001110010|Rm|10|imm2|opcode|Rn|Rd|
        // +-----------+--+--+----+------+--+--+

        unsigned short int Rm, imm2, opcode, Rn, Rd;
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        imm2 = bits::extract<unsigned short int, std::uint32_t, 12, 13>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 10, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00, InstructionID::INSN_SM3TT1A},
            {0b01, InstructionID::INSN_SM3TT1B},
            {0b10, InstructionID::INSN_SM3TT2A},
            {0b11, InstructionID::INSN_SM3TT2B}
        };

        const auto it{insnTable.find(opcode)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rd
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
			->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b101});
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rn
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
			->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b101});
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rm
			)
		);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
            ->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b1010, imm2});

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::CryptographicThreeRegisterImm2 */
