
#include <OGRE/ExampleApplication.h>
#include <OGRE/OgreLogManager.h>
#include "Actions/Action.h"
#include "Common/Common.h"
#include "Networking/Network.h"
#include "Graphics/Graphics.h"

using namespace std;

class Example : public ExampleApplication {
public:
	
  void createScene()
  {
    Ogre::Entity* peng = mSceneMgr->createEntity("Peng", "penguin.mesh");
    Ogre::Entity* ogre = mSceneMgr->createEntity("Ogre", "ogrehead.mesh");
    
    mSceneMgr->getRootSceneNode()->attachObject(peng);
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->
      createChildSceneNode("Test", Ogre::Vector3( 50, 0, 0 ));
    headNode->attachObject(ogre);
    
    // Set ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    // Create a light
    Ogre::Light* l = mSceneMgr->createLight("MainLight");
    l->setPosition(20,80,50);
    

  }
  
private:
  
  
};

int main(int argc, char **argv)
{
  //Network* x = Network::instance();
  //x->connect("64.90.182.55");
  /* 
  Action a(func);
  
  if (a.handle()) {
    return 0;
  }*/
  
  Example app;
  try {
    app.go();
  } 
  catch( Ogre::Exception& e ){
    std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;

  }
  
  //while (1) { }
  
  
  return 0;
}
