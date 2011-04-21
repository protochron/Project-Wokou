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

#include "Action.h"
#include <sstream>

/*
Action::Action(const handler_t& handler, const std::string& msg)
  : handler_ (handler), logger_ (msg), target (NO_PLAYER), id(NO_PLAYER)
{ }


inline 
bool Action::handle()
{
    return handler_(*this);
}


inline
void Action::log()
{
  if (logger_ != "")
    std::cerr << "Action::log() : " << logger_ << std::endl;
}

std::string Action::toNetworkFormat() const
{
  std::string object;
  std::stringstream stream(object);
  
  stream << "{ \"id\": ";
  stream << id;
  
  if (target != NO_PLAYER) {
    stream << ", \"target\": ";
    stream << target;
  }
  
  if (name != "") {
    stream << ", \"name\": ";
    stream << name;
  }
  
  // Need to translate Vector3 to
  
  if (text != "") {
    stream << ", \"text\": ";
    stream << text;
  }
  
  stream << " }";
  return stream.str();
}
*/