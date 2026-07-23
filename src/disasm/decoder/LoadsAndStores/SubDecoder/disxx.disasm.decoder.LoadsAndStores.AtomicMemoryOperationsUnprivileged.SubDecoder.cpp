module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperationsUnprivileged.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::AtomicMemoryOperationsUnprivileged
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
        // +-+--+------+-+-+-+--+--+---+--+--+--+
        // |0|sz|011001|A|R|1|Rs|o3|opc|01|Rn|Rt|
        // +-+--+------+-+-+-+--+--+---+--+--+--+

        unsigned short int sz, A, R, Rs, o3, opc, Rn, Rt;
        sz = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        A = bits::extract<unsigned short int, std::uint32_t, 23, 23>(this->m_Insn);
        R = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        Rs = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        o3 = bits::extract<unsigned short int, std::uint32_t, 15, 15>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 12, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const static std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable = {
            {0b000000, {InstructionID::INSN_LDTADD, InstructionID::INSN_STTADD}},
            {0b000001, {InstructionID::INSN_LDTCLR, InstructionID::INSN_STTCLR}},
            {0b000011, {InstructionID::INSN_LDTSET, InstructionID::INSN_STTSET}},
            {0b001000, {InstructionID::INSN_SWPT, std::nullopt}},
            {0b010000, {InstructionID::INSN_LDTADDL, InstructionID::INSN_STTADDL}},
            {0b010001, {InstructionID::INSN_LDTCLRL, InstructionID::INSN_STTCLRL}},
            {0b010011, {InstructionID::INSN_LDTSETL, InstructionID::INSN_STTSETL}},
            {0b011000, {InstructionID::INSN_SWPTL, std::nullopt}},
            {0b100000, {InstructionID::INSN_LDTADDA, std::nullopt}},
            {0b100001, {InstructionID::INSN_LDTCLRA, std::nullopt}},
            {0b100011, {InstructionID::INSN_LDTSETA, std::nullopt}},
            {0b101000, {InstructionID::INSN_SWPTA, std::nullopt}},
            {0b110000, {InstructionID::INSN_LDTADDAL, std::nullopt}},
            {0b110001, {InstructionID::INSN_LDTCLRAL, std::nullopt}},
            {0b110011, {InstructionID::INSN_LDTSETAL, std::nullopt}},
            {0b111000, {InstructionID::INSN_SWPTAL, std::nullopt}}
        };

        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sz
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rs
			)
		);
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sz
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rt
			)
		);
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

        const unsigned short int encoding = (A << 5) | (R << 4) | (o3 << 3) | opc;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, alias]{it->second};
        
        return std::make_pair
        (
            A == 0b0 && Rt == 0b11111 && alias
                ? *alias
                : insn,
            std::move(this->m_Operands)
        );
	}
} /* disxx::disasm::decoder::LoadsAndStores::AtomicMemoryOperationsUnprivileged */
