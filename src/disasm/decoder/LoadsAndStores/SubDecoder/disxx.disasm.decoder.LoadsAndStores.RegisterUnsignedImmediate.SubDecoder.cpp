module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <tuple>

module disxx.disasm.decoder.LoadsAndStores.RegisterUnsignedImmediate.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.PrefetchOperand;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::RegisterUnsignedImmediate
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
        // +----+---+--+--+---+-----+--+--+
        // |size|111|VR|01|opc|imm12|Rn|Rt|
        // +----+---+--+--+---+-----+--+--+

        unsigned short int size, VR, opc, imm12, Rn, Rt;
        size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        VR = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        imm12 = bits::extract<unsigned short int, std::uint32_t, 10, 21>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
        
        /*
        if ((size & 0b10) == 0b10 && VR == 0b0 && opc == 0b11)
            throw DisassemblyError(this->m_Insn);
        else if (size != 0b00 && VR == 0b1 && (opc & 0b10) == 0b10)
            throw DisassemblyError(this->m_Insn);
        */


        std::unordered_map<unsigned short int, std::tuple<InstructionID, unsigned short int, unsigned short int>> insnTable = {
        //     |encoding|mnemonic|register size|data size (already in bytes)|
            {0b00000, {InstructionID::INSN_STRB, 32, 1}},
            {0b00001, {InstructionID::INSN_LDRB, 32, 1}},
            {0b00010, {InstructionID::INSN_LDRSB, 64, 1}},
            {0b00011, {InstructionID::INSN_LDRSB, 32, 1}},
            {0b00100, {InstructionID::INSN_STR, 8, 1}},
            {0b00101, {InstructionID::INSN_LDR, 8, 1}},
            {0b00110, {InstructionID::INSN_STR, 128, 16}},
            {0b00111, {InstructionID::INSN_LDR, 128, 16}},
            {0b01000, {InstructionID::INSN_STRH, 32, 2}},
            {0b01001, {InstructionID::INSN_LDRH, 32, 2}},
            {0b01010, {InstructionID::INSN_LDRSH, 64, 2}},
            {0b01011, {InstructionID::INSN_LDRSH, 32, 2}},
            {0b01100, {InstructionID::INSN_STR, 16, 2}},
            {0b01101, {InstructionID::INSN_LDR, 16, 2}},
            {0b10000, {InstructionID::INSN_STR, 32, 4}},
            {0b10001, {InstructionID::INSN_LDR, 32, 4}},
            {0b10010, {InstructionID::INSN_LDRSW, 64, 4}},
            {0b10100, {InstructionID::INSN_STR, 32, 4}},
            {0b10101, {InstructionID::INSN_LDR, 32, 4}},
            {0b11000, {InstructionID::INSN_STR, 64, 8}},
            {0b11001, {InstructionID::INSN_LDR, 64, 8}},
            {0b11100, {InstructionID::INSN_STR, 64, 8}},
            {0b11101, {InstructionID::INSN_LDR, 64, 8}}
        };

        unsigned short int encoding = (size << 3) | (VR << 2) | opc;
        if (const auto it{insnTable.find(encoding)}; it != insnTable.end())
        {
            const auto &[insn, regSize, dataSize]{it->second};

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

			disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));
            if (imm12)
                static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                    ->AddImmediatePreIndexedOffset(static_cast<signed short int>(imm12) * dataSize, false);
            return std::make_pair(insn, std::move(this->m_Operands));
        }

        if (encoding != 0b11010) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::PrefetchOperand>(Rt));
		disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));
        if (imm12)
            static_cast<disxx::disasm::operand::LoadsAndStoresAddress *>(this->m_Operands.rbegin()->get())
                ->AddImmediatePreIndexedOffset(static_cast<signed short int>(imm12) * 8, false);
        return std::make_pair(InstructionID::INSN_PRFM, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::RegisterUnsignedImmediate */
