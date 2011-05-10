require 'rubygems'
require 'eventmachine'
require 'json'

$pc = 1




module EchoServer
  
  def send_new_entity(client)
    action =  { :type => "NetworkCreateEntity", 
               :name => "Player #{$pc}", 
               :mesh => "BetterShip.mesh", 
               :x => 0, 
               :y => 0, 
               :z => 0 
             }
         
    client.send_data(JSON.generate(action) + "\n")    
  end
  
  def send_create_yourself
    action =  { :type => "NetworkCreateYourself", 
               :name => "Player #{$pc}", 
               :mesh => "BetterShip.mesh", 
               :x => 0, 
               :y => 0, 
               :z => 0 
             }
         
    send_data(JSON.generate(action) + "\n")
  end
  
  
  
  
  def post_init
    $clients_list ||= {}
    @identifier = self.object_id
    $clients_list.merge!({@identifier => self})

    # Send this to message to the newly connected player
    send_create_yourself()
    
    # Alert all other players that a ship connected
    $clients_list.values.each do |c|
      send_new_entity(c) if c != self
    end
    $pc += 1
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


