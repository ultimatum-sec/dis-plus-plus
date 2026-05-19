module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <tuple>

module disxx.disasm.decoder.LoadsAndStores.RegisterOffset.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.PrefetchOperand;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.Shift;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::RegisterOffset
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
        // +----+---+--+--+---+-+--+------+-+--+--+--+
        // |size|111|VR|00|opc|1|Rm|option|S|10|Rn|Rt|
        // +----+---+--+--+---+-+--+------+-+--+--+--+

        unsigned short int size, VR, opc, Rm, option, S, Rn, Rt;
        size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        VR = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        option = bits::extract<unsigned short int, std::uint32_t, 13, 15>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 12, 12>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static const std::unordered_map<unsigned short int, std::tuple<InstructionID, unsigned short int, unsigned short int>> insnTable = {
        // |encoding|mnemonic|size|amount|
            {0b00000, {InstructionID::INSN_STRB, 32, 1}},
            {0b00001, {InstructionID::INSN_LDRB, 32, 1}},
            {0b00010, {InstructionID::INSN_LDRSB, 64, 1}},
            {0b00011, {InstructionID::INSN_LDRSB, 32, 1}},
            {0b00100, {InstructionID::INSN_STR, 8, 1}},
            {0b00101, {InstructionID::INSN_LDR, 8, 1}},
            {0b00110, {InstructionID::INSN_STR, 128, 4}},
            {0b00111, {InstructionID::INSN_LDR, 128, 4}},
            {0b01000, {InstructionID::INSN_STRH, 32, 1}},
            {0b01001, {InstructionID::INSN_LDRH, 32, 1}},
            {0b01010, {InstructionID::INSN_LDRSH, 64, 1}},
            {0b01011, {InstructionID::INSN_LDRSH, 32, 1}},
            {0b01100, {InstructionID::INSN_STR, 16, 1}},
            {0b01101, {InstructionID::INSN_LDR, 16, 1}},
            {0b10000, {InstructionID::INSN_STR, 32, 2}},
            {0b10001, {InstructionID::INSN_LDR, 32, 2}},
            {0b10010, {InstructionID::INSN_LDRSW, 64, 2}},
            {0b10100, {InstructionID::INSN_STR, 32, 2}},
            {0b10101, {InstructionID::INSN_LDR, 32, 2}},
            {0b11000, {InstructionID::INSN_STR, 64, 3}},
            {0b11001, {InstructionID::INSN_LDR, 64, 3}},
            {0b11100, {InstructionID::INSN_STR, 64, 3}},
            {0b11101, {InstructionID::INSN_LDR, 64, 3}}
        };

        const unsigned short int encoding = (size << 3) | (VR << 2) | opc;
        if (const auto it{insnTable.find(encoding)}; it != insnTable.end())
        {
            const auto &[insn, regSize, amount]{it->second};

            const auto type
            {
                VR
                    ? disxx::disasm::operand::Register::Type::TYPE_NEON
                    : disxx::disasm::operand::Register::Type::TYPE_GPR
            };

            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rt, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
    
            if (option != 0b011)
            {
                static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())->AddRegisterOffset
                (
                    disxx::disasm::operand::Register
                    {
                        disxx::disasm::operand::Register::Type::TYPE_GPR,
                        Rm,
                        static_cast<unsigned short int>
                        (
                            32
                                << bits::extract<unsigned short int, unsigned short int, 0, 0>(option)
                        )
                    }
                );

                if (encoding <= 0b00101 || (option != 0b010 && option != 0b110 && option != 0b111))
                    static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                        ->AddExtension(option, amount);
                else if (S && amount)
                    static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                        ->AddShift(disxx::disasm::operand::Shift::Type::SHIFT_LSL, amount);
                return std::make_pair(insn, std::move(this->m_Operands));
            }
            
            static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                ->AddRegisterOffset(disxx::disasm::operand::Register{disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, 64});
            if (S && amount)
                static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                    ->AddShift(disxx::disasm::operand::Shift::Type::SHIFT_LSL, amount);
            return std::make_pair(insn, std::move(this->m_Operands));
        }
        
        if (encoding != 0b11010) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        InstructionID insn{};
        if ((option & 0b010) == 0b010) [[likely]]
            insn = (Rt & 0b11000) == 0b11000 ? InstructionID::INSN_RPRFM : InstructionID::INSN_PRFM;
        else [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        /*
        if ((Rt & 0b11000) == 0b11000)
        {
            auto imm6{bits::extract<unsigned short int, unsigned short int, 2, 2>(option) << 5};
            imm6 |= bits::extract<unsigned short int, unsigned short int, 0, 0>(option) << 4;
            imm6 |= S << 3;
            imm6 |= bits::extract<unsigned short int, unsigned short int, 0, 2>(Rt);
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imm6));
        }
        */

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::PrefetchOperand>(encoding));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
        static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())->AddRegisterOffset
        (
            disxx::disasm::operand::Register
            {
                disxx::disasm::operand::Register::Type::TYPE_GPR,
                Rm,
                static_cast<unsigned short int>
                (
                    32
                        << ((Rt & 0b11000) == 0b11000 || bits::extract<unsigned short int, unsigned short int, 0, 0>(option) == 0b1)
                )
            }
        );

        if ((Rt & 0b11000) != 0b11000)
        {
            switch (option)
            {
              case 0b010: [[fallthrough]];
              case 0b110: [[fallthrough]];
              case 0b111:
                static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.at(2).get())
                    ->AddExtension(option, S ? 3 : 0);
                return std::make_pair(insn, std::move(this->m_Operands));

              default:
                if (S)
                    static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.at(2).get())
                        ->AddShift(disxx::disasm::operand::Shift::Type::SHIFT_LSL, 3);
                return std::make_pair(insn, std::move(this->m_Operands));
            }
        }

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::RegisterOffset */
