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

#include "Graphics.h"

#include "Terrain/Terrain.h"

boost::shared_ptr<Graphics> Graphics::instance_;

Graphics::Graphics() : gamePhysics_(GamePhysics::instance())
{

}

//Blatantly stolen from Cody's Network code.
Graphics* Graphics::instance()
{
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Graphics);
    return instance_.get();
    }
}

void Graphics::setSceneManager( Ogre::SceneManager* sceneMgr ){
  sceneMgr_ = sceneMgr;
}

void Graphics::constructCamera()
{
  camera_ = sceneMgr_->createCamera("Camera");

  roty = 0;
  
  float angle = 1.40;
  
  camera_->setPosition( Vector3( 40, 20, 0 ) );
  camera_->lookAt( Vector3( 0, 0, 0 ) );
  
  camera_->setNearClipDistance(1);
}

void Graphics::moveCamera( double dx, double dy, double dz ){
  camera_->moveRelative( Vector3(dx, dy, dz) );
}

void Graphics::warpCamera( double x, double y, double z ){
  camera_->setPosition( Vector3(x, y, z) );
}

void Graphics::rotateCamera( double dx, double dy ){
  double PI = 3.1415926535;

  roty = roty + dy;
  
  Ogre::Vector3 temp = camera_->getDirection();
  //
  
  if( roty <= -PI / 2 ){
    roty = -PI/2;
    dy = -1 - temp.y;  
  }
  if( roty >= PI / 2 ){
    roty = PI/2;  
    dy = 1 - temp.y;
  }
 
  camera_->yaw(Ogre::Radian(dx));
  camera_->pitch(Ogre::Radian(dy));
  
  temp = camera_->getDirection();
  //std::cout << temp.x << " " << temp.y << " " << temp.z << std::endl;  
}


void Graphics::zoomCamera( double distance ){
  //camera_->moveRelative( Vector3(distance*normalx, distance*normaly, distance*normalz) );
}

void Graphics::setAmbient( double r, double g, double b ){
  sceneMgr_->setAmbientLight(Ogre::ColourValue(r, g, b));
}

bool Graphics::createLight( String name, double x, double y, double z ){
  for( unsigned int i = 0; i < lights.size(); i++ ){
    if( name == lights[i]->getName() )
      return 1; //A light with the same name exists. Fail.
  }
  //No other light exists. Add it!
  Ogre::Light* l = sceneMgr_->createLight( name );
  l->setPosition( x, y, z );
  lights.push_back( l );
  return 0;
}

void Graphics::destroyLight( String name ){
  for( unsigned int i = 0; i < lights.size(); i++ ){
    if( name == lights[i]->getName() )
      sceneMgr_->destroyLight( lights[i] );
      lights.erase( lights.begin()+i );
      i--;
  }
}

bool Graphics::createEntity( String name, String mesh, double x, double y, double z ){
  if( sceneMgr_->hasEntity( name ) == true ){
    return 1; //Duplicate name exists. Do NOT proceed or Ogre will throw a fit.
  }
  Ogre::Entity* temp = sceneMgr_->createEntity( name, mesh );
  Ogre::SceneNode* tempNode = sceneMgr_->getRootSceneNode()->createChildSceneNode();
  tempNode->setPosition( x, y, z );
  tempNode->attachObject( temp );
  return 0;
}

bool Graphics::moveEntity( String name, double dx, double dy, double dz ){
  if( sceneMgr_->hasEntity( name ) == false ){
    return 1; //The entity does not exist. Flee!
  }
  Ogre::Entity* temp = sceneMgr_->getEntity( name );
  Ogre::SceneNode* tempNode = temp->getParentSceneNode();
  tempNode->translate( Vector3( dx, dy, dz ) );
  return 0;
}

bool Graphics::rotateEntity( String name, Radian yrot, bool scene ){
  if( sceneMgr_->hasEntity( name ) == false ){
    return 1; //The entity does not exist. Flee!
  }
  Ogre::Entity* ship = sceneMgr_->getEntity( name );
  Ogre::SceneNode* shipNode = ship->getParentSceneNode();
  shipNode->yaw( yrot );
  return 0;
}

void Graphics::destroyEntity( String name ){
  if( sceneMgr_->hasEntity( name ) == true ){
    Ogre::Entity* temp = sceneMgr_->getEntity( name );
    Ogre::SceneNode* tempNode = temp->getParentSceneNode();
    sceneMgr_->destroyEntity( temp );
    sceneMgr_->destroySceneNode( tempNode );
  }
}

void Graphics::setAspectRatio(double x, double y){
  if( y != 0 ){
    camera_->setAspectRatio( x/y );
  }
}

Ogre::SceneManager *Graphics::getSceneManager()
{
    return sceneMgr_;
}

void Graphics::setup(){
   createLight( "MainLight", 20, 80, 50 );
  
  createEntity( "Ogre", "Sinbad.mesh", 10, 0, 0 );
  createEntity( "Ogre", "Sinbad.mesh", 20, 0, 0 );
  destroyEntity( "Ogre" );
  
  //createEntity( "Player 1", "BetterShip.mesh", 0, 0, 0 );
  //sceneMgr_->getEntity("Player 1")->getParentSceneNode()->scale( .1, .1, .1);
  createEntity( "Player 2", "penguin.mesh", 0, 0, 0 );
  //rotateEntity( "Player 2", Ogre::Radian(1.4) );
  /*createEntity( "Player 3", "BetterShip.mesh", 0, 0, 0 );
  createEntity( "Player 4", "BetterShip.mesh", 0, 0, 0 );
  moveEntity( "Player 2", 10, 0, 10 );
  moveEntity( "Player 3", 10, 0, 10 );
  moveEntity( "Player 4", 10, 0, 10 );
  rotateEntity( "Player 3", Ogre::Radian(.5) );
  rotateEntity( "Player 4", Ogre::Radian(.5) );
  moveEntity( "Player 4", -10, 0, 0 );*/

  //Set up motion state callbacks
  //GamePhysics::instance()->setupRigidDynamicsBody(sceneMgr_->getEntity("Player 1")->getParentSceneNode());
  //GamePhysics::instance()->setupRigidDynamicsBody(sceneMgr_->getEntity("Player 2")->getParentSceneNode());
  
  // Set ambient light
  sceneMgr_->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));

  // Add a sky dome to the game
  //sceneMgr_->setSkyDome(true, "Examples/CloudySky", 5, 8);

  // Create the ocean
  Ogre::Plane oceanSurface;
  oceanSurface.normal = Ogre::Vector3::UNIT_Y;
  oceanSurface.d = 20;
  
  Ogre::MeshManager::getSingleton().createPlane("OceanSurface",
     Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
     oceanSurface,
     11000, 11000, 50, 50, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);

  Entity* mOceanSurfaceEnt = sceneMgr_->createEntity( "OceanSurface", "OceanSurface" );
  sceneMgr_->getRootSceneNode()->createChildSceneNode()->attachObject(mOceanSurfaceEnt);
  //mOceanSurfaceEnt->setMaterialName("Ocean2_HLSL_GLSL");
  mOceanSurfaceEnt->setMaterialName("WOKOU-Ocean");

  //TerrainGenerator terr(sceneMgr_);
}
