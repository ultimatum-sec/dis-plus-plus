module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>
#include <array>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDThreeDifferent.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeDifferent
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
        // +-+-+-+-----+----+-+--+------+--+--+--+
        // |0|Q|U|01110|size|1|Rm|opcode|00|Rn|Rd|
        // +-+-+-+-----+----+-+--+------+--+--+--+

        unsigned short int Q, U, size, Rm, opcode, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const auto addSingleTaFirst{[](std::string Ta, std::string Tb) -> std::array<std::string, 3> { return {Ta, Tb, Tb}; }};
        const auto addDoubleTaFirst{[](std::string Ta, std::string Tb) -> std::array<std::string, 3> { return {Ta, Ta, Tb}; }};
        const auto addSingleTbFirst{[](std::string Ta, std::string Tb) -> std::array<std::string, 3> { return {Tb, Ta, Ta}; }};

        std::unordered_map<unsigned short int, std::pair<InstructionID, std::function<std::array<std::string, 3>(std::string, std::string)>>> insnTable = {
            {0b000000, {InstructionID::INSN_SADDL, addSingleTaFirst}},
            {0b100000, {InstructionID::INSN_SADDL2, addSingleTaFirst}},
            {0b000001, {InstructionID::INSN_SAADW, addDoubleTaFirst}},
            {0b100001, {InstructionID::INSN_SAADW2, addDoubleTaFirst}},
            {0b000010, {InstructionID::INSN_SSUBL, addSingleTaFirst}},
            {0b000010, {InstructionID::INSN_SSUBL2, addSingleTaFirst}},
            {0b000011, {InstructionID::INSN_SSUBW, addDoubleTaFirst}},
            {0b100011, {InstructionID::INSN_SSUBW2, addDoubleTaFirst}},
            {0b000100, {InstructionID::INSN_ADDHN, addSingleTbFirst}},
            {0b100100, {InstructionID::INSN_ADDHN2, addSingleTbFirst}},
            {0b000101, {InstructionID::INSN_SABAL, addSingleTaFirst}},
            {0b100101, {InstructionID::INSN_SABAL2, addSingleTaFirst}},
            {0b000110, {InstructionID::INSN_SUBHN, addSingleTbFirst}},
            {0b100110, {InstructionID::INSN_SUBHN2, addSingleTbFirst}},
            {0b000111, {InstructionID::INSN_SABDL, addSingleTaFirst}},
            {0b100111, {InstructionID::INSN_SABDL2, addSingleTaFirst}},
            {0b001000, {InstructionID::INSN_SMLAL, addSingleTaFirst}},
            {0b101000, {InstructionID::INSN_SMLAL2, addSingleTaFirst}},
            {0b001001, {InstructionID::INSN_SQDMLAL, addSingleTaFirst}},
            {0b101001, {InstructionID::INSN_SQDMLAL2, addSingleTaFirst}},
            {0b001010, {InstructionID::INSN_SMLSL, addSingleTaFirst}},
            {0b101010, {InstructionID::INSN_SMLSL2, addSingleTaFirst}},
            {0b001011, {InstructionID::INSN_SQDMLSL, addSingleTaFirst}},
            {0b101011, {InstructionID::INSN_SQDMLSL2, addSingleTaFirst}},
            {0b001100, {InstructionID::INSN_SMULL, addSingleTaFirst}},
            {0b101100, {InstructionID::INSN_SMULL2, addSingleTaFirst}},
            {0b001101, {InstructionID::INSN_SQDMULL, addSingleTaFirst}},
            {0b101101, {InstructionID::INSN_SQDMULL2, addSingleTaFirst}},
            {0b001110, {InstructionID::INSN_PMULL, addSingleTaFirst}},
            {0b101110, {InstructionID::INSN_PMULL2, addSingleTaFirst}},
            {0b010000, {InstructionID::INSN_UADDL, addSingleTaFirst}},
            {0b110000, {InstructionID::INSN_UADDL2, addSingleTaFirst}},
            {0b010001, {InstructionID::INSN_UAADW, addDoubleTaFirst}},
            {0b110001, {InstructionID::INSN_UAADW2, addDoubleTaFirst}},
            {0b010010, {InstructionID::INSN_USUBL, addSingleTaFirst}},
            {0b110010, {InstructionID::INSN_USUBL2, addSingleTaFirst}},
            {0b010011, {InstructionID::INSN_USUBW, addDoubleTaFirst}},
            {0b110011, {InstructionID::INSN_USUBW2, addDoubleTaFirst}},
            {0b010100, {InstructionID::INSN_RADDHN, addSingleTbFirst}},
            {0b110100, {InstructionID::INSN_RADDHN2, addSingleTbFirst}},
            {0b010101, {InstructionID::INSN_UABAL, addSingleTaFirst}},
            {0b110101, {InstructionID::INSN_UABAL2, addSingleTaFirst}},
            {0b010110, {InstructionID::INSN_RSUBHN, addSingleTbFirst}},
            {0b110110, {InstructionID::INSN_RSUBHN2, addSingleTbFirst}},
            {0b010111, {InstructionID::INSN_UABDL, addSingleTaFirst}},
            {0b110111, {InstructionID::INSN_UABDL2, addSingleTaFirst}},
            {0b011000, {InstructionID::INSN_UMLAL, addSingleTaFirst}},
            {0b111000, {InstructionID::INSN_UMLAL2, addSingleTaFirst}},
            {0b011010, {InstructionID::INSN_UMLSL, addSingleTaFirst}},
            {0b111010, {InstructionID::INSN_UMLSL2, addSingleTaFirst}},
            {0b011100, {InstructionID::INSN_UMULL, addSingleTaFirst}},
            {0b111100, {InstructionID::INSN_UMULL2, addSingleTaFirst}}
        };

        const unsigned short int encoding = (Q << 5) | (U << 4) | opcode;
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end() || (size == 0b11 && opcode != 0b1110)) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[insn, fmtf]{it->second};

        const auto result
        {
            [this, &opcode, &size, &Q] -> std::expected<std::pair<std::string, std::string>, disxx::utility::error::DisassemblyError>
            {
                if (opcode == 0b1001 || opcode == 0b1011 || opcode == 0b1101)
                {
                    if (size == 0b00) [[unlikely]]
                        return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                    
                    return std::make_pair
                    (
                        size == 0b01 ? "4s" : "2d",
                        disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q).data()
                    );
                }
                else if (opcode == 0b1110)
                {
                    if (size == 0b01 || size == 0b10) [[unlikely]]
                        return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                
                    return std::make_pair
                    (
                        size == 0b00 ? "8h" : "1Q",
                        (size == 0b00)
                            ? (Q == 0b0 ? "8b" : "16b")
                            : (Q == 0b0 ? "1d" : "2d")
                    );
                }

                return std::make_pair
                (
                    std::array<const char *, 3>{"8h", "4s", "2d"}.at(size),
                    disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q).data()
                );
            }()
        };

        if (!result) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        const auto &[Ta, Tb]{result.value()};

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
        for (auto i{0}; const auto &spec : fmtf(Ta, Tb))
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(i++).get())->SetArrangementSpecifier(spec);
        return std::make_pair(insn, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDThreeDifferent */
