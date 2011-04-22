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

#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include "Actions/Action.h"

#include <string>
#include <sstream>
#include <map>

#include <iostream>

/**
 * This class works as a simple JSON parser. It is a hand-coded recursive
 * descent parser. It is a header-only library and quite simple to use. All
 * methods are static, so there is no need to instantiate a parser object.
 */

class JsonParser {
public:

  /**
   * Parses a string and returns a key-value store of the JSON object.
   */
  static Action parse(std::string& str);


private:


  static Action parseObject(std::stringstream& str);
  static void parseSpaces(std::stringstream& str);
  static std::string parseString(std::stringstream& str);
  static void parseNull(std::stringstream& str);
  static bool parseTrue(std::stringstream& str);
  static bool parseFalse(std::stringstream& str);
  static float parseNumber(std::stringstream& str);
    
};



Action JsonParser::parse(std::string& str)
{
  std::stringstream stream(str);
  return JsonParser::parseObject(stream);
}


Action JsonParser::parseObject(std::stringstream& str)
{
  JsonParser::parseSpaces(str);
  
  if (str.get() != '{')
    throw "Expected but did not receive {";
  
  std::string key;
  Action value;
  
  while (true) {
    JsonParser::parseSpaces(str);
  
    key = JsonParser::parseString(str);
  
    JsonParser::parseSpaces(str);

    if (str.get() != ':')
      throw "Expected but did not receive :";

    JsonParser::parseSpaces(str);
  
    switch (str.peek()) {
      case 'n':
        JsonParser::parseNull(str);
        break;
      case 't':
        value[key] = JsonParser::parseTrue(str);
        break;
      case 'f':
        value[key] = JsonParser::parseFalse(str);
        break;
      case '"':
        value[key] = JsonParser::parseString(str);
        break;
      case '{':
        throw "Recursive objects are not supported yet. :(";
        break;
      default:
        value[key] = JsonParser::parseNumber(str);
        break;
    }

    JsonParser::parseSpaces(str);
  
    if (str.peek() == '}')
      break;
    
    if (str.get() != ',')
      throw "Expected but did not receive ,";
  }
 
  return value;
}

void JsonParser::parseSpaces(std::stringstream& str)
{
  char c;
  
  while ((c = str.peek()) && (c == '\n' || c == ' ' || c == '\t'))
    str.seekg(1, std::ios::cur);
}

std::string JsonParser::parseString(std::stringstream& str)
{
  if (str.get() != '"')
    throw "Expected but did not receive \"";
  
  char c;
  std::string s = "";
  
  while ((c = str.get()) && (c != '"' && *s.end() != '\\'))
    s += c;

  return s;
}


void JsonParser::parseNull(std::stringstream& str)
{
  const char *comp = "null";
  
  for (int i = 0; i < 4; ++i)
    if (str.get() != comp[i])
      throw "Expected but did not receive null";
}

bool JsonParser::parseTrue(std::stringstream& str)
{
  const char *comp = "true";

  for (int i = 0; i < 4; ++i)
    if (str.get() != comp[i])
      throw "Expected true";

  return true;
}

bool JsonParser::parseFalse(std::stringstream& str)
{
  const char *comp = "false";

  for (int i = 0; i < 5; ++i)
    if (str.get() != comp[i])
      throw "Expected true";

  return false;
}

float JsonParser::parseNumber(std::stringstream& str)
{
  float number;
  str >> number;
  return number;
}



#endif