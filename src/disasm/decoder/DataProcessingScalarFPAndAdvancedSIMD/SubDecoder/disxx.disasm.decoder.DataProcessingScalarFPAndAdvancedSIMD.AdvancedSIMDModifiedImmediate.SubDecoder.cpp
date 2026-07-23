module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <vector>

module disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDModifiedImmediate.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.operand.Shift;
import disxx.disasm.utility.bits;
import disxx.disasm.InstructionID;

namespace disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDModifiedImmediate
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
        // +-+-+--+----------+-+-+-+-----+--+-+-+-+-+-+-+--+
        // |0|Q|op|0111100000|a|b|c|cmode|o2|1|d|e|f|g|h|Rd|
        // +-+-+--+----------+-+-+-+-----+--+-+-+-+-+-+-+--+

        unsigned short int Q, op, a, b, c, cmode, o2, d, e, f, g, h, Rd;
        Q = bits::extract<unsigned short int, std::uint32_t, 30, 30>(this->m_Insn);
        op = bits::extract<unsigned short int, std::uint32_t, 29, 29>(this->m_Insn);
        a = bits::extract<unsigned short int, std::uint32_t, 18, 18>(this->m_Insn);
        b = bits::extract<unsigned short int, std::uint32_t, 17, 17>(this->m_Insn);
        c = bits::extract<unsigned short int, std::uint32_t, 16, 16>(this->m_Insn);
        cmode = bits::extract<unsigned short int, std::uint32_t, 12, 15>(this->m_Insn);
        o2 = bits::extract<unsigned short int, std::uint32_t, 11, 11>(this->m_Insn);
        d = bits::extract<unsigned short int, std::uint32_t, 9, 9>(this->m_Insn);
        e = bits::extract<unsigned short int, std::uint32_t, 8, 8>(this->m_Insn);
        f = bits::extract<unsigned short int, std::uint32_t, 7, 7>(this->m_Insn);
        g = bits::extract<unsigned short int, std::uint32_t, 6, 6>(this->m_Insn);
        h = bits::extract<unsigned short int, std::uint32_t, 5, 5>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000000, InstructionID::INSN_MOVI},
            {0b0000010, InstructionID::INSN_ORR},
            {0b0010000, InstructionID::INSN_MOVI},
            {0b0010010, InstructionID::INSN_ORR},
            {0b0011000, InstructionID::INSN_MOVI},
            {0b0011100, InstructionID::INSN_MOVI},
            {0b0011110, InstructionID::INSN_FMOV},
            {0b0011111, InstructionID::INSN_FMOV},
            {0b0100000, InstructionID::INSN_MVNI},
            {0b0100010, InstructionID::INSN_BIC},
            {0b0110000, InstructionID::INSN_MVNI},
            {0b0110010, InstructionID::INSN_BIC},
            {0b0111000, InstructionID::INSN_MVNI},
            {0b0111100, InstructionID::INSN_MOVI},
            {0b1111100, InstructionID::INSN_MOVI},
            {0b1111110, InstructionID::INSN_FMOV}
        };

        unsigned short int encoding = (Q << 6) | (op << 5) | (cmode << 1) | o2;
        if ((cmode >> 3) == 0b0)
            encoding &= ~(0b11 << 2);
        else if ((cmode >> 2) == 0b10)
            encoding &= ~(0b1 << 2);
        else if ((cmode >> 1) == 0b110)
            encoding &= ~(0b1 << 1);
        const auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        const auto imm8{(a << 7) | (b << 6) | (c << 5) | (d << 4) | (e << 3) | (f << 2) | (g << 1) | h};
        if (cmode == 0b1111 && o2 == 0b0)
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rd
				)
			);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetVectorArrangementSpecifier
			(
				disxx::disasm::operand::VectorArrangementSpecifier
				{
					static_cast<unsigned short int>
					(
						0b100
							| ((Q == 0b1 && op == 0b1) << 1)
							| Q
					)
				}
			);
		
			const auto imm{bits::AdvSIMDExpandImm(op, cmode, imm8)};
			if (!imm) [[unlikely]]
				return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned long long int, 64>>(*imm));

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        // fmov Half-precision
        else if (cmode == 0b1111 && o2 == 0b1)
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rd
				)
			);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetVectorArrangementSpecifier
			(
				disxx::disasm::operand::VectorArrangementSpecifier
				{
					static_cast<unsigned short int>
					(
						0b010
							| Q
					)
				}
			);

            std::uint16_t imm16{0};
            imm16 |= (imm8 >> 7) << 15;
            imm16 |= ~bits::extract<unsigned short int, std::uint16_t, 6, 6>(imm8) << 14;
            imm16 |= bits::Replicate<unsigned short int, unsigned short int, 1>(bits::extract<unsigned short int, unsigned short int, 6, 6>(imm8), 2) << 13;
            imm16 |= bits::extract<unsigned short int, unsigned short int, 0, 5>(imm8) << 6;
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Immediate<unsigned long long int, 64>>
                (
                    bits::Replicate<unsigned long long int, unsigned short int, 16>
                    (
                        imm16,
                        64 << Q
                    )
                )
            );

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (cmode == 0b0001 || cmode == 0b1001)
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rd
				)
			);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())->SetVectorArrangementSpecifier
			(
				disxx::disasm::operand::VectorArrangementSpecifier
				{
					static_cast<unsigned short int>
					(
						((0b1 + (cmode == 0b0001)) << 1)
							| Q
					)
				}
			);
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 8>>(imm8));

            if (cmode == 0b0001)
            {
                this->m_Operands.emplace_back
                (
                    std::make_unique<disxx::disasm::operand::Shift>
                    (
                        0b000,
                        bits::extract<unsigned short int, unsigned short int, 1, 2>(cmode) << 3
                    )
                );
            }
            else
            {
                this->m_Operands.emplace_back
                (
                    std::make_unique<disxx::disasm::operand::Shift>
                    (
                        0b000,
                        bits::extract<unsigned short int, unsigned short int, 1, 1>(cmode) << 3
                    )
                );
            }

            return std::make_pair(it->second, std::move(this->m_Operands));

        }
        else if (op == 0b1 && (cmode == 0b0000 || cmode == 0b1000 || cmode == 0b1100))
        {
            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rd
				)
			);
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 8>>(imm8));

            switch (cmode)
            {
              case 0b1000:
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())->SetVectorArrangementSpecifier
				(
					disxx::disasm::operand::VectorArrangementSpecifier
					{
						static_cast<unsigned short int>
						(
							0b010
								| Q
						)
					}
				);
                this->m_Operands.emplace_back
                (
                    std::make_unique<disxx::disasm::operand::Shift>
                    (
                        0b000,
                        bits::extract<unsigned short int, unsigned short int, 1, 1>(cmode) << 3
                    )
                );

                return std::make_pair(it->second, std::move(this->m_Operands));

              case 0b0000:
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())->SetVectorArrangementSpecifier
				(
					disxx::disasm::operand::VectorArrangementSpecifier
					{
						static_cast<unsigned short int>
						(
							0b100
								| Q
						)
					}
				);
                this->m_Operands.emplace_back
                (
                    std::make_unique<disxx::disasm::operand::Shift>
                    (
                        0b000,
                        bits::extract<unsigned short int, unsigned short int, 1, 2>(cmode) << 3
                    )
                );

                return std::make_pair(it->second, std::move(this->m_Operands));

              default:
                static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())->SetVectorArrangementSpecifier
				(
					disxx::disasm::operand::VectorArrangementSpecifier
					{
						static_cast<unsigned short int>
						(
							0b100
								| Q
						)
					}
				);
                this->m_Operands.emplace_back
                (
                    std::make_unique<disxx::disasm::operand::Shift>
                    (
                        0b100,
                        cmode & 1 ? 16 : 8
                    )
                );

                return std::make_pair(it->second, std::move(this->m_Operands));
            }
        }
            
        if (op == 0b1 && cmode == 0b1110)
        {
            auto imm64{0ULL};
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(a, 8) << 56;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(b, 8) << 48;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(c, 8) << 40;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(d, 8) << 32;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(e, 8) << 24;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(f, 8) << 16;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(g, 8) << 8;
            imm64 |= bits::Replicate<std::uint64_t, unsigned short int, 1>(h, 8);
    
            if (Q == 0b0)
            {
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						disxx::disasm::operand::Register::Type::TYPE_D,
						Rd
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned long long int, 64>>(imm64));

                return std::make_pair(it->second, std::move(this->m_Operands));
            }

            this->m_Operands.emplace_back
			(
				std::make_unique<disxx::disasm::operand::Register>
				(
					disxx::disasm::operand::Register::Type::TYPE_V,
					Rd
				)
			);
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.rbegin()->get())
				->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{0b111});
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned long long int, 64>>(imm64));

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				disxx::disasm::operand::Register::Type::TYPE_V,
				Rd
			)
		);
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 8>>(imm8));

        if (op == 0b0 && cmode == 0b1110)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())
				->SetVectorArrangementSpecifier(disxx::disasm::operand::VectorArrangementSpecifier{Q});
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Shift>(0b000, 0));

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (op == 0 && cmode == 0b1000)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())->SetVectorArrangementSpecifier
			(
				disxx::disasm::operand::VectorArrangementSpecifier
				{
					static_cast<unsigned short int>
					(
						0b010
							| ~Q
					)
				}
			);
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Shift>
                (
                    0b000,
                    bits::extract<unsigned short int, unsigned short int, 1, 1>(cmode) << 3
                )
            );

            return std::make_pair(it->second, std::move(this->m_Operands));
        }
        else if (op == 0b0 && cmode == 0b0000)
        {
            static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())->SetVectorArrangementSpecifier
			(
				disxx::disasm::operand::VectorArrangementSpecifier
				{
					static_cast<unsigned short int>
					(
						0b100
							| ~Q
					)
				}
			);
            this->m_Operands.emplace_back
            (
                std::make_unique<disxx::disasm::operand::Shift>
                (
                    0b000,
                    bits::extract<unsigned short int, unsigned short int, 1, 2>(cmode) << 3
                )
            );

            return std::make_pair(it->second, std::move(this->m_Operands));
        }

        static_cast<disxx::disasm::operand::Register *>(this->m_Operands.begin()->get())->SetVectorArrangementSpecifier
		(
			disxx::disasm::operand::VectorArrangementSpecifier
			{
				static_cast<unsigned short int>
				(
					0b100
						| ~Q
				)
			}
		);
        this->m_Operands.emplace_back
        (
			std::make_unique<disxx::disasm::operand::Shift>
            (
                0b100,
                cmode & 1 ? 16 : 8
            )
        );

        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingScalarFPAndAdvancedSIMD::AdvancedSIMDModifiedImmediate */
