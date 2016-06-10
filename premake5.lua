include 'premake'

make_solution 'cg3lz'

-------------------------------
configuration 'windows'
	defines {
		'NOGDI',
	}
configuration '*'

-------------------------------

zeromq = assert(require 'zeromq')
zeromq.dirs()

boost = assert(dofile 'premake/recipes/boost.lua')
boost:set_defines()
boost:set_includedirs()
boost:set_libdirs()

includedirs {
	'deps/cppzmq',
	'deps/crow/include',
	'deps/crow/amalgamate',
	'deps/mstch/include',
	'deps/mstch/src',
	'deps/spdlog/include',
	'deps/picojson',
	'deps/picojson_serializer',
    'deps/websocketpp'
}

defines {
	'PICOJSON_USE_INT64'
}

function default_links()
	links { 'mstch' }

	zeromq.link()
	zeromq.deploy()

	configuration 'not windows'
		links {
			'boost_system',
			'boost_date_time',
			'boost_regex',
			'boost_filesystem',
			'pthread',
			'z'
		}
	configuration '*'
end

--------------------------------------------------------------------
make_static_lib('mstch', {
	'deps/mstch/src/**.cpp',
	'deps/mstch/src/**.hpp'
})

use_standard('c++11')

--------------------------------------------------------------------
make_console_app('cg3lz', {
	'src/cg3lz/**.*'
})
use_standard('c++14')
default_links()

--------------------------------------------------------------------
make_console_app('log_some', { 'src/log_some/**.*' })
use_standard('c++14')

zeromq.link()

configuration 'not windows'
	links {
		'pthread'
	}
configuration '*'
