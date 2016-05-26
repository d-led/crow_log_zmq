# crow + g3log + 0mq == a simple log server #

> no warranties / use at your own risk

- [crow](https://github.com/ipkn/crow) is a C++11 micro web framework
- [g3log](https://github.com/KjellKod/g3log) is an asynchronous logging library
- [ØMQ](http://zero.mq/) is a set of communication protocols their implementations for the XXI century

This project is a hommage to the aforementioned libraries. Its own feature set is arbitrary and will grow at will, but it builds upon the tiny giants mentioned above. 

<!-- [![Build Status](https://travis-ci.org/d-led/crow_example.svg)](https://travis-ci.org/d-led/crow_example) -->

## Log via HTTP ##

using [httpie](http://httpie.org):

```
echo "bla\c" | http put http://localhost:18080/log
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

<!-- 
813008/s, total: 100000
452489/s, total: 200000
117096/s, total: 300000
182149/s, total: 400000
154321/s, total: 500000
132450/s, total: 600000
134953/s, total: 700000
134953/s, total: 800000
253165/s, total: 900000
156006/s, total: 1000000
138313/s, total: 1100000
200000/s, total: 1200000
90497.7/s, total: 1300000
135685/s, total: 1400000
228311/s, total: 1500000
134953/s, total: 1600000
... -->