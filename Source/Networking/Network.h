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

#include <boost/shared_ptr.hpp>
//#include <boost/asio.hpp>


/** This serves as the interface to all network-related actions in the game. It
 * is a Singleton, so only one instance can be created in the game. This means
 * that the network connection can be accessed from anywhere in the code
 * without problems (barring anything terrible about threading).
 */

class Network {
public:
  
  //! Retrieves the networking instance or creates it if it hasn't been.
  static Network* instance();
  
  
  
  
protected:
  
  // Do not allow creation of this class. It's a Singleton, baby!
  Network();
  
private:
  
  static boost::shared_ptr<Network> instance_;
  
};


boost::shared_ptr<Network> Network::instance_;






#endif