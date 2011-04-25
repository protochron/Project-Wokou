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

class TerrainGen
{
  private:
    Ogre::TerrainGlobalOptions *mTerrainGlobals;
    Ogre::TerrainGroup *mTerrainGroup;
    Ogre::SceneManager *mSceneMgr;
    bool mTerrainsImported;
    //OgreBites::Label *mInfoLabel;

    void getTerrainImage(bool flipX, bool FlipY, Ogre::Image &img);

  public:
    TerrainGen(Ogre::SceneManager *scene) : mSceneMgr(scene), mTerrainGlobals(OGRE_NEW Ogre::TerrainGlobalOptions()), mTerrainsImported(false)
    {}


    ~TerrainGen()
    {
      //OGRE_DELETE mTerrainGroup;
      //OGRE_DELETE mTerrainGlobals;
      mSceneMgr = NULL;
    }

    void configureTerrainDefaults(Ogre::Light *light);
    void initBlendMaps(Ogre::Terrain *terrain);
    void defineTerrain(long x, long y);

    Ogre::TerrainGlobalOptions *getTerrainGlobals()
    { return mTerrainGlobals; }

    Ogre::TerrainGroup *getTerrainGroup()
    { return mTerrainGroup; }

    bool getTerrainsImported()
    { return mTerrainsImported; }

    void setTerrainGroup(Ogre::TerrainGroup *t)
    { mTerrainGroup = t; }
};

#endif
