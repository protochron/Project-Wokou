
#include <OGRE/ExampleApplication.h>
#include <OGRE/OgreLogManager.h>
#include "Actions/Action.h"
#include "Common/Common.h"
#include <iostream>
#include <queue>
#include <boost/shared_ptr.hpp>

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
  Example app;
  app.go();
  
  return 0;
}