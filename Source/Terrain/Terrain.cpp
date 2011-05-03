/*
 * =====================================================================================
 *
 *       Filename:  terrain.cpp
 *
 *    Description:  Implementation of class for handling terrain generation.
 *
 *        Version:  1.0
 *        Created:  04/24/2011 04:41:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Norris (), dnorris10@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "Terrain.h"

void TerrainGen::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
  img.load("test1.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  if (flipX)
    img.flipAroundY();
  if (flipY)
    img.flipAroundX();
}

void TerrainGen::defineTerrain(long x, long y)
{
  Ogre::String filename = terrainGroup_->generateFilename(x, y);
  if (Ogre::ResourceGroupManager::getSingleton().resourceExists(terrainGroup_->getResourceGroup(), filename))
  {
    terrainGroup_->defineTerrain(x, y);
  }
  else
  {
    Ogre::Image img;
    getTerrainImage(x % 2 != 0, y % 2 != 0, img);
    terrainGroup_->defineTerrain(x, y, &img);
    terrainsImported_ = true;
  }
}

void TerrainGen::initBlendMaps(Ogre::Terrain *terrain)
{
  Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
  Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
  Ogre::Real minHeight0 = 70;
  Ogre::Real fadeDist0 = 40;
  Ogre::Real minHeight1 = 70;
  Ogre::Real fadeDist1 = 15;
  float* pBlend1 = blendMap1->getBlendPointer();
  for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
  {
    for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
    {
      Ogre::Real tx, ty;

      blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
      Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
      Ogre::Real val = (height - minHeight0) / fadeDist0;
      val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);

      val = (height - minHeight1) / fadeDist1;
      val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
      *pBlend1++ = val;
    }
  }
  blendMap0->dirty();
  blendMap1->dirty();
  blendMap0->update();
  blendMap1->update();
}

void TerrainGen::configureTerrainDefaults(Ogre::Light *light)
{
  // Configure global
  terrainGlobals_->setMaxPixelError(8);
  // testing composite map
  terrainGlobals_->setCompositeMapDistance(3000);

  // Important to set these so that the terrain knows what to use for derived (non-realtime) data
  terrainGlobals_->setLightMapDirection(light->getDerivedDirection());
  terrainGlobals_->setCompositeMapAmbient(Graphics::instance()->getSceneManager()->getAmbientLight());
  terrainGlobals_->setCompositeMapDiffuse(light->getDiffuseColour());

  // Configure default import settings for if we use imported image
  Ogre::Terrain::ImportData& defaultimp = terrainGroup_->getDefaultImportSettings();
  defaultimp.terrainSize = 256;
  defaultimp.worldSize = 12000.0f;
  defaultimp.inputScale = 600;
  defaultimp.minBatchSize = 33;
  defaultimp.maxBatchSize = 65;
  // textures
  //defaultimp.layerList.resize(3);
  //defaultimp.layerList[0].worldSize = 100;
  //defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
  //defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
  //defaultimp.layerList[1].worldSize = 30;
  //defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
  //defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
  //defaultimp.layerList[2].worldSize = 200;
  //defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
  //defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}
