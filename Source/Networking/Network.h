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
 
#ifndef _PIRATE_NETWORK_H_
#define _PIRATE_NETWORK_H_

class Action;

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <deque>
#include <utility>
#include <string.h>
#include <asio.hpp>

#include "Common/Common.h"

using asio::ip::tcp;
using std::string;
using std::pair;

/** This serves as the interface to all network-related actions in the game. It
 * is a Singleton, so only one instance can be created in the game. This means
 * that the network connection can be accessed from anywhere in the code
 * without problems (barring anything terrible about threading).
 *
 * Requests to send data do not necessarily result in data being sent
 * immediately. Rather, the system will send the data when it has finished 
 * sending all other data that was requested first.
 */

class Network {
public:
  
  //! Every action is paired with a specific action type.
  typedef std::pair<const Action, action_t> action_pair_t;
  
public:
  
  //! Retrieves the network instance or creates it if it hasn't been.
  static Network* instance();
  
  //! Connect to a remote server, given by an IP or address.
  bool connect(const std::string& addr);
  
  //! Sends an Action across the network.
  void send(const Action& action, const action_t& type);
  
  //! Provides a means of accessing the IO service that powers networking
  //! This is used to spawn threads off for the networking component of the code.
  static asio::io_service& service();
  
protected:
  
  // Do not allow creation of this class. It's a Singleton, baby!
  Network();
  
  //! Called when the socket makes a connection
  void handle_connect(const asio::error_code&, tcp::resolver::iterator);
  
  //! Called after data has been written to the server
  void handle_write(const asio::error_code& error);

  //! Called after data has been read from the server
  void handle_read(const asio::error_code& error, std::size_t size);

  //! Places the send request on the queue
  void write(const Action& action, const action_t& type);
  
private:  
  static boost::shared_ptr<Network> instance_;
  static asio::io_service io_;
  tcp::socket socket_;
  std::deque<action_pair_t> out_queue_;
  asio::streambuf input_buffer_;
};






#endif