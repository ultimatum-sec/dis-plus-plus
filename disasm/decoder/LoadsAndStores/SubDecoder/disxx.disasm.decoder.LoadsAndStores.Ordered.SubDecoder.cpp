module;

#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>

module disxx.disasm.decoder.LoadsAndStores.Ordered.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::Ordered
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
        // +----+-------+-+-+--+--+---+--+--+
        // |size|0010001|L|0|Rs|o0|Rt2|Rn|Rt|
        // +----+-------+-+-+--+--+---+--+--+

        unsigned short int size, L, o0, Rn, Rt;
        size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        L = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        // Rs = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        o0 = bits::extract<unsigned short int, std::uint32_t, 15, 15>(this->m_Insn);
        // Rt2 = bits::extract<unsigned short int, std::uint32_t, 10, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
 
        static constexpr std::array<InstructionID, 12> insnTable = {
            InstructionID::INSN_STLLRB, InstructionID::INSN_STLRB,
            InstructionID::INSN_LDLARB, InstructionID::INSN_LDARB,
            InstructionID::INSN_STLLRH, InstructionID::INSN_STLRH,
            InstructionID::INSN_LDLARH, InstructionID::INSN_LDARH,
            InstructionID::INSN_STLLR, InstructionID::INSN_STLR,
            InstructionID::INSN_LDLAR, InstructionID::INSN_LDAR
        };

        const auto insn
        {
            [size, L, o0](void) -> InstructionID
            {
                const unsigned short int encoding = (L << 1) | o0;
                if (size == 0b00)
                    return insnTable.at(encoding);
                else if (size == 0b00)
                    return insnTable.at(encoding + 4);
                return insnTable.at(encoding + 8);
            }()
        };

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, size == 0b11 ? 64 : 32));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::Ordered */
