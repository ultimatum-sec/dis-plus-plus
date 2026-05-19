if __FILE__ == $0
	Dir.foreach('.') do |entry|
		if File.file?(entry) && entry =~ /\S+\.cppm$/
			contents = Array::new()
			
			File.open(entry, 'r') do |file|
				file.readlines.each do |line|
					contents << line.gsub(/::Decoder/, '::SubDecoder')
				end
			end

			File.open(entry, 'w') do |file|
				contents.each do |line|
					file.write line
				end
			end
		end
	end
end
