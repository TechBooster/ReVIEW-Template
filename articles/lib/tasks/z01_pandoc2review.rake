# Copyright (c) 2020-2022 Kenshi Muto
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

require 'fileutils'
require 'yaml'
require 'date'

def make_mdre(ch, p2r, path)
  if File.exist?(ch) # re file
    FileUtils.cp(ch, path)
  elsif File.exist?(ch.sub(/\.re\Z/, '.md')) # md file
    system("#{p2r} #{ch.sub(/\.re\Z/, '.md')} > #{path}/#{ch}")
  end
end

def yaml_load_file_compatible(file)
  if YAML.respond_to?(:safe_load_file)
    YAML.safe_load_file(file, aliases: true, permitted_classes: [Date])
  else
    File.open(file, 'rt:bom|utf-8') do |f|
      begin
        # < Ruby 3.1
        YAML.safe_load(f, filename: file, aliases: true, permitted_classes: [Date])
      rescue ArgumentError
        # < Ruby 2.7
        YAML.safe_load(f, [Date])
      rescue Psych::DisallowedClass
        # < Ruby 2.5
        YAML.safe_load(File.read(file), [Date])
      end
    end
  end
end

desc 'run pandoc2review'
task :pandoc2review do
  config = yaml_load_file_compatible('config.yml')
  if config['contentdir'] == '_refiles'
    path = '_refiles'
    p2r = 'pandoc2review'

    unless File.exist?(path)
      Dir.mkdir(path)
      File.write("#{path}/THIS_FOLDER_IS_TEMPORARY", '')
    end

    catalog = yaml_load_file_compatible('catalog.yml')
    %w(PREDEF CHAPS APPENDIX POSTDEF).each do |block|
      if catalog[block].kind_of?(Array)
        catalog[block].each do |ch|
          if ch.kind_of?(Hash) # Parts
            ch.each_pair do |k, v|
              make_mdre(k, p2r, path)
              # Chapters
              v.each {|subch| make_mdre(subch, p2r, path) }
            end
          elsif ch.kind_of?(String) # Chapters
            make_mdre(ch, p2r, path)
          end
        end
      end
    end
  end
end

CLEAN.include('_refiles')
Rake::Task[BOOK_PDF].enhance([:pandoc2review])
Rake::Task[BOOK_EPUB].enhance([:pandoc2review])
Rake::Task[WEBROOT].enhance([:pandoc2review])
Rake::Task[TEXTROOT].enhance([:pandoc2review])
Rake::Task[TOPROOT].enhance([:pandoc2review])
Rake::Task[IDGXMLROOT].enhance([:pandoc2review])
