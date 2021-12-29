#!/usr/bin/env ruby

class String
	def no_windows; self.gsub("\r\n", "\n"); end
	def no_tabs; self.gsub("\t", ' '); end
	def no_brackets
		a = self
		['(', ')'].each { |b| a.gsub! b, '' }
		return a
	end
end

def show_help
	puts "Usage: #{$0} /path/to/opcodes.h"
	exit 1
end

def parse_opcodes(ctnt)
	result = {}

	ctnt.no_windows.split("\n").each do |line|
		rline = line.lstrip.no_tabs.no_brackets.split(' ')

		# detect opcode name
		if rline.size >= 3 and rline.shift == "#define" and rline.first.start_with? 'OP_'
			# create command name from opcode name
			name = rline.shift.split('_').last.downcase
			
			# value
			value = rline.first
			raise("Invalid opcode value for '#{name}' (got #{value.inspect})") if not value.start_with? '0x'

			value = value.to_i(16)

			# save it
			result[name] = value
		end
	end

	return result
end

source_header = ARGV.first
show_help if source_header.nil? or (not File.exists? source_header)

instructions_map = parse_opcodes(File.read(source_header))

puts "MARASMV0"
# generate output
instructions_map.each do |pp, pv|
	puts "#{pp}:#{pv}"
end
