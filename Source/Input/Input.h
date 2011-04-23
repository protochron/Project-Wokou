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
 public:
  Input( RenderWindow* win, Camera* cam, bool bufferedKeys, bool bufferedMouse, bool bufferedJoy );
  virtual ~Input();
  
  virtual void windowResized( RenderWindow* rw );
  virtual void windowClosed( RenderWindow* rw );
  virtual bool processUnbufferedKeyInput( const FrameEvent& evt );
  virtual bool processUnbufferedMouseInput( const FrameEvent& evt );
  virtual void moveCamera();
  virtual void showDebugOverlay( bool show );
  bool frameRenderingQueued( const FrameEvent& evt );
  bool frameEnded( const FrameEvent& evt );
  #ifdef USE_RTSHADER_SYSTEM
    virtual void processShaderGeneratorInput();
  #endif
  
 protected:
  virtual void updateStats();

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
