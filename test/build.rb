
compile_executable("dream-test-runner") do
	def source_files(environment)
		FileList[root, "**/*.cpp"]
	end
end
