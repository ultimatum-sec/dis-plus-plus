module;

#include <unordered_map>
#include <numeric>
#include <utility>
#include <cstdint>
#include <memory>
#include <ranges>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.SIMDMultipleStructures.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::SIMDMultipleStructures
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
        // +-+-+-------+-+------+------+----+--+--+
        // |0|Q|0011000|L|000000|opcode|size|Rn|Rt|
        // +-+-+-------+-+------+------+----+--+--+

        unsigned short int Q, L, opcode, size, Rn, Rt;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        L = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 10, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, unsigned short int>> insnTable = {
            {0b00000, {InstructionID::INSN_ST4, 4}},
            {0b00010, {InstructionID::INSN_ST4, 4}},
            {0b00100, {InstructionID::INSN_ST3, 3}},
            {0b00110, {InstructionID::INSN_ST3, 3}},
            {0b00111, {InstructionID::INSN_ST1, 1}},
            {0b01000, {InstructionID::INSN_ST2, 2}},
            {0b01010, {InstructionID::INSN_ST2, 2}},
            {0b10000, {InstructionID::INSN_LD4, 4}},
            {0b10010, {InstructionID::INSN_LD4, 4}},
            {0b10100, {InstructionID::INSN_LD3, 3}},
            {0b10110, {InstructionID::INSN_LD3, 3}},
            {0b10111, {InstructionID::INSN_LD1, 1}},
            {0b11000, {InstructionID::INSN_LD2, 2}},
            {0b11010, {InstructionID::INSN_LD2, 2}}
        };

        const auto it{insnTable.find((L << 1) | opcode)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, regs]{it->second};
        for (const auto spec{disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q)}; auto Ri : std::views::iota(Rt, std::add_sat<unsigned short int>(Rt, regs)))
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Ri, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier(spec.data());
        }
       
		disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));
    
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::SIMDMultipleStructures */
