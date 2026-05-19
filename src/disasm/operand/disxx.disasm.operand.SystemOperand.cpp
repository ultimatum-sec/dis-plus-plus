module;

#include <unordered_map>
#include <stdexcept>

module disxx.disasm.operand.SystemOperand;

namespace disxx::disasm::operand
{
	/* 	SystemOperand::Impl */

	class SystemOperand::Impl final : public AbstractOperand::AbstractImpl
	{
	  private:
		enum class Type : unsigned short int
		{
			TYPE_S1E1R = 0b0001000000,
			TYPE_S1E1W = 0b0001000001,
			TYPE_S1E0R = 0b0001000010,
			TYPE_S1E0W = 0b0001000011,
			TYPE_S1E1RP = 0b0001001000,
			TYPE_S1E1WP = 0b0001001001,
			TYPE_S1E1A = 0b0001001010,
			TYPE_S1E2R = 0b1001000000,
			TYPE_S1E2W = 0b1001000001,
			TYPE_S12E1R = 0b1001000100,
			TYPE_S12E1W = 0b1001000101,
			TYPE_S12E0R = 0b1001000110,
			TYPE_S12E0W = 0b1001000111,
			TYPE_S1E2A = 0b1001001010,
			TYPE_S1E3R = 0b1101000000,
			TYPE_S1E3W = 0b1101000001,
			TYPE_S1E3A = 0b1101001010,
			TYPE_RCTX = 0b0011,
			TYPE_IALL = 0b100,
			TYPE_INJ = 0b101,
			TYPE_IVAC = 0b0000110001,
			TYPE_ISW = 0b0000110010,
			TYPE_IGVAC = 0b0000110011,
			TYPE_IGSW = 0b0000110100,
			TYPE_IGDVAC = 0b0000110101,
			TYPE_IGDSW = 0b0000110110,
			TYPE_CSW = 0b0001010010,
			TYPE_CGSW = 0b0001010100,
			TYPE_CGDSW = 0b0001010110,
			TYPE_CISW = 0b0001110010,
			TYPE_CIGSW = 0b0001110100,
			TYPE_CIGDSW = 0b0001110110,
			TYPE_CIVAPS = 0b0001111001,
			TYPE_CIGDVAPS = 0b0001111101,
			TYPE_ZVA = 0b0110100001,
			TYPE_GVA = 0b0110100011,
			TYPE_GZVA = 0b0110100100,
			TYPE_CVAC = 0b0111010001,
			TYPE_CGVAC = 0b0111010011,
			TYPE_CGDVAC = 0b0111010101,
			TYPE_CVAOC = 0b0111011000,
			TYPE_CVAU = 0b0111011001,
			TYPE_CGDVAOC = 0b0111011111,
			TYPE_CVAP = 0b0111100001,
			TYPE_CGVAP = 0b0111100110,
			TYPE_CGDVAP = 0b0111100101,
			TYPE_CVADP = 0b0111101001,
			TYPE_CGVADP = 0b0111101011,
			TYPE_CGDVADP = 0b0111101101,
			TYPE_CIVAC = 0b0111110001,
			TYPE_CIGVAC = 0b0111110011,
			TYPE_CIGDVAC = 0b0111110101,
			TYPE_CIVAOC = 0b0111111000,
			TYPE_CIGDVAOC = 0b0111111111,
			TYPE_CIPAE = 0b1001110000,
			TYPE_CIGDPAE = 0b1001110111,
			TYPE_CIPAPA = 0b1101110001,
			TYPE_CIGDPAPA = 0b1101110101,
			TYPE_IALLUIS = 0b0000001000,
			TYPE_IALLU = 0b0000101000,
			TYPE_IVAU = 0b0110101001,
			TYPE_VMALLE1OS = 0b00010000001000,
			TYPE_VAE1OS = 0b00010000001001,
			TYPE_ASIDE1OS = 0b00010000001010,
			TYPE_VAAE1OS = 0b00010000001011,
			TYPE_VALE1OS = 0b00010000001101,
			TYPE_VAALE1OS = 0b00010000001111,
			TYPE_RVAE1IS = 0b00010000010001,
			TYPE_RVAAE1IS = 0b00010000010011,
			TYPE_RVALE1IS = 0b00010000010101,
			TYPE_RVAALE1IS = 0b00010000010111,
			TYPE_VMALLE1IS = 0b00010000011000,
			TYPE_VAE1IS = 0b00010000011001,
			TYPE_ASIDE1IS = 0b00010000011010,
			TYPE_VAAE1IS = 0b00010000011011,
			TYPE_VALE1IS = 0b00010000011101,
			TYPE_VAALE1IS = 0b00010000011111,
			TYPE_RVAE1OS = 0b00010000101001,
			TYPE_RVAAE1OS = 0b00010000101011,
			TYPE_RVALE1OS = 0b00010000101101,
			TYPE_RVAALE1OS = 0b00010000101111,
			TYPE_RVAE1 = 0b00010000110001,
			TYPE_RVAAE1 = 0b00010000110011,
			TYPE_RVALE1 = 0b00010000110101,
			TYPE_RVAALE1 = 0b00010000110111,
			TYPE_VMALLE1 = 0b00010000111000,
			TYPE_VAE1 = 0b00010000111001,
			TYPE_ASIDE1 = 0b00010000111010,
			TYPE_VAAE1 = 0b00010000111011,
			TYPE_VALE1 = 0b00010000111101,
			TYPE_VAALE1 = 0b00010000111111,
			TYPE_VMALLE1OSNXS = 0b00010010001000,
			TYPE_VAE1OSNXS = 0b00010010001001,
			TYPE_ASIDE1OSNXS = 0b00010010001010,
			TYPE_VAAE1OSNXS = 0b00010010001011,
			TYPE_VALE1OSNXS = 0b00010010001101,
			TYPE_VAALE1OSNXS = 0b00010010001111,
			TYPE_RVAE1ISNXS = 0b00010010010001,
			TYPE_RVAAE1ISNXS = 0b00010010010011,
			TYPE_RVALE1ISNXS = 0b00010010010101,
			TYPE_RVAALE1ISNXS = 0b00010010010111,
			TYPE_VMALLE1ISNXS = 0b00010010011000,
			TYPE_VAE1ISNXS = 0b00010010011001,
			TYPE_ASIDE1ISNXS = 0b00010010011010,
			TYPE_VAAE1ISNXS = 0b00010010011011,
			TYPE_VALE1ISNXS = 0b00010010011101,
			TYPE_VAALE1ISNXS = 0b00010010011111,
			TYPE_RVAE1OSNXS = 0b00010010101001,
			TYPE_RVAAE1OSNXS = 0b00010010101011,
			TYPE_RVALE1OSNXS = 0b00010010101101,
			TYPE_RVAALE1OSNXS = 0b00010010101111,
			TYPE_RVAE1NXS = 0b00010010110001,
			TYPE_RVAAE1NXS = 0b00010010110011,
			TYPE_RVALE1NXS = 0b00010010110101,
			TYPE_RVAALE1NXS = 0b00010010110111,
			TYPE_VMALLE1NXS = 0b00010010111000,
			TYPE_VAE1NXS = 0b00010010111001,
			TYPE_ASIDE1NXS = 0b00010010111010,
			TYPE_VAAE1NXS = 0b00010010111011,
			TYPE_VALE1NXS = 0b00010010111101,
			TYPE_VAALE1NXS = 0b00010010111111,
			TYPE_IPAS2E1IS = 0b10010000000001,
			TYPE_RIPAS2E1IS = 0b10010000000010,
			TYPE_IPAS2LE1IS = 0b10010000000101,
			TYPE_RIPAS2LE1IS = 0b10010000000110,
			TYPE_ALLE2OS = 0b10010000001000,
			TYPE_VAE2OS = 0b10010000001001,
			TYPE_ALLE1OS = 0b10010000001100,
			TYPE_VALE2OS = 0b10010000001101,
			TYPE_VMALLS12E1OS = 0b10010000001110,
			TYPE_RVAE2IS = 0b10010000010001,
			TYPE_VMALLWS2E1IS = 0b10010000010010,
			TYPE_RVALE2IS = 0b10010000010101,
			TYPE_ALLE2IS = 0b10010000011000,
			TYPE_VAE2IS = 0b10010000011001,
			TYPE_ALLE1IS = 0b10010000011100,
			TYPE_VALE2IS = 0b10010000011101,
			TYPE_VMALLS12E1IS = 0b10010000011110,
			TYPE_IPAS2E1OS = 0b10010000100000,
			TYPE_IPAS2E1 = 0b10010000100001,
			TYPE_RIPAS2E1 = 0b10010000100010,
			TYPE_RIPAS2E1OS = 0b10010000100011,
			TYPE_IPAS2LE1OS = 0b10010000100100,
			TYPE_IPAS2LE1 = 0b10010000100101,
			TYPE_RIPAS2LE1 = 0b10010000100110,
			TYPE_RIPAS2LE1OS = 0b10010000100111,
			TYPE_RVAE2OS = 0b10010000101001,
			TYPE_VMALLWS2E1OS = 0b10010000101010,
			TYPE_RVALE2OS = 0b10010000101101,
			TYPE_RVAE2 = 0b10010000110001,
			TYPE_VMALLWS2E1 = 0b10010000110010,
			TYPE_RVALE2 = 0b10010000110101,
			TYPE_ALLE2 = 0b10010000111000,
			TYPE_VAE2 = 0b10010000111001,
			TYPE_ALLE1 = 0b10010000111100,
			TYPE_VALE2 = 0b10010000111101,
			TYPE_VMALLS12E1 = 0b10010000111110,
			TYPE_IPAS2E1ISNXS = 0b10010010000001,
			TYPE_RIPAS2E1ISNXS = 0b10010010000010,
			TYPE_IPAS2LE1ISNXS = 0b10010010000101,
			TYPE_RIPAS2LE1ISNXS = 0b10010010000110,
			TYPE_ALLE2OSNXS = 0b10010010001000,
			TYPE_VAE2OSNXS = 0b10010010001001,
			TYPE_ALLE1OSNXS = 0b10010010001100,
			TYPE_VALE2OSNXS = 0b10010010001101,
			TYPE_VMALLS12E1OSNXS = 0b10010010001110,
			TYPE_RVAE2ISNXS = 0b10010010010001,
			TYPE_VMALLWS2E1ISNXS = 0b10010010010010,
			TYPE_RVALE2ISNXS = 0b10010010010101,
			TYPE_ALLE2ISNXS = 0b10010010011000,
			TYPE_VAE2ISNXS = 0b10010010011001,
			TYPE_ALLE1ISNXS = 0b10010010011100,
			TYPE_VALE2ISNXS = 0b10010010011101,
			TYPE_VMALLS12E1ISNXS = 0b10010010011110,
			TYPE_IPAS2E1OSNXS = 0b10010010100000,
			TYPE_IPAS2E1NXS = 0b10010010100001,
			TYPE_RIPAS2E1NXS = 0b10010010100010,
			TYPE_RIPAS2E1OSNXS = 0b10010010100011,
			TYPE_IPAS2LE1OSNXS = 0b10010010100100,
			TYPE_IPAS2LE1NXS = 0b10010010100101,
			TYPE_RIPAS2LE1NXS = 0b10010010100110,
			TYPE_RIPAS2LE1OSNXS = 0b10010010100111,
			TYPE_RVAE2OSNXS = 0b10010010101001,
			TYPE_VMALLWS2E1OSNXS = 0b10010010101010,
			TYPE_RVALE2OSNXS = 0b10010010101101,
			TYPE_RVAE2NXS = 0b10010010110001,
			TYPE_VMALLWS2E1NXS = 0b10010010110010,
			TYPE_RVALE2NXS = 0b10010010110101,
			TYPE_ALLE2NXS = 0b10010010111000,
			TYPE_VAE2NXS = 0b10010010111001,
			TYPE_ALLE1NXS = 0b10010010111100,
			TYPE_VALE2NXS = 0b10010010111101,
			TYPE_VMALLS12E1NXS = 0b10010010111110,
			TYPE_ALLE3OS = 0b11010000001000,
			TYPE_VAE3OS = 0b11010000001001,
			TYPE_PAALLOS = 0b11010000001100,
			TYPE_VALE3OS = 0b11010000001101,
			TYPE_RVAE3IS = 0b11010000010001,
			TYPE_RVALE3IS = 0b11010000010101,
			TYPE_ALLE3IS = 0b11010000011000,
			TYPE_VAE3IS = 0b11010000011001,
			TYPE_VALE3IS = 0b11010000011101,
			TYPE_RPAOS = 0b11010000100011,
			TYPE_RPALOS = 0b11010000100111,
			TYPE_RVAE3OS = 0b11010000101001,
			TYPE_RVALE3OS = 0b11010000101101,
			TYPE_RVAE3 = 0b11010000110001,
			TYPE_RVALE3 = 0b11010000110101,
			TYPE_ALLE3 = 0b11010000111000,
			TYPE_VAE3 = 0b11010000111001,
			TYPE_PAALL = 0b11010000111100,
			TYPE_VALE3 = 0b11010000111101,
			TYPE_ALLE3OSNXS = 0b11010010001000,
			TYPE_VAE3OSNXS = 0b11010010001001,
			TYPE_VALE3OSNXS = 0b11010010001101,
			TYPE_RVAE3ISNXS = 0b11010010010001,
			TYPE_RVALE3ISNXS = 0b11010010010101,
			TYPE_ALLE3ISNXS = 0b11010010011000,
			TYPE_VAE3ISNXS = 0b11010010011001,
			TYPE_VALE3ISNXS = 0b11010010011101,
			TYPE_RVAE3OSNXS = 0b11010010101001,
			TYPE_RVALE3OSNXS = 0b11010010101101,
			TYPE_RVAE3NXS = 0b11010010110001,
			TYPE_RVALE3NXS = 0b11010010110101,
			TYPE_ALLE3NXS = 0b11010010111000,
			TYPE_VAE3NXS = 0b11010010111001,
			TYPE_VALE3NXS = 0b11010010111101
		};

