module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <format>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDVectorXIndexedElement.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDVectorXIndexedElement
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
        // +-+-+-+-----+----+-+-+--+------+-+-+--+--+
        // |0|Q|U|01111|size|L|M|Rm|opcode|H|0|Rn|Rd|
        // +-+-+-+-----+----+-+-+--+------+-+-+--+--+

		// Unable to compile this code due to libstdc++/115939 STL bug
		return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

		/*
        unsigned short int Q, U, size, L, M, Rm, opcode, H, Rn, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        U = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        size = bits::extract<unsigned short int, std::uint32_t, 22, 23>(this->m_Insn);
        L = bits::extract<unsigned short int, std::uint32_t, 21, 21>(this->m_Insn);
        M = bits::extract<unsigned short int, std::uint32_t, 20, 20>(this->m_Insn);
        Rm = bits::extract<unsigned short int, std::uint32_t, 16, 19>(this->m_Insn);
        opcode = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        H = bits::extract<unsigned short int, std::uint32_t, 11, 11>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        const auto getArrangementSpecifiers
        {
            [this, size, Q, opcode](bool isSingle = false) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError>
            {
                if (size == 0b00 || size == 0b11) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                // fcmla insn has reserved arrangement specifier if size:Q == 0b100
                else if (opcode == 0b0001 && isSingle && size == 0b10 && Q == 0b0) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

                return std::make_pair<std::string_view, std::string_view>
                (
                    disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q),
                    isSingle
                        ? disxx::disasm::operand::Register::GetArrangementSpecifier(size, Q)
                        : size == 0b1 ? "4s" : "2d"
                );
            }
        };

        const auto getArrangementSpecifiersDotProd
        {
            [Q](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError>
            {
                return std::make_pair<std::string_view, std::string_view>
                (
                    Q == 0b1 ? "4s" : "2s",
                    Q == 0b1 ? "16b" : "8b"
                );
            }
        };

        const auto getArrangementSpecifiersHalfPrecision
        {
            [Q](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError>
            {
                return std::make_pair<std::string_view, std::string_view>
                (
                    Q == 0b1 ? "8h" : "4h",
                    Q == 0b1 ? "8h" : "4h"
                );
            }
        };

        const auto getArrangementSpecifiersSingleAndDoublePrecisions
        {
            [this, Q, size](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError>
            {
                if (Q == 0b0 && (size & 0b01) == 0b1) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                
                if (Q == 0b0 && size == 0b0)
                    return std::make_pair<std::string_view, std::string_view>("2s", "2s");
                else if (Q == 0b1 && size == 0b0)
                    return std::make_pair<std::string_view, std::string_view>("4s", "4s");
                return std::make_pair<std::string_view, std::string_view>("2d", "2d");
            }
        };

        const auto getArrangementSpecifiersFHMAndBF16
        {
            [Q](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError>
            {
                return std::make_pair<std::string_view, std::string_view>
                (
                    Q == 0b1 ? "4s" : "2s",
                    Q == 0b1 ? "4h" : "8h"
                );
            }
        };

        const auto getArrangementSpecifiersFP8DOT2
        {
            [Q](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError>
            {
                return std::make_pair<std::string_view, std::string_view>
                (
                    Q == 0b1 ? "8h" : "4h",
                    Q == 0b1 ? "16b" : "8b"
                );
            }
        };

        std::unordered_map
        <
            unsigned short int,
            std::pair
            <
                InstructionID,
                std::function
                <
                    std::expected
                    <
                        std::pair
                        <
                            std::string_view,
                            std::string_view
                        >,
                        disxx::utility::error::DisassemblyError
                    >(void)
                >
            >
        > insnTable = {
            {0b00010, {Q == 0b1 ? InstructionID::INSN_SMLAL2 : InstructionID::INSN_SMLAL, getArrangementSpecifiers}},
            {0b00011, {Q == 0b1 ? InstructionID::INSN_SQDMLAL2 : InstructionID::INSN_SQDMLAL, getArrangementSpecifiers}},
            {0b00110, {Q == 0b1 ? InstructionID::INSN_SMLSL2 : InstructionID::INSN_SMLSL, getArrangementSpecifiers}},
            {0b00111, {Q == 0b1 ? InstructionID::INSN_SQDMLSL2 : InstructionID::INSN_SQDMLSL, getArrangementSpecifiers}},
            {0b01000, {InstructionID::INSN_MUL, std::bind(getArrangementSpecifiers, true)}},
            {0b01010, {Q == 0b1 ? InstructionID::INSN_SMULL2 : InstructionID::INSN_SMULL, getArrangementSpecifiers}},
            {0b01011, {Q == 0b1 ? InstructionID::INSN_SQDMULL2 : InstructionID::INSN_SQDMULL, getArrangementSpecifiers}},
            {0b01100, {InstructionID::INSN_SQDMULH, std::bind(getArrangementSpecifiers, true)}},
            {0b01101, {InstructionID::INSN_SQDRMULH, std::bind(getArrangementSpecifiers, true)}},
            {0b01110, {InstructionID::INSN_SDOT, getArrangementSpecifiersDotProd}},
            {0b10001, {InstructionID::INSN_FCMLA, std::bind(getArrangementSpecifiers, true)}},    // rotate
            {0b10000, {InstructionID::INSN_MLA, std::bind(getArrangementSpecifiers, true)}},
            {0b10010, {Q == 0b1 ? InstructionID::INSN_UMLAL2 : InstructionID::INSN_UMLAL, getArrangementSpecifiers}},
            {0b10100, {InstructionID::INSN_MLS, std::bind(getArrangementSpecifiers, true)}},
            {0b10110, {Q == 0b1 ? InstructionID::INSN_UMLSL2 : InstructionID::INSN_UMLSL, getArrangementSpecifiers}},
            {0b11010, {Q == 0b1 ? InstructionID::INSN_UMULL2 : InstructionID::INSN_UMULL, getArrangementSpecifiers}},
            {0b11101, {InstructionID::INSN_SQRDMLAH, std::bind(getArrangementSpecifiers, true)}},
            {0b11110, {InstructionID::INSN_UDOT, getArrangementSpecifiersDotProd}},
            {0b11111, {InstructionID::INSN_SQRDMLSH, std::bind(getArrangementSpecifiers, true)}}
        }, insnTableWithSize = {
            {0b00000000, {InstructionID::INSN_FDOT, getArrangementSpecifiersDotProd}},
            {0b00000001, {InstructionID::INSN_FMLA, getArrangementSpecifiersHalfPrecision}},
            {0b00000101, {InstructionID::INSN_FMLS, getArrangementSpecifiersHalfPrecision}},
            {0b00001001, {InstructionID::INSN_FMUL, getArrangementSpecifiersHalfPrecision}},
            {0b00001111, {InstructionID::INSN_SUDOT, getArrangementSpecifiersDotProd}},
            {0b00010000, {InstructionID::INSN_FDOT, getArrangementSpecifiersFP8DOT2}},
            {0b00011111, {InstructionID::INSN_BFDOT, getArrangementSpecifiersFHMAndBF16}},
            {0b00100001, {InstructionID::INSN_FMLA, getArrangementSpecifiersSingleAndDoublePrecisions}},
            {0b00100101, {InstructionID::INSN_FMLS, getArrangementSpecifiersSingleAndDoublePrecisions}},
            {0b00101001, {InstructionID::INSN_FMUL, getArrangementSpecifiersSingleAndDoublePrecisions}},
            {0b00100000, {InstructionID::INSN_FMLAL, getArrangementSpecifiersFHMAndBF16}},
            {0b00100100, {InstructionID::INSN_FMLSL, getArrangementSpecifiersFHMAndBF16}},
            {0b00101111, {InstructionID::INSN_USDOT, getArrangementSpecifiersDotProd}},
            {0b00111111, {Q == 0b1 ? InstructionID::INSN_BFMLALT : InstructionID::INSN_BFMLALB, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("4s", "8h"); }}},
            {0b01001001, {InstructionID::INSN_FMULX, getArrangementSpecifiersHalfPrecision}},
            {0b01101001, {InstructionID::INSN_FMULX, getArrangementSpecifiersSingleAndDoublePrecisions}},
            {0b01101000, {InstructionID::INSN_FMLAL2, getArrangementSpecifiersFHMAndBF16}},
            {0b01101100, {InstructionID::INSN_FMLSL2, getArrangementSpecifiersFHMAndBF16}},
            {0b00110000, {InstructionID::INSN_FMLALB, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("8h", "16b"); }}},
            {0b01001000, {InstructionID::INSN_FMLALLBB, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("4s", "16b"); }}},
            {0b01011000, {InstructionID::INSN_FMLALLBT, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("4s", "16b"); }}},
            {0b10110000, {InstructionID::INSN_FMLALT, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("8h", "16b"); }}},
            {0b11001000, {InstructionID::INSN_FMLALLTB, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("4s", "16b"); }}},
            {0b11011000, {InstructionID::INSN_FMLALLTT, [](void) -> std::expected<std::pair<std::string_view, std::string_view>, disxx::utility::error::DisassemblyError> { return std::make_pair("4s", "16b"); }}}
        };

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rd, 128 + 'V'));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rn, 128 + 'V'));
    
        unsigned short int encoding = (U << 4) | opcode;
        if (opcode >> 3 == 0b0 && opcode << 3 == 0b1)
            encoding &= ~(0b11 << 2);
        auto it{insnTable.find(encoding)};
        if (it == insnTable.end())
        {
            encoding = (U << 6) | (size << 4) | opcode;
            if ((U == 0b1 && (((size == 0b00 || size == 0b01) && opcode == 0b1000))) || (size == 0b11 && opcode == 0b0000))
                encoding |= Q << 7;
            else if (U == 0b1 && opcode == 0b1001)
                encoding &= ~(0b1 << 4);
            else if (U == 0b0 && (opcode == 0b0001 || opcode == 0b0101 || opcode == 0b1001))
                encoding &= ~(0b1 << 4);
            it = insnTableWithSize.find(encoding);
            if (it == insnTableWithSize.end()) [[unlikely]]
                return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
        }
        const auto &[insn, specifiersFunc]{it->second};
        const auto &result{specifiersFunc()};
        if (!result) [[unlikely]]
            return std::unexpected{result.error()};
        const auto [Tb, Ta]{result.value()};
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(0).get())->SetArrangementSpecifier(Tb.data());
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.at(1).get())->SetArrangementSpecifier(Ta.data());
        if (U == 0b0 && size == 0b00 && opcode == 0b0000)
        {
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Register>
                (
                    disxx::disasm::operand::Register::Type::TYPE_NEON,
                    (M << 4) | Rm,
                    128 + 'V'
                )
            );
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "4b[{}]",
                    (H << 1) | L
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (opcode == 0b0001 || opcode == 0b0101 || opcode == 0b1001)
        {
            if (size == 0b00)
            {
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
                (
                    std::format
                    (
                        "h[{}]",
                        (H << 2) | (L << 1) | M
                    )
                );
            }
            else
            {
                this->m_Operands.emplace_back
                   (
                       std::make_unique<disxx::disasm::operand::Register>
                       (
                           disxx::disasm::operand::Register::Type::TYPE_NEON,
                           (M << 4) | Rm,
                           128 + 'V'
                       )
                   );
                const auto sz{size & 0b01};
                if (sz == 0b1 && L == 0b1) [[unlikely]]
                    return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
                (
                    std::format
                    (
                        "{:c}[{}]",
                        sz == 0b1
                            ? 'd'
                            : 's',
                        sz == 0b1 && L == 0b0
                            ? H
                            : (H << 1) | L
                    )
                );
            }
            
            if ((opcode & ~(0b11 << 1)) == 0b0001)
            {
                this->m_Operands.emplace_back
                (
                    std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 2>>
                    (
                        bits::extract<unsigned short int, unsigned short int, 13, 14>(this->m_Insn)
                            * 90
                    )
                );
            }

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (U == 0b0 && size == 0b00 && opcode == 0b1111)
        {
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Register>
                (
                    disxx::disasm::operand::Register::Type::TYPE_NEON,
                    (M << 4) | Rm,
                    128 + 'V'
                )
            );
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "2h[{}]",
                    (H << 1) | L
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (U == 0b0 && size == 0b10 && opcode == 0b1111)
        {
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Register>
                (
                    disxx::disasm::operand::Register::Type::TYPE_NEON,
                    (M << 4) | Rm,
                    128 + 'V'
                )
            );
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "4b[{}]",
                    (H << 1) | L
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (U == 0b0 && size == 0b11 && opcode == 0b1111)
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "h[{}]",
                    (H << 2) | (L << 1) | M
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if ((U == 0b0 && (opcode == 0b0000 || opcode == 0b0100)) || (U == 0b1 && (opcode == 0b1000 || opcode == 0b1100)))
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "h[{}]",
                    (H << 2) | (L << 1) | M
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (U == 0b1 && (opcode == 0b0000 || opcode == 0b1000))
        {
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rm & 0b111, 128 + 'V'));
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "b[{}]",
                    (H << 3) | (L << 2) | (M << 1) | (Rm >> 3)
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
        else if (opcode == 0b1110)
        {
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Register>
                (
                    disxx::disasm::operand::Register::Type::TYPE_NEON,
                    (M << 4) | Rm,
                    128 + 'V'
                )
            );
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
            (
                std::format
                (
                    "4b[{}]",
                    (H << 1) | L
                )
            );

            return std::make_pair(insn, std::move(this->m_Operands));
        }
         
        this->m_Operands.emplace_back
        (
            std::make_unique<disxx::disasm::operand::Register>
            (
                disxx::disasm::operand::Register::Type::TYPE_NEON,
                (size == 0b01 ? 0 : M << 4) | Rm,
                128 + 'V'
            )
        );
        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetArrangementSpecifier
        (
            std::format
            (
                "{:c}[{}]",
                size == 0b01
                    ? 'h'
                    : 's',
                size == 0b01
                       ? (H << 2) | (L << 1) | M
                       : (H << 1) | L
            )
        );

        if ((opcode & ~(0b11 << 1)) == 0b0001)
        {
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 2>>
                (
                    bits::extract<unsigned short int, unsigned short int, 13, 14>(this->m_Insn)
                        * 90
                )
            );
        }

        return std::make_pair(insn, std::move(this->m_Operands));
		*/
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDVectorXIndexedElement */
