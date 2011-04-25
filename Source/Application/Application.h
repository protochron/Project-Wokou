#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>

#include "Common/Common.h"
#include "Engine/Engine.h"
#include "Input/Input.h"
#include "Networking/Network.h"
#include "Graphics/Graphics.h"
#include "Physics/Physics.h"

#include <boost/shared_ptr.hpp>


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
#   include "macUtils.h"
#endif


/*#ifdef OGRE_STATIC_LIB
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
#  include "OgreStaticPluginLoader.h"
#endif*/




using namespace Ogre;

class Application {
public:
  
  Application();
  ~Application();
  
  //! Tells the Application to begin its event loop
  void go();
  
private:
  
  //! Return the path to the application's resource files
  Ogre::String getResourcePath();
  
  //! Return the path to the applicaton's configuration files
  Ogre::String getConfigPath();
  
  //! Initializes the Root system
  Ogre::Root* initializeRoot(const Ogre::String& plugin, 
    const Ogre::String& config, 
    const Ogre::String& log);
  
  //! Initializes the window by setting the configuration
  Ogre::RenderWindow* initializeWindow();
  
  //! Uses the configuration file to load all the resources
  void initializeResources();
  
  //! Constructs a new basic scene manager
  Ogre::SceneManager* initializeSceneManager();
  
  //! Initializes the viewports
  void initializeViewport();
  
  //! Initializes the input subsystem
  Input* initializeInput(Ogre::Root* root, Ogre::RenderWindow* window);
  
  /*
  #ifdef USE_RTSHADER_SYSTEM
    RTShader::ShaderGenerator* mShaderGenerator; // The Shader generator instance.
    ShaderGeneratorTechniqueResolverListener* mMaterialMgrListener; // Material manager listener.	
    virtual bool initializeShaderGenerator(SceneManager* sceneMgr);
    virtual void finalizeShaderGenerator();
  #endif*/
  
  
  
private:
  boost::shared_ptr<Ogre::Root> root_;
  boost::shared_ptr<Input> input_system_;

  Ogre::SceneManager* scene_manager_;
  Ogre::RenderWindow* window_;
  
  Ogre::String resource_path_;
  Ogre::String config_path_;
  
};




//!*************************************HERE BE DRAGONS*******************************************
//!***********************************************************************************************
/*#ifdef USE_RTSHADER_SYSTEM
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
//!***********************************************************************************************
//!************************************************************************************************/










#endif
