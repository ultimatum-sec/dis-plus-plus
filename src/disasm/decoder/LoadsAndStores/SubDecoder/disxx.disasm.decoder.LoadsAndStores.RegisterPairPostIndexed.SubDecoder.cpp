module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <tuple>

module disxx.disasm.decoder.LoadsAndStores.RegisterPairPostIndexed.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::RegisterPairPostIndexed
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
        // +---+---+--+---+-+----+---+--+--+
        // |opc|101|VR|001|L|imm7|Rt2|Rn|Rt|
        // +---+---+--+---+-+----+---+--+--+

        unsigned short int opc, VR, L, Rt2, Rn, Rt;
        opc = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        VR = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        L = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        Rt2 = bits::extract<unsigned short int, std::uint32_t, 10, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
        const auto imm7
        {
            disxx::disasm::operand::Immediate<signed short int, 7>
            {
                bits::extract<signed short int, std::uint32_t, 15, 21>(this->m_Insn),
                disxx::disasm::operand::Immediate<signed short int, 7>::Option::OPT_SIGNEXTEND
            }
        };

        const std::unordered_map<unsigned short int, std::tuple<InstructionID, unsigned short int, unsigned short int>> insnTable = {
        //  |encoding|mnemonic|size|scale|
            {0b0000, {InstructionID::INSN_STP, 32, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b0001, {InstructionID::INSN_LDP, 32, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b0010, {InstructionID::INSN_STP, 32, 2 + opc}},
            {0b0011, {InstructionID::INSN_LDP, 32, 2 + opc}},
            {0b0100, {InstructionID::INSN_STGP, 64, 4}},
            {0b0101, {InstructionID::INSN_LDPSW, 64, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b0110, {InstructionID::INSN_STP, 64, 2 + opc}},
            {0b0111, {InstructionID::INSN_LDP, 64, 2 + opc}},
            {0b1000, {InstructionID::INSN_STP, 64, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b1001, {InstructionID::INSN_LDP, 64, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b1010, {InstructionID::INSN_STP, 128, 2 + opc}},
            {0b1011, {InstructionID::INSN_LDP, 128, 2 + opc}},
            {0b1100, {InstructionID::INSN_STTP, 64, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b1101, {InstructionID::INSN_LDTP, 64, 2 + bits::extract<unsigned short int, unsigned short int, 1, 1>(opc)}},
            {0b1110, {InstructionID::INSN_STTP, 128, 4}},
            {0b1111, {InstructionID::INSN_LDTP, 128, 4}}
        };

        const unsigned short int encoding = (opc << 2) | (VR << 1) | L;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, regSize, scale]{it->second};

        const auto type
        {
            VR == 0b1
                ? disxx::disasm::operand::Register::Type::TYPE_NEON
                : disxx::disasm::operand::Register::Type::TYPE_GPR
        };
            
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rt, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(type, Rt2, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64, true));
        if (imm7.GetValue())
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<signed short int, 7>>(imm7 << scale));

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::RegisterPairPostIndexed */
