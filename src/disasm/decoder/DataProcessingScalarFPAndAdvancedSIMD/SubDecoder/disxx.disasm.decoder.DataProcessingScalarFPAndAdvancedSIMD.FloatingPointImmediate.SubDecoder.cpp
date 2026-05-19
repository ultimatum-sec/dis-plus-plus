module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.FloatingPointImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointImmediate
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
        // +-+-+-+-----+-----+-+----+---+----+--+
        // |M|0|S|11110|ftype|1|imm8|100|imm5|Rd|
        // +-+-+-+-----+-----+-+----+---+----+--+

        unsigned short int M, S, ftype, imm8, imm5, Rd;
        M = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        S = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        ftype = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        imm8 = bits::extract<unsigned short int, std::uint32_t, 13, 20>(this->m_Insn);
        imm5 = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b000000000, InstructionID::INSN_FMOV},
            {0b000100000, InstructionID::INSN_FMOV},
            {0b001100000, InstructionID::INSN_FMOV}
        };

        const unsigned short int encoding = (M << 8) | (S << 7) | (ftype << 5) | imm5;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 64 >> (ftype - (ftype != 0b00 ? 1 : -1))));
        switch (ftype ^ 0b10)
        {
          case 0b01:
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<std::uint16_t, 16>>(imm8, disxx::disasm::operand::Immediate<std::uint16_t, 16>::Option::OPT_VFPEXPANDIMM));
            return std::make_pair(it->second, std::move(this->m_Operands));

          case 0b10:
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<std::uint32_t, 32>>(imm8, disxx::disasm::operand::Immediate<std::uint32_t, 32>::Option::OPT_VFPEXPANDIMM));
            return std::make_pair(it->second, std::move(this->m_Operands));

          case 0b11:
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<std::uint64_t, 64>>(imm8, disxx::disasm::operand::Immediate<std::uint64_t, 64>::Option::OPT_VFPEXPANDIMM));
            return std::make_pair(it->second, std::move(this->m_Operands));

          default:
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            // Never reahced
            return std::make_pair(it->second, std::move(this->m_Operands));
        }
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::FloatingPointImmediate */
