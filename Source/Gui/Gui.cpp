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

#include "Gui.h"

#include <iostream>


boost::shared_ptr<Gui> Gui::instance_;

Gui::Gui()
  : gorilla_ (new Gorilla::Silverback()),
    viewport_ (NULL),
    screen_ (NULL)
{

}


Gui* Gui::instance() 
{
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Gui);
    return instance_.get();
  }
}

void Gui::setViewport(Ogre::Viewport* viewport)
{
  viewport_ = viewport;
}

void Gui::initialize()
{
  if (viewport_ == NULL)
    return;
  
  gorilla_->loadAtlas("dejavu");
  screen_ = gorilla_->createScreen(viewport_, "dejavu");
  //screen_->setOrientation(Ogre::OrientationMode::OR_DEGREE_270);
  
  Gorilla::Layer *layer = screen_->createLayer(0);
  Gorilla::Rectangle* rect = layer->createRectangle(0, 0, 300, 20);
  rect->background_gradient(Gorilla::Gradient_Diagonal, Gorilla::rgb(98, 0, 63), Gorilla::rgb(255, 180, 174));
  Gorilla::MarkupText* markup;
  
  markup = layer->createMarkupText(14, 0, 0, "HUD!");

  
}


bool Gui::frameStarted(const Ogre::FrameEvent& event)
{

}

bool Gui::frameRenderingQueued(const Ogre::FrameEvent& event)
{

}

bool Gui::frameEnded(const Ogre::FrameEvent& event)
{

}

