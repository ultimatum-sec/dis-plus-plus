module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingRegister.EvaluateIntoFlags.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingRegister::EvaluateIntoFlags
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+--+-+--------+-------+--+----+--+--+----+
        // |sf|op|S|11010000|opcode2|sz|0010|Rn|o3|mask|
        // +--+--+-+--------+-------+--+----+--+--+----+

        unsigned short int sf, op, S, opcode2, sz, Rn, o3, mask;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        opcode2 = bits::extract<unsigned short int, std::uint32_t, 15, 20>(this->m_Insn);
        sz = bits::extract<unsigned short int, std::uint32_t, 14, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        o3 = bits::extract<unsigned short int, std::uint32_t, 4, 4>(this->m_Insn);
        mask = bits::extract<unsigned short int, std::uint32_t, 0, 3>(this->m_Insn);

        static const std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b001000000001101, InstructionID::INSN_SETF8},
            {0b001000000101101, InstructionID::INSN_SETF16}
        };

        const unsigned short int encoding = (sf << 14) | (op << 13) | (S << 12) | (opcode2 << 6) | (sz << 5) | (o3 << 4) | mask;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 32));

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingRegister::EvaluateIntoFlags */
