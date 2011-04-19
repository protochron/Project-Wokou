
#include <OGRE/ExampleApplication.h>
#include <OGRE/OgreLogManager.h>
#include "Actions/Action.h"
#include "Common/Common.h"
#include "Networking/Network.h"

using namespace std;

class Example : public ExampleApplication {
public:
	
  void createScene()
  {
    Ogre::Entity *ent = mSceneMgr->createEntity("MyEntity", "penguin.mesh");
    mSceneMgr->getRootSceneNode()->attachObject(ent);
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
  
//  Example app;
//  app.go();

    while (1) { }
  
  
  return 0;
}