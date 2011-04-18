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

#include <boost/function.hpp>
#include <OGRE/OgreVector3.h>

#include "Common/Common.h"

class Action;

 //! Actions are handled by methods called "handlers". A pointer to a handler
 //! function is captured in the handler_t type.
typedef boost::function<bool (const Action& a)> handler_t;


/** This is an abstraction of game mechanics. Instead of directly handling
 * things like keypresses, the system generates an Action object that 
 * contains data about the event. This object is bound to a method that knows
 * how to handle that specific message. These are then handled at the game's
 * discretion.
 *
 * Currently, this class just contains all the possible parameters that could
 * be necessary for actions instead of only having the ones that are relevant 
 * to a specific action. This may be changed later.
 */

class Action {
public:
  
  Action(const handler_t& handler);

  /** Calls the handler function with the current action as a parameter.
   */
  inline bool handle();

public:

  //! The id of the player who generated the action
  player_id id;
  
  //! The location to move the player to, if they are moving
  Ogre::Vector3 to;


private:
  handler_t handler_;
  
};



#endif