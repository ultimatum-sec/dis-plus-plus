module;

#include <unordered_map>
#include <utility>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>

module disxx.disasm.decoder.LoadsAndStores.AtomicMemoryOperations.SubDecoder;

import disxx.disasm.operand.LoadsAndStoresAddress;
import disxx.utility.error.DisassemblyError;
import disxx.disasm.operand.Register;
import disxx.disasm.InstructionID;
import disxx.disasm.utility.bits;

namespace disxx::disasm::decoder::LoadsAndStores::AtomicMemoryOperations
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
        // +----+---+--+--+-+-+-+--+--+---+--+--+--+
        // |size|111|VR|00|A|R|1|Rs|o3|opc|00|Rn|Rt|
        // +----+---+--+--+-+-+-+--+--+---+--+--+--+

        unsigned short int size, VR, A, R, Rs, o3, opc, Rn, Rt;
        size = bits::extract<unsigned short int, std::uint32_t, 30, 31>(this->m_Insn);
        VR = bits::extract<unsigned short int, std::uint32_t, 26, 26>(this->m_Insn);
        A = bits::extract<unsigned short int, std::uint32_t, 23, 23>(this->m_Insn);
        R = bits::extract<unsigned short int, std::uint32_t, 22, 22>(this->m_Insn);
        Rs = bits::extract<unsigned short int, std::uint32_t, 16, 20>(this->m_Insn);
        o3 = bits::extract<unsigned short int, std::uint32_t, 15, 15>(this->m_Insn);
        opc = bits::extract<unsigned short int, std::uint32_t, 12, 14>(this->m_Insn);
        Rn = bits::extract<unsigned short int, std::uint32_t, 5, 9>(this->m_Insn);
        Rt = bits::extract<unsigned short int, std::uint32_t, 0, 4>(this->m_Insn);
    
        static const std::unordered_map<unsigned short int, std::pair<InstructionID, std::optional<InstructionID>>> insnTable1 = {
            {0b000000000, {InstructionID::INSN_LDADDB, InstructionID::INSN_STADDB}},
            {0b000010000, {InstructionID::INSN_LDADDLB, InstructionID::INSN_STADDLB}},
            {0b000100000, {InstructionID::INSN_LDADDAB, std::nullopt}},
            {0b000110000, {InstructionID::INSN_LDADDALB, std::nullopt}},
            {0b000000001, {InstructionID::INSN_LDCLRB, InstructionID::INSN_STCLRB}},
            {0b000010001, {InstructionID::INSN_LDCLRLB, InstructionID::INSN_STCLRLB}},
            {0b000100001, {InstructionID::INSN_LDCLRAB, std::nullopt}},
            {0b000110001, {InstructionID::INSN_LDCLRALB, std::nullopt}},
            {0b000000010, {InstructionID::INSN_LDEORB, InstructionID::INSN_STEORB}},
            {0b000010010, {InstructionID::INSN_LDEORLB, InstructionID::INSN_STEORLB}},
            {0b000100010, {InstructionID::INSN_LDEORAB, std::nullopt}},
            {0b000110010, {InstructionID::INSN_LDEORALB, std::nullopt}},
            {0b000000011, {InstructionID::INSN_LDSETB, InstructionID::INSN_STSETB}},
            {0b000010011, {InstructionID::INSN_LDSETLB, InstructionID::INSN_STSETLB}},
            {0b000100011, {InstructionID::INSN_LDSETAB, std::nullopt}},
            {0b000110011, {InstructionID::INSN_LDSETALB, std::nullopt}},
            {0b000000100, {InstructionID::INSN_LDSMAXB, InstructionID::INSN_STSMAXB}},
            {0b000010100, {InstructionID::INSN_LDSMAXLB, InstructionID::INSN_STSMAXLB}},
            {0b000100100, {InstructionID::INSN_LDSMAXAB, std::nullopt}},
            {0b000110100, {InstructionID::INSN_LDSMAXALB, std::nullopt}},
            {0b000000101, {InstructionID::INSN_LDSMINB, InstructionID::INSN_STSMINB}},
            {0b000010101, {InstructionID::INSN_LDSMINLB, InstructionID::INSN_STSMINLB}},
            {0b000100101, {InstructionID::INSN_LDSMINAB, std::nullopt}},
            {0b000110101, {InstructionID::INSN_LDSMINALB, std::nullopt}},
            {0b000000110, {InstructionID::INSN_LDUMAXB, InstructionID::INSN_STUMAXB}},
            {0b000010110, {InstructionID::INSN_LDUMAXLB, InstructionID::INSN_STUMAXLB}},
            {0b000100110, {InstructionID::INSN_LDUMAXAB, std::nullopt}},
            {0b000110110, {InstructionID::INSN_LDUMAXALB, std::nullopt}},
            {0b000000111, {InstructionID::INSN_LDUMINB, InstructionID::INSN_STUMINB}},
            {0b000010111, {InstructionID::INSN_LDUMINLB, InstructionID::INSN_STUMINLB}},
            {0b000100111, {InstructionID::INSN_LDUMINAB, std::nullopt}},
            {0b000110111, {InstructionID::INSN_LDUMINALB, std::nullopt}},
            {0b000001000, {InstructionID::INSN_SWPB, std::nullopt}},
            {0b000011000, {InstructionID::INSN_SWPLB, std::nullopt}},
            {0b000101000, {InstructionID::INSN_SWPAB, std::nullopt}},
            {0b000111000, {InstructionID::INSN_SWPALB, std::nullopt}},
            {0b000101100, {InstructionID::INSN_LDAPRB, std::nullopt}},
            {0b010000000, {InstructionID::INSN_LDADDH, InstructionID::INSN_STADDH}},
            {0b010010000, {InstructionID::INSN_LDADDLH, InstructionID::INSN_STADDLH}},
            {0b010100000, {InstructionID::INSN_LDADDAH, std::nullopt}},
            {0b010110000, {InstructionID::INSN_LDADDALH, std::nullopt}},
            {0b010000001, {InstructionID::INSN_LDCLRH, InstructionID::INSN_STCLRH}},
            {0b010010001, {InstructionID::INSN_LDCLRLH, InstructionID::INSN_STCLRLH}},
            {0b010100001, {InstructionID::INSN_LDCLRAH, std::nullopt}},
            {0b010110001, {InstructionID::INSN_LDCLRALH, std::nullopt}},
            {0b010000010, {InstructionID::INSN_LDEORH, InstructionID::INSN_STEORH}},
            {0b010010010, {InstructionID::INSN_LDEORLH, InstructionID::INSN_STEORLH}},
            {0b010100010, {InstructionID::INSN_LDEORAH, std::nullopt}},
            {0b010110010, {InstructionID::INSN_LDEORALH, std::nullopt}},
            {0b010000011, {InstructionID::INSN_LDSETH, InstructionID::INSN_STSETH}},
            {0b010010011, {InstructionID::INSN_LDSETLH, InstructionID::INSN_STSETLH}},
            {0b010100011, {InstructionID::INSN_LDSETAH, std::nullopt}},
            {0b010110011, {InstructionID::INSN_LDSETALH, std::nullopt}},
            {0b010000100, {InstructionID::INSN_LDSMAXH, InstructionID::INSN_STSMAXH}},
            {0b010010100, {InstructionID::INSN_LDSMAXLH, InstructionID::INSN_STSMAXLH}},
            {0b010100100, {InstructionID::INSN_LDSMAXAH, std::nullopt}},
            {0b010110100, {InstructionID::INSN_LDSMAXALH, std::nullopt}},
            {0b010000101, {InstructionID::INSN_LDSMINH, InstructionID::INSN_STSMINH}},
            {0b010010101, {InstructionID::INSN_LDSMINLH, InstructionID::INSN_STSMINLH}},
            {0b010100101, {InstructionID::INSN_LDSMINAH, std::nullopt}},
            {0b010110101, {InstructionID::INSN_LDSMINALH, std::nullopt}},
            {0b010000110, {InstructionID::INSN_LDUMAXH, InstructionID::INSN_STUMAXH}},
            {0b010010110, {InstructionID::INSN_LDUMAXLH, InstructionID::INSN_STUMAXLH}},
            {0b010100110, {InstructionID::INSN_LDUMAXAH, std::nullopt}},
            {0b010110110, {InstructionID::INSN_LDUMAXALH, std::nullopt}},
            {0b010000111, {InstructionID::INSN_LDUMINH, InstructionID::INSN_STUMINH}},
            {0b010010111, {InstructionID::INSN_LDUMINLH, InstructionID::INSN_STUMINLH}},
            {0b010100111, {InstructionID::INSN_LDUMINAH, std::nullopt}},
            {0b010110111, {InstructionID::INSN_LDUMINALH, std::nullopt}},
            {0b010001000, {InstructionID::INSN_SWPH, std::nullopt}},
            {0b010011000, {InstructionID::INSN_SWPLH, std::nullopt}},
            {0b010101000, {InstructionID::INSN_SWPAH, std::nullopt}},
            {0b010111000, {InstructionID::INSN_SWPALH, std::nullopt}},
            {0b010101100, {InstructionID::INSN_LDAPRH, std::nullopt}},
            {0b100000000, {InstructionID::INSN_LDADD, InstructionID::INSN_STADD}},
            {0b100010000, {InstructionID::INSN_LDADDL, InstructionID::INSN_STADDL}},
            {0b100100000, {InstructionID::INSN_LDADDA, std::nullopt}},
            {0b100110000, {InstructionID::INSN_LDADDAL, std::nullopt}},
            {0b100000001, {InstructionID::INSN_LDCLR, InstructionID::INSN_STCLR}},
            {0b100010001, {InstructionID::INSN_LDCLRL, InstructionID::INSN_STCLRL}},
            {0b100100001, {InstructionID::INSN_LDCLRA, std::nullopt}},
            {0b100110001, {InstructionID::INSN_LDCLRAL, std::nullopt}},
            {0b100000010, {InstructionID::INSN_LDEOR, InstructionID::INSN_STEOR}},
            {0b100010010, {InstructionID::INSN_LDEORL, InstructionID::INSN_STEORL}},
            {0b100100010, {InstructionID::INSN_LDEORA, std::nullopt}},
            {0b100110010, {InstructionID::INSN_LDEORAL, std::nullopt}},
            {0b100000011, {InstructionID::INSN_LDSET, InstructionID::INSN_STSET}},
            {0b100010011, {InstructionID::INSN_LDSETL, InstructionID::INSN_STSETL}},
            {0b100100011, {InstructionID::INSN_LDSETA, std::nullopt}},
            {0b100110011, {InstructionID::INSN_LDSETAL, std::nullopt}},
            {0b100000100, {InstructionID::INSN_LDSMAX, InstructionID::INSN_STSMAX}},
            {0b100010100, {InstructionID::INSN_LDSMAXL, InstructionID::INSN_STSMAXL}},
            {0b100100100, {InstructionID::INSN_LDSMAXA, std::nullopt}},
            {0b100110100, {InstructionID::INSN_LDSMAXAL, std::nullopt}},
            {0b100000101, {InstructionID::INSN_LDSMIN, InstructionID::INSN_STSMIN}},
            {0b100010101, {InstructionID::INSN_LDSMINL, InstructionID::INSN_STSMINL}},
            {0b100100101, {InstructionID::INSN_LDSMINA, std::nullopt}},
            {0b100110101, {InstructionID::INSN_LDSMINAL, std::nullopt}},
            {0b100000110, {InstructionID::INSN_LDUMAX, InstructionID::INSN_STUMAX}},
            {0b100010110, {InstructionID::INSN_LDUMAXL, InstructionID::INSN_STUMAXL}},
            {0b100100110, {InstructionID::INSN_LDUMAXA, std::nullopt}},
            {0b100110110, {InstructionID::INSN_LDUMAXAL, std::nullopt}},
            {0b100000111, {InstructionID::INSN_LDUMIN, InstructionID::INSN_STUMIN}},
            {0b100010111, {InstructionID::INSN_LDUMINL, InstructionID::INSN_STUMINL}},
            {0b100100111, {InstructionID::INSN_LDUMINA, std::nullopt}},
            {0b100110111, {InstructionID::INSN_LDUMINAL, std::nullopt}},
            {0b100001000, {InstructionID::INSN_SWP, std::nullopt}},
            {0b100011000, {InstructionID::INSN_SWPL, std::nullopt}},
            {0b100101000, {InstructionID::INSN_SWPA, std::nullopt}},
            {0b100111000, {InstructionID::INSN_SWPAL, std::nullopt}},
            {0b100101100, {InstructionID::INSN_LDAPR, std::nullopt}},
            {0b110000000, {InstructionID::INSN_LDADD, InstructionID::INSN_STADD}},
            {0b110010000, {InstructionID::INSN_LDADDL, InstructionID::INSN_STADDL}},
            {0b110100000, {InstructionID::INSN_LDADDA, std::nullopt}},
            {0b110110000, {InstructionID::INSN_LDADDAL, std::nullopt}},
            {0b110000001, {InstructionID::INSN_LDCLR, InstructionID::INSN_STCLR}},
            {0b110010001, {InstructionID::INSN_LDCLRL, InstructionID::INSN_STCLRL}},
            {0b110100001, {InstructionID::INSN_LDCLRA, std::nullopt}},
            {0b110110001, {InstructionID::INSN_LDCLRAL, std::nullopt}},
            {0b110000010, {InstructionID::INSN_LDEOR, InstructionID::INSN_STEOR}},
            {0b110010010, {InstructionID::INSN_LDEORL, InstructionID::INSN_STEORL}},
            {0b110100010, {InstructionID::INSN_LDEORA, std::nullopt}},
            {0b110110010, {InstructionID::INSN_LDEORAL, std::nullopt}},
            {0b110000011, {InstructionID::INSN_LDSET, InstructionID::INSN_STSET}},
            {0b110010011, {InstructionID::INSN_LDSETL, InstructionID::INSN_STSETL}},
            {0b110100011, {InstructionID::INSN_LDSETA, std::nullopt}},
            {0b110110011, {InstructionID::INSN_LDSETAL, std::nullopt}},
            {0b110000100, {InstructionID::INSN_LDSMAX, InstructionID::INSN_STSMAX}},
            {0b110010100, {InstructionID::INSN_LDSMAXL, InstructionID::INSN_STSMAXL}},
            {0b110100100, {InstructionID::INSN_LDSMAXA, std::nullopt}},
            {0b110110100, {InstructionID::INSN_LDSMAXAL, std::nullopt}},
            {0b110000101, {InstructionID::INSN_LDSMIN, InstructionID::INSN_STSMIN}},
            {0b110010101, {InstructionID::INSN_LDSMINL, InstructionID::INSN_STSMINL}},
            {0b110100101, {InstructionID::INSN_LDSMINA, std::nullopt}},
            {0b110110101, {InstructionID::INSN_LDSMINAL, std::nullopt}},
            {0b110000110, {InstructionID::INSN_LDUMAX, InstructionID::INSN_STUMAX}},
            {0b110010110, {InstructionID::INSN_LDUMAXL, InstructionID::INSN_STUMAXL}},
            {0b110100110, {InstructionID::INSN_LDUMAXA, std::nullopt}},
            {0b110110110, {InstructionID::INSN_LDUMAXAL, std::nullopt}},
            {0b110000111, {InstructionID::INSN_LDUMIN, InstructionID::INSN_STUMIN}},
            {0b110010111, {InstructionID::INSN_LDUMINL, InstructionID::INSN_STUMINL}},
            {0b110100111, {InstructionID::INSN_LDUMINA, std::nullopt}},
            {0b110110111, {InstructionID::INSN_LDUMINAL, std::nullopt}},
            {0b110001000, {InstructionID::INSN_SWP, std::nullopt}},
            {0b110011000, {InstructionID::INSN_SWPL, std::nullopt}},
            {0b110101000, {InstructionID::INSN_SWPA, std::nullopt}},
            {0b110111000, {InstructionID::INSN_SWPAL, std::nullopt}},
            {0b110001010, {InstructionID::INSN_ST64BVO, std::nullopt}},
            {0b110001011, {InstructionID::INSN_ST64BV, std::nullopt}},
            {0b110101100, {InstructionID::INSN_LDAPR, std::nullopt}}
        };

        static const std::unordered_map<unsigned short int, InstructionID> insnTable2 = {
            {0b001000000, InstructionID::INSN_LDBFADD},
            {0b001010000, InstructionID::INSN_LDBFADDL},
            {0b001100000, InstructionID::INSN_LDBFADDA},
            {0b001110000, InstructionID::INSN_LDBFADDAL},
            {0b001000100, InstructionID::INSN_LDBFMAX},
            {0b001010100, InstructionID::INSN_LDBFMAXL},
            {0b001100100, InstructionID::INSN_LDBFMAXA},
            {0b001110100, InstructionID::INSN_LDBFMAXAL},
            {0b001000101, InstructionID::INSN_LDBFMIN},
            {0b001010101, InstructionID::INSN_LDBFMINL},
            {0b001100101, InstructionID::INSN_LDBFMINA},
            {0b001110101, InstructionID::INSN_LDBFMINAL},
            {0b001000110, InstructionID::INSN_LDBFMAXNM},
            {0b001010110, InstructionID::INSN_LDBFMAXNML},
            {0b001100110, InstructionID::INSN_LDBFMAXNMA},
            {0b001110110, InstructionID::INSN_LDBFMAXNMAL},
            {0b001000111, InstructionID::INSN_LDBFMINNM},
            {0b001010111, InstructionID::INSN_LDBFMINNML},
            {0b001100111, InstructionID::INSN_LDBFMINNMA},
            {0b001110111, InstructionID::INSN_LDBFMINNMAL},
            {0b011000000, InstructionID::INSN_LDFADD},
            {0b011010000, InstructionID::INSN_LDFADDL},
            {0b011100000, InstructionID::INSN_LDFADDA},
            {0b011110000, InstructionID::INSN_LDFADDAL},
            {0b011000100, InstructionID::INSN_LDFMAX},
            {0b011010100, InstructionID::INSN_LDFMAXL},
            {0b011100100, InstructionID::INSN_LDFMAXA},
            {0b011110100, InstructionID::INSN_LDFMAXAL},
            {0b011000101, InstructionID::INSN_LDFMIN},
            {0b011010101, InstructionID::INSN_LDFMINL},
            {0b011100101, InstructionID::INSN_LDFMINA},
            {0b011110101, InstructionID::INSN_LDFMINAL},
            {0b011000110, InstructionID::INSN_LDFMAXNM},
            {0b011010110, InstructionID::INSN_LDFMAXNML},
            {0b011100110, InstructionID::INSN_LDFMAXNMA},
            {0b011110110, InstructionID::INSN_LDFMAXNMAL},
            {0b011000111, InstructionID::INSN_LDFMINNM},
            {0b011010111, InstructionID::INSN_LDFMINNML},
            {0b011100111, InstructionID::INSN_LDFMINNMA},
            {0b011110111, InstructionID::INSN_LDFMINNMAL},
            {0b101000000, InstructionID::INSN_LDFADD},
            {0b101010000, InstructionID::INSN_LDFADDL},
            {0b101100000, InstructionID::INSN_LDFADDA},
            {0b101110000, InstructionID::INSN_LDFADDAL},
            {0b101000100, InstructionID::INSN_LDFMAX},
            {0b101010100, InstructionID::INSN_LDFMAXL},
            {0b101100100, InstructionID::INSN_LDFMAXA},
            {0b101110100, InstructionID::INSN_LDFMAXAL},
            {0b101000101, InstructionID::INSN_LDFMIN},
            {0b101010101, InstructionID::INSN_LDFMINL},
            {0b101100101, InstructionID::INSN_LDFMINA},
            {0b101110101, InstructionID::INSN_LDFMINAL},
            {0b101000110, InstructionID::INSN_LDFMAXNM},
            {0b101010110, InstructionID::INSN_LDFMAXNML},
            {0b101100110, InstructionID::INSN_LDFMAXNMA},
            {0b101110110, InstructionID::INSN_LDFMAXNMAL},
            {0b101000111, InstructionID::INSN_LDFMINNM},
            {0b101010111, InstructionID::INSN_LDFMINNML},
            {0b101100111, InstructionID::INSN_LDFMINNMA},
            {0b101110111, InstructionID::INSN_LDFMINNMAL},
            {0b111000000, InstructionID::INSN_LDFADD},
            {0b111010000, InstructionID::INSN_LDFADDL},
            {0b111100000, InstructionID::INSN_LDFADDA},
            {0b111110000, InstructionID::INSN_LDFADDAL},
            {0b111000100, InstructionID::INSN_LDFMAX},
            {0b111010100, InstructionID::INSN_LDFMAXL},
            {0b111100100, InstructionID::INSN_LDFMAXA},
            {0b111110100, InstructionID::INSN_LDFMAXAL},
            {0b111000101, InstructionID::INSN_LDFMIN},
            {0b111010101, InstructionID::INSN_LDFMINL},
            {0b111100101, InstructionID::INSN_LDFMINA},
            {0b111110101, InstructionID::INSN_LDFMINAL},
            {0b111000110, InstructionID::INSN_LDFMAXNM},
            {0b111010110, InstructionID::INSN_LDFMAXNML},
            {0b111100110, InstructionID::INSN_LDFMAXNMA},
            {0b111110110, InstructionID::INSN_LDFMAXNMAL},
            {0b111000111, InstructionID::INSN_LDFMINNM},
            {0b111010111, InstructionID::INSN_LDFMINNML},
            {0b111100111, InstructionID::INSN_LDFMINNMA},
            {0b111110111, InstructionID::INSN_LDFMINNMAL}
        };

        static const std::unordered_map<unsigned short int, InstructionID> insnTable3 = {
            {0b00100100011111, InstructionID::INSN_STBFADD},
            {0b00101100011111, InstructionID::INSN_STBFADDL},
            {0b00100110011111, InstructionID::INSN_STBFMAX},
            {0b00101110011111, InstructionID::INSN_STBFMAXL},
            {0b00100110111111, InstructionID::INSN_STBFMIN},
            {0b00101110111111, InstructionID::INSN_STBFMINL},
            {0b00100111011111, InstructionID::INSN_STBFMAXNM},
            {0b00101111011111, InstructionID::INSN_STBFMAXNML},
            {0b00100111111111, InstructionID::INSN_STBFMINNM},
            {0b00101111111111, InstructionID::INSN_STBFMINNML},
            {0b01100100011111, InstructionID::INSN_STFADD},
            {0b01101100011111, InstructionID::INSN_STFADDL},
            {0b01100110011111, InstructionID::INSN_STFMAX},
            {0b01101110011111, InstructionID::INSN_STFMAXL},
            {0b01100110111111, InstructionID::INSN_STFMIN},
            {0b01101110111111, InstructionID::INSN_STFMINL},
            {0b01100111011111, InstructionID::INSN_STFMAXNM},
            {0b01101111011111, InstructionID::INSN_STFMAXNML},
            {0b01100111111111, InstructionID::INSN_STFMINNM},
            {0b01101111111111, InstructionID::INSN_STFMINNML},
            {0b10100100011111, InstructionID::INSN_STFADD},
            {0b10101100011111, InstructionID::INSN_STFADDL},
            {0b10100110011111, InstructionID::INSN_STFMAX},
            {0b10101110011111, InstructionID::INSN_STFMAXL},
            {0b10100110111111, InstructionID::INSN_STFMIN},
            {0b10101110111111, InstructionID::INSN_STFMINL},
            {0b10100111011111, InstructionID::INSN_STFMAXNM},
            {0b10101111011111, InstructionID::INSN_STFMAXNML},
            {0b10100111111111, InstructionID::INSN_STFMINNM},
            {0b10101111111111, InstructionID::INSN_STFMINNML},
            {0b11100100011111, InstructionID::INSN_STFADD},
            {0b11101100011111, InstructionID::INSN_STFADDL},
            {0b11100110011111, InstructionID::INSN_STFMAX},
            {0b11101110011111, InstructionID::INSN_STFMAXL},
            {0b11100110111111, InstructionID::INSN_STFMIN},
            {0b11101110111111, InstructionID::INSN_STFMINL},
            {0b11100111011111, InstructionID::INSN_STFMAXNM},
            {0b11101111011111, InstructionID::INSN_STFMAXNML},
            {0b11100111111111, InstructionID::INSN_STFMINNM},
            {0b11101111111111, InstructionID::INSN_STFMINNML}
        };

        static const std::unordered_map<unsigned short int, InstructionID> insnTable4 = {
            {0b000001001, InstructionID::INSN_RCWCLR},
            {0b000011001, InstructionID::INSN_RCWCLRL},
            {0b000101001, InstructionID::INSN_RCWCLRA},
            {0b000111001, InstructionID::INSN_RCWCLRAL},
            {0b000001010, InstructionID::INSN_RCWSWP},
            {0b000011010, InstructionID::INSN_RCWSWPL},
            {0b000101010, InstructionID::INSN_RCWSWPA},
            {0b000111010, InstructionID::INSN_RCWSWPAL},
            {0b000001011, InstructionID::INSN_RCWSET},
            {0b000011011, InstructionID::INSN_RCWSETL},
            {0b000101011, InstructionID::INSN_RCWSETA},
            {0b000111011, InstructionID::INSN_RCWSETAL},
            {0b010001001, InstructionID::INSN_RCWSCLR},
            {0b010011001, InstructionID::INSN_RCWSCLRL},
            {0b010101001, InstructionID::INSN_RCWSCLRA},
            {0b010111001, InstructionID::INSN_RCWSCLRAL},
            {0b010001010, InstructionID::INSN_RCWSSWP},
            {0b010011010, InstructionID::INSN_RCWSSWPL},
            {0b010101010, InstructionID::INSN_RCWSSWPA},
            {0b010111010, InstructionID::INSN_RCWSSWPAL},
            {0b010001011, InstructionID::INSN_RCWSSET},
            {0b010011011, InstructionID::INSN_RCWSSETL},
            {0b010101011, InstructionID::INSN_RCWSSETA},
            {0b010111011, InstructionID::INSN_RCWSSETAL}
        };

        const static std::unordered_map<unsigned short int, InstructionID> insnTable5 = {
            {0b11000111111001, InstructionID::INSN_ST64B},
            {0b11000111111101, InstructionID::INSN_LD64B}
        };

        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rn, 64));

        const unsigned short int encoding = (size << 7) | (VR << 6) | (A << 5) | (R << 4) | (o3 << 3) | opc;
        if (const auto it{insnTable1.find(encoding)}; it != insnTable1.end())
        {
            const auto &[insn, alias]{it->second};
            unsigned short int regSize = size == 0b11 ? 64 : 32;
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rs, regSize));
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, regSize));

            return std::make_pair(alias ? alias.value() : insn, std::move(this->m_Operands));
        }
        else if (const auto it2{insnTable2.find(encoding)}; it2 != insnTable2.end())
        {
            constexpr std::array<unsigned short int, 4> sizeTable{16, 16, 32, 64};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rs, sizeTable.at(size)));
               this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rt, sizeTable.at(size)));
           
            return std::make_pair(it2->second, std::move(this->m_Operands));
        }
        else if (const auto it3{insnTable3.find(encoding)}; it3 != insnTable3.end())
        {
              this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::Register>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rs, 64));
              this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));
          
            return std::make_pair(it3->second, std::move(this->m_Operands));
        }
        else if (const auto it4{insnTable4.find((encoding << 5) | 0b11111)}; it4 != insnTable4.end())
        {
               constexpr std::array<unsigned short int, 4> sizeTable{16, 16, 32, 64};
            this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_NEON, Rs, sizeTable.at(size)));

            return std::make_pair(it4->second, std::move(this->m_Operands));
        }
        
        auto it5{insnTable5.find((size << 12) | (VR << 11) | (A << 10) | (R << 9) | (Rs << 4) | (o3 << 3) | opc)};
        if (it5 == insnTable5.end()) [[unlikely]]
            return std::unexpected{disxx::utility::error::DisassemblyError{this->m_Insn}};
            
        this->m_Operands.emplace_back(std::make_unique<disxx::disasm::operand::LoadsAndStoresAddress>(disxx::disasm::operand::Register::Type::TYPE_GPR, Rt, 64));

        return std::make_pair(it5->second, std::move(this->m_Operands)); 
	}
} /* disxx::disasm::decoder::LoadsAndStores::AtomicMemoryOperations */
