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

#ifndef _PIRATE_INPUT_H_
#define _PIRATE_INPUT_H_

#include <OGRE/Ogre.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreFrameListener.h>
#include "OgreStringConverter.h"
#include "OgreException.h"


#define OIS_DYNAMIC_LIB
#include <OIS.h>

using namespace Ogre;

#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif


/**
 * Responsible for handling mouse and keyboard input from the user, converting
 * it into Action objects, and placing them into the ActionPump. 
 *
 * The Input class should not directly affect anything except the ActionPump. 
 * The game's engine should be responsible for doing all the actual updates 
 * when it gets to handling the ActionPump.
 */
 
class Input : public FrameListener, 
              public WindowEventListener, 
              public OIS::KeyListener, 
              public OIS::MouseListener {
public:
  
  //! Attaches an input listener to the specified window
  Input(Ogre::RenderWindow* window);
  
  
  virtual ~Input();
  
  //! Called when a frame is about to begin rendering
  virtual bool frameStarted(const Ogre::FrameEvent& event);
  
  //! Called after all render targets have had their rendering commands issued,
  //! but before render windows have been asked to flip their buffers.
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& event);
  
  //! Called just after a frame has been rendered
  virtual bool frameEnded(const Ogre::FrameEvent& event);
  
  //! Called when the window has been resized
  void windowResized(Ogre::RenderWindow* window);
  
  //! Called when a window has been closed
  void windowClosed(Ogre::RenderWindow* window);


  bool keyPressed(const OIS::KeyEvent& event);
  
  bool keyReleased(const OIS::KeyEvent& event);
  
  bool mouseMoved(const OIS::MouseEvent& event);
  
  bool mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id);
  
  bool mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id);



private:
  
  //! Does the "dirty work" for initializing the OIS system
  void initializeInputSystem();

  //! Creates buffered inputs for the keyboard and mouse
  void createBufferedInputDevices();

private:
  Ogre::RenderWindow* window_;
  OIS::InputManager* input_manager_;
  OIS::Keyboard* keyboard_;
  OIS::Mouse* mouse_;
  bool continue_;
  
  // This is a simple hack until we decide on controls
  bool keys_[256];
  
};
 
 
/*
class Input : public FrameListener, public WindowEventListener {
public:
  
  // Constructor takes a RenderWindow because it uses that to determine input context
  Input(RenderWindow* win, bool keys = false, bool mouse = false, bool joy = false );
  virtual ~Input(); 

  //Adjust mouse clipping area
  virtual void windowResized(RenderWindow* rw);


  //Unattach OIS before window shutdown (very important under Linux)
  virtual void windowClosed(RenderWindow* rw);

  //CODY. THIS BEAST IS YOURS TO TAME.
  virtual bool processUnbufferedKeyInput(const FrameEvent& evt);

  virtual bool processUnbufferedMouseInput(const FrameEvent& evt);

  virtual void moveCamera();

  virtual void showDebugOverlay(bool show);
  
  // Override frameRenderingQueued event to process that (don't care about frameEnded)
  bool frameRenderingQueued(const FrameEvent& evt);
  bool frameEnded(const FrameEvent& evt );
  
#ifdef USE_RTSHADER_SYSTEM
  virtual void processShaderGeneratorInput();
#endif

protected:
  Vector3 mTranslateVector;
  Real mCurrentSpeed;
  RenderWindow* mWindow;
  bool mStatsOn;

  String mDebugText;

  unsigned int mNumScreenShots;
  float mMoveScale;
  float mSpeedLimit;
  Degree mRotScale;
  // just to stop toggles flipping too fast
  Real mTimeUntilNextToggle ;
  Radian mRotX, mRotY;
  TextureFilterOptions mFiltering;
  int mAniso;

  int mSceneDetailIndex ;
  Real mMoveSpeed;
  Degree mRotateSpeed;
  Overlay* mDebugOverlay;

  //OIS Input devices
  OIS::InputManager* mInputManager;
  OIS::Mouse*    mMouse;
  OIS::Keyboard* mKeyboard;
  OIS::JoyStick* mJoy;
};
*/
#endif
