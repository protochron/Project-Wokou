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

#ifndef _PIRATE_ENGINE_H_
#define _PIRATE_ENGINE_H_

#include "Physics/Physics.h"
#include "Graphics/Graphics.h"
#include "Networking/Network.h"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

class Engine {
 public:
  static Engine* instance();
  
  void setEntity( String ship ){ ship_ = ship; }
  
  bool moveShip( double x, double y, double z ); //These are still iffy.
  bool rotateShip( double roty );                //
  
 protected:
  Engine();
  
 private:
  static boost::shared_ptr<Engine> instance_;
  
  GamePhysics* physics; //May be canned if this is a singleton.
  String ship_; //The name of the controlled entity.
};

#endif
