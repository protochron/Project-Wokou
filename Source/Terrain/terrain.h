/*
 * =====================================================================================
 *
 *       Filename:  terrain.h
 *
 *    Description:  Class for handling terrain generation. 
 *
 *        Version:  1.0
 *        Created:  04/24/2011 04:36:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Norris (), dnorris10@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef TERRAIN_GEN_H
#define TERRAIN_GEN_H
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

class Terrain
{
  private:
    Ogre::TerrainGlobalOptions *mTerrainGlobals;
    Ogre::TerrainGroup *mTerrainGroup;
    bool mTerrainsImported;
    //OgreBites::Label *mInfoLabel;

    void defineTerrain(long x, long y);
    void initBlendMaps(OgreTerrain::Terrain *terrain);
    void configureTerrainDefaults(Ogre::Light *light);

  public:
    Terrain() : mTerrainGlobals(0), mTerrainsImported(false), mInfoLabel(0)
    {}

    ~Terrain()
    {
      OGRE_DELETE mTerrainGroup;
      OGRE_DELETE mTerrainGlobals;
    }

    Ogre::TerrainGlobalOptions *getTerrainGlobals()
    { return mTerrainGlobals; }

    Ogre::TerrainGroup *getTerrainGroup()
    { return mTerrainGroup; }

    bool getTerrainsImported()
    { return mTerrainsImported; }
};

#endif
