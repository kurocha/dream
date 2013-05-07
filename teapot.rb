
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "0.8.0"

define_target "dream" do |target|
	target.build do |environment|
		build_directory(package.path, 'source', environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"

	target.depends "Library/png"
	target.depends "Library/jpeg"
	target.depends "Library/freetype"
	target.depends "Library/vorbis"
	
	target.depends "Library/OpenAL"
	target.depends "Library/OpenGL"
	target.depends "Aggregate/Display"
	
	target.depends "Library/Euclid"
	
	target.provides "Library/Dream" do
		append linkflags "-lDream"
	end
end

define_target "dream-tests" do |target|
	target.build do |environment|
		build_directory(package.path, 'test', environment)
	end
	
	target.depends :platform
	target.depends "Language/C++11"
	target.depends "Library/UnitTest"
	
	target.depends "Library/png"
	target.depends "Library/jpeg"
	target.depends "Library/freetype"
	target.depends "Library/vorbis"
	
	target.depends "Library/OpenAL"
	target.depends "Library/OpenGL"
	target.depends "Aggregate/Display"
	
	target.depends "Library/Euclid"
	target.depends "Library/Dream"
	
	target.provides "Test/Dream"
end

define_configuration "dream-local" do |configuration|
	configuration[:source] = "../"
	configuration.import! "local"
	
	configuration.require "png"
	configuration.require "jpeg"
	
	configuration.require "freetype"
	
	configuration.require "ogg"
	configuration.require "vorbis"

	# Unit testing
	configuration.require "unit-test"
	
	configuration.require "euclid"
end
