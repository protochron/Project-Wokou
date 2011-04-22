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

#ifndef _PIRATES_PUMP_H_
#define _PIRATES_PUMP_H_

#include <deque>
#include <boost/shared_ptr.hpp>
#include "Actions/Action.h"


/**
 * ActionPump is a thin Singleton wrapper over the STL's deque class. It is
 * used as a central location for all Actions to be stored. Actions are placed
 * on the queue when they are generated, but they are removed at the program's
 * best discretion.
 *
 * ActionPump is not thread-safe.
 */

class ActionPump {
public:
  
  //! Provides access to the single ActionPump instance
  static ActionPump* instance();
  
  //! Provides a reference to the internal ActionPump queue.
  std::deque<Action>& queue();
  
protected:
  
  // Disallow creation of this object. It's a Singleton, baby!
  ActionPump();
  
private:
  static boost::shared_ptr<ActionPump> instance_;
  std::deque<Action> queue_;
  
  
};




#endif