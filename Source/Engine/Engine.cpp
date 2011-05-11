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
#include "../Physics/MotionState.h"
#include <iostream>

boost::shared_ptr<Engine> Engine::instance_;

extern std::map<std::string, handler_t> handler_mappings;

Engine::Engine(){
   FPS = 30;
   angle_= Ogre::Math::HALF_PI;
   health_ = 100;
   atk_ = 1;
   def_ = 1;
   gamePhysics_ = GamePhysics::instance(); //get an instance of our physics object
}

Engine* Engine::instance(){
   if (instance_)
      return instance_.get();
   else {
      instance_.reset(new Engine);
      return instance_.get();
   }
}

void Engine::bindCamera( bool type ){
   Ogre::Camera* camera = sceneMgr_->getCamera("Camera");
   Ogre::Entity* player = sceneMgr_->getEntity(ship_);
   Ogre::SceneNode* node = player->getParentSceneNode();
   node->attachObject(camera);
}

bool Engine::frameStarted(const Ogre::FrameEvent& event){
   timer.reset();
   GamePhysics::instance()->getDynamicsWorld()->stepSimulation(1/60.f, 10);
   //GamePhysics::instance()->getDynamicsWorld()->debugDrawWorld();
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

void Engine::handleLocalMovePlayer(Action a){
   float* distance = boost::get<float>(&a["distance"]);
   float* height =   boost::get<float>(&a["height"]);
   double dx = *distance * -Math::Cos( angle_ );
   double dy = *height; 
   double dz = *distance * Math::Sin( angle_ ); 
   //Get updated coordinates from Physics
  
   Ogre::SceneNode* temp = sceneMgr_->getEntity( ship_ )->getParentSceneNode();
   temp->translate( dx, dy, dz );
   //Graphics::instance()->moveEntity( ship_, dx, dy, dz );
   //Graphics::instance()->moveCamera( dx, dy, dz );
  
   //Network::instance()->update
}

void Engine::handleLocalRotatePlayer(Action a){
   //Update Physics if it cares about angles.
   Ogre::Radian* data = boost::get<Ogre::Radian>(&a["data"]);
   angle_ = angle_ + *data;
   if( angle_ > Radian(Math::PI) ) angle_ = angle_ - Radian(Math::TWO_PI);
   if( angle_ < -Radian(Math::PI)) angle_ = angle_ + Radian(Math::TWO_PI);
   Graphics::instance()->rotateEntity( ship_, *data, true );
   //Network::instance()->update
   //}
}

void Engine::handleLocalUDCamera(Action a){ //DEFUNCT
   /*  Ogre::Camera* camera = sceneMgr_->getCamera("Camera");
       float* data = boost::get<float>(&a["data"]);
       Ogre::Vector3 temp = camera->getPosition();
       camera->moveRelative( Ogre::Vector3( 0, *data, 0 ) );
       camera->setDirection( -temp.x, -temp.y, -temp.z );*/
}

void Engine::handleLocalLRCamera(Action a){ //DEFUNCT
   /*  Ogre::Camera* camera = sceneMgr_->getCamera("Camera");
       Ogre::Radian* data = boost::get<Ogre::Radian>(&a["data"]);
       Ogre::Vector3 temp = camera->getPosition();
       //std::cout << temp.x << " " << temp.y << " " << temp.z << std::endl;
       camera->moveRelative( Ogre::Vector3( 1, 0, 0 ) );
       camera->setDirection( -temp.x, -temp.y, -temp.z );*/
}

void Engine::handleLocalZoomCamera(Action a){
   Ogre::Camera* camera = sceneMgr_->getCamera("Camera");
  
}

void Engine::handleLocalFireCannon(Action a){
   Ogre::Vector3* direction = boost::get<Ogre::Vector3>(&a["direction"]);
   Ogre::Vector3 position = sceneMgr_->getEntity( ship_ )->getParentSceneNode()->getPosition();
   if( sceneMgr_->hasEntity( ship_ + " CBall" ) == false ){
      //The entity does not exist. Create it.
      Graphics::instance()->createEntity( ship_ + " CBall", "penguin.mesh", position.x, position.y, position.z);
   }
   else{
      Graphics::instance()->moveEntity( ship_ + " CBall", direction->x, direction->y, direction->z );
   }
}

void Engine::handleNetworkCreateYourself(Action a)
{
  std::string* name = boost::get<std::string>(&a["name"]);
  std::string* mesh = boost::get<std::string>(&a["mesh"]);
  float* x = boost::get<float>(&a["x"]);
  float* y = boost::get<float>(&a["y"]);
  float* z = boost::get<float>(&a["z"]);
  
  if (!name || !mesh || !x || !y || !z) {
    std::cout << "ERROR IN HANDLENETWORKCREATEYOURSELF: " << name << " ";
    std::cout << mesh << " " << x << " " << y << " " << z << std::endl;
  }
  
  setEntity(*name);
  
  Graphics::instance()->createEntity(*name, *mesh, *x, *y, *z);
  bindCamera(false);
}


void Engine::handleNetworkCreateEntity( Action a ){
   std::string* name = boost::get<std::string>(&a["name"]);
   std::string* mesh = boost::get<std::string>(&a["mesh"]);
   float* x = boost::get<float>(&a["x"]);
   float* y = boost::get<float>(&a["y"]);
   float* z = boost::get<float>(&a["z"]);

   if (!name || !mesh || !x || !y || !z) {
     std::cout << "ERROR IN HANDLENETWORKCREATEENTITY: " << name << " ";
     std::cout << mesh << " " << x << " " << y << " " << z << std::endl;
   }
   
   Graphics::instance()->createEntity(*name, *mesh, *x, *y, *z);
   
   //Ogre::Vector3* data = boost::get<Ogre::Vector3>(&a["data"]);
   //Graphics::instance()->createEntity( *name, *mesh, data->x, data->y, data->z );
   //Update Physics.
}

void Engine::handleNetworkDestroyEntity( Action a ){
   std::string* name = boost::get<std::string>(&a["name"]);
   
   if (!name)
     std::cout << "ERROR IN HANDLENETWORKDESTROYENTITY: " << name << std::endl;
   
   Graphics::instance()->destroyEntity( *name );
   //Update Physics.
}

void Engine::handleNetworkMoveEntity( Action a ){
   //This is from the network; do not need to check Physics or update Network.
   std::string* name = boost::get<std::string>(&a["name"]);
   float* x = boost::get<float>(&a["x"]);
   float* y = boost::get<float>(&a["y"]);
   float* z = boost::get<float>(&a["z"]);
   
   if (!name || !x || !y || !z) {
     std::cout << "ERROR IN HANDLENETWORKMOVEENTITY: " << name << " ";
     std::cout << x << " " << y << " " << z << std::endl;
   }
   
   //Ogre::Vector3* data = boost::get<Ogre::Vector3>(&a["data"]);
   Graphics::instance()->moveEntity(*name, *x, *y, *z);
   //Update Physics.
}

void Engine::handleNetworkRotateEntity( Action a ){
   //This is from the network; do not need to check Physics or update Network.
   std::string* name = boost::get<std::string>(&a["name"]);
   Ogre::Radian* data = boost::get<Ogre::Radian>(&a["data"]);
   Graphics::instance()->rotateEntity( *name, *data );
   //Update Physics (if it cares about angles)
}

void Engine::handleNetworkDamagePlayer(Action a){
   
}

void Engine::handleNetworkSetAmbient( Action a ){
   Ogre::Vector3* data = boost::get<Ogre::Vector3>(&a["data"]);
   Graphics::instance()->setAmbient( data->x, data->y, data->z );
}

