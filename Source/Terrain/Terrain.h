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
#include <Ogre.h>
#include "../Graphics/Graphics.h"

class TerrainGen
{
  private:
    Ogre::TerrainGlobalOptions *terrainGlobals_;
    Ogre::TerrainGroup *terrainGroup_;
    bool terrainsImported_;

    void getTerrainImage(bool flipX, bool FlipY, Ogre::Image &img);

  public:
    TerrainGen(Ogre::SceneManager *scene) : terrainGlobals_(OGRE_NEW Ogre::TerrainGlobalOptions()), terrainsImported_(false)
    {
        Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
        lightdir.normalise();
        Ogre::Light *light = Graphics::instance()->getSceneManager()->createLight("TerrainLight");
        
        light->setType(Ogre::Light::LT_DIRECTIONAL);
        light->setDirection(lightdir);
        light->setDiffuseColour(Ogre::ColourValue::White);
        light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

        //Construct TerrainGroup
        terrainGroup_ = OGRE_NEW Ogre::TerrainGroup(Graphics::instance()->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, 256, 12000.f);
        terrainGroup_->setFilenameConvention(Ogre::String("TestTerrain1"), Ogre::String("dat"));
        terrainGroup_->setOrigin(Ogre::Vector3::ZERO);

        configureTerrainDefaults(light);

        for(long x = 0; x <= 0; ++x)
        {
            for(long y = 0; y <=0; ++y)
                defineTerrain(x,y);
        }

        terrainGroup_->loadAllTerrains(true);

        if(terrainsImported_)
        {
            Ogre::TerrainGroup::TerrainIterator ti = terrainGroup_->getTerrainIterator();
            while(ti.hasMoreElements())
            {
                Ogre::Terrain *t = ti.getNext()->instance;
                initBlendMaps(t);
            }
        }

        terrainGroup_->freeTemporaryResources();

        //Optional skybox
        Graphics::instance()->getSceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);
    }


    ~TerrainGen()
    {
      //OGRE_DELETE terrainGroup_;
      //OGRE_DELETE terrainGlobals_;
    }

    void configureTerrainDefaults(Ogre::Light *light);
    void initBlendMaps(Ogre::Terrain *terrain);
    void defineTerrain(long x, long y);

    Ogre::TerrainGlobalOptions *getTerrainGlobals()
    { return terrainGlobals_; }

    Ogre::TerrainGroup *getTerrainGroup()
    { return terrainGroup_; }

    bool getTerrainsImported()
    { return terrainsImported_; }

    void setTerrainGroup(Ogre::TerrainGroup *t)
    { terrainGroup_ = t; }
};

#endif
