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

#include "Terrain.h"

TerrainGenerator::TerrainGenerator(Ogre::SceneManager *scene) 
  : terrains_imported_(false),
    scene_manager_(scene)
{
  /*mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
  mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));
  mCamera->setNearClipDistance(0.1);
  mCamera->setFarClipDistance(50000);*/

  Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
  Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

  Ogre::Light* light = scene_manager_->createLight("tstLight");
  light->setDiffuseColour(Ogre::ColourValue::White);
  light->setSpecularColour(Ogre::ColourValue(1, 1, 1));

  scene_manager_->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
  light->setAttenuation(100000, 1, 1, 1);
  terrain_globals_ = OGRE_NEW Ogre::TerrainGlobalOptions();

  terrain_group_ = OGRE_NEW Ogre::TerrainGroup(scene_manager_, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
  terrain_group_->setFilenameConvention(Ogre::String("BasicTutorial3Terrain"), Ogre::String("dat"));
  terrain_group_->setOrigin(Ogre::Vector3::ZERO);



  configureTerrainDefaults(light);

  for (long x = 0; x <= 0; ++x)
      for (long y = 0; y <= 0; ++y)
          defineTerrain(x, y);

  // sync load since we want everything in place when we start
  terrain_group_->loadAllTerrains(true);

  if (terrains_imported_)
  {
      Ogre::TerrainGroup::TerrainIterator ti = terrain_group_->getTerrainIterator();
      while(ti.hasMoreElements())
      {
          Ogre::Terrain* t = ti.getNext()->instance;
          t->setPosition(Ogre::Vector3(0, -315, 0));
          initBlendMaps(t);
      }
  }

  terrain_group_->freeTemporaryResources();
  
  scene_manager_->setSkyDome(true, "Examples/CloudySky", 5, 8);

  //Optional skybox
  //Graphics::instance()->getSceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);
}


TerrainGenerator::~TerrainGenerator()
{
  
}


void TerrainGenerator::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
  img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  if (flipX)
    img.flipAroundY();
  if (flipY)
    img.flipAroundX();
}

void TerrainGenerator::defineTerrain(long x, long y)
{
  Ogre::String filename = terrain_group_->generateFilename(x, y);
  if (Ogre::ResourceGroupManager::getSingleton().resourceExists(terrain_group_->getResourceGroup(), filename))
    terrain_group_->defineTerrain(x, y);
  else
  {
    Ogre::Image img;
    getTerrainImage(x % 2 != 0, y % 2 != 0, img);
    terrain_group_->defineTerrain(x, y, &img);
    terrains_imported_ = true;
  }
}

void TerrainGenerator::initBlendMaps(Ogre::Terrain *terrain)
{
  Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
  Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
  Ogre::Real minHeight0 = 70; //Doesn't matter, apparently.
  Ogre::Real fadeDist0 = 40;
  Ogre::Real minHeight1 = 295;
  Ogre::Real fadeDist1 = 20;
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

void TerrainGenerator::configureTerrainDefaults(Ogre::Light *light)
{
  // Configure global
  terrain_globals_->setMaxPixelError(8);
  // testing composite map
  terrain_globals_->setCompositeMapDistance(3000);

  // Important to set these so that the terrain knows what to use for derived (non-realtime) data
  terrain_globals_->setLightMapDirection(light->getDerivedDirection());
  terrain_globals_->setCompositeMapAmbient(scene_manager_->getAmbientLight());
  terrain_globals_->setCompositeMapDiffuse(light->getDiffuseColour());

  // Configure default import settings for if we use imported image
  Ogre::Terrain::ImportData& defaultimp = terrain_group_->getDefaultImportSettings();
  defaultimp.terrainSize = 513;
  defaultimp.worldSize = 12000.0f;
  defaultimp.inputScale = 600;
  defaultimp.minBatchSize = 33;
  defaultimp.maxBatchSize = 65;
  // textures
  defaultimp.layerList.resize(3);
  defaultimp.layerList[1].worldSize = 200;
  defaultimp.layerList[1].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
  defaultimp.layerList[1].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
  defaultimp.layerList[0].worldSize = 100;
  defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
  defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
  defaultimp.layerList[2].worldSize = 30;
  defaultimp.layerList[2].textureNames.push_back("grass_green-01_diffusespecular.dds");
  defaultimp.layerList[2].textureNames.push_back("grass_green-01_normalheight.dds");
}
