
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
	
	target.run do |environment|
		run_executable("bin/dream-test-runner", environment)
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

def scope_for_namespace(namespace)
	open = namespace.collect{|name| "namespace #{name}\n{\n"}
	close = namespace.collect{ "}\n" }
	
	return open + close
end

define_generator "dream.scene" do |generator|
	generator.description = <<-EOF
		Generates a basic scene file in the project.
		
		usage: teapot generate dream.scene Namespace::NamedScene
	EOF
	
	generator.generate do |class_name|
		*path, class_name = class_name.split(/::/)
		
		if path == []
			raise GeneratorError.new("You must specify a class name with a namespace!")
		end
		
		directory = Pathname('source') + path.join('/')
		directory.mkpath
		
		name = Name.new(class_name)
		substitutions = Substitutions.new
		
		# e.g. Foo Bar, typically used as a title, directory, etc.
		substitutions['CLASS_NAME'] = name.identifier
		substitutions['CLASS_FILE_NAME'] = name.identifier
		
		# e.g. FooBar, typically used as a namespace
		substitutions['GUARD_NAME'] = name.macro(path) + '_H'
		
		# e.g. foo-bar, typically used for targets, executables
		substitutions['NAMESPACE'] = scope_for_namespace(path)
		
		# The user's current name:
		substitutions['AUTHOR_NAME'] = `git config --global user.name`.chomp!
		
		substitutions['PROJECT_NAME'] = context.project.name
		substitutions['LICENSE'] = context.project.license
		
		current_date = Time.new
		substitutions['DATE'] = current_date.strftime("%-d/%-m/%Y")
		substitutions['YEAR'] = current_date.strftime("%Y")
		
		generator.copy('templates/scene', directory, substitutions)
	end
end

define_configuration "dream-local" do |configuration|
	configuration[:source] = "../"
	configuration.import! "local"
	
	configuration.require "png"
	configuration.require "jpeg"
	
	configuration.require "freetype"
	
	configuration.require "ogg"
	configuration.require "vorbis"

	configuration.require "euclid"
end
