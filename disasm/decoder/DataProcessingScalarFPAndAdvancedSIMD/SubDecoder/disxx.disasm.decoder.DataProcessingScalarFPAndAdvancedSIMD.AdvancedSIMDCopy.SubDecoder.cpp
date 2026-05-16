module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <format>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDCopy.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDCopy
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
        // +-+-+--+--------+----+-+----+-+--+--+
        // |0|Q|op|01110000|imm5|0|imm4|1|Rn|Rd|
        // +-+-+--+--------+----+-+----+-+--+--+

        unsigned short int Q, op, imm5, imm4, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        imm5 = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        imm4 = bits::extract<unsigned short int, std::uint32_t, 11, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000000, InstructionID::INSN_DUP},
            {0b000001, InstructionID::INSN_DUP},
            {0b000101, InstructionID::INSN_SMOV},
            {0b000111, InstructionID::INSN_UMOV},
            {0b100011, InstructionID::INSN_INS},
            {0b100101, InstructionID::INSN_SMOV},
            {0b100111, InstructionID::INSN_UMOV},
            {0b110000, InstructionID::INSN_INS}
        };

        unsigned short int encoding = (Q << 5) | (op << 4) | imm4;
        if (imm4 <= 0b0001)
            encoding &= ~(1 << 5);
        else if (encoding == 0b100111 && (imm5 & 0xF) != 0b1000) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        else if (Q == 0b1 && op == 0b1)
            encoding &= ~0xF;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end() || (imm5 & ~(1 << 4)) == 0b00000) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto index{bits::LowestSetBitNZ<unsigned short int, 5>(imm5)};
        if (encoding <= 0b000001)
        {
            if ((index == 3 && Q == 0b0) || index == 4) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::array<const char *, 4>
                {
                    Q ? "16b" : "8b",
                    Q ? "8h" : "4h",
                    Q ? "4s" : "2s",
                    "2d"
                }.at(index)
            );

            if (encoding == 0b000000)
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
                (
                    std::format
                    (
                        "{:c}[{}]",
                        std::array<char, 4>{'b', 'h', 's', 'd'}.at(index),
                        imm5 >> (index + 1)
                    )
                );
            }
            else
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, index == 3 ? 64 : 32));

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (imm4 == 0b0101)
        {
            if ((Q == 0b0 && index >= 2) || (Q == 0b1 && index >= 3)) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, Q == 0b1 ? 64 : 32));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "{}[{}]",
                    std::array<char, 3>{'b', 'h', 's'}.at(index),
                    imm5 >> (index + 1)
                )
            );

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (imm4 == 0b0111)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, Q == 0b1 ? 64 : 32));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));

            if (Q == 0b1)
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
                    ->SetArrangementSpecifier(std::format("d[{}]", imm5 >> 4));
            else
            {
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
                (
                    std::format
                    (
                        "{:c}[{}]",
                        std::array<char, 3>{'b', 'h', 's'}.at(index),
                        imm5 >> (index + 1)
                    )
                );
            }

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
            
        if (index == 4) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto Ts{std::array<char, 4>{'b', 'h', 's', 'd'}.at(index)};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
        (
            std::format
            (
                "{:c}[{}]",
                Ts,
                imm5 >> (index + 1)
            )
        );

        if (op == 0b1)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "{:c}[{}]",
                    Ts,
                    imm4 >> index
                )
            );
        }

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDCopy */
