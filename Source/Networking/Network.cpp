/* Copyright (c) 2011 Cody Miller, Daniel Norris, Brett Hitchcock.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *****************************************************************************/

#include "Network.h"
#include "Actions/Action.h"
#include "Actions/ActionPump.h"

#include "Common/Parsers/JsonParser.h"
#include <boost/thread.hpp>

 
boost::shared_ptr<Network> Network::instance_;
asio::io_service Network::io_;


Network::Network()
  : socket_(io_)
{ }


Network* Network::instance()
{
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Network);
    return instance_.get();
  }
}


asio::io_service& Network::service()
{
  return io_;
}

void Network::connect(const char* addr, const char* port)
{
  tcp::resolver resolver(io_);
  tcp::resolver::query query(addr, port);
  tcp::resolver::iterator endpoints = resolver.resolve(query);
  
  tcp::endpoint endpoint = *endpoints;

  socket_.async_connect(endpoint,
    boost::bind(&Network::handle_connect, this, 
      boost::asio::placeholders::error, ++endpoints));
}

void Network::disconnect()
{
  socket_.close();
}

void Network::handle_connect( const boost::system::error_code& error,
                              tcp::resolver::iterator endpoint_iterator)
{
  if (!error) {

    asio::async_read_until(socket_, input_buffer_, '\n',
        boost::bind(&Network::handle_read, this,
        asio::placeholders::error, 35));
     
  } else if (endpoint_iterator != tcp::resolver::iterator()) {

    socket_.close();
    tcp::endpoint endpoint = *endpoint_iterator;
    socket_.async_connect(endpoint,
        boost::bind(&Network::handle_connect, this,
          asio::placeholders::error, ++endpoint_iterator));

  } else {
    throw "Could not connect to remote server. :(";
  }
}

void Network::send(const Action& action, const action_t& type)
{
  io_.post(boost::bind(&Network::write, this, action, type));
}


void Network::handle_write(const boost::system::error_code& error)
{
  if (!error) {
    static boost::mutex m;
    boost::mutex::scoped_lock lock(m);
    
    out_queue_.pop_front();
    
    lock.unlock();
    
    const Action& action(out_queue_.front().first);
    const action_t type = out_queue_.front().second;
    
    if (!out_queue_.empty()) {
      std::string msg = toNetworkFormat(action);
  
      asio::async_write(socket_, 
        asio::buffer(msg, msg.length()),
        boost::bind(&Network::handle_write, this,
          asio::placeholders::error));
    }
  }
}

void Network::handle_read(const boost::system::error_code& error, const std::size_t bytes)
{
  std::string str;
  std::istream is(&input_buffer_);
  std::getline(is, str);
  
  Action obj = JsonParser::parse(str);

  ActionPump::instance()->push_back(obj);

  asio::async_read_until(socket_, input_buffer_, '\n',
      boost::bind(&Network::handle_read, this,
      asio::placeholders::error, 35));  
}


void Network::write(const Action& action, const action_t& type)
{
  bool write_in_progress = !out_queue_.empty();
  out_queue_.push_back(make_pair(action, type));

  if (!write_in_progress) {
    std::string msg = toNetworkFormat(action);
  
    asio::async_write(socket_, asio::buffer(msg, msg.length()),
      boost::bind(&Network::handle_write, this,
        asio::placeholders::error));
  }
}
