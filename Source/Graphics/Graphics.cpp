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

boost::shared_ptr<Graphics> Graphics::instance_;

Graphics::Graphics()
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
  
  camerax = 0;
  cameray = 0;
  cameraz = 500;
  normalx = 0;
  normaly = 0;
  normalz = -1;

  camera_->setPosition( Vector3(camerax,cameray,cameraz) );
  camera_->lookAt( Vector3(normalx,normaly,normalz) );

  camera_->setNearClipDistance(5);
}

void Graphics::moveCamera( double dx, double dy, double dz ){
  camera_->moveRelative( Vector3(dx, dy, dz) );
}

void Graphics::warpCamera( double x, double y, double z ){
  camera_->moveRelative( Vector3(x-camerax, y-cameray, z-cameraz) );
}

void Graphics::rotateCamera( double dx, double dy ){
  camera_->rotate(Ogre::Vector3(0,1,0), Ogre::Radian(dx));
  camera_->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(dy));
}


void Graphics::zoomCamera( double distance ){
  camera_->moveRelative( Vector3(distance*normalx, distance*normaly, distance*normalz) );
}

bool Graphics::createLight( String str, double x, double y, double z ){
  for( unsigned int i = 0; i < lights.size(); i++ ){
    if( str == lights[i]->getName() )
      return 1; //A light with the same name exists. Fail.
  }
  //No other light exists. Add it!
  Ogre::Light* l = sceneMgr_->createLight( str );
  l->setPosition( x, y, z );
  lights.push_back( l );
  return 0;
}

void Graphics::destroyLight( String str ){
  for( unsigned int i = 0; i < lights.size(); i++ ){
    if( str == lights[i]->getName() )
      sceneMgr_->destroyLight( lights[i] );
      lights.erase( lights.begin()+i );
      i--;
  }
}


void Graphics::render(){
  createLight( "Hello", 1, 2, 3 );
  createLight( "Hello2", -1, -2, -1 );
  destroyLight( "Hello" );
  
  camera_->setPosition(Vector3(camerax, cameray, cameraz));
  camera_->lookAt(Vector3(normalx, normaly, normalz));
  
  Ogre::Entity* cube = sceneMgr_->createEntity("Cube", "Ship.mesh");
  sceneMgr_->getRootSceneNode()->attachObject(cube);

  
  Ogre::SceneNode* headNode = sceneMgr_->getRootSceneNode()->createChildSceneNode();
        
  // Set ambient light
  sceneMgr_->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
    
  // Create a light
  Ogre::Light* l = sceneMgr_->createLight("MainLight");
  l->setPosition(20,80,50);
}
