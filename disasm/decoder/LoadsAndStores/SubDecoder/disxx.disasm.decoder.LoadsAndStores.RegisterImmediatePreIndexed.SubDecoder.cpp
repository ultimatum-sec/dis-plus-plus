module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.RegisterImmediatePreIndexed.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::RegisterImmediatePreIndexed
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
        // +----+---+--+--+---+-+----+--+--+--+
        // |size|111|VR|00|opc|0|imm9|11|Rn|Rt|
        // +----+---+--+--+---+-+----+--+--+--+

        unsigned short int size, VR, opc, Rn, Rt;
        size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        VR = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
        const auto imm9
        {
            disxx::disasm::operand::Immediate<signed short int, 9>
            {
                bits::extract<signed short int, std::uint32_t, 12, 20>(this->m_Insn),
                disxx::disasm::operand::Immediate<signed short int, 9>::Option::OPT_SIGNEXTEND
            }
        };

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, unsigned short int>> insnTable = {
            {0b00000, {InstructionID::INSN_STRB, 32}},
            {0b00001, {InstructionID::INSN_LDRB, 32}},
            {0b00010, {InstructionID::INSN_LDRSB, 64}},
            {0b00011, {InstructionID::INSN_LDRSB, 32}},
            {0b00100, {InstructionID::INSN_STR, 8}},
            {0b00101, {InstructionID::INSN_LDR, 8}},
            {0b00110, {InstructionID::INSN_STR, 128}},
            {0b00111, {InstructionID::INSN_LDR, 128}},
            {0b01000, {InstructionID::INSN_STRH, 32}},
            {0b01001, {InstructionID::INSN_LDRH, 32}},
            {0b01010, {InstructionID::INSN_LDRSH, 64}},
            {0b01011, {InstructionID::INSN_LDRSH, 32}},
            {0b01100, {InstructionID::INSN_STR, 16}},
            {0b01101, {InstructionID::INSN_LDR, 16}},
            {0b10000, {InstructionID::INSN_STR, 32}},
            {0b10001, {InstructionID::INSN_LDR, 32}},
            {0b10010, {InstructionID::INSN_LDRSW, 64}},
            {0b10100, {InstructionID::INSN_STR, 32}},
            {0b10101, {InstructionID::INSN_LDR, 32}},
            {0b11000, {InstructionID::INSN_STR, 64}},
            {0b11001, {InstructionID::INSN_LDR, 64}},
            {0b11100, {InstructionID::INSN_STR, 64}},
            {0b11101, {InstructionID::INSN_LDR, 64}}
        };

        unsigned short int encoding = (size << 3) | (VR << 2) | opc;
        auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, regSize]{it->second};
        
        this->m_Operands.emplace_back
        (
            std::make_unique<disxx::disasm::operand::Register>
            (
                VR == 0b1
                    ? disxx::disasm::operand::Register::Type::TYPE_NEON
                    : disxx::disasm::operand::Register::Type::TYPE_GPR,
                Rt,
                regSize
            )
        );

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
        static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
            ->AddImmediatePreIndexedOffset(imm9.GetValue(), true);
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::RegisterImmediatePreIndexed */
