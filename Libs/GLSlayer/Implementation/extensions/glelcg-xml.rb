
require 'net/http'
require 'rexml/document'


class GLFunc
	attr_accessor :name, :retType, :params
	def initialize
		@name = ""
		@retType = ""
		@params = []
	end
end

class GLExtension
	attr_accessor :name, :functions
	def initialize(name)
		@name = name
		@functions = []
	end
	def empty?
		@functions.empty?
	end
	def flagName
		@name.split("_", 2)[1]
		#arr = @name.split("_", 2).slice(1..-1)
		#arr.collect{ |e|
		#	(!!(e =~ /^[-+]?[0-9]+$/)) ? "_" + e : e.capitalize
		#}
	end
end

class GLVersion
	attr_accessor :name, :functions, :extensions
	def initialize(name)
		@name = name
		@functions = []
		@extensions = []
	end
	def flagName
		@name.slice(3..-1)
	end
	def empty?
		@functions.empty? && @extensions.empty?
	end
end

def printUsage
	puts "\tusage: ruby glelcg-xml.rb [options] xml_file [output_files_basename]"
	puts "\toptions:"
	puts "\t\t-l                Use local file xml_file."
	puts "\t\t-u <source dir>   Process only GL functions that are used in source code."
	puts "\t\t-f <filter file>  Include only versions and extensions specified in filter file."
end

def parseCmdLine
	if ARGV.length < 1
		puts "Error: expecting at least xml file name."
		return false
	end

	index = 0
	$useLocalFile = false
	$onlyUsedInSource = false
	$filterFileName = ""

	while index < ARGV.length && ARGV[index][0] == "-"
		if ARGV[index] == "-l"
			$useLocalFile = true
		elsif ARGV[index] == "-u"
			index += 1
			if index < ARGV.length
				$onlyUsedInSource = true
				$sourcePath = ARGV[index]
			else
				puts "Error: expecting source directory path."
				return false
			end
		elsif ARGV[index] == "-f"
			index += 1
			if index < ARGV.length
				$filterFileName = ARGV[index]
			else
				puts "Error: expecting filter file path."
				return false
			end
		elsif ARGV[index] == "-h"
			return false
		else
			puts "Error: unknown option #{ARGV[index]}."
			return false
		end
		index += 1
	end

	if index < ARGV.length
		$xmlFile = ARGV[index]
		index += 1
	else
		puts "Error: expecting xml file name."
		return false
	end

	$extPrefix = File.basename($xmlFile, ".*").upcase

	if index < ARGV.length
		$outputFileName = ARGV[index]
		index += 1
	else
		$outputFileName = File.basename($xmlFile, ".*")
	end

	if index < ARGV.length
		puts "Error: too many arguments."
		return false
	end

	return true
end

def downloadRegistryXML(xmlName)
	begin
		puts "Downloading #{$xmlFile}..."
		url = "https://cvs.khronos.org/svn/repos/ogl/trunk/doc/registry/public/api/#{xmlName}"
		uri = URI.parse(url)
		http = Net::HTTP.new(uri.host, uri.port)
		http.use_ssl = true
		http.verify_mode = OpenSSL::SSL::VERIFY_NONE
		reply = http.get(uri.request_uri)
		return reply.body
	rescue Exception => ex
		puts "Failed to download #{xmlName} from the registry."
		puts ex.message
	end

	return nil
end

def loadRegistryXMLFromFile(xmlName)
	begin
		puts "Reading xml from file #{$xmlFile}..."
		xmlStr = File.read($xmlFile)
	rescue Exception => ex
		puts "Failed to load file #{xmlName}."
		puts ex.message
	end
end

