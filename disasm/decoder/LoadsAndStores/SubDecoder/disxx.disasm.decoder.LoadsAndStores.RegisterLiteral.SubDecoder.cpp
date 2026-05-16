module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.LoadsAndStores.RegisterLiteral.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.PrefetchOperand;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::RegisterLiteral
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
        // +---+---+--+--+-----+--+
        // |opc|011|VR|00|imm19|Rt|
        // +---+---+--+--+-----+--+

        unsigned short int opc, VR, Rt;
        unsigned long int imm19;

        opc = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        VR = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        imm19 = bits::extract<unsigned long int, std::uint32_t, 5, 23>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        if (opc == 0b11 && VR == 0b1) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        auto &&[insn, opr]
        {
            [opc, VR, Rt](void)
                -> std::pair<InstructionID, std::variant<std::unique_ptr<disxx::disasm::operand::PrefetchOperand>, std::unique_ptr<disxx::disasm::operand::Register>>>
            {
                if (opc == 0b11 && VR == 0b0)
                    return std::make_pair(InstructionID::INSN_PRFM, std::make_unique<disxx::disasm::operand::PrefetchOperand>(Rt));
                else
                {
                    return std::make_pair
                    (
                        opc == 0b10 && VR == 0b0
                            ? InstructionID::INSN_LDRSW
                            : InstructionID::INSN_LDR,
                        std::make_unique<disxx::disasm::operand::Register>
                        (
                            VR == 0b1
                                ? disxx::disasm::operand::Register::Type::TYPE_NEON
                                : disxx::disasm::operand::Register::Type::TYPE_GPR,
                            Rt,
                            opc == 0b10
                                ? (64 << VR)
                                : (opc == 0b01 ? 64 : 32)
                        )
                    );
                }
            }()
        };

        std::visit
        (
            [this](auto &&var) -> void
            { this->m_Operands.emplace_back(std::move(var)); },
            opr
        );
        this->m_Operands.emplace_back
        (
            std::make_unique<disxx::disasm::operand::Immediate<unsigned long int, 19>>
            (
                imm19,
                disxx::disasm::operand::Immediate<unsigned long int, 19>::Option::OPT_SIGNEXTEND
            )
        );
        *static_cast<disxx::disasm::operand::Immediate<unsigned long int, 19> *>(this->m_Operands.rbegin()->get()) += this->m_ProgramCounter;
        this->m_ProgramCounterRelevantAddress = std::ref(**this->m_Operands.rbegin());

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::RegisterLiteral */
