module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingImmediate.Extract.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingImmediate::Extract
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
	{ return std::make_unique<std::decay_t<std::decay_t<decltype(*this)>>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept(false)
	{
        // +--+----+------+-+--+--+----+--+--+
        // |sf|op21|100111|N|o0|Rm|imms|Rn|Rd|
        // +--+----+------+-+--+--+----+--+--+

        unsigned short int sf, op21, N, o0, Rm, imms, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        op21 = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
        N = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        o0 = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        imms = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        static std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b00000100000, InstructionID::INSN_EXTR},
            {0b10010000000, InstructionID::INSN_EXTR}
        };

        const unsigned short int encoding = (sf << 10) | (op21 << 8) | (N << 7) | (o0 << 6) | (sf == 0b0 ? (imms & 0b100000) : 0b000000);
        auto it = insnTable.find(encoding);
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        unsigned short int regSize = 32 << sf;
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rd, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, regSize));
        if (Rn != Rm)
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rm, regSize));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));

        return std::make_pair
        (
            Rn == Rm
                ? InstructionID::INSN_ROR
                : it->second,
            std::move(this->m_Operands)
        );
	}
} /* disxx::disasm::decoder::DataProcessingImmediate::Extract */
