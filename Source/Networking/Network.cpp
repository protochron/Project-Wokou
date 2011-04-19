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
#include <iostream>
 using namespace std;
 
boost::shared_ptr<Network> Network::instance_;

Network::Network()
  : socket_(io_)
{
  
}

Network* Network::instance()
{
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Network);
    return instance_.get();
  }
}

bool Network::connect(const std::string& addr)
{
  tcp::resolver resolver(io_);
  tcp::resolver::query query(addr.c_str(), "daytime");
  tcp::resolver::iterator endpoints = resolver.resolve(query);
  asio::async_connect(socket_, endpoints, 
    boost::bind(&Network::handle_connect, this, 
      asio::placeholders::error));
  //asio::connect(socket_, endpoint_iterator);
}


void Network::handle_connect(const asio::error_code& error)
{
  cout << "Yaaaaaay!" << endl;
}


