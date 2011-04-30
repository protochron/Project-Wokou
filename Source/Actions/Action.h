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

#ifndef _PIRATES_ACTION_H_
#define _PIRATES_ACTION_H_

#include <string>
#include <boost/function.hpp>
#include <boost/variant.hpp>
#include <OGRE/OgreVector3.h>

#include "Common/Common.h"
#include "Actions/Action.h"

class Engine;


/** Represents the possible values that can be stored in an Action. */
typedef boost::variant< float,
                        std::string,
                        Ogre::Vector3,
                        Ogre::Radian
                      > value_t;


/** 
 * This is an abstraction of game mechanics. Instead of directly handling
 * things like keypresses, the system generates an Action object that 
 * contains data about the event. This object is bound to a method that knows
 * how to handle that specific message. These are then handled at the game's
 * discretion.
 *
 * An Action is just an associative array of values. It is easy to add 
 * new fields -- simply put them in. Make certain that value_t's definition
 * is changed to include any new types that can be stored.
 */

typedef std::map<std::string, value_t> Action;


/**
 * Actions are handled by methods called "handlers". A pointer to a handler
 * function is captured in the handler_t type.
 */
typedef boost::function<void (Engine*, const Action& a)> handler_t;


/**
 * This is a mapping of handler functions to Action types. The keys of the
 * mapping are strings that dictate the "type" of the action, while the 
 * values are the functions that should be called to handle the action.
 *
 * The type of an Action is set in the Action object under the key of "type".
 */

extern std::map<std::string, handler_t> handler_mappings;




/**
 * Converts an Action to a network-friendly format.
 * @param[in] a The action to convert
 * @return A string containing the formatted action
 */
 
std::string toNetworkFormat(const Action& a);

#endif
