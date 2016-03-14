
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "1.0.0"

define_project "Dream" do |project|
	project.add_author "Samuel Williams"
	project.license = "MIT License"
	
	project.version = "1.0.0"
end

define_target "dream" do |target|
	target.build do
		source_root = target.package.path + 'source'
		
		copy headers: source_root.glob('Dream/**/*.{h,hpp}')
		
		build static_library: "Dream", source_files: source_root.glob('Dream/**/*.cpp')
	end
	
	target.depends :platform
	target.depends "Build/Files"
	target.depends "Build/Clang"
	target.depends "Language/C++11"
	
	target.depends "Library/utf8"
	target.depends "Library/Euclid"

	target.provides "Library/Dream" do
		append linkflags {install_prefix + "lib/libDream.a"}
	end
end

define_target "dream-tests" do |target|
	target.build do
		run tests: "Dream", source_files: target.package.path.glob('test/Dream/**/*.cpp')
	end
	
	target.depends "Library/UnitTest"
	target.depends "Library/Dream"

	target.provides "Test/Dream"
end

define_generator "dream.scene" do |generator|
	generator.description = <<-EOF
		Generates a basic scene file in the project.
		
		usage: teapot generate dream.scene Namespace::NamedScene
	EOF
	
	def scope_for_namespace(namespace)
		open = namespace.collect{|name| "namespace #{name}\n{\n"}
		close = namespace.collect{ "}\n" }
	
		return open + close
	end
	
	generator.generate do |class_name|
		*path, class_name = class_name.split(/::/)
		
		if path == []
			raise GeneratorError.new("You must specify a class name with a namespace!")
		end
		
		directory = Pathname('source') + path.join('/')
		directory.mkpath
		
		name = Name.new(class_name)
		substitutions = Substitutions.for_context(context)

		# e.g. Foo Bar, typically used as a title, directory, etc.
		substitutions['CLASS_NAME'] = name.identifier
		substitutions['CLASS_FILE_NAME'] = name.identifier

		# e.g. FooBar, typically used as a namespace
		substitutions['GUARD_NAME'] = name.header_guard

		# e.g. foo-bar, typically used for targets, executables
		substitutions['NAMESPACE'] = scope_for_namespace(path)

		# Copy the class files:
		generator.copy('templates/scene', directory, substitutions)

		# Copy some binary resources:
		resource_directory = Pathname('resources')
		generator.copy('templates/resources', resource_directory)
	end
end

define_configuration "dream" do |configuration|
	configuration.public!

	configuration.require "utf8"
	configuration.require "euclid"
	
	configuration.require "unit-test"
end

define_configuration "test" do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	
	configuration.require "platforms"
	configuration.require "build-files"
	configuration.import "dream"
end

