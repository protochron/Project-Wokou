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

#ifndef _PIRATE_GUI_H_
#define _PIRATE_GUI_H_

#include <OGRE/OgreFrameListener.h>
#include <OGRE/OgreViewport.h>
#include <boost/shared_ptr.hpp>
#include "Gui/Gorilla.h"


class Gui : public Ogre::FrameListener {
public:

  static Gui* instance();

  //! Called when a frame is about to begin rendering.
  bool frameStarted(const Ogre::FrameEvent& event);
  
  //! Called after all render targets have had their rendering commands issued,
  //! but before render windows have been asked to flip their buffers over. 
  bool frameRenderingQueued(const Ogre::FrameEvent& event);

  //! Called just after a frame has been rendered. 
  bool frameEnded(const Ogre::FrameEvent& event);

  //! Sets the viewport for the GUI (this must be set)
  void setViewport(Ogre::Viewport* viewport);

  //! Initializes the GUI system. setViewport needs to have been called
  void initialize();

protected:
  
  // Do not allow construction of this class. It's a Singleton!
  Gui();


private:
  static boost::shared_ptr<Gui> instance_;
  
  Ogre::Viewport* viewport_;
  Gorilla::Silverback* gorilla_;
  Gorilla::Screen* screen_;

};






#endif