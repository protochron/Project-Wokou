#include "Application.h"

Application::Application() 

{
  resource_path_ = getResourcePath();
  config_path_ = getConfigPath();
  
  // Initialize the root system
  root_.reset(initializeRoot(resource_path_, config_path_, resource_path_));
  
  // Initialize the window 
  window_ = initializeWindow();
  
  // Set up references to all the resources
  initializeResources();
  
  // Set up the scene manager
  scene_manager_ = initializeSceneManager();
  
  // Create a new camera
  Graphics::instance()->constructCamera();
  
  // Constructs the viewports
  initializeViewport();
  
  // Initialize the input subsystem
  input_system_.reset(initializeInput(root_.get(), window_));
  
  // Set default mipmap level
  TextureManager::getSingleton().setDefaultNumMipmaps(5);
  
  // Create any resource listeners (for loading screens)
  //createResourceListener();
  
  // Load resources
  ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  // Create the scene
  Graphics::instance()->render();
}

Application::~Application()
{
  // The boost::shared_ptrs free what needs to be freed here.
}


void Application::go()
{
  root_->startRendering();

  // clean up
  //destroyScene();
  
  /*#ifdef USE_RTSHADER_SYSTEM
    // Finalize shader generator.
    finalizeShaderGenerator();
  #endif*/
}

Ogre::Root* Application::initializeRoot(const Ogre::String& plugin, 
                                        const Ogre::String& config, 
                                        const Ogre::String& log)
{
  Ogre::Root *r = OGRE_NEW Root( plugin + "plugins.cfg", config + "ogre.cfg", log + "Ogre.log");
  
  if (r == 0)
    throw "Could not initialize Ogre::Root!";
    
  return r;
}


Ogre::RenderWindow* Application::initializeWindow()
{
  Ogre::RenderWindow* win = NULL;
  
  if (root_->showConfigDialog())
    win = root_->initialise(true);

  if (win == 0)
    throw "Could not initialize the window!";

  return win;
}

void Application::initializeResources()
{
  Ogre::ConfigFile config;
  Ogre::String section, type, arch;
  
  config.load(resource_path_ + "resources.cfg");
  
  Ogre::ConfigFile::SectionIterator itr = config.getSectionIterator();
  
  while (itr.hasMoreElements()) {
    section = itr.peekNextKey();
    
    Ogre::ConfigFile::SettingsMultiMap* settings = itr.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator i;
    
    for (i = settings->begin(); i != settings->end(); ++i) {
      type = i->first; 
      arch = i->second;
      
      #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        
        // OS X does not set the working directory relative to the app,
        // In order to make things portable on OS X we need to provide
        // the loading with it's own bundle path location
        
        if (!Ogre::StringUtil::startsWith(arch, "/", false))
    	    arch = String(macBundlePath() + "/" + arch);      
      #endif
      
      ResourceGroupManager::getSingleton().addResourceLocation(arch, type, section);
    }
  }
}

Input* Application::initializeInput(Ogre::Root* root, Ogre::RenderWindow* window)
{
  Input* i = new Input(window);
  root->addFrameListener(i);
  return i;
}

Ogre::SceneManager* Application::initializeSceneManager()
{
  Ogre::SceneManager *mgr = root_->createSceneManager(ST_GENERIC, "ExampleSMInstance");
  Graphics::instance()->setSceneManager(mgr);
  
  return mgr;
}

void Application::initializeViewport()
{
  // Create one viewport, entire window
  Viewport* vp = window_->addViewport(Graphics::instance()->camera());
  
  vp->setBackgroundColour(ColourValue(0,0,0));

  // Alter the camera aspect ratio to match the viewport
  Graphics::instance()->camera()->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}

Ogre::String Application::getResourcePath()
{
  #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    return macBundlePath() + "/Contents/Resources/";
  #else
    return "";
  #endif
}


Ogre::String Application::getConfigPath()
{
  return getResourcePath();
}




/*

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
#endif*/
