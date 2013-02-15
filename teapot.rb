
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

required_version "0.6"

define_target "dream" do |target|
	target.install do |environment|
		install_directory(package.path, 'source', environment)
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
	target.install do |environment|
		install_directory(package.path, 'test', environment)
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
	
	target.provides "Test/Dream"
end
