#!/usr/bin/env ruby

require 'rufus-scheduler'
require 'net/http'

scheduler = Rufus::Scheduler.new

def start(what,para)
    p = ''
    p = para unless para.nil?
    pipe = IO.popen(what + ' ' + p)
    pipe.each do |line|
        puts line.chomp
    end
    pipe.close
end

def start_latest(what,para)
    absolute_path_to_executable = (
            (Dir.glob("bin/**/"+ what +".exe")) +
            (Dir.glob("bin/**/"+ what        ))
        ).max_by {|f| File.mtime(f)}

    if !absolute_path_to_executable then
        puts "Could not find #{what}! Terminating."
        exit 1
    end

    # puts absolute_path_to_executable
    start(absolute_path_to_executable,para)
end

scheduler.in '1s' do
    puts "starting cg3lz..."
    start_latest('cg3lz', '')
end

scheduler.in '3s' do
    http = Net::HTTP.new('localhost', 18080)
    3.times do |i|
        response = http.send_request('PUT', '/log', "Hello \##{i+1}")
    end
end

scheduler.in '5s' do
    http = Net::HTTP.new('localhost', 18080)
    puts(http.send_request('GET', '/').body)
end

scheduler.in '15s' do
    puts "stopping cg3lz..."
    http = Net::HTTP.new('localhost', 18080)
    response = http.send_request('GET', '/kill')
    exit
end

scheduler.join
