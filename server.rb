require 'rubygems'
require 'eventmachine'

module EchoServer
  def post_init
    puts "Client connected!"
  end
  
  def receive_data(data)
    puts data
    send_data(data + "\n")
  end
end

EventMachine::run do
  host = '0.0.0.0'
  post = 8885
  EventMachine::start_server host, post, EchoServer
  puts "Started EchoServer on #{host}:#{post}..."

end


