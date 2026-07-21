module;

#include <unordered_map>
#include <optional>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDShiftByImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDShiftByImmediate
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
		: disxx::disasm::decoder::abstract::SubDecoder{std::forward<SubDecoder &&>(other)}
	{}

	SubDecoder &SubDecoder::operator=(SubDecoder &&other) noexcept
	{
		if (this != &other) [[likely]]
			disxx::disasm::decoder::abstract::SubDecoder::operator=(std::forward<SubDecoder &&>(other));
		return *this;
	}

	std::unique_ptr<disxx::disasm::decoder::abstract::SubDecoder> SubDecoder::Clone(void) const noexcept
	{ return std::make_unique<std::decay_t<decltype(*this)>>(*this); }

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +-+-+-+------+----+----+------+-+--+--+
        // |0|Q|U|011110|immh|immb|opcode|1|Rn|Rd|
        // +-+-+-+------+----+----+------+-+--+--+

        unsigned short int Q, U, immh, immb, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint64_t, 29, 29>(this->m_Insn);
        immh = bits::extract<unsigned short int, std::uint64_t, 19, 22>(this->m_Insn);
        immb = bits::extract<unsigned short int, std::uint64_t, 16, 18>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint64_t, 11, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint64_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint64_t, 0, 4>(this->m_Insn);

        if (immh == 0b1111) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto getAmountInTheRange0
        {
            [=](signed short int border = -1, bool shouldExtract = false)
				-> std::optional<signed short int>
            {
                const auto index
                {
                    bits::HighestSetBitNZ<short int, 4>
                    (
                        shouldExtract
                            ? bits::extract<unsigned short int, unsigned short int, 0, 2>(immh)
                            : immh
                    )
                };

                if (index == border) [[unlikely]]
                	return std::nullopt;
				return ((immh << 3) | immb) - (8 << index);
            }
        };

        const auto getAmountInTheRange1
        {
            [=](signed short int border = -1, bool shouldExtract = false)
				-> std::optional<signed short int>
            {
                const auto index
                {
                    bits::HighestSetBitNZ<short int, 4>
                    (
                        shouldExtract
                            ? bits::extract<unsigned short int, unsigned short int, 0, 2>(immh)
                            : immh
                    )
                };

                if (index == border) [[unlikely]]
                	return std::nullopt;
				return ((8 << index) * 2) - ((immh << 3) | immb);
            }
        };

        std::unordered_map<unsigned short int, std::pair<InstructionID, std::function<std::optional<signed short int>(void)>>> insnTable = {
            {0b000000, {InstructionID::INSN_SSHR, getAmountInTheRange1}},
            {0b000010, {InstructionID::INSN_SSRA, getAmountInTheRange1}},
            {0b000100, {InstructionID::INSN_SRSRA, getAmountInTheRange1}},
            {0b001010, {InstructionID::INSN_SHL, getAmountInTheRange0}},
            {0b001110, {InstructionID::INSN_SQSHL, getAmountInTheRange0}},
            {0b010000, {Q == 0b1 ? InstructionID::INSN_SHRN2 : InstructionID::INSN_SHRN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b010001, {Q == 0b1 ? InstructionID::INSN_RSHRN2 : InstructionID::INSN_RSHRN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b010010, {Q == 0b1 ? InstructionID::INSN_SQSHRN2 : InstructionID::INSN_SQSHRN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b010011, {Q == 0b1 ? InstructionID::INSN_SQRSHRN2 : InstructionID::INSN_SQRSHRN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b010100, {Q == 0b1 ? InstructionID::INSN_SSHLL2 : InstructionID::INSN_SSHLL, std::bind(getAmountInTheRange0, 3, true)}},        // Alias!
            {0b011100, {InstructionID::INSN_SCVTF, std::bind(getAmountInTheRange1, 0)}},
            {0b011111, {InstructionID::INSN_FCVTZS, std::bind(getAmountInTheRange1, 0)}},
            {0b100000, {InstructionID::INSN_USHR, getAmountInTheRange1}},
            {0b100010, {InstructionID::INSN_USRA, getAmountInTheRange1}},
            {0b100100, {InstructionID::INSN_URSHR, getAmountInTheRange1}},
            {0b100110, {InstructionID::INSN_URSRA, getAmountInTheRange1}},
            {0b110000, {InstructionID::INSN_SRI, getAmountInTheRange1}},
            {0b101010, {InstructionID::INSN_SLI, getAmountInTheRange0}},
            {0b101100, {InstructionID::INSN_SQSHLU, getAmountInTheRange0}},
            {0b101110, {InstructionID::INSN_UQSHL, getAmountInTheRange0}},
            {0b110000, {Q == 0b1 ? InstructionID::INSN_SQSHRUN2 : InstructionID::INSN_SQSHRUN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b110001, {Q == 0b1 ? InstructionID::INSN_SQRSHRUN2 : InstructionID::INSN_SQRSHRUN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b110010, {Q == 0b1 ? InstructionID::INSN_UQSHRN2 : InstructionID::INSN_UQSHRN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b110011, {Q == 0b1 ? InstructionID::INSN_UQRSHRN2 : InstructionID::INSN_UQRSHRN, std::bind(getAmountInTheRange1, 3, true)}},
            {0b110100, {Q == 0b1 ? InstructionID::INSN_USHLL2 : InstructionID::INSN_USHLL, std::bind(getAmountInTheRange0, 3, true)}},        // Alias!
            {0b111100, {InstructionID::INSN_UCVTF, std::bind(getAmountInTheRange1, 0)}},
            {0b111111, {InstructionID::INSN_FCVTZU, std::bind(getAmountInTheRange1, 0)}}
        };

        const unsigned short int encoding = (U << 9) | (immh << 5) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        auto [insn, amountFunc]{it->second};
    
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rd
			)
		);
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rn
			)
		);
        if (const auto amount{amountFunc()}) [[likely]]
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<signed short int, 16>>(*amount));
        else [[unlikely]]
   			return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
 
        auto size{bits::HighestSetBitNZ<unsigned short int, 4>(immh)};
        if (size <= 0b10)
			size >>= 1;
        else if (size == 3)
			size &= 1;
        else
			size = 0b11;

        //if (bits::HighestSetBitNZ<unsigned short int, 4>(immh))
        if (opcode >= 0b10000 && opcode <= 0b10100)
        {
            const disxx::disasm::operand::VectorArrangementSpecifier spec{static_cast<unsigned short int>(((size + 1) << 1) | 0b1)};
            if (opcode == 10100)
            {
                if (immb == 0b000 && bits::BitCount<unsigned short int, 4>(immh) == 1)
				{
                    insn = std::array<InstructionID, 4>
					{
						InstructionID::INSN_SXTL,
						InstructionID::INSN_SXTL2,
						InstructionID::INSN_UXTL,
						InstructionID::INSN_UXTL2
					}[(U << 1) | Q];
				}
                
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())
                    ->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{static_cast<unsigned short int>((size << 1) | Q)});
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetVectorArrangementSpecifier(spec);

                return std::make_pair(insn, std::move(this->m_Operands));
            }
        
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetVectorArrangementSpecifier(spec);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())
                ->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{static_cast<unsigned short int>((size << 1) | Q)});

            return std::make_pair(insn, std::move(this->m_Operands));
        }
    
        const disxx::disasm::operand::VectorArrangementSpecifier spec{static_cast<unsigned short int>((size << 1) | Q)};
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetVectorArrangementSpecifier(spec);
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetVectorArrangementSpecifier(spec);

        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDShiftByImmediate */
