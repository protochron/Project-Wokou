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

#include "Actions/Action.h"
#include "Actions/ActionPump.h"
#include "Physics/Physics.h"
#include "Graphics/Graphics.h"
#include "Networking/Network.h"
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

class Engine : public FrameListener {
 public:
  static Engine* instance();
  
  void setEntity( String ship ){ ship_ = ship; } //This must be called before use.
  void setWindow( Ogre::RenderWindow* window ){ window_ = window; }
  void setSceneManager( SceneManager* sceneMgr ){ sceneMgr_ = sceneMgr; }
  
  void bindCamera(bool type); //0 - Passive (controlled)   1 - Active (follows Player)
  
  bool frameStarted(const Ogre::FrameEvent& event);
  bool frameEnded(const Ogre::FrameEvent& event);
  bool doAction(Action a);
  
  //! Handles the Local actions (Input)
  void handleLocalMovePlayer(Action a);
  void handleLocalRotatePlayer(Action a);
  void handleLocalUDCamera(Action a);
  void handleLocalLRCamera(Action a);
  void handleLocalZoomCamera(Action a);
  void handleLocalFireCannon(Action a);
  
  //! Handles the actions from Network
  void handleNetworkCreateEntity(Action a);
  void handleNetworkDestroyEntity(Action a);
  void handleNetworkMoveEntity(Action a);
  void handleNetworkRotateEntity(Action a);
  
  void handleNetworkSetAmbient(Action a);
  
 protected:
  Engine();
  
 private:
  static boost::shared_ptr<Engine> instance_;
  
  int FPS; //Defaults to 30. Set in the constructor.

  GamePhysics *gamePhysics_;
  
  SceneManager* sceneMgr_;
  String ship_; //The name of the controlled entity.
  Radian angle_;//The angle of the ship. Maintained for efficiency.
  int health_;
  int atk_;
  int def_;
  Ogre::RenderWindow* window_;
  Ogre::Timer timer;
};

#endif
