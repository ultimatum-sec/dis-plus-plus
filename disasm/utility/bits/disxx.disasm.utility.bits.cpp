module;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"

#include <unordered_map>
#include <stdexcept>
#include <cstdint>
#include <limits>

module disxx.disasm.utility.bits;

namespace bits
{
	std::uint64_t AdvSIMDExpandImm(unsigned short int op, unsigned short int cmode, unsigned short int imm8) noexcept(false)
	{
		// Maybe it gets imm8 as 16-bit int, but in fact it's an 8-bit integer
		if (imm8 > std::numeric_limits<std::uint8_t>::max()) [[unlikely]]
			throw std::overflow_error{"IntegerOverflowError"};

		std::uint64_t imm64{0};
		switch (extract<unsigned short int, unsigned short int, 1, 3>(cmode))
		{
		  case 0b000:
			imm64 = Replicate<std::uint64_t, std::uint32_t, 32>(imm8, 2);
			break;

		  case 0b001:
			imm64 = Replicate<std::uint64_t, std::uint32_t, 32>(imm8 << 8, 2);
			break;

		  case 0b010:
			imm64 = Replicate<std::uint64_t, std::uint32_t, 32>(imm8 << 16, 2);
			break;

		  case 0b011:
			imm64 = Replicate<std::uint64_t, std::uint32_t, 32>(imm8 << 24, 2);
			break;

		  case 0b100:
			imm64 = Replicate<std::uint64_t, std::uint16_t, 16>(imm8, 4);
			break;

		  case 0b101:
			imm64 = Replicate<std::uint64_t, std::uint16_t, 16>(imm8 << 8, 4);
			break;

		  case 0b110:
			if (cmode & ~0b0)
				imm64 = Replicate<std::uint64_t, std::uint32_t, 32>((imm8 << 16) | UINT16_MAX, 2);
			else
				imm64 = Replicate<std::uint64_t, std::uint32_t, 32>((imm8 << 8) | UINT8_MAX, 2);
			break;

		  case 0b111:
			if ((cmode & ~0b0) == 0b0 && op == 0b0)
				imm64 = Replicate<std::uint64_t, std::uint8_t, 8>(static_cast<std::uint8_t>(imm8), 8);
			else if ((cmode & ~0b0) == 0b0 && op == 0b1)
			{
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 7, 7>(imm8), 8) << 56;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 6, 6>(imm8), 8) << 48;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 5, 5>(imm8), 8) << 40;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 4, 4>(imm8), 8) << 32;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 3, 3>(imm8), 8) << 24;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 2, 2>(imm8), 8) << 16;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 1, 1>(imm8), 8) << 8;
				imm64 |= Replicate<std::uint64_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 0, 0>(imm8), 8);
			}
			else
			{
				std::uint32_t imm32{0};
				imm32 |= (imm8 >> 7) << 31;
				imm32 |= (~extract<std::uint8_t, std::uint8_t, 6, 6>(imm8)) << 30;
				imm32 |= Replicate<std::uint8_t, std::uint8_t, 1>(extract<std::uint8_t, std::uint8_t, 6, 6>(imm8), 5) << 25;
				imm32 |= extract<std::uint8_t, std::uint8_t, 0, 5>(imm8) << 19;

				if (op == 0b1)
					imm64 = imm32 << 29;
				else
					imm64 = Replicate<std::uint64_t, std::uint32_t, 32>(imm32, 2);
			}
			
			break;

		  default:
			break;
		}

		return imm64;
	}

	unsigned short int SysOp(unsigned short int op1, unsigned short int CRn, unsigned short int CRm, unsigned short int op2)
	{
		static std::unordered_map<unsigned short int, unsigned short int> SysOpTable = {
			{0b00001111000000, 1},
			{0b00001111000001, 1},
			{0b00001111000010, 1},
			{0b00001111000011, 1},
			{0b00001111001000, 1},
			{0b00001111001001, 1},
			{0b00001111001010, 1},
			{0b10001111000000, 1},
			{0b10001111000001, 1},
			{0b10001111001010, 1},
			{0b10001111000100, 1},
			{0b10001111000101, 1},
			{0b10001111000110, 1},
			{0b10001111000111, 1},
			{0b11001111000000, 1},
			{0b11001111000001, 1},
			{0b11001111001010, 1},
			{0b00101110010100, 2},
			{0b00101110010101, 2},
			{0b00001110110001, 3},
			{0b00001110110010, 3},
			{0b00001110110011, 3},
			{0b00001110110100, 3},
			{0b00001110110101, 3},
			{0b00001110110110, 3},
			{0b00001111010010, 3},
			{0b00001111010100, 3},
			{0b00001111010110, 3},
			{0b00001111110010, 3},
			{0b00001111110100, 3},
			{0b00001111110110, 3},
			{0b01101110100001, 3},
			{0b01101110100011, 3},
			{0b01101110100100, 3},
			{0b01101111010001, 3},
			{0b01101111010011, 3},
			{0b01101111010101, 3},
			{0b01101111011001, 3},
			{0b01101111100001, 3},
			{0b01101111100011, 3},
			{0b01101111100101, 3},
			{0b01101111101001, 3},
			{0b01101111101011, 3},
			{0b01101111101101, 3},
			{0b01101111110001, 3},
			{0b01101111110011, 3},
			{0b01101111110101, 3},
			{0b10001111110000, 3},
			{0b10001111110111, 3},
			{0b11001111110001, 3},
			{0b11001111110101, 3},
			{0b00001111111001, 3},
			{0b00001111111101, 3},
			{0b00001110001000, 4},
			{0b00001110101000, 4},
			{0b01101110101001, 4},
			{0b00010000001000, 5},
			{0b00010000001001, 5},
			{0b00010000001010, 5},
			{0b00010000001011, 5},
			{0b00010000001101, 5},
			{0b00010000001111, 5},
			{0b00010000010001, 5},
			{0b00010000010011, 5},
			{0b00010000010101, 5},
			{0b00010000010111, 5},
			{0b00010000011000, 5},
			{0b00010000011001, 5},
			{0b00010000011010, 5},
			{0b00010000011011, 5},
			{0b00010000011101, 5},
			{0b00010000011111, 5},
			{0b00010000101001, 5},
			{0b00010000101011, 5},
			{0b00010000101101, 5},
			{0b00010000101111, 5},
			{0b00010000110001, 5},
			{0b00010000110011, 5},
			{0b00010000110101, 5},
			{0b00010000110111, 5},
			{0b00010000111000, 5},
			{0b00010000111001, 5},
			{0b00010000111010, 5},
			{0b00010000111011, 5},
			{0b00010000111101, 5},
			{0b00010000111111, 5},
			{0b00010010001000, 5},
			{0b00010010001001, 5},
			{0b00010010001010, 5},
			{0b00010010001011, 5},
			{0b00010010001101, 5},
			{0b00010010001111, 5},
			{0b00010010010001, 5},
			{0b00010010010011, 5},
			{0b00010010010101, 5},
			{0b00010010010111, 5},
			{0b00010010011000, 5},
			{0b00010010011001, 5},
			{0b00010010011010, 5},
			{0b00010010011011, 5},
			{0b00010010011101, 5},
			{0b00010010011111, 5},
			{0b00010010101001, 5},
			{0b00010010101011, 5},
			{0b00010010101101, 5},
			{0b00010010101111, 5},
			{0b00010010110001, 5},
			{0b00010010110011, 5},
			{0b00010010110101, 5},
			{0b00010010110111, 5},
			{0b00010010111000, 5},
			{0b00010010111001, 5},
			{0b00010010111010, 5},
			{0b00010010111011, 5},
			{0b00010010111101, 5},
			{0b00010010111111, 5},
			{0b10010000000001, 5},
			{0b10010000000010, 5},
			{0b10010000000101, 5},
			{0b10010000000110, 5},
			{0b10010000001000, 5},
			{0b10010000001001, 5},
			{0b10010000001100, 5},
			{0b10010000001101, 5},
			{0b10010000001110, 5},
			{0b10010000010001, 5},
			{0b10010000010101, 5},
			{0b10010000011000, 5},
			{0b10010000011001, 5},
			{0b10010000011100, 5},
			{0b10010000011101, 5},
			{0b10010000011110, 5},
			{0b10010000100000, 5},
			{0b10010000100001, 5},
			{0b10010000100010, 5},
			{0b10010000100011, 5},
			{0b10010000100100, 5},
			{0b10010000100101, 5},
			{0b10010000100110, 5},
			{0b10010000100111, 5},
			{0b10010000101001, 5},
			{0b10010000101101, 5},
			{0b10010000110001, 5},
			{0b10010000110101, 5},
			{0b10010000111000, 5},
			{0b10010000111001, 5},
			{0b10010000111100, 5},
			{0b10010000111101, 5},
			{0b10010000111110, 5},
			{0b10010010000001, 5},
			{0b10010010000010, 5},
			{0b10010010000101, 5},
			{0b10010010000110, 5},
			{0b10010010001000, 5},
			{0b10010010001001, 5},
			{0b10010010001100, 5},
			{0b10010010001101, 5},
			{0b10010010001110, 5},
			{0b10010010010001, 5},
			{0b10010010010101, 5},
			{0b10010010011000, 5},
			{0b10010010011001, 5},
			{0b10010010011100, 5},
			{0b10010010011101, 5},
			{0b10010010011110, 5},
			{0b10010010100000, 5},
			{0b10010010100001, 5},
			{0b10010010100010, 5},
			{0b10010010100011, 5},
			{0b10010010100100, 5},
			{0b10010010100101, 5},
			{0b10010010100110, 5},
			{0b10010010100111, 5},
			{0b10010010101001, 5},
			{0b10010010101101, 5},
			{0b10010010110001, 5},
			{0b10010010110101, 5},
			{0b10010010111000, 5},
			{0b10010010111001, 5},
			{0b10010010111100, 5},
			{0b10010010111101, 5},
			{0b10010010111110, 5},
			{0b11010000001000, 5},
			{0b11010000001001, 5},
			{0b11010000001100, 5},
			{0b11010000001101, 5},
			{0b11010000010001, 5},
			{0b11010000010101, 5},
			{0b11010000011000, 5},
			{0b11010000011001, 5},
			{0b11010000011101, 5},
			{0b11010000100011, 5},
			{0b11010000100111, 5},
			{0b11010000101001, 5},
			{0b11010000101101, 5},
			{0b11010000110001, 5},
			{0b11010000110101, 5},
			{0b11010000111000, 5},
			{0b11010000111001, 5},
			{0b11010000111100, 5},
			{0b11010000111101, 5},
			{0b11010010001000, 5},
			{0b11010010001001, 5},
			{0b11010010001101, 5},
			{0b11010010010001, 5},
			{0b11010010010101, 5},
			{0b11010010011000, 5},
			{0b11010010011001, 5},
			{0b11010010011101, 5},
			{0b11010010101001, 5},
			{0b11010010101101, 5},
			{0b11010010110001, 5},
			{0b11010010110101, 5},
			{0b11010010111000, 5},
			{0b11010010111001, 5},
			{0b11010010111101, 5}	
		};

		unsigned short int encoding = (op1 << 11) | (CRn << 7) | (CRm << 3) | op2;
		if (auto it{SysOpTable.find(encoding)}; it != SysOpTable.end())
			return it->second;
		return 0;
	}

	bool SysOp128(unsigned short int op1, unsigned short int CRn, unsigned short int CRm, unsigned short int op2)
	{
		unsigned short int encoding = (op1 << 11) | (CRn << 7) | (CRm << 3) | op2;
		switch (encoding)
		{
		  case 0b00010000001001:
			[[fallthrough]];
		  case 0b00010000001011:
			[[fallthrough]];
		  case 0b00010000001101:
			[[fallthrough]];
		  case 0b00010000001111:
			[[fallthrough]];
		  case 0b00010000011001:
			[[fallthrough]];
		  case 0b00010000011011:
			[[fallthrough]];
		  case 0b00010000011101:
			[[fallthrough]];
		  case 0b00010000011111:
			[[fallthrough]];
		  case 0b00010000111001:
			[[fallthrough]];
		  case 0b00010000111011:
			[[fallthrough]];
		  case 0b00010000111101:
			[[fallthrough]];
		  case 0b00010000111111:
			[[fallthrough]];
		  case 0b00010010001001:
			[[fallthrough]];
		  case 0b00010010001011:
			[[fallthrough]];
		  case 0b00010010001101:
			[[fallthrough]];
		  case 0b00010010001111:
			[[fallthrough]];
		  case 0b00010010011001:
			[[fallthrough]];
		  case 0b00010010011011:
			[[fallthrough]];
		  case 0b00010010011101:
			[[fallthrough]];
		  case 0b00010010011111:
			[[fallthrough]];
		  case 0b00010010111001:
			[[fallthrough]];
		  case 0b00010010111011:
			[[fallthrough]];
		  case 0b00010010111101:
			[[fallthrough]];
		  case 0b00010010111111:
			[[fallthrough]];
		  case 0b10010000001001:
			[[fallthrough]];
		  case 0b10010000001101:
			[[fallthrough]];
		  case 0b10010000011001:
			[[fallthrough]];
		  case 0b10010000011101:
			[[fallthrough]];
		  case 0b10010000111001:
			[[fallthrough]];
		  case 0b10010000111101:
			[[fallthrough]];
		  case 0b10010010001001:
			[[fallthrough]];
		  case 0b10010010001101:
			[[fallthrough]];
		  case 0b10010010011001:
			[[fallthrough]];
		  case 0b10010010011101:
			[[fallthrough]];
		  case 0b10010010111001:
			[[fallthrough]];
		  case 0b10010010111101:
			[[fallthrough]];
		  case 0b11010000001001:
			[[fallthrough]];
		  case 0b11010000001101:
			[[fallthrough]];
		  case 0b11010000011001:
			[[fallthrough]];
		  case 0b11010000011101:
			[[fallthrough]];
		  case 0b11010000111001:
			[[fallthrough]];
		  case 0b11010000111101:
			[[fallthrough]];
		  case 0b11010010001001:
			[[fallthrough]];
		  case 0b11010010001101:
			[[fallthrough]];
		  case 0b11010010011001:
			[[fallthrough]];
		  case 0b11010010011101:
			[[fallthrough]];
		  case 0b11010010111001:
			[[fallthrough]];
		  case 0b11010010111101:
			[[fallthrough]];
		  case 0b10010000000001:
			[[fallthrough]];
		  case 0b10010000000101:
			[[fallthrough]];
		  case 0b10010000100000:
			[[fallthrough]];
		  case 0b10010000100001:
			[[fallthrough]];
		  case 0b10010000100100:
			[[fallthrough]];
		  case 0b10010000100101:
			[[fallthrough]];
		  case 0b10010010000001:
			[[fallthrough]];
		  case 0b10010010000101:
			[[fallthrough]];
		  case 0b10010010100000:
			[[fallthrough]];
		  case 0b10010010100001:
			[[fallthrough]];
		  case 0b10010010100100:
			[[fallthrough]];
		  case 0b10010010100101:
			[[fallthrough]];
		  case 0b00010000010001:
			[[fallthrough]];
		  case 0b00010000010011:
			[[fallthrough]];
		  case 0b00010000010101:
			[[fallthrough]];
		  case 0b00010000010111:
			[[fallthrough]];
		  case 0b00010000101001:
			[[fallthrough]];
		  case 0b00010000101011:
			[[fallthrough]];
		  case 0b00010000101101:
			[[fallthrough]];
		  case 0b00010000101111:
			[[fallthrough]];
		  case 0b00010000110001:
			[[fallthrough]];
		  case 0b00010000110011:
			[[fallthrough]];
		  case 0b00010000110101:
			[[fallthrough]];
		  case 0b00010000110111:
			[[fallthrough]];
		  case 0b00010010010001:
			[[fallthrough]];
		  case 0b00010010010011:
			[[fallthrough]];
		  case 0b00010010010101:
			[[fallthrough]];
		  case 0b00010010010111:
			[[fallthrough]];
		  case 0b00010010101001:
			[[fallthrough]];
		  case 0b00010010101011:
			[[fallthrough]];
		  case 0b00010010101101:
			[[fallthrough]];
		  case 0b00010010101111:
			[[fallthrough]];
		  case 0b00010010110001:
			[[fallthrough]];
		  case 0b00010010110011:
			[[fallthrough]];
		  case 0b00010010110101:
			[[fallthrough]];
		  case 0b00010010110111:
			[[fallthrough]];
		  case 0b10010000010001:
			[[fallthrough]];
		  case 0b10010000010101:
			[[fallthrough]];
		  case 0b10010000101001:
			[[fallthrough]];
		  case 0b10010000101101:
			[[fallthrough]];
		  case 0b10010000110001:
			[[fallthrough]];
		  case 0b10010000110101:
			[[fallthrough]];
		  case 0b10010010010001:
			[[fallthrough]];
		  case 0b10010010010101:
			[[fallthrough]];
		  case 0b10010010101001:
			[[fallthrough]];
		  case 0b10010010101101:
			[[fallthrough]];
		  case 0b10010010110001:
			[[fallthrough]];
		  case 0b10010010110101:
			[[fallthrough]];
		  case 0b11010000010001:
			[[fallthrough]];
		  case 0b11010000010101:
			[[fallthrough]];
		  case 0b11010000101001:
			[[fallthrough]];
		  case 0b11010000101101:
			[[fallthrough]];
		  case 0b11010000110001:
			[[fallthrough]];
		  case 0b11010000110101:
			[[fallthrough]];
		  case 0b11010010010001:
			[[fallthrough]];
		  case 0b11010010010101:
			[[fallthrough]];
		  case 0b11010010101001:
			[[fallthrough]];
		  case 0b11010010101101:
			[[fallthrough]];
		  case 0b11010010110001:
			[[fallthrough]];
		  case 0b11010010110101:
			[[fallthrough]];
		  case 0b10010000000010:
			[[fallthrough]];
		  case 0b10010000000110:
			[[fallthrough]];
		  case 0b10010000100010:
			[[fallthrough]];
		  case 0b10010000100011:
			[[fallthrough]];
		  case 0b10010000100110:
			[[fallthrough]];
		  case 0b10010000100111:
			[[fallthrough]];
		  case 0b10010010000010:
			[[fallthrough]];
		  case 0b10010010000110:
			[[fallthrough]];
		  case 0b10010010100010:
			[[fallthrough]];
		  case 0b10010010100011:
			[[fallthrough]];
		  case 0b10010010100110:
			[[fallthrough]];
		  case 0b10010010100111:
			return true;
		  default:
			return false;
		}
	}

	bool MoveWidePreferred(bool sf, bool immN, unsigned short int imms, unsigned short int immr) noexcept
	{
		unsigned short int width = sf ? 64 : 32;
		if (sf && (((immN << 6) | imms) >> 6) != 0b1)
			return false;
		if (!sf && (((immN << 6) | imms) >> 5) != 0b00)
			return false;

		if (imms < 16)
			return ((16 - (immr % 16)) % 16) <= (15 - imms);

		if (imms >= width - 15)
			return (immr % 16) <= (imms - (width - 15));

		return false;
	}

	bool BFXPreffered(bool sf, bool uns, unsigned short int imms, unsigned short int immr) noexcept
	{
		if (imms < immr)
			return false;

		if (imms == ((sf << 1) | 0b11111))
			return false;

		if (immr == 0b000000)
		{
			if (sf == 0b0 && (imms == 0b000111 || imms == 0b001111))
				return false;
			if (((sf << 1) | uns) == 0b10 && (imms == 0b000111 || imms == 0b001111 || imms == 0b011111))
				return false;
		}

		return true;
	}
} /* bits */

#pragma clang diagnostic pop








