#!/usr/bin/ruby

if __FILE__ == $0
	contents = Array::new()

	File.open("./decoder/DataProcessingScalarFPAndAdvancedSIMD/SubDecoder/disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneous.SubDecoder.cpp", 'r') do |file|
		file.readlines.each do |line|
			contents << line.gsub("-> std::string_view", "-> std::expected<std::string_view, disxx::utility::error::DisassemblyError>")
		end
	end

	File.open("./decoder/DataProcessingScalarFPAndAdvancedSIMD/SubDecoder/disxx.disasm.decoder.DataProcessingScalarFPAndAdvancedSIMD.AdvancedSIMDTwoRegisterMiscellaneous.SubDecoder.cpp", 'w') do |file|
		contents.each do |line|
			file.write line
		end
	end
end
