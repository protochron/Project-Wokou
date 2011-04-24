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
#include <string>
#include <boost/foreach.hpp>

std::map<std::string, handler_t> handler_mappings;



std::string toNetworkFormat(const Action& a)
{
  std::string converted;
  std::stringstream stream(converted);
  stream << "{";

  std::pair <std::string, value_t> kv;
  
  BOOST_FOREACH(kv, a) {
    
    // Surround a string in quotes if we're translating a string.
    if (std::string* pStr = boost::get<std::string>(&kv.second))
      stream << '"' << kv.first << "\": \"" << *pStr << "\", ";
    else
      stream << '"' << kv.first << "\": " << kv.second << ", ";
  }
  
  converted = stream.str();
  
  // Remove the trailing ", " on the string and add a closing brace
  return converted.substr(0, converted.length()-2) + "}";
}



handler_t handlerFunction(const Action& a)
{
  
  
}