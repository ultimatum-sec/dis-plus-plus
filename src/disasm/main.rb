def walkdir(path)
	Dir.foreach(path) do |entry|
		next if entry =~ /\.$/
		if File.directory? "#{path}/#{entry}"
			walkdir "#{path}/#{entry}"
		elsif File.file? "#{path}/#{entry}"
			contents = Array::new()
		
			if entry =~ /\.cpp$/	
				File.open("#{path}/#{entry}", 'r') do |file|
					file.readlines.each do |line|
						contents << line.gsub("if (this != &other) [[likely]]", "if (this != &other)").gsub("decltype(*this)", "std::decay_t<decltype(*this)>")
						#contents << line.gsub("import disxx.disasm.InstructionID;", "import disxx.disasm.InstructionID;\nimport disxx.disasm.utility.bits;")
						#contents << line.gsub("this->m_PC", "this->m_ProgramCounter")
					end
				end

				File.open("#{path}/#{entry}", 'w') do |file|
					contents.each do |line|
						file.write line
					end
				end

				#puts "import #{entry.gsub /\.cpp$/, ''};"
			end
		end
	end
end

if __FILE__ == $0
	walkdir "./decoder/BranchesExceptionsAndSystemInstructions/SubDecoder"
end