def parseRegistryXML(xmlStr)
	begin
		puts "Parsing #{$xmlFile}..."
		doc = REXML::Document.new(xmlStr)

		# Parse function prototypes
		$glFunctions = {}
		doc.elements.each('registry/commands/command') do |cmdElement|
			func = GLFunc.new
			cmdElement.elements["proto"].children.each do |child|
				if child.is_a?(REXML::Text)
					func.retType += child.value
				elsif child.is_a?(REXML::Element)
					if child.name == "name"
						func.name = child.text
					else
						func.retType += child.text
					end
				end
			end

			cmdElement.each_element('param') do |paramElement|
				paramType = ""
				paramName = ""
				paramElement.children.each do |child|
					if child.is_a?(REXML::Text)
						paramType += child.value
					elsif child.is_a?(REXML::Element)
						if child.name == "name"
							paramName = child.text
						else
							paramType += child.text
						end
					end
				end

				# These parameter names are defined as macros so we need to change them.
				if paramName == "near"
					paramName = "nearVal"
				end

				if paramName == "far"
					paramName = "farVal"
				end

				func.params.push([paramType, paramName])
			end

			$glFunctions[func.name] = func
		end

		# Parse versions
		$glVersions = []
		doc.elements.each('registry/feature') do |featElement|
			verName = featElement.attributes["name"]
			if checkFilter(verName)
				# Add functions for each GL version
				glVer = GLVersion.new(verName)
				featElement.each_element('require') do |reqElement|
					comment = reqElement.attributes["comment"]
					if comment != nil
						# Add extension and it's functions
						if /(Reuse |Promoted )(\w+\s+)*(\w+_\w+)\s*\w*/ =~ comment
							extName = ($3.start_with? "GL_") ? $3 : "GL_#{$3}"
							checkFilter(extName)		# delete from filter list
							ext = GLExtension.new(extName)
							reqElement.each_element('command') do |cmdElement|
								func = cmdElement.attributes["name"]
								ext.functions.push(func) if checkIfUsedInSource(func)
							end
							if extName == "GL_ARB_imaging"
								# Make exeption for GL_ARB_imaging, do not check for extension
								# just add it's functions to current GL version.
								ext.functions.each do |func|
									glVer.functions.push(func) if checkIfUsedInSource(func)
								end
							else
								# Add extension if not already added;
								# If it's already added concatenate functions
								index = glVer.extensions.index{ |e| ext.name == e.name }
								if index.nil?
									glVer.extensions.push(ext)
								else
									glVer.extensions[index].functions.concat(ext.functions)
								end
							end
						end
					else
						# Add functions
						reqElement.each_element('command') do |cmdElement|
							func = cmdElement.attributes["name"]
							glVer.functions.push(func) if checkIfUsedInSource(func)
						end
					end
				end
				$glVersions.push(glVer)

				# Remove functions under <remove> tag
				featElement.each_element('remove/command') do |cmdElement|
					cmdName = cmdElement.attributes["name"]
					$glVersions.each do |ver|
						ver.functions.delete(cmdName)
					end
				end
			end
		end

		# Remove duplicate functions.
		$glVersions.each do |ver|
			ver.functions.each do |func|
				$glVersions.each do |ver2|
					ver2.extensions.each { |ext| ext.functions.delete(func) }
				end
			end
			ver.extensions.each do |ext|
				ext.functions.each do |func|
					$glVersions.each do |ver2|
						ver2.extensions.each { |ext2| ext2.functions.delete(func) if ext != ext2 }
					end
				end
			end
		end

		# Parse extensions
		$glExtensions = []
		doc.elements.each('registry/extensions/extension') do |extElement|
			extName = extElement.attributes["name"]
			if checkFilter(extName)
				ext = GLExtension.new(extName)
				extElement.each_element('require/command') do |cmdElement|
					func = cmdElement.attributes["name"]
					# Check if the function already exists in one of GL versions.
					alreadyExists = false
					$glVersions.each do |ver|
						if ver.functions.include?(func)
							alreadyExists = true
							break
						end
					end
					# Check if the function already exists in any other extension.
					$glExtensions.each do |otherExt|
						if otherExt.functions.include?(func)
							alreadyExists = true
							break
						end
					end
					# Add function to list if it does not exist and it's used in source.
					ext.functions.push(func) if not alreadyExists and checkIfUsedInSource(func)
				end
				$glExtensions.push(ext)
			end
		end

		return true
	rescue Exception => ex
		puts "Failed to parse registry xml."
		puts ex.message
		puts ex.backtrace
	end

	return false
end

def parseFilterFile
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
		puts "Loaded filter file: #{$filterFileName}"
	rescue
		puts "Error processing filter file. Processing all extensions."
	ensure
		in_file.close unless in_file.nil?
	end
end

# Check if ext is in filter list. Return true if list does not exist.
# If the list exists and ext is found, it is deleted from list and true
# is returned. If the list exists and ext is not found, false is returned.
def checkFilter(ext)
	return true unless $extensionsToProcess
	return (not $extensionsToProcess.delete(ext).nil?)
end

