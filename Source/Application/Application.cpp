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

#include "Application.h"

#include <CEGUI.h>
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"

Application::Application() 
  : resource_path_ (getResourcePath()),
    config_path_ (getConfigPath())
{  
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
  Graphics::instance()->setup();
  
  // Add event listeners
//  root_->addFrameListener(Gui::instance());
  
  // Initialize the GUI system
  //Gui::instance()->initialize();
  
  
  //CEGUI::OgreRenderer& myRenderer = CEGUI::OgreRenderer::bootstrapSystem(*window_);//new CEGUI::OgreRenderer(*window_);
  
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

  // The case that the user didn't want to configure/wanted to quit
  if (win == 0) 
    exit(1);

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
  
  // Set the viewport for the GUI class
//  Gui::instance()->setViewport(vp);
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
