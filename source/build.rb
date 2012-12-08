
# require 'teapot/build/component'

@variants = {}

def component(environment)
	variant_name = environment[:variant]
		
	@variants.fetch(variant_name) do
		component = Component.new(root, "Dream", environment)
			
		component.add("Dream")
			
		case environment[:platform_name]
		when /darwin-osx/
			component.add("Dream-NSFileManager")
			component.add("Dream-CoreVideo")
			component.add("Dream-Cocoa")
		when /darwin-ios/
			component.add("Dream-NSFileManager")
			component.add("Dream-CoreVideo")
			component.add("Dream-UIKit")
		when /linux/
			component.add("Dream-Unix")
			component.add("Dream-X11")
		end
			
		component.prepare!
			
		@variants[variant_name] = component
	end
end

compile_library 'Dream' do
	def source_files(environment)
		component = parent.component(environment)
		
		FileList[component.destination_path, 'Dream/**/*.{cpp,m,mm}']
	end
end

copy_headers do
	def source_files(environment)
		component = parent.component(environment)
		
		FileList[component.destination_path, 'Dream/**/*.{h,hpp}']
	end
end