def generatePtrsDeclFile(outPtrsDeclFile)
	$glVersions.each do |ver|
		outPtrsDeclFile << "\n// #{ver.name}\n\n"

		ver.functions.each do |func|
			outPtrsDeclFile << "EXTPTR PFN#{func.upcase}PROC ptr_#{func};\n"
		end

		ver.extensions.each do |ext|
			outPtrsDeclFile << "\n// #{ext.name}\n\n"

			ext.functions.each do |func|
				outPtrsDeclFile << "EXTPTR PFN#{func.upcase}PROC ptr_#{func};\n"
			end
		end
	end

	$glExtensions.each do |ext|
		outPtrsDeclFile << "\n// #{ext.name}\n\n"

		ext.functions.each do |func|
			outPtrsDeclFile << "EXTPTR PFN#{func.upcase}PROC ptr_#{func};\n"
		end
	end

	outPtrsDeclFile.flush
	puts "Wrote #{outPtrsDeclFile.path}"
end

def generateLoadDeclFile(outLoadDeclFile)
	$glVersions.each do |ver|
		outLoadDeclFile << "bool glextLoad_#{ver.name}();\n"

		ver.extensions.each do |ext|
			outLoadDeclFile << "bool glextLoad_#{ext.name}();\n"
		end
	end

	$glExtensions.each do |ext|
		outLoadDeclFile << "bool glextLoad_#{ext.name}();\n"
	end

	outLoadDeclFile.flush
	puts "Wrote #{outLoadDeclFile.path}"
end

def writeExtLoadFunc(ext, outLoadDefFile)
	if ext.empty?
		outLoadDefFile << "\nbool GLRenderContext::glextLoad_#{ext.name}()\n{\n"
		outLoadDefFile << "\t_info.features#{$extPrefix}.#{ext.flagName} = IsExtSupported(\"#{ext.name}\");\n"
		outLoadDefFile << "\treturn _info.features#{$extPrefix}.#{ext.flagName};\n}\n"
	else
		outLoadDefFile << "\nbool GLRenderContext::glextLoad_#{ext.name}()\n{\n"
		outLoadDefFile << "\tbool result = IsExtSupported(\"#{ext.name}\");\n"
		outLoadDefFile << "\tif(!result)\n\t\treturn false;\n"
		ext.functions.each do |func|
			outLoadDefFile << "\tINIT_FUNC_PTR(#{func});\n"
		end
		outLoadDefFile << "\t_info.features#{$extPrefix}.#{ext.flagName} = result;\n\treturn result;\n}\n"
	end
end

def generateLoadDefFile(outLoadDefFile)
	$glVersions.each do |ver|
		outLoadDefFile << "\nbool GLRenderContext::glextLoad_#{ver.name}()\n{\n"
		outLoadDefFile << "\tbool result = true;\n"

		ver.functions.each do |func|
			outLoadDefFile << "\tINIT_FUNC_PTR(#{func});\n"
		end

		ver.extensions.each do |ext|
			outLoadDefFile << "\tresult = glextLoad_#{ext.name}() && result;\n"
		end
		outLoadDefFile << "\t_info.features#{$extPrefix}.#{ver.flagName} = result;\n\treturn result;\n}\n"

		ver.extensions.each do |ext|
			writeExtLoadFunc(ext, outLoadDefFile)
		end
	end

	$glExtensions.each do |ext|
		writeExtLoadFunc(ext, outLoadDefFile)
	end

	outLoadDefFile.flush
	puts "Wrote #{outLoadDefFile.path}"
end

def writeFunc(glFunc, outFuncsFile)
	params = glFunc.params.collect{ |p| "#{p[0]}#{p[1]}" }.join(", ")
	outFuncsFile << "\ninline #{glFunc.retType}#{glFunc.name}(#{params})\n{\n"
	outFuncsFile << "\tassert(ptr_#{glFunc.name});\n"
	args = glFunc.params.collect{ |p| "#{p[1]}" }.join(", ")

	if glFunc.retType == "void "
		outFuncsFile << "\tptr_#{glFunc.name}(#{args});\n"

		if glFunc.name != "glGetError"
			outFuncsFile << "#if defined(DEBUG_GL_CHECK_FOR_ERROR)\n"
			outFuncsFile << "\tGLenum error;\n"
			outFuncsFile << "\tassert((error = glGetError()) == GL_NO_ERROR);\n"
			outFuncsFile << "#endif\n"
		end

		outFuncsFile << "}\n"
	else
		outFuncsFile << "\t#{glFunc.retType}result;\n"
		outFuncsFile << "\tresult = ptr_#{glFunc.name}(#{args});\n"

		if glFunc.name != "glGetError"
			outFuncsFile << "#if defined(DEBUG_GL_CHECK_FOR_ERROR)\n"
			outFuncsFile << "\tGLenum error;\n"
			outFuncsFile << "\tassert((error = glGetError()) == GL_NO_ERROR);\n"
			outFuncsFile << "#endif\n"
		end

		outFuncsFile << "\treturn result;\n}\n"
	end
