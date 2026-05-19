module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <ranges>
#include <vector>
#include <array>

module disxx.disasm.decoder.LoadsAndStores.MemoryTags.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::MemoryTags
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
        // +--------+---+-+----+---+--+--+
        // |11011001|opc|1|imm9|op2|Rn|Rt|
        // +--------+---+-+----+---+--+--+

        static constexpr std::array<InstructionID, 4> insnTable = {
            InstructionID::INSN_STG, InstructionID::INSN_STZG,
            InstructionID::INSN_ST2G, InstructionID::INSN_STZ2G
        };

        unsigned short int opc, op2, Rn, Rt;
        opc = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        op2 = bits::extract<unsigned short int, std::uint32_t, 10, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
        disxx::disasm::operand::Immediate<signed short int, 9> imm9
        {
            bits::extract<signed short int, std::uint32_t, 12, 20>(this->m_Insn),
            disxx::disasm::operand::Immediate<signed short int, 9>::Option::OPT_SIGNEXTEND
        }; imm9 <<= static_cast<unsigned short int>(4);

        if (opc != 0b01 && imm9.GetValue() != 0b000000000 && op2 == 0b00) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto result
        {
            [this, opc, op2, &imm9](void) -> std::expected<InstructionID, disxx::utility::error::DisassemblyError>
            {
                if (op2 != 0b00)
                    return insnTable.at(opc);

                // Return a value or throw an error
                const auto raw{imm9.GetValue()};
                switch (opc)
                {
                  case 0b00:
                    if (raw == 0b000000000) [[likely]]
                        return InstructionID::INSN_STZGM;
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
                  case 0b01:
                    return InstructionID::INSN_LDG;
    
                  case 0b10:
                    if (raw == 0b000000000) [[likely]]
                        return InstructionID::INSN_STGM;
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
    
                  default:
                    if (raw == 0b000000000) [[likely]]
                        return InstructionID::INSN_LDGM;
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                }
            }()
        };

        if (!result) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{result.error()}};
        const auto &insn{result.value()};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64, !imm9.GetValue() && !op2 ? false : true));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
        
        switch (op2)
        {
          case 0b01:
            if (imm9.GetValue())
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<signed short int, 9>>(std::move(imm9)));
            return std::make_pair(insn, std::move(this->m_Operands));

          case 0b10:
            static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                ->AddImmediatePreIndexedOffset(imm9.GetValue(), false);
            return std::make_pair(insn, std::move(this->m_Operands));

          case 0b11:
            static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                ->AddImmediatePreIndexedOffset(imm9.GetValue(), true);
            return std::make_pair(insn, std::move(this->m_Operands));

          default:
            static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                ->AddImmediatePreIndexedOffset(imm9.GetValue(), false);
            return std::make_pair(insn, std::move(this->m_Operands));
        }
	}
} /* disxx::disasm::decoder::LoadsAndStores::MemoryTags */