	  private:
		static const std::unordered_map<Type, const char *> s_OprsTable;

	  private:
		Type m_Opr;

	  public:
		explicit Impl(void) noexcept;
		explicit Impl(unsigned short int) noexcept;

		explicit Impl(const Impl &) noexcept;
		Impl &operator=(const Impl &) noexcept;

		explicit Impl(const Impl &&) noexcept;
		Impl &operator=(const Impl &&) noexcept;

		virtual std::string GetMnemonic(void) const noexcept(false) override;
	};

	SystemOperand::Impl::Impl(void) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_SYSTEMOPERAND}
		, m_Opr{}
	{}

	SystemOperand::Impl::Impl(unsigned short int bits) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_SYSTEMOPERAND}
		, m_Opr{static_cast<Type>(bits)}
	{}

	SystemOperand::Impl::Impl(const Impl &other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_SYSTEMOPERAND}
		, m_Opr{other.m_Opr}
	{}

	SystemOperand::Impl &SystemOperand::Impl::operator=(const Impl &other) noexcept
	{
		if (this != &other) [[likely]]
			this->m_Opr = other.m_Opr;
		return *this;
	}

	SystemOperand::Impl::Impl(const Impl &&other) noexcept
		: AbstractImpl{AbstractOperand::Type::TYPE_SYSTEMOPERAND}
		, m_Opr{other.m_Opr}
	{}

	SystemOperand::Impl &SystemOperand::Impl::operator=(const Impl &&other) noexcept
	{
		this->m_Opr = other.m_Opr;
		return *this;
	}

	std::string SystemOperand::Impl::GetMnemonic(void) const noexcept(false)
	{
		return s_OprsTable
			.at(this->m_Opr);
	}

	const std::unordered_map<SystemOperand::Impl::Type, const char *> SystemOperand::Impl::s_OprsTable = {
		{Type::TYPE_S1E1R, "s1e1r"},
		{Type::TYPE_S1E1W, "s1e1w"},
		{Type::TYPE_S1E0R, "s1e0r"},
		{Type::TYPE_S1E0W, "s1e0w"},
		{Type::TYPE_S1E1RP, "s1e1rp"},
		{Type::TYPE_S1E1WP, "s1e1wp"},
		{Type::TYPE_S1E1A, "s1e1a"},
		{Type::TYPE_S1E2R, "s1e2r"},
		{Type::TYPE_S1E2W, "s1e2w"},
		{Type::TYPE_S12E1R, "s12e1r"},
		{Type::TYPE_S12E1W, "s12e1w"},
		{Type::TYPE_S12E0R, "s12e0r"},
		{Type::TYPE_S12E0W, "s12e0w"},
		{Type::TYPE_S1E2A, "s1e2a"},
		{Type::TYPE_S1E3R, "s1e3r"},
		{Type::TYPE_S1E3W, "s1e3w"},
		{Type::TYPE_S1E3A, "s1e3a"},
		{Type::TYPE_RCTX, "rctx"},
		{Type::TYPE_IALL, "iall"},
		{Type::TYPE_INJ, "inj"},
		{Type::TYPE_IVAC, "ivac"},
		{Type::TYPE_ISW, "isw"},
		{Type::TYPE_IGVAC, "igvac"},
		{Type::TYPE_IGSW, "igsw"},
		{Type::TYPE_IGDVAC, "igdvac"},
		{Type::TYPE_IGDSW, "igdsw"},
		{Type::TYPE_CSW, "csw"},
		{Type::TYPE_CGSW, "cgsw"},
		{Type::TYPE_CGDSW, "cgdsw"},
		{Type::TYPE_CISW, "cisw"},
		{Type::TYPE_CIGSW, "cigsw"},
		{Type::TYPE_CIGDSW, "cigdsw"},
		{Type::TYPE_CIVAPS, "civaps"},
		{Type::TYPE_CIGDVAPS, "cigdvaps"},
		{Type::TYPE_ZVA, "zva"},
		{Type::TYPE_GVA, "gva"},
		{Type::TYPE_GZVA, "gzva"},
		{Type::TYPE_CVAC, "cvac"},
		{Type::TYPE_CGVAC, "cgvac"},
		{Type::TYPE_CGDVAC, "cgdvac"},
		{Type::TYPE_CVAOC, "cvaoc"},
		{Type::TYPE_CVAU, "cvau"},
		{Type::TYPE_CGDVAOC, "cgdvaoc"},
		{Type::TYPE_CVAP, "cvap"},
		{Type::TYPE_CGVAP, "cgvap"},
		{Type::TYPE_CGDVAP, "cgdvap"},
		{Type::TYPE_CVADP, "cvadp"},
		{Type::TYPE_CGVADP, "cgvadp"},
		{Type::TYPE_CGDVADP, "cgdvadp"},
		{Type::TYPE_CIVAC, "civac"},
		{Type::TYPE_CIGVAC, "cigvac"},
		{Type::TYPE_CIGDVAC, "cigdvac"},
		{Type::TYPE_CIVAOC, "civaoc"},
		{Type::TYPE_CIGDVAOC, "cigdvaoc"},
		{Type::TYPE_CIPAE, "cipae"},
		{Type::TYPE_CIGDPAE, "cigdpae"},
		{Type::TYPE_CIPAPA, "cipapa"},
		{Type::TYPE_CIGDPAPA, "cigdpapa"},
		{Type::TYPE_IALLUIS, "ialluis"},
		{Type::TYPE_IALLU, "iallu"},
		{Type::TYPE_IVAU, "ivau"},
		{Type::TYPE_VMALLE1OS, "vmalle1os"},
		{Type::TYPE_VAE1OS, "vae1os"},
		{Type::TYPE_ASIDE1OS, "aside1os"},
		{Type::TYPE_VAAE1OS, "vaae1os"},
		{Type::TYPE_VALE1OS, "vale1os"},
		{Type::TYPE_VAALE1OS, "vaale1os"},
		{Type::TYPE_RVAE1IS, "rvae1is"},
		{Type::TYPE_RVAAE1IS, "rvaae1is"},
		{Type::TYPE_RVALE1IS, "rvale1is"},
		{Type::TYPE_RVAALE1IS, "rvaale1is"},
		{Type::TYPE_VMALLE1IS, "vmalle1is"},
		{Type::TYPE_VAE1IS, "vae1is"},
		{Type::TYPE_ASIDE1IS, "aside1is"},
		{Type::TYPE_VAAE1IS, "vaae1is"},
		{Type::TYPE_VALE1IS, "vale1is"},
		{Type::TYPE_VAALE1IS, "vaale1is"},
		{Type::TYPE_RVAE1OS, "rvae1os"},
		{Type::TYPE_RVAAE1OS, "rvaae1os"},
		{Type::TYPE_RVALE1OS, "rvale1os"},
		{Type::TYPE_RVAALE1OS, "rvaale1os"},
		{Type::TYPE_RVAE1, "rvae1"},
		{Type::TYPE_RVAAE1, "rvaae1"},
		{Type::TYPE_RVALE1, "rvale1"},
		{Type::TYPE_RVAALE1, "rvaale1"},
		{Type::TYPE_VMALLE1, "vmalle1"},
		{Type::TYPE_VAE1, "vae1"},
		{Type::TYPE_ASIDE1, "aside1"},
		{Type::TYPE_VAAE1, "vaae1"},
		{Type::TYPE_VALE1, "vale1"},
		{Type::TYPE_VAALE1, "vaale1"},
		{Type::TYPE_VMALLE1OSNXS, "vmalle1osnxs"},
		{Type::TYPE_VAE1OSNXS, "vae1osnxs"},
		{Type::TYPE_ASIDE1OSNXS, "aside1osnxs"},
		{Type::TYPE_VAAE1OSNXS, "vaae1osnxs"},
		{Type::TYPE_VALE1OSNXS, "vale1osnxs"},
		{Type::TYPE_VAALE1OSNXS, "vaale1osnxs"},
		{Type::TYPE_RVAE1ISNXS, "rvae1isnxs"},
		{Type::TYPE_RVAAE1ISNXS, "rvaae1isnxs"},
		{Type::TYPE_RVALE1ISNXS, "rvale1isnxs"},
		{Type::TYPE_RVAALE1ISNXS, "rvaale1isnxs"},
		{Type::TYPE_VMALLE1ISNXS, "vmalle1isnxs"},
		{Type::TYPE_VAE1ISNXS, "vae1isnxs"},
		{Type::TYPE_ASIDE1ISNXS, "aside1isnxs"},
		{Type::TYPE_VAAE1ISNXS, "vaae1isnxs"},
		{Type::TYPE_VALE1ISNXS, "vale1isnxs"},
		{Type::TYPE_VAALE1ISNXS, "vaale1isnxs"},
		{Type::TYPE_RVAE1OSNXS, "rvae1osnxs"},
		{Type::TYPE_RVAAE1OSNXS, "rvaae1osnxs"},
		{Type::TYPE_RVALE1OSNXS, "rvale1osnxs"},
		{Type::TYPE_RVAALE1OSNXS, "rvaale1osnxs"},
		{Type::TYPE_RVAE1NXS, "rvae1nxs"},
		{Type::TYPE_RVAAE1NXS, "rvaae1nxs"},
		{Type::TYPE_RVALE1NXS, "rvale1nxs"},
		{Type::TYPE_RVAALE1NXS, "rvaale1nxs"},
		{Type::TYPE_VMALLE1NXS, "vmalle1nxs"},
		{Type::TYPE_VAE1NXS, "vae1nxs"},
		{Type::TYPE_ASIDE1NXS, "aside1nxs"},
		{Type::TYPE_VAAE1NXS, "vaae1nxs"},
		{Type::TYPE_VALE1NXS, "vale1nxs"},
		{Type::TYPE_VAALE1NXS, "vaale1nxs"},
		{Type::TYPE_IPAS2E1IS, "ipas2e1is"},
		{Type::TYPE_RIPAS2E1IS, "ripas2e1is"},
		{Type::TYPE_IPAS2LE1IS, "ipas2le1is"},
		{Type::TYPE_RIPAS2LE1IS, "ripas2le1is"},
		{Type::TYPE_ALLE2OS, "alle2os"},
		{Type::TYPE_VAE2OS, "vae2os"},
		{Type::TYPE_ALLE1OS, "alle1os"},
		{Type::TYPE_VALE2OS, "vale2os"},
		{Type::TYPE_VMALLS12E1OS, "vmalls12e1os"},
		{Type::TYPE_RVAE2IS, "rvae2is"},
		{Type::TYPE_VMALLWS2E1IS, "vmallws2e1is"},
		{Type::TYPE_RVALE2IS, "rvale2is"},
		{Type::TYPE_ALLE2IS, "alle2is"},
		{Type::TYPE_VAE2IS, "vae2is"},
		{Type::TYPE_ALLE1IS, "alle1is"},
		{Type::TYPE_VALE2IS, "vale2is"},
		{Type::TYPE_VMALLS12E1IS, "vmalls12e1is"},
		{Type::TYPE_IPAS2E1OS, "ipas2e1os"},
		{Type::TYPE_IPAS2E1, "ipas2e1"},
		{Type::TYPE_RIPAS2E1, "ripas2e1"},
		{Type::TYPE_RIPAS2E1OS, "ripas2e1os"},
		{Type::TYPE_IPAS2LE1OS, "ipas2le1os"},
		{Type::TYPE_IPAS2LE1, "ipas2le1"},
		{Type::TYPE_RIPAS2LE1, "ripas2le1"},
		{Type::TYPE_RIPAS2LE1OS, "ripas2le1os"},
		{Type::TYPE_RVAE2OS, "rvae2os"},
		{Type::TYPE_VMALLWS2E1OS, "vmallws2e1os"},
		{Type::TYPE_RVALE2OS, "rvale2os"},
		{Type::TYPE_RVAE2, "rvae2"},
		{Type::TYPE_VMALLWS2E1, "vmallws2e1"},
		{Type::TYPE_RVALE2, "rvale2"},
		{Type::TYPE_ALLE2, "alle2"},
		{Type::TYPE_VAE2, "vae2"},
		{Type::TYPE_ALLE1, "alle1"},
		{Type::TYPE_VALE2, "vale2"},
		{Type::TYPE_VMALLS12E1, "vmalls12e1"},
		{Type::TYPE_IPAS2E1ISNXS, "ipas2e1isnxs"},
		{Type::TYPE_RIPAS2E1ISNXS, "ripas2e1isnxs"},
		{Type::TYPE_IPAS2LE1ISNXS, "ipas2le1isnxs"},
		{Type::TYPE_RIPAS2LE1ISNXS, "ripas2le1isnxs"},
		{Type::TYPE_ALLE2OSNXS, "alle2osnxs"},
		{Type::TYPE_VAE2OSNXS, "vae2osnxs"},
		{Type::TYPE_ALLE1OSNXS, "alle1osnxs"},
		{Type::TYPE_VALE2OSNXS, "vale2osnxs"},
		{Type::TYPE_VMALLS12E1OSNXS, "vmalls12e1osnxs"},
		{Type::TYPE_RVAE2ISNXS, "rvae2isnxs"},
		{Type::TYPE_VMALLWS2E1ISNXS, "vmallws2e1isnxs"},
		{Type::TYPE_RVALE2ISNXS, "rvale2isnxs"},
		{Type::TYPE_ALLE2ISNXS, "alle2isnxs"},
		{Type::TYPE_VAE2ISNXS, "vae2isnxs"},
		{Type::TYPE_ALLE1ISNXS, "alle1isnxs"},
		{Type::TYPE_VALE2ISNXS, "vale2isnxs"},
		{Type::TYPE_VMALLS12E1ISNXS, "vmalls12e1isnxs"},
		{Type::TYPE_IPAS2E1OSNXS, "ipas2e1osnxs"},
		{Type::TYPE_IPAS2E1NXS, "ipas2e1nxs"},
		{Type::TYPE_RIPAS2E1NXS, "ripas2e1nxs"},
		{Type::TYPE_RIPAS2E1OSNXS, "ripas2e1osnxs"},
		{Type::TYPE_IPAS2LE1OSNXS, "ipas2le1osnxs"},
		{Type::TYPE_IPAS2LE1NXS, "ipas2le1nxs"},
		{Type::TYPE_RIPAS2LE1NXS, "ripas2le1nxs"},
		{Type::TYPE_RIPAS2LE1OSNXS, "ripas2le1osnxs"},
		{Type::TYPE_RVAE2OSNXS, "rvae2osnxs"},
		{Type::TYPE_VMALLWS2E1OSNXS, "vmallws2e1osnxs"},
		{Type::TYPE_RVALE2OSNXS, "rvale2osnxs"},
		{Type::TYPE_RVAE2NXS, "rvae2nxs"},
		{Type::TYPE_VMALLWS2E1NXS, "vmallws2e1nxs"},
		{Type::TYPE_RVALE2NXS, "rvale2nxs"},
		{Type::TYPE_ALLE2NXS, "alle2nxs"},
		{Type::TYPE_VAE2NXS, "vae2nxs"},
		{Type::TYPE_ALLE1NXS, "alle1nxs"},
		{Type::TYPE_VALE2NXS, "vale2nxs"},
		{Type::TYPE_VMALLS12E1NXS, "vmalls12e1nxs"},
		{Type::TYPE_ALLE3OS, "alle3os"},
		{Type::TYPE_VAE3OS, "vae3os"},
		{Type::TYPE_PAALLOS, "paallos"},
		{Type::TYPE_VALE3OS, "vale3os"},
		{Type::TYPE_RVAE3IS, "rvae3is"},
		{Type::TYPE_RVALE3IS, "rvale3is"},
		{Type::TYPE_ALLE3IS, "alle3is"},
		{Type::TYPE_VAE3IS, "vae3is"},
		{Type::TYPE_VALE3IS, "vale3is"},
		{Type::TYPE_RPAOS, "rpaos"},
		{Type::TYPE_RPALOS, "rpalos"},
		{Type::TYPE_RVAE3OS, "rvae3os"},
		{Type::TYPE_RVALE3OS, "rvale3os"},
		{Type::TYPE_RVAE3, "rvae3"},
		{Type::TYPE_RVALE3, "rvale3"},
		{Type::TYPE_ALLE3, "alle3"},
		{Type::TYPE_VAE3, "vae3"},
		{Type::TYPE_PAALL, "paall"},
		{Type::TYPE_VALE3, "vale3"},
		{Type::TYPE_ALLE3OSNXS, "alle3osnxs"},
		{Type::TYPE_VAE3OSNXS, "vae3osnxs"},
		{Type::TYPE_VALE3OSNXS, "vale3osnxs"},
		{Type::TYPE_RVAE3ISNXS, "rvae3isnxs"},
		{Type::TYPE_RVALE3ISNXS, "rvale3isnxs"},
		{Type::TYPE_ALLE3ISNXS, "alle3isnxs"},
		{Type::TYPE_VAE3ISNXS, "vae3isnxs"},
		{Type::TYPE_VALE3ISNXS, "vale3isnxs"},
		{Type::TYPE_RVAE3OSNXS, "rvae3osnxs"},
		{Type::TYPE_RVALE3OSNXS, "rvale3osnxs"},
		{Type::TYPE_RVAE3NXS, "rvae3nxs"},
		{Type::TYPE_RVALE3NXS, "rvale3nxs"},
		{Type::TYPE_ALLE3NXS, "alle3nxs"},
		{Type::TYPE_VAE3NXS, "vae3nxs"},
		{Type::TYPE_VALE3NXS, "vale3nxs"}
	};

	/* SystemOperand */

	SystemOperand::SystemOperand(void) noexcept
		: AbstractOperand{}
	{}

	SystemOperand::SystemOperand(unsigned short int bits) noexcept
		: AbstractOperand{std::make_unique<Impl>(bits)}
	{}

	SystemOperand::SystemOperand(const SystemOperand &other) noexcept(false)
		: AbstractOperand{std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)))}
	{}

	SystemOperand &SystemOperand::operator=(const SystemOperand &other) noexcept(false)
	{
		if (this != &other) [[unlikely]]
		{
			if (this->m_pImpl)
				this->m_pImpl.Get().reset();
			this->m_pImpl.Get() = std::make_unique<Impl>(*dynamic_cast<const Impl *>(&(*other.m_pImpl)));
		}

		return *this;
	}

	SystemOperand::SystemOperand(SystemOperand &&other) noexcept
		: AbstractOperand{std::move(other.m_pImpl.Get())}
	{}

	SystemOperand &SystemOperand::operator=(SystemOperand &&other) noexcept
	{
		if (this->m_pImpl)
			this->m_pImpl.Get().reset();
		this->m_pImpl.Get() = std::move(other.m_pImpl.Get());

		return *this;
	}

	std::unique_ptr<AbstractOperand> SystemOperand::Clone(void) const noexcept
	{ return std::make_unique<SystemOperand>(*this); }
} /* operand */



