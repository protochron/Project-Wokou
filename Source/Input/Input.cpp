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

#include "Input.h"

#include <utility>
#include <sstream>

#include "Graphics/Graphics.h"
#include <CEGUI.h>

Input::Input(Ogre::RenderWindow* window)
  : window_ (window), continue_ (true), mouse_ (0), keyboard_ (0)
{
  initializeInputSystem();
  createBufferedInputDevices();
  
  // Initialize the keypress array
  for (int i = 0; i < 256; ++i)
    keys_[i] = false;
}


Input::~Input()
{
  if (mouse_)
    input_manager_->destroyInputObject(mouse_);

  if (keyboard_)
    input_manager_->destroyInputObject(keyboard_);
    
  OIS::InputManager::destroyInputSystem(input_manager_); 
}

void Input::initializeInputSystem()
{
  // Retrieve the window handle
  OIS::ParamList param_list;
  size_t window_handle = 0;
  std::ostringstream stream;
  
  window_->getCustomAttribute("WINDOW", &window_handle);
  stream << window_handle;
  
  param_list.insert(std::make_pair("WINDOW", stream.str()));
  
  input_manager_ = OIS::InputManager::createInputSystem(param_list);
}


void Input::createBufferedInputDevices()
{
  if (input_manager_->getNumberOfDevices(OIS::OISKeyboard) > 0) {
    keyboard_ = (OIS::Keyboard*)input_manager_->createInputObject(OIS::OISKeyboard, true);
    keyboard_->setEventCallback(this);
  }
  
  if (input_manager_->getNumberOfDevices(OIS::OISMouse) > 0) {
    mouse_ = (OIS::Mouse*)input_manager_->createInputObject(OIS::OISMouse, true);
    mouse_->setEventCallback(this);
  
    // Set the mouse boundaries
    windowResized(window_);
  }
  
}


void Input::windowResized(Ogre::RenderWindow* window)
{
  unsigned int width, height, depth;
  int left, top;
  
  window_->getMetrics(width, height, depth, left, top);
  
  const OIS::MouseState &mouse = mouse_->getMouseState();
  mouse.width = width;
  mouse.height = height;
}

void Input::windowClosed(Ogre::RenderWindow* window)
{}


bool Input::frameStarted(const Ogre::FrameEvent& event)
{
  return true;
}

bool Input::frameRenderingQueued(const Ogre::FrameEvent& event)
{
  // Capture the mouse and keyboard with each rendered frame
  keyboard_->capture();
  mouse_->capture();

  /*if (keys_[OIS::KC_Z]) {
     Graphics::instance()->moveCamera(0, 0, 3);
  }
  if (keys_[OIS::KC_Q]){
     Graphics::instance()->setAmbient( .3, .3, .3 );
  }
  if (keys_[OIS::KC_E]){
     Graphics::instance()->setAmbient( .9, .9, .9 );
     }*/
  if (keys_[OIS::KC_N]){
     Action a;
     a["type"] = "LocalFireCannon";
     a["dir"] = 1; //Left
     ActionPump::instance()->push_back( a );
  }
  if (keys_[OIS::KC_M]){
     Action a;
     a["type"] = "LocalFireCannon";
     a["dir"] = -1; //Right
     ActionPump::instance()->push_back( a );
  }
  if( keys_[OIS::KC_UP] || keys_[OIS::KC_W] ){
    Action a;
    a["type"] = "LocalMovePlayer";
    a["distance"] = 2;
    a["height"] = 0;
    ActionPump::instance()->push_back( a ); 
  }  
  if( keys_[OIS::KC_DOWN] || keys_[OIS::KC_S]){
    Action a;
    a["type"] = "LocalMovePlayer";
    a["distance"] = -2;
    a["height"] = 0;
    ActionPump::instance()->push_back( a );
  }
  if( keys_[OIS::KC_LEFT] || keys_[OIS::KC_A]){
    Action a;
    a["type"] = "LocalRotatePlayer";
    a["data"] = Ogre::Radian( 0.01 );
    ActionPump::instance()->push_back( a ); 
  }  
  if( keys_[OIS::KC_RIGHT] || keys_[OIS::KC_D]){
    Action a;
    a["type"] = "LocalRotatePlayer";
    a["data"] = Ogre::Radian( -0.01 );
    ActionPump::instance()->push_back( a );
  }
  /*if( keys_[OIS::KC_I] ){
    Action a;
    a["type"] = "LocalUDCamera";
    a["data"] = 0.2;
    ActionPump::instance()->push_back( a );
  }
  if( keys_[OIS::KC_K] ){
    Action a;
    a["type"] = "LocalUDCamera";
    a["data"] = -0.2;
    ActionPump::instance()->push_back( a );
  }
  if( keys_[OIS::KC_J] ){
    Action a;
    a["type"] = "LocalLRCamera";
    a["data"] = Ogre::Radian(0.1);
    ActionPump::instance()->push_back( a );
  }
  if( keys_[OIS::KC_L] ){
    Action a;
    a["type"] = "LocalLRCamera";
    a["data"] = -Ogre::Radian(0.1);
    ActionPump::instance()->push_back( a );
    }*/

  
  return continue_;
}

bool Input::frameEnded(const Ogre::FrameEvent& event)
{
  return true;
}




bool Input::keyPressed(const OIS::KeyEvent& event)
{
  if (keyboard_->isKeyDown(OIS::KC_ESCAPE))
      return (continue_ = false);  
  
  keys_[event.key] = true;
  
  return true;
}



bool Input::keyReleased(const OIS::KeyEvent& event)
{
  keys_[event.key] = false;
  
  return true;
}



bool Input::mouseMoved(const OIS::MouseEvent& event)
{
  const OIS::MouseState& state = event.state;
  
  if (state.Z.abs != 0)
    Graphics::instance()->zoomCamera(state.Z.abs / 500.0);

  //CEGUI::System::getSingleton().injectMouseMove(state.X.rel, state.Y.rel);
  //Graphics::instance()->rotateCamera(-state.X.rel * 0.013, -state.Y.rel * 0.013);
  
  return true;
}

bool Input::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
   //CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)id);
  return true;
}

bool Input::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
   //CEGUI::System::getSingleton().injectMouseButtonUp((CEGUI::MouseButton) id);
  return true;
}

