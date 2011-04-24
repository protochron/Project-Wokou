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

  camera_->setPosition(Vector3(camerax,cameray,cameraz));
  camera_->lookAt(Vector3(normalx,normaly,normalz));

  camera_->setNearClipDistance(5);
}

void Graphics::moveCamera( double dx, double dy, double dz ){
  camera_->moveRelative(Vector3(dx, dy, dz));
}

void Graphics::warpCamera( double x, double y, double z ){
  
}

void Graphics::rotateCamera( double dx, double dy ){
  double PI = 3.14159265;
  
  rotx = rotx + dx;
  roty = roty + dy;
  
  normalx = cos(rotx) * cos(roty);
  normaly = sin(rotx) * cos(roty);
  normalz = sin(roty); 
  
  //Handle rotational limits. rotx can go in a full circle. roty can go straight up to straight down.
  if(rotx > 2*PI) //360
    rotx = rotx - 2*PI;
  if(rotx < -2*PI)
    rotx = rotx + 2*PI;
  if(roty >= PI/2) //90
    roty = PI/2;
  if(roty <= -PI/2)
    roty = -PI/2;

  camera_->lookAt( camerax+normalx, cameray+normaly, cameraz+normalz );
}


void Graphics::zoomCamera( double distance ){
  
}

void Graphics::render(){
  camera_->setPosition(Vector3(camerax, cameray, cameraz));
  camera_->lookAt(Vector3(normalx, normaly, normalz));
  
  Ogre::Entity* cube = sceneMgr_->createEntity("Cube", "Ship.mesh");
    
  sceneMgr_->getRootSceneNode()->attachObject(cube);
  Ogre::SceneNode* headNode = sceneMgr_->getRootSceneNode()->createChildSceneNode();
        
  // Set ambient light
  sceneMgr_->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
  // Create a light
  Ogre::Light* l = sceneMgr_->createLight("MainLight");
  l->setPosition(20,80,50);
}
