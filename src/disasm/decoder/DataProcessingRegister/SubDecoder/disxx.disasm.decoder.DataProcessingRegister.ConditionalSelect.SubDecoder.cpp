module;

#include <unordered_map>
#include <optional>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.ConditionalSelect.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Condition;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::ConditionalSelect
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
		: disxx::disasm::decoder::abstract::SubDecoder{std::move(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(std::forward<SubDecoder &&>(other));
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<std::decay_t<decltype(*this)>>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +--+--+-+--------+--+----+---+--+--+
        // |sf|op|S|11010100|Rm|cond|op2|Rn|Rd|
        // +--+--+-+--------+--+----+---+--+--+

        unsigned short int sf, op, S, Rm, cond, op2, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        cond = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 10, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const auto csincAlias
        {
            [Rm, Rn, cond](void) -> std::optional<InstructionID>
            {
                if (Rm != 0b11111 && Rn != 0b11111 && (cond & ~0b1) != 0b1110 && Rn == Rm)
                    return InstructionID::INSN_CINC;
                else if (Rm == 0b11111 && Rn == 0b11111 && (cond & ~0b1) != 0b1110)
                    return InstructionID::INSN_CSET;
                return std::nullopt;
            }
        };

        const auto csinvAlias
        {
            [Rm, Rn, cond](void) -> std::optional<InstructionID>
            {
                if (Rm != 0b11111 && Rn != 0b11111 && (cond & ~0b1) != 0b1110 && Rn == Rm)
                    return InstructionID::INSN_CINV;
                else if (Rm == 0b11111 && Rn == 0b11111 && (cond & ~0b1) != 0b1110)
                    return InstructionID::INSN_CSETM;
                return std::nullopt;
            }
        };

        const auto csnegAlias
        {
            [Rm, Rn, cond](void) -> std::optional<InstructionID>
            {
                if ((cond & ~0b1) != 0b1110 && Rn == Rm)
                    return InstructionID::INSN_CNEG;
                return std::nullopt;
            }
        };

        std::unordered_map<unsigned short int, std::pair<InstructionID, std::function<std::optional<InstructionID>(void)>>> insnTable = {
            {0b00000, {InstructionID::INSN_CSEL, [] -> std::optional<InstructionID> { return std::nullopt; }}},
            {0b00001, {InstructionID::INSN_CSINC, csincAlias}},
            {0b01000, {InstructionID::INSN_CSINV, csinvAlias}},
            {0b01001, {InstructionID::INSN_CSNEG, csnegAlias}},
            {0b10000, {InstructionID::INSN_CSEL, [] -> std::optional<InstructionID> { return std::nullopt; }}},
            {0b10001, {InstructionID::INSN_CSINC, csincAlias}},
            {0b11000, {InstructionID::INSN_CSINV, csinvAlias}},
            {0b11001, {InstructionID::INSN_CSNEG, csnegAlias}}
        };

        const unsigned short int encoding = (sf << 4) | (op << 3) | (S << 2) | op2;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, aliasFunc]{it->second};

        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sf
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rd
			)
		);
		this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sf
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rn
			)
		);
		this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sf
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rm
			)
		);
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Condition>(cond));

        const auto alias{aliasFunc()};

        return std::make_pair
        (
            alias
                ? *alias
                : insn,
            std::move(this->m_Operands)
        );
	}
} /* disxx::disasm::decoder::DataProcessingRegister::ConditionalSelect */
