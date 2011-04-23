#include "Application.h"

Application::Application(){
  mFrameListener = 0;
  mRoot = 0;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
  mResourcePath = macBundlePath() + "/Contents/Resources/";
  mConfigPath = mResourcePath;
#elif OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
  mResourcePath = macBundlePath() + "/";
  mConfigPath = mResourcePath;
#else
  mResourcePath = "";
  mConfigPath = mResourcePath;
#endif

#ifdef USE_RTSHADER_SYSTEM
  mShaderGenerator	 = NULL;		
  mMaterialMgrListener = NULL;
#endif
}

Application::~Application(){
  if (mFrameListener)
    delete mFrameListener;
  if (mRoot)
    OGRE_DELETE mRoot;

#ifdef OGRE_STATIC_LIB
  mStaticPluginLoader.unload();
#endif
}

void Application::go(){
  if (!setup())
    return;

  mRoot->startRendering();

  // clean up
  destroyScene();	

#ifdef USE_RTSHADER_SYSTEM
  // Finalize shader generator.
  finalizeShaderGenerator();
#endif
}

bool Application::setup(){
  String pluginsPath;
  // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
#if OGRE_DEBUG_MODE
  pluginsPath = mResourcePath + "plugins_d.cfg";
#else
  pluginsPath = mResourcePath + "plugins.cfg";
#endif
#endif
		
  mRoot = OGRE_NEW Root(pluginsPath, 
			mConfigPath + "ogre.cfg", mResourcePath + "Ogre.log");
#ifdef OGRE_STATIC_LIB
  mStaticPluginLoader.load();
#endif
  setupResources();

  bool carryOn = configure();
  if (!carryOn) 
    return false;

  chooseSceneManager();
  createCamera();
  createViewports();
#ifdef USE_RTSHADER_SYSTEM
  // Initialize shader generator.
  carryOn = initializeShaderGenerator(mSceneMgr);
  if (!carryOn) 
    return false;
#endif
  // Set default mipmap level (NB some APIs ignore this)
  TextureManager::getSingleton().setDefaultNumMipmaps(5);
  // Create any resource listeners (for loading screens)
  createResourceListener();
  // Load resources
  loadResources();

  // Create the scene
  createScene();
  createFrameListener();
  return true;
}

bool Application::configure(){
  if(mRoot->showConfigDialog()){
    // If returned true, user clicked OK so initialise
    // Here we choose to let the system create a default rendering window by passing 'true'
    mWindow = mRoot->initialise(true);
    return true;
  }
  else{
    return false;
  }
}

void Application::chooseSceneManager(){
  // Create the SceneManager, in this case a generic one
  mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
}

void Application::createCamera(){
  // Create the camera
  mCamera = mSceneMgr->createCamera("PlayerCam");
  // Position it at 500 in Z direction
  mCamera->setPosition(Vector3(0,0,500));
  // Look back along -Z
  mCamera->lookAt(Vector3(0,0,-300));
  mCamera->setNearClipDistance(5);
}

void Application::createFrameListener(){
  //#if OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
  //mFrameListener= new Input(mWindow, mCamera, true, true, true);
  //#else
  mFrameListener= new Input(mWindow, mCamera); //Risk of screw-uppage here.
  //#endif
  mFrameListener->showDebugOverlay(true);
  mRoot->addFrameListener(mFrameListener);
}

void Application::createScene(){
  Ogre::Entity* cube = mSceneMgr->createEntity("Cube", "penguin.mesh");
    
  mSceneMgr->getRootSceneNode()->attachObject(cube);
  Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
        
  // Set ambient light
  mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
  // Create a light
  Ogre::Light* l = mSceneMgr->createLight("MainLight");
  l->setPosition(20,80,50);
}

void Application::destroyScene(){
  
}

void Application::createViewports(){
  // Create one viewport, entire window
  Viewport* vp = mWindow->addViewport(mCamera);
  vp->setBackgroundColour(ColourValue(0,0,0));

  // Alter the camera aspect ratio to match the viewport
  mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

void Application::setupResources(){
  // Load resource paths from config file
  ConfigFile cf;
#if OGRE_DEBUG_MODE
  cf.load(mResourcePath + "resources_d.cfg");
#else
  cf.load(mResourcePath + "resources.cfg");
#endif

  // Go through all sections & settings in the file
  ConfigFile::SectionIterator seci = cf.getSectionIterator();

  String secName, typeName, archName;
  while (seci.hasMoreElements()){
    secName = seci.peekNextKey();
    ConfigFile::SettingsMultiMap *settings = seci.getNext();
    ConfigFile::SettingsMultiMap::iterator i;
    for (i = settings->begin(); i != settings->end(); ++i){
      typeName = i->first;
      archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE
      // OS X does not set the working directory relative to the app,
      // In order to make things portable on OS X we need to provide
      // the loading with it's own bundle path location
      if (!StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
	archName = String(macBundlePath() + "/" + archName);
#endif
      ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
    }
  }
}

void Application::createResourceListener(){
  
}

void Application::loadResources(){
  // Initialise, parse scripts etc
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}




//!*********************************HERE BE DRAGONS****************************************
//!****************************************************************************************
//!****************************************************************************************
//!****************************************************************************************
#ifdef USE_RTSHADER_SYSTEM
bool Application::initializeShaderGenerator(SceneManager* sceneMgr){
  if (RTShader::ShaderGenerator::initialize()){
    mShaderGenerator = RTShader::ShaderGenerator::getSingletonPtr();

    // Set the scene manager.
    mShaderGenerator->setSceneManager(sceneMgr);

    // Setup core libraries and shader cache path.
    ResourceGroupManager::LocationList resLocationsList = ResourceGroupManager::getSingleton().getResourceLocationList(ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
    ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
    String shaderCoreLibsPath;
    String shaderCachePath;

    // Default cache path is current directory;
    shaderCachePath = "./";

    // Try to find the location of the core shader lib functions and use it
    // as shader cache path as well - this will reduce the number of generated files
    // when running from different directories.
    for (; it != itEnd; ++it){
      if ((*it)->archive->getName().find("RTShaderLib") != String::npos){
	shaderCoreLibsPath = (*it)->archive->getName() + "/";
	shaderCachePath    = shaderCoreLibsPath;
	break;
      }
    }
    // Core shader libs not found -> shader generating will fail.
    if (shaderCoreLibsPath.empty())			
      return false;			
    // Add resource location for the core shader libs. 
    ResourceGroupManager::getSingleton().addResourceLocation(shaderCoreLibsPath , "FileSystem");
    // Set shader cache path.
    mShaderGenerator->setShaderCachePath(shaderCachePath);		
  }
  
  void Application::finalizeShaderGenerator(){
    // Unregister the material manager listener.
    if (mMaterialMgrListener != NULL){			
      MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
      delete mMaterialMgrListener;
      mMaterialMgrListener = NULL;
    }

    // Finalize CRTShader system.
    if (mShaderGenerator != NULL){
      RTShader::ShaderGenerator::finalize();
      mShaderGenerator = NULL;
    }
  }
#endif
