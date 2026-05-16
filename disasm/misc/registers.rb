#!/usr/bin/ruby

require "optparse"
require "parser"

class RegistersParser < AbstractParser
  public
	def initialize(input)
		@registers = Hash.new {|hash, key| hash[key] = []} 
		super(input, 3)
	end

	def WriteInterface(path)
		puts "Writing #{path}..."

		File.open(path, 'w') do |file|
			file.write _Declare(true)

			# Declare enums for all registers
			@registers.each_key do |type|
                file.write "\tenum class #{type.upcase} : unsigned short int;\n"
            end

			file.write "\n\tclass __DISXX_PRIVATE__ RegsTable\n\t{\n\t  public:\n"
			@registers.each_key do |type|
                file.write "\t\tstatic const std::unordered_map<#{type.upcase}, const char *> m_s#{type.upcase}Table;\n"
            end

			file.write "\t};\n} /* operand */\n\n"
		end
	end

	def WriteImpl(path)
		puts "Writing #{path}..."

		File.open(path, 'w') do |file|
			#file.write _Declare(false)
			file.write _Declare(true)

			#ADDED
			@registers.each_key do |type|
                file.write "\tenum class #{type.upcase} : unsigned short int;\n"
            end

            file.write "\n\tclass __DISXX_PRIVATE__ RegsTable\n\t{\n\t  public:\n"
            @registers.each_key do |type|
                file.write "\t\tstatic const std::unordered_map<#{type.upcase}, const char *> m_s#{type.upcase}Table;\n"
            end

			file.write "\t};\n} /* disxx::disasm::operand */\n\n"
			file.write "namespace disxx::disasm::operand\n{\n"	
			#END	
			# Define enums for all registers	
            @registers.each_key do |type|
                file.write "\tenum class #{type.upcase} : unsigned short int\n\t{\n"
                @registers[type].each do |reg|
                    file.write "\t\tREG_#{reg.regname.upcase} = #{reg.regval},\n"
                end
                file.write "\t};\n"
            end
		
			file.write "\n"
	
			# Define tables for all registers
			@registers.each_key do |type|
				file.write "\tconst std::unordered_map<#{type.upcase}, const char *> RegsTable::m_s#{type.upcase}Table = {\n"
				@registers[type].each do |reg|
					file.write "\t\t{#{reg.regtype.upcase}::REG_#{reg.regname.upcase}, \"#{reg.regname}\"},\n"
				end

				file.write "\t};\n"
			end

			file.write "} /* disxx::disasm::operand */" 
		end	
	end

  protected
	def _SetData(fields, fieldsCount)
		fields.each_slice fieldsCount do |slice|
			reg = Register.new(*slice)
			case reg.regtype
            when "GPR32", "GPR64", "Neon8", "Neon16", "Neon32", "Neon64", "Neon128", "Neon128v", "SysReg"
                @registers[reg.regtype] << reg
            else
                raise "RegisterTypeError: #{reg.regtype} - Unknown type"
			end
		end
	end

  private
	class Register
      public
        attr_accessor :regname, :regtype, :regval

        def initialize(regname, regtype, regval)
            @regname = regname
            @regtype = regtype
            @regval = regval
        end
    end

	def _Declare(isImpl)
		# Write comment
		str = "/*\n *  This file is auto-generated from register.dat by the #{__FILE__}, - don't edit!\n"
		str += " *  This file in included by disxx.disasm.operand.Register.cpp\n */\n\n"
	
		# Write includes
		str += "module;\n\n"
		str += "#include <disconf.hpp>\n\n"
		str += "#include <unordered_map>\n\n"
		str += "#{isImpl ? "export " : ""}module disxx.disasm.operand.Register:RegsTable;\n\n"

		# Write namespace
		str += "#{isImpl ? "export " : ""}namespace disxx::disasm::operand\n{\n"

		return str
	end
end

if __FILE__ == $0
	options = {}
	OptionParser.new do |opts|
		opts.banner = "Auto-generation script for RegsTable.cpp\n"
		opts.banner += "USAGE: #{__FILE__} --output-file=<output> --input-file=<input>"
		
		
		opts.on("--input-file=<input>", "Input file with registers, classes and values") do |input|
			raise "InputFileError: #{input}" unless input =~ /[a-zA-Z0-9_]+\.dat$/
			options[:input] = input
		end

		opts.on("--interface-file=<interface>", "Interface file with defenition of enums") do |interface|
			raise "FileTypeError: #{interface}" unless interface =~ /[a-zA-Z0-9_\.]+\.cppm$/
			options[:interface] = interface
		end

		opts.on("--impl-file=<impl>", "Implementation file with generated enums and tables of the registers") do |impl|
			raise "FileTypeError: #{impl}" unless impl =~ /[a-zA-Z0-9_\.]+\.cpp$/
			options[:impl] = impl 
		end
	end.parse!
	
	regs = RegistersParser.new(options[:input])

	#if options[:interface]
	#	regs.WriteInterface options[:interface]
	#end
	#if options[:impl]
	#	regs.WriteImpl options[:impl]
	#end

	regs.WriteImpl options[:interface]
end
