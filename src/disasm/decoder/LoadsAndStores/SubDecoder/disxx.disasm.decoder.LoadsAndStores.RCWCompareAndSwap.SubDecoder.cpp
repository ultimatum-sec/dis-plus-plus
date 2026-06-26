module;

#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>

module disxx.disasm.decoder.LoadsAndStores.RCWCompareAndSwap.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::RCWCompareAndSwap
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
        // +-+-+------+-+-+-+--+------+--+--+
        // |0|S|011001|A|R|1|Rs|000010|Rn|Rt|
        // +-+-+------+-+-+-+--+------+--+--+

        unsigned short int S, A, R, Rs, Rn, Rt;
        S = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        A = bits::extract<unsigned short int, std::uint32_t, 23, 23>(this->m_Insn);
        R = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        Rs = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
    
        static constexpr std::array<InstructionID, 8> insnTable = {
            InstructionID::INSN_RCWCAS, InstructionID::INSN_RCWCASL,
            InstructionID::INSN_RCWCASA, InstructionID::INSN_RCWCASAL,
            InstructionID::INSN_RCWSCAS, InstructionID::INSN_RCWSCASL,
            InstructionID::INSN_RCWSCASA, InstructionID::INSN_RCWSCASAL
        };
    
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rs, 64));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
        disxx::disasm::operand::Register reg{disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64};
		this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(std::move(reg)));

        return std::make_pair(insnTable.at((S << 2) | (A << 1) | R), std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::LoadsAndStores::RCWCompareAndSwap */
