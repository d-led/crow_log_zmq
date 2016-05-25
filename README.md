# crow + g3log + 0mq == a simple log server #

- [crow](https://github.com/ipkn/crow) is a C++11 micro web framework
- [g3log](https://github.com/KjellKod/g3log) is an asynchronous logging library

<!-- [![Build Status](https://travis-ci.org/d-led/crow_example.svg)](https://travis-ci.org/d-led/crow_example) -->

## Log via HTTP ##

using [httpie](http://httpie.org):

```
echo bla | http put http://localhost:18080/log
```

## Log via 0mq ##

i.e. in Python

(`apt-get install python-pip python-dev libzmq3-dev python3-zmq`)

```python
#!/usr/bin/env python
import zmq

context = zmq.Context()
push = context.socket(zmq.PUSH)
server = "localhost"
push.connect("tcp://"+server+":18090")
push.send("bla")
```

## Toggle console logging of Crow ##

```
http get http://localhost:18080/toggle_logging
```

## Stop the server (rather) gracefully ##

```
http get http://localhost:18080/kill
```

## Source ##

- [cg3lz (server)](src/main.cpp)
- [log_some](src/log_some.py)
- [build config](premake5.lua)
