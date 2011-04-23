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

#include "ActionPump.h"
#include <boost/thread.hpp>

boost::shared_ptr<ActionPump> ActionPump::instance_;


ActionPump::ActionPump()
{ }


ActionPump* ActionPump::instance()
{
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new ActionPump);
    return instance_.get();
  }
}


std::deque<Action>& ActionPump::queue()
{
  return queue_;
}

void ActionPump::push_back(Action& a)
{
  // Lock the ActionPump deque
  static boost::mutex m;
  boost::mutex::scoped_lock lock(m);
  
  // Push the latest action onto it
  queue_.push_back(a);
  
  // Unlock the ActionPump deque
  lock.unlock();
}
  
void ActionPump::pop()
{
  // Lock the ActionPump deque
  static boost::mutex m;
  boost::mutex::scoped_lock lock(m);
  
  // Push the latest action onto it
  queue_.pop_front();
  
  // Unlock the ActionPump deque
  lock.unlock();
}

Action ActionPump::front() const
{
  // Lock the ActionPump deque
  static boost::mutex m;
  boost::mutex::scoped_lock lock(m);
  
  // Push the latest action onto it
  return queue_.front();

  // The mutex is automatically unlocked at the end of scope.
}
