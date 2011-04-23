#ifndef __INPUT_H__
#define __INPUT_H__

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>

using namespace Ogre;

#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"
#endif

class Input : public FrameListener, public WindowEventListener {
 protected:
  virtual void updateStats();
  
 public:
  // Constructor takes a RenderWindow because it uses that to determine input context
  Input(RenderWindow* win, Camera* cam, bool bufferedKeys = false, 
	bool bufferedMouse = false, bool bufferedJoy = false );
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
  Camera* mCamera;

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

#endif
