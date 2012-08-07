#!/usr/bin/env ruby

require 'stringio'
require 'fileutils'
require 'yaml'

PROJECT_PATH = File.dirname(__FILE__)
SOURCE_PATH = File.join(PROJECT_PATH, "src")
INCLUDE_PATH = File.join(PROJECT_PATH, "include", "Dream")
CPP_SOURCE = ['h', 'c', 'cpp']

Dir["./tasks/*.rake"].each do |path|
	load path
end