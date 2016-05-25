# crow + g3log + 0mq == a simple log server #

> no warranties / use at your own risk

- [crow](https://github.com/ipkn/crow) is a C++11 micro web framework
- [g3log](https://github.com/KjellKod/g3log) is an asynchronous logging library
- [ØMQ](http://zero.mq/) is a communication protocol and a set of library for the XXI century

This project is a hommage to the aforementioned libraries. Its own feature set is arbitrary and will grow at will, but it builds upon the tiny giants mentioned above. 

<!-- [![Build Status](https://travis-ci.org/d-led/crow_example.svg)](https://travis-ci.org/d-led/crow_example) -->

## Log via HTTP ##

using [httpie](http://httpie.org):

```
echo bla | http put http://localhost:18080/log
```

## Log via 0mq ##

i.e. in Python:

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
