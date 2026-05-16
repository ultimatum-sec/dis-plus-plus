class AbstractParser
  public
	def initialize(input, fieldsCount)
		fields = []

		File.open(input, 'r') do |file|
            puts "Reading #{input}..."
            
			file.readlines.each do |line|
                line = line.sub /;.*$/, ''

                next if not line =~ /\w/
                line = line.split /\s+/
                raise "FieldsCountError" unless line.size == fieldsCount

                line.each do |field|
                    fields << field
                end
            end
        end

		_SetData fields, fieldsCount
	end

  protected
	def _SetData(fields, fieldsCount)
		raise NotImplementedError, "_SetData(#{fields}, #{fieldsCount})"
	end
end
