#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Ogre.h"
#include "OgreConfigFile.h"

#include "Actions/Action.h"
#include "Application/Application.h"
#include "Common/Common.h"
#include "Input/Input.h"
#include "Networking/Network.h"
#include "Graphics/Graphics.h"

#include "ExampleFrameListener.h"

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
     // dx10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#    if OGRE_USE_D3D11
#      define OGRE_STATIC_Direct3D11
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#     undef OGRE_STATIC_CgProgramManager
#     undef OGRE_STATIC_GL
#     define OGRE_STATIC_GLES 1
#     ifdef __OBJC__
#       import <UIKit/UIKit.h>
#     endif
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#   include "macUtils.h"
#endif

#ifdef USE_RTSHADER_SYSTEM
class ShaderGeneratorTechniqueResolverListener : public MaterialManager::Listener {
 public:
  ShaderGeneratorTechniqueResolverListener(RTShader::ShaderGenerator* pShaderGenerator) {
    mShaderGenerator = pShaderGenerator;
  }
  virtual Technique* handleSchemeNotFound(unsigned short schemeIndex, const String& schemeName, 
					  Material* originalMaterial, unsigned short lodIndex, 
					  const Renderable* rend)	{		
    // Case this is the default shader generator scheme.
    if (schemeName == RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME){
      MaterialRegisterIterator itFind = mRegisteredMaterials.find(originalMaterial);
      bool techniqueCreated = false;
      // This material was not registered before.
      if (itFind == mRegisteredMaterials.end()){
	techniqueCreated = mShaderGenerator->
	  createShaderBasedTechnique(originalMaterial->getName(),
				     MaterialManager::DEFAULT_SCHEME_NAME, 
				     schemeName);				
      }
      mRegisteredMaterials[originalMaterial] = techniqueCreated;
    }
    return NULL;
  }
 protected:
  typedef std::map<Material*, bool>		MaterialRegisterMap;
  typedef MaterialRegisterMap::iterator	MaterialRegisterIterator;
 protected:
  MaterialRegisterMap	mRegisteredMaterials;	// Registered material map.
  RTShader::ShaderGenerator*	mShaderGenerator;// The shader generator instance.
  };
#endif

using namespace Ogre;

class Application {
 public:
  Application();
  virtual ~Application();
  
  virtual void go();

 protected:
  Root *mRoot;
#ifdef OGRE_STATIC_LIB
  StaticPluginLoader mStaticPluginLoader;
#endif
  Camera* mCamera;
  SceneManager* mSceneMgr;
  //ExampleFrameListener* mFrameListener;   //!**************************************
  Input* mFrameListener;
  RenderWindow* mWindow;
  Ogre::String mResourcePath;
  Ogre::String mConfigPath;
#ifdef USE_RTSHADER_SYSTEM
  RTShader::ShaderGenerator* mShaderGenerator; // The Shader generator instance.
  ShaderGeneratorTechniqueResolverListener* mMaterialMgrListener; // Material manager listener.	
#endif

  virtual bool setup();
  virtual bool configure();
  virtual void chooseSceneManager();
  virtual void createCamera();
  virtual void createFrameListener();
  virtual void createScene();
  virtual void destroyScene();
  virtual void createViewports();
  virtual void setupResources();
  virtual void createResourceListener();
  virtual void loadResources();
#ifdef USE_RTSHADER_SYSTEM
  virtual bool initializeShaderGenerator(SceneManager* sceneMgr);
  virtual void finalizeShaderGenerator();
#endif

 private:
  //Graphics graphics;
  //Network network;
};

#endif
