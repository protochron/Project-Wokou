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

#ifndef _PIRATE_GRAPHICS_H_
#define _PIRATE_GRAPHICS_H_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

#include <OgreCamera.h>

#include "ExampleFrameListener.h"

class Graphics {
 public:
  //Only allow one instance of the class.
  static Graphics* instance();
  
  //Custom graphics functions go here.

  /**
   * Takes the SceneManager from Application so we know what to draw to.
   */
  void setSceneManager( Ogre::SceneManager* mSceneMgr );
  
  /**
   * Constructs a new camera.
   * NOTE: Must be called after setSceneManager();
   */ 
  void constructCamera(  );
  
  /**
   * Moves the camera according to the amounts specified. 
   * 
   * The parameters represent deltas or how much the object should move in each
   * direction.
   */ 
  void moveCamera(double dx, double dy, double dz);
  
  /**
   * Warps the camera to the given location.
   *
   * The parameters indicate a location, not deltas.
   */
  void warpCamera(double x, double y, double z);
   
  /**
   * Rotates the camera according to the amounts specified.
   *
   * The parameters represent deltas for the x and y directions.
   */
  void rotateCamera(double dx, double dy);

  /**
   * Zooms the camera a total distance in the direction of the camera normal.
   *
   * Positive distance indicates zoom in the direction of the normal; negative opposite.
   */
  void zoomCamera(double distance);
  
  /**
   * Renders everything to the screen.
   */
  void render();
  
  //! This is used within the Application class to set up a scene.
  Ogre::Camera* camera() const
    { return camera_; }
    
    
 protected:
  Graphics();
  
 private:
  static boost::shared_ptr<Graphics> instance_;

  Ogre::SceneManager* sceneMgr_;
  Ogre::Camera* camera_;
  
  double camerax;
  double cameray;
  double cameraz;
  
  double rotx; //Horizontal angle in radians. ( 0 ... 2PI )
  double roty; //Vertical angle in radians ( -PI/2 ... PI/2 )
  
  double normalx;
  double normaly;
  double normalz;
};

#endif
