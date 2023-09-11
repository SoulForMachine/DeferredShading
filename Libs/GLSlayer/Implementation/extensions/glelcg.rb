

def ParseCmdLine
	if ARGV.length == 0
		puts "\tusage: ruby glelcg.rb ext_header [filter_file] [output_files_basename]"
		return false
	end
	
	$inputFileName = ARGV[0]
	$filterFileName = ARGV[1]
	if ARGV.length >= 3
		$outputFileName = ARGV[2]
	else
		$outputFileName = ""
	end
	
	return true
end

def ParseFilterFile
	return if $filterFileName.nil? or $filterFileName.empty?

	begin
		in_file = File.new($filterFileName, "r");
		$extensionsToProcess = Array.new
		
		while line = in_file.gets
			line.strip!
			if not line.empty? and line[0] != ?#
				$extensionsToProcess.push line
			end
		end
		
		$extensionsToProcess.uniq!
	rescue
		puts "Error processing filter file. Processing all extensions."
	ensure
		in_file.close unless in_file.nil?
	end
end

def OkToProcess(ext)
	return true unless $extensionsToProcess
	result = $extensionsToProcess.find { |str| str == ext }
	if result
		$extensionsToProcess.delete result
		return true
	else
		return false
	end
end

def GenerateSource
	return false if $inputFileName.nil? or $inputFileName.empty?

	result = true
	
	begin
		in_file = File.new($inputFileName, "r")
		
		dot_index = $inputFileName.rindex('.')
		if dot_index == nil
			fpath = $inputFileName
		else
			fpath = $inputFileName.slice(0, dot_index)
		end
			
		bslash_index = fpath.rindex('\\')
		if bslash_index == nil
			in_fname = fpath
		else
			in_fname = fpath.slice(bslash_index + 1, fpath.length)
		end

		if $outputFileName == ""
			out_fname_base = in_fname
		else
			out_fname_base = $outputFileName
		end
		
		c_file = File.new("#{out_fname_base}_ptrs.inc", "w")
		h_file = File.new("#{out_fname_base}_ptrs.h", "w")

		ext_name = ""
		while line = in_file.gets()
			if line =~ /\s*#ifndef\s+(GL_|WGL_|GLX_)/
				ext_name = line.slice(/\s+(GL_|WGL_|GLX_)[a-z|A-Z|0-9|_]+/)
				ext_name.strip!
			elsif !ext_name.empty? && line =~ /#ifdef.+_PROTOTYPES/
				if OkToProcess(ext_name)
					h_file << "\n// #{ext_name}\n"
					h_file << "\nEXTPTR bool glextLoad_#{ext_name}();\n\n"
						
					c_file << "\n// #{ext_name}\n"
					c_file << "\nbool glextLoad_#{ext_name}()\n{\n"
						
					while line = in_file.gets()
						break if line =~ /\s*#endif.*/
							
						line = line.slice(/[a-z|A-Z|0-9|_]+\s*\(/)
						func_name = line.slice(/[a-z|A-Z|0-9|_]+/)
						func_type = "PFN#{func_name.upcase}PROC"
							
						h_file << "EXTPTR #{func_type} #{func_name};\n"
							
						c_file << "\tINIT_FUNC_PTR(#{func_name});\n"
					end
						
					c_file << "\n\treturn true;\n}\n"
				end
			end
		end
		
	rescue Exception => ex
		puts "Error occured while processing extension file."
		puts ex.message
		result = false
	ensure
		in_file.close() unless in_file.nil?
		c_file.close() unless c_file.nil?
		h_file.close() unless h_file.nil?
	end

	return result
end

def DumpUnprocessedExts
	if $extensionsToProcess and not $extensionsToProcess.empty?
		puts "The following extensions were in the filter file but were not found or they had no entry points:\n\n"
		
		$extensionsToProcess.each do |ext| 
			puts "\t#{ext}"
		end
	end
end


if ParseCmdLine()
	ParseFilterFile()
	if GenerateSource()
		DumpUnprocessedExts()
		puts "Done."
	end
end
