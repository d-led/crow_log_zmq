#!/usr/bin/env python
import time
import sys
import zmq

context = zmq.Context()
push = context.socket(zmq.PUSH)
server = "localhost" if len(sys.argv)==1 else sys.argv[1]
print("connecting to "+server)
push.connect("tcp://"+server+":18090")
count = 100000
total = 0

while True:
	begin = time.time()
	for num in range(count):
    		push.send(str(num))
	end = time.time()
	total += count
	print("{}/s, total: {}".format(count/(end-begin),total))
