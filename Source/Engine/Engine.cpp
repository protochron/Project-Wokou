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

extern std::map<std::string, handler_t> handler_mappings;

Engine::Engine(){
  FPS = 30;
  ship_ = "Player 1";
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
    std::cout << "Pumping action: " << a["type"] << " " << a["data"] << std::endl;
    doAction( a );
  }
  
  return 1;
}

bool Engine::doAction(Action a)
{
  std::string* type = boost::get<std::string>(&a["type"]);
  
  if (type == NULL)
    return false;

  if (handler_mappings.find(*type) != handler_mappings.end()) {
    handler_mappings[*type](Engine::instance(), a);
    return true;
  }
  
  return false;
}

void Engine::handleLocalMovePlayer(Action a)
{
  //if( physics allows it ){
  Ogre::Vector3* data = boost::get<Ogre::Vector3>(&a["data"]);
  Graphics::instance()->moveEntity( ship_, data->x, data->y, data->z );
  //Network::instance()->update
  //}
}

void Engine::handleLocalRotatePlayer(Action a){
  //if( physics allows it ){
  Ogre::Radian* data = boost::get<Ogre::Radian>(&a["data"]);
  Graphics::instance()->rotateEntity( ship_, *data );
  //Network::instance()->update
  //}
}

void Engine::handleNetworkMovePlayer( Action a ){
  //This is from the network; do not need to check Physics or update Network.
  std::string* name = boost::get<std::string>(&a["name"]);
  Ogre::Vector3* data = boost::get<Ogre::Vector3>(&a["data"]);
  Graphics::instance()->moveEntity( *name, data->x, data->y, data->z );
}

void Engine::handleNetworkRotatePlayer( Action a ){
  //This is from the network; do not need to check Physics or update Network.
  std::string* name = boost::get<std::string>(&a["name"]);
  Ogre::Radian* data = boost::get<Ogre::Radian>(&a["data"]);
  Graphics::instance()->rotateEntity( "Player 1", *data );
}

Engine* Engine::instance(){
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Engine);
    return instance_.get();
  }
}
