module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <ranges>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.CryptographicTwoRegisterSHA.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::CryptographicTwoRegisterSHA
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
		if (this != &other)
			[[maybe_unused]] const auto &_{disxx::disasm::decoder::abstract::SubDecoder::operator=(other)};
		return *this;
	}

	SubDecoder::SubDecoder(SubDecoder &&other) noexcept
		: disxx::disasm::decoder::abstract::SubDecoder{std::move(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		[[maybe_unused]] const auto &_{disxx::disasm::decoder::abstract::SubDecoder::operator=(std::move(other))};
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--------+----+-----+------+--+--+--+
        // |01011110|size|10100|opcode|10|Rn|Rd|
        // +--------+----+-----+------+--+--+--+

        unsigned short int size, opcode, Rn, Rd;
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 16>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000, InstructionID::INSN_SHA1H},
            {0b00001, InstructionID::INSN_SHA1SU1},
            {0b00010, InstructionID::INSN_SHA256SU0}
        };

        const auto it{insnTable.find(opcode)};
        if (it == insnTable.end() || size == 0b00) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto regSize{opcode != 0b00000 ? 128 + 'V' : 32};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, regSize));
        if (opcode != 0b00000)
            for (const auto s{this->m_Operands.size()}; auto i : std::views::iota(static_cast<decltype(s)>(0), s))
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(i).get())->SetArrangementSpecifier("4s");
        
        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::CryptographicTwoRegisterSHA */
