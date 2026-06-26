module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <ranges>
#include <memory>
#include <vector>
#include <array>

module disxx.disasm.decoder.LoadsAndStores.SIMDSingleStructurePostIndexed.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::SIMDSingleStructurePostIndexed
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
        // +-+-+-------+-+-+--+------+-+----+--+--+
        // |0|Q|0011011|L|R|Rm|opcode|S|size|Rn|Rt|
        // +-+-+-------+-+-+--+------+-+----+--+--+

        unsigned short int Q, L, R, Rm, opcode, S, size, Rn, Rt;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        L = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        R = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 13, 15>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 12, 12>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 10, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        if ((opcode & 0b110) == 0b010 && (size & 0b01) == 0b01) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if ((opcode & 0b110) == 0b100 && (size & 0b10) == 0b10) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if ((opcode & 0b110) && S == 0b1 && size == 0b01) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if (L == 0b0 && (opcode & 0b110) == 0b110) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        static const std::unordered_map<unsigned short int, std::pair<InstructionID, unsigned short int>> insnTable = {
            {0b00000, {InstructionID::INSN_ST1, 1}},
            {0b00001, {InstructionID::INSN_ST3, 3}},
            {0b00010, {InstructionID::INSN_ST1, 1}},
            {0b00011, {InstructionID::INSN_ST3, 3}},
            {0b00100, {InstructionID::INSN_ST1, 1}},
            {0b00101, {InstructionID::INSN_ST3, 3}},
            {0b01000, {InstructionID::INSN_ST2, 2}},
            {0b01001, {InstructionID::INSN_ST4, 4}},
            {0b01010, {InstructionID::INSN_ST2, 2}},
            {0b01011, {InstructionID::INSN_ST4, 4}},
            {0b01100, {InstructionID::INSN_ST2, 2}},
            {0b01101, {InstructionID::INSN_ST4, 4}},
            {0b10000, {InstructionID::INSN_LD1, 1}},
            {0b10001, {InstructionID::INSN_LD3, 3}},
            {0b10010, {InstructionID::INSN_LD1, 1}},
            {0b10011, {InstructionID::INSN_LD3, 3}},
            {0b10100, {InstructionID::INSN_LD1, 1}},
            {0b10101, {InstructionID::INSN_LD3, 3}},
            {0b10110, {InstructionID::INSN_LD1R, 1}},
            {0b10111, {InstructionID::INSN_LD3R, 3}},
            {0b11000, {InstructionID::INSN_LD2, 2}},
            {0b11001, {InstructionID::INSN_LD4, 4}},
            {0b11010, {InstructionID::INSN_LD2, 2}},
            {0b11011, {InstructionID::INSN_LD4, 4}},
            {0b11100, {InstructionID::INSN_LD2, 2}},
            {0b11101, {InstructionID::INSN_LD4, 4}},
            {0b11110, {InstructionID::INSN_LD2R, 2}},
            {0b11111, {InstructionID::INSN_LD4R, 4}}
        };

        const std::unordered_map<unsigned short int, unsigned short int> indexTable = {
            {8, (Q << 3) | (S << 2) | size},
            {16, (Q << 2) | (S << 1) | bits::extract<unsigned short int, unsigned short int, 1, 1>(size)},
            {32, (Q << 1) | S},
            {64, Q}
        };

        static constexpr std::array<const char *, 4> specifiersTable{"b", "h", "s", "d"};

        const unsigned short int encoding = ((L << 4) | (R << 3) | opcode);
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, regs]{it->second};

        // Calculating registers size
        const auto result
        {
            [this, opcode, S, size](void) -> std::expected<unsigned short int, disxx::utility::error::DisassemblyError>
            {
                const auto index{bits::HighestSetBit<unsigned short int, 3>(opcode)};
                if (index == 2 && (size != 0b00 || (S != 0b0 && size != 0b01) || opcode & ~(1 << 2))) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                return (8 << (index + (index < 0 ? 1 : 0))) << (S == 0b0 && size == 0b01);
            }()
        };

        if (!result) [[unlikely]]
            return std::unexpected{result.error()};
        const auto &regSize{result.value()};

        const auto [spec, index]
        {
            [&indexTable, Q, L, opcode, S, size, regSize](void) -> std::pair<std::string, std::optional<unsigned short int>>
            {
                if (L == 0b1 && (opcode == 0b110 || opcode == 0b111) && S == 0b0)
                    return std::make_pair(disxx::disasm::operand::Register::GetArrangementSpecifier(Q, size).data(), std::nullopt);
                return std::make_pair
                (
                    specifiersTable.at(regSize / 8 - 1),
                    std::optional<unsigned short int>{indexTable.at(regSize)}
                );
            }()
        };
        
        for (const auto Ri : std::views::iota(Rt, std::add_sat<unsigned short int>(Rt, regs)))
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Ri, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier(spec);
        }

        if (index)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 4>>(*index));
        disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true};
		this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));
        if (Rm != 0b11111)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64, true));
        else
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>((regSize * regs) / 8));
    
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::SIMDSingleStructurePostIndexed */
