include 'premake'

make_solution 'cg3lz'

-------------------------------
configuration 'windows'
	defines {
		'NOGDI',
		'CROW_MSVC_WORKAROUND'
	}
configuration '*'
-------------------------------

boost = assert(dofile 'premake/recipes/boost.lua')
boost:set_defines()
boost:set_includedirs()
boost:set_libdirs()

includedirs {
	'deps/crow/include',
	'deps/crow/amalgamate',
	'deps/g3log_config',
	'deps/g3log/src',
}

--------------------------------------------------------------------
make_static_lib('g3log', {
	'deps/g3log/src/**.cpp',
	'deps/g3log/src/**.hpp'
})

configuration 'windows'
	excludes {
		'deps/g3log/src/crashhandler_unix.cpp'
	}
configuration 'not windows'
	excludes {
		'deps/g3log/src/*_windows.cpp',
	}

use_standard('c++14')

--------------------------------------------------------------------
make_console_app('cg3lz', { 'src/main.cpp' })
use_standard('c++14')

links { 'g3log' }

configuration 'not windows'
	links {
		'boost_system',
		'boost_date_time',
		'boost_regex',
		'boost_filesystem',
		'pthread'
	}
configuration '*'
