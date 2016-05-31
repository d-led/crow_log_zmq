local zeromq = {}
-------------------------------
zeromq.root = {
    x32 = [[C:\Program Files (x86)\ZeroMQ 4.0.4\]],
    x64 = [[C:\Program Files\ZeroMQ 4.0.4\]],
    osx = [[/usr/local/Cellar/zeromq/4.1.4/]] -- brew install zeromq
}

zeromq.lib = {
    win = 'libzmq-v120-mt-4_0_4',
    osx = 'zmq',
    lin = { 'zmq', 'pthread' }
}

zeromq.dirs = function()
    -- libzmq config
    configuration {'x32','windows'}
        includedirs { path.join(zeromq.root.x32,'include') }
        libdirs { path.join(zeromq.root.x32,'lib') }
    configuration {'x64','windows'}
        includedirs { path.join(zeromq.root.x64,'include') }
        libdirs { path.join(zeromq.root.x64,'lib') }
    configuration 'macosx'
        includedirs { path.join(zeromq.root.osx,'include') }
        libdirs { path.join(zeromq.root.osx,'lib') }
    configuration '*'
end

zeromq.deploy = function()
    configuration {'x32','windows'}
        postbuildcommands {
            [[xcopy "]]..path.getabsolute(path.join(zeromq.root.x32,"bin/*.dll")):gsub('/','\\')..[[" "$(TargetDir)" /s /d /y]]
        }
    configuration {'x64','windows'}
        postbuildcommands {
            [[xcopy "]]..path.getabsolute(path.join(zeromq.root.x64,"bin/*.dll")):gsub('/','\\')..[[" "$(TargetDir)" /s /d /y]]
        }
    configuration '*'
end

zeromq.link = function()
    configuration 'windows'
        links {
            -- 'dbghelp',
            zeromq.lib.win
        }
    configuration 'macosx'
        links {
            zeromq.lib.osx,
        }
    configuration 'linux'
        links {
            zeromq.lib.lin,
        }
    configuration '*'
end

return zeromq
