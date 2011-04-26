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

#ifndef _PIRATE_APPLICATION_H_
#define _PIRATE_APPLICATION_H_

#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>

#include "Common/Common.h"
#include "Engine/Engine.h"
#include "Input/Input.h"
#include "Networking/Network.h"
#include "Graphics/Graphics.h"
#include "Physics/Physics.h"

#include <boost/shared_ptr.hpp>


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
  #include "macUtils.h"
#endif


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
  
private:
  boost::shared_ptr<Ogre::Root> root_;
  boost::shared_ptr<Input> input_system_;

  Ogre::SceneManager* scene_manager_;
  Ogre::RenderWindow* window_;
  
  Ogre::String resource_path_;
  Ogre::String config_path_;
  
};

#endif
