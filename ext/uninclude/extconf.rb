require 'mkmf'

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

$CFLAGS << " #{ENV["CFLAGS"]}"

if RUBY_VERSION >= '2.1.0'
  $CFLAGS << ' -DRUBY_2_1_x'
end

create_makefile('uninclude/uninclude')
