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

#include "Engine.h"

boost::shared_ptr<Engine> Engine::instance_;

Engine::Engine(){
  FPS = 30;
}

bool Engine::moveShip( double x, double y, double z ){
  //MUST contact Physics for confirmation/reduction of values.
  Graphics::instance()->moveEntity( ship_, x, y, z );
  //MUST inform Network of change.
  
  
  return 0;
}

bool Engine::rotateShip( double roty ){
  
  //Probably shouldn't need to check with Physics first.
  Graphics::instance()->rotateEntity( ship_, roty );
  
  //MUST inform Network of change.

  return 0;
}

bool Engine::frameStarted(const Ogre::FrameEvent& event){
  timer.reset();
  return 1;
}

bool Engine::frameEnded(const Ogre::FrameEvent& event){
  unsigned long time = timer.getMilliseconds();
  //std::cout << time << std::endl;

  //This currently pumps out ALL the actions.
  while( !ActionPump::instance()->queue().empty() ){
    Action a = ActionPump::instance()->front();
    ActionPump::instance()->pop();
    std::cout << "Pumping action: " << std::endl;
    
  }
  
  return 1;
}




Engine* Engine::instance(){
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Engine);
    return instance_.get();
  }
}