end

def generateFuncsFile(outFuncsFile)
	outFuncsFile << "\nGLenum glGetError();\n"

	$glVersions.each do |ver|
		outFuncsFile << "\n// #{ver.name}\n"
		ver.functions.each do |func|
			writeFunc($glFunctions[func], outFuncsFile)
		end

		ver.extensions.each do |ext|
			unless ext.empty?
				outFuncsFile << "\n// #{ext.name}\n"
				ext.functions.each do |func|
					writeFunc($glFunctions[func], outFuncsFile)
				end
			end
		end
	end

	$glExtensions.each do |ext|
		unless ext.empty?
			outFuncsFile << "\n// #{ext.name}\n"
			ext.functions.each do |func|
				writeFunc($glFunctions[func], outFuncsFile)
			end
		end
	end

	outFuncsFile.flush
	puts "Wrote #{outFuncsFile.path}"
end

def generateFeatureFlagsFile(outFeatureFlagsFile)
	outFeatureFlagsFile << "\nstruct Features#{$extPrefix}\n{\n"
	$glVersions.each do |ver|
		outFeatureFlagsFile << "\tbool #{ver.flagName} : 1;\n"

		ver.extensions.each do |ext|
			outFeatureFlagsFile << "\tbool #{ext.flagName} : 1;\n"
		end
	end

	$glExtensions.each do |ext|
		outFeatureFlagsFile << "\tbool #{ext.flagName} : 1;\n"
	end
	outFeatureFlagsFile << "};\n"

	outFeatureFlagsFile.flush
	puts "Wrote #{outFeatureFlagsFile.path}"
end

def generateSourceFiles
	begin
		puts "Generating source files..."
		outPtrsDeclFile = File.new("#{$outputFileName}_ptrs.h", "w")
		outLoadDeclFile = File.new("#{$outputFileName}_load_decl.h", "w")
		outLoadDefFile = File.new("#{$outputFileName}_load_def.inc", "w")
		outFuncsFile = File.new("#{$outputFileName}_funcs.h", "w")
		outFeatureFlagsFile = File.new("#{$outputFileName}_flags.h", "w")

		generatePtrsDeclFile(outPtrsDeclFile)
		generateLoadDeclFile(outLoadDeclFile)
		generateLoadDefFile(outLoadDefFile)
		generateFuncsFile(outFuncsFile)
		generateFeatureFlagsFile(outFeatureFlagsFile)
	rescue Exception => ex
		puts "Failed to generate source files."
		puts ex.message
		puts ex.backtrace
	ensure
		outPtrsDeclFile.close unless outPtrsDeclFile.nil?
		outLoadDeclFile.close unless outLoadDeclFile.nil?
		outLoadDefFile.close unless outLoadDefFile.nil?
		outFuncsFile.close unless outFuncsFile.nil?
		outFeatureFlagsFile.close unless outFeatureFlagsFile.nil?
	end
end

def getFuncsUsedInSource(srcPath)
	$glFunctionsUsed = []
	Dir.glob("#{srcPath}/*.{cpp,h}") do |file|
		src = File.read(file)
		src.scan(/(\W|\A)((gl|wgl|glX)[A-Z]\w+)\s*\(/) do |func|
			$glFunctionsUsed.push(func[1])
		end
	end

	$glFunctionsUsed.sort!
	$glFunctionsUsed.uniq!
	puts "Functions used in source: #{$glFunctionsUsed.length}"
end

def checkIfUsedInSource(func)
	return true unless $onlyUsedInSource
	return $glFunctionsUsed.include?(func)
end

# main

if parseCmdLine
	parseFilterFile
	if $onlyUsedInSource
		getFuncsUsedInSource($sourcePath)
	end

	if $useLocalFile
		xmlStr = loadRegistryXMLFromFile($xmlFile)
	else
		xmlStr = downloadRegistryXML($xmlFile)
	end

	if xmlStr != nil
		if parseRegistryXML(xmlStr)
			generateSourceFiles
		end
	end
else
	printUsage
end
