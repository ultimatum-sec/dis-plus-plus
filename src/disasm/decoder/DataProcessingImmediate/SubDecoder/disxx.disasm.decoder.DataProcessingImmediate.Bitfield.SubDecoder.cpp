module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>

module disxx.disasm.decoder.DataProcessingImmediate.Bitfield.SubDecoder;

import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Immediate;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::DataProcessingImmediate::Bitfield
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

	DisassemblyResult SubDecoder::Decode(void) const noexcept
	{
        // +--+---+------+-+----+----+--+--+
        // |sf|opc|100110|N|immr|imms|Rn|Rd|
        // +--+---+------+-+----+----+--+--+

        unsigned short int sf, opc, N, immr, imms, Rn, Rd;
        sf = bits::extract<unsigned short int, std::uint32_t, 31, 31>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 29, 30>(this->m_Insn);
        N = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        immr = bits::extract<unsigned short int, std::uint32_t, 16, 21>(this->m_Insn);
        imms = bits::extract<unsigned short int, std::uint32_t, 10, 15>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rd = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);

        std::unordered_map<unsigned short int, InstructionID> insnTable = {
            {0b0000, InstructionID::INSN_SBFM},
            {0b0010, InstructionID::INSN_BFM},
            {0b0100, InstructionID::INSN_UBFM},
            {0b1001, InstructionID::INSN_SBFM},
            {0b1011, InstructionID::INSN_BFM},
            {0b1101, InstructionID::INSN_UBFM}
        };

        unsigned short int encoding = (sf << 3) | (opc << 1) | N;
        auto it{insnTable.find(encoding)};
        if (it == insnTable.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};

        switch (it->second)
        {
          case InstructionID::INSN_SBFM:
          {
            if (imms == 0b011111 || imms == 0b111111)
            {
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						(imms >> 5)
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						(imms >> 5)
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                
                return std::make_pair(InstructionID::INSN_ASR, std::move(this->m_Operands));
            }
            else if (imms < immr)
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));

                return std::make_pair(InstructionID::INSN_SBFIZ, std::move(this->m_Operands));
            }
            else if (bits::BFXPreffered(sf, opc >> 1, imms, immr))
            {
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
				this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));

                return std::make_pair(InstructionID::INSN_SBFX, std::move(this->m_Operands));
            }
            else if (immr == 0b000000)
            {
                static const std::unordered_map<unsigned short int, InstructionID> aliasTable = {
                    {0b000111, InstructionID::INSN_SXTB},
                    {0b001111, InstructionID::INSN_SXTH},
                    {0b011111, InstructionID::INSN_SXTW}
                };

                const auto aliasIt{aliasTable.find(imms)};
                if (aliasIt == aliasTable.end()) [[unlikely]]
                    break;
                
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);

                return std::make_pair(aliasIt->second, std::move(this->m_Operands));
            }

            break;
          }

          case InstructionID::INSN_BFM:
          {
            if (imms < immr)
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
				if (Rn != 0b11111)
				{
                	this->m_Operands.emplace_back
					(
						std::make_unique<disxx::disasm::operand::Register>
						(
							sf
								? disxx::disasm::operand::Register::Type::TYPE_X
								: disxx::disasm::operand::Register::Type::TYPE_W,
							Rn
						)
					);
				}
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));
               
                return std::make_pair(Rn == 0b11111 ? InstructionID::INSN_BFC : InstructionID::INSN_BFI, std::move(this->m_Operands));
            }
            else if (imms >= immr)
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));
               
                return std::make_pair(InstructionID::INSN_BFXIL, std::move(this->m_Operands));
            }

            break;
          }

          case InstructionID::INSN_UBFM:
          {
            if ((imms != 0b011111 || imms != 0b11111) && imms + 1 == immr)
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						(imms >> 5)
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						(imms >> 5)
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));

                return std::make_pair(InstructionID::INSN_LSL, std::move(this->m_Operands));
            }
            else if (imms == 0b011111 || imms == 0b111111)
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						(imms >> 5)
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						(imms >> 5)
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));

                return std::make_pair(InstructionID::INSN_LSR, std::move(this->m_Operands));
            }
            else if (imms < immr)
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));
              
                return std::make_pair(InstructionID::INSN_UBFIZ, std::move(this->m_Operands));
            }
            else if (bits::BFXPreffered(sf, opc >> 1, imms, immr))
            {
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
                this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));
              
                return std::make_pair(InstructionID::INSN_UBFX, std::move(this->m_Operands));
            }
            else if (immr == 0b000000 && (imms == 0b000111 || imms == 0b001111))
            {
                static const std::unordered_map<unsigned short int, InstructionID> aliasTable = {
                    {0b000111, InstructionID::INSN_UXTB},
                    {0b001111, InstructionID::INSN_UXTH}
                };

                const auto aliasIt{aliasTable.find(imms)};
                if (aliasIt == aliasTable.end()) [[unlikely]]
                    break;
 
				this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rd
					)
				);
                this->m_Operands.emplace_back
				(
					std::make_unique<disxx::disasm::operand::Register>
					(
						sf
							? disxx::disasm::operand::Register::Type::TYPE_X
							: disxx::disasm::operand::Register::Type::TYPE_W,
						Rn
					)
				);

                return std::make_pair(aliasIt->second, std::move(this->m_Operands));
            }

            break;
          }

          default:
            break;
        }
        
		this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sf
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rd
			)
		);
        this->m_Operands.emplace_back
		(
			std::make_unique<disxx::disasm::operand::Register>
			(
				sf
					? disxx::disasm::operand::Register::Type::TYPE_X
					: disxx::disasm::operand::Register::Type::TYPE_W,
				Rn
			)
		);
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(immr));
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Immediate<unsigned short int, 6>>(imms));
    
        return std::make_pair(it->second, std::move(this->m_Operands));
	}
} /* disxx::disasm::decoder::DataProcessingImmediate::Bitfield */
