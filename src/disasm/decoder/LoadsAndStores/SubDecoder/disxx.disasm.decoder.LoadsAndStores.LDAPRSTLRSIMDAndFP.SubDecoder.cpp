module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.LDAPRSTLRSIMDAndFP.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::LDAPRSTLRSIMDAndFP
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +----+------+---+-+----+--+--+--+
        // |size|011101|opc|0|imm9|10|Rn|Rt|
        // +----+------+---+-+----+--+--+--+

        unsigned short int size, opc, Rn, Rt;
        size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
        disxx::disasm::operand::Immediate<signed short int, 9> imm9
        {
            bits::extract<signed short int, std::uint32_t, 12, 20>(this->m_Insn),
            disxx::disasm::operand::Immediate<signed short int, 9>::Option::OPT_SIGNEXTEND
        };

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, disxx::disasm::operand::Register::Type>> insnTable = {
            {0b0000, {InstructionID::INSN_STLUR, disxx::disasm::operand::Register::Type::TYPE_B}},
            {0b0001, {InstructionID::INSN_LDAPUR, disxx::disasm::operand::Register::Type::TYPE_B}},
            {0b0010, {InstructionID::INSN_STLUR, disxx::disasm::operand::Register::Type::TYPE_Q}},
            {0b0011, {InstructionID::INSN_LDAPUR, disxx::disasm::operand::Register::Type::TYPE_Q}},
            {0b0100, {InstructionID::INSN_STLUR, disxx::disasm::operand::Register::Type::TYPE_H}},
            {0b0101, {InstructionID::INSN_LDAPUR, disxx::disasm::operand::Register::Type::TYPE_H}},
            {0b1000, {InstructionID::INSN_STLUR, disxx::disasm::operand::Register::Type::TYPE_S}},
            {0b1001, {InstructionID::INSN_LDAPUR, disxx::disasm::operand::Register::Type::TYPE_S}},
            {0b1100, {InstructionID::INSN_STLUR, disxx::disasm::operand::Register::Type::TYPE_D}},
            {0b1101, {InstructionID::INSN_LDAPUR, disxx::disasm::operand::Register::Type::TYPE_D}}
        };

        const unsigned short int encoding = (size << 2) | opc;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, rtype]{it->second};
        
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(rtype, Rt));
		this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>
			(
				disxx::disasm::operand::Register
				{
					disxx::disasm::operand::Register::Type::TYPE_X,
					Rn,
					true
				}
			)
		);
        static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
            ->AddImmediatePreIndexedOffset(imm9, disxx::disasm::operand::LoadsAndStoresAddress::PreIndexedOffsetKind::IDX_REGULAR);
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::LDAPRSTLRSIMDAndFP */
