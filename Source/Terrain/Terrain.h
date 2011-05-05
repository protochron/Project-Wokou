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

#ifndef _PIRATE_TERRAIN_H_
#define _PIRATE_TERRAIN_H_

#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <Ogre.h>
//#include "../Graphics/Graphics.h"

class TerrainGenerator {
public:
  TerrainGenerator(Ogre::SceneManager *scene);

  ~TerrainGenerator();
  
  void configureTerrainDefaults(Ogre::Light *light);
  void initBlendMaps(Ogre::Terrain *terrain);
  void defineTerrain(long x, long y);

  Ogre::TerrainGlobalOptions *getTerrainGlobals()
  { return terrain_globals_; }

  Ogre::TerrainGroup *getTerrainGroup()
  { return terrain_group_; }

  bool getTerrainsImported()
  { return terrains_imported_; }

  void setTerrainGroup(Ogre::TerrainGroup *t)
  { terrain_group_ = t; }

private:

    Ogre::TerrainGlobalOptions *terrain_globals_;
    Ogre::TerrainGroup *terrain_group_;
    Ogre::SceneManager *scene_manager_;
    bool terrains_imported_;

    void getTerrainImage(bool flipX, bool FlipY, Ogre::Image &img);

};

#endif
