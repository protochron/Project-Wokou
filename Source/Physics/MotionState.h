/*
 * =====================================================================================
 *
 *       Filename:  MotionState.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/28/2011 12:56:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Norris (), dnorris10@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef CUST_MOTION_STATE_H
#define CUST_MOTION_STATE_H

#include "Physics.h"
#include <Ogre.h>

class ObjectMotionState : public btMotionState
{
    protected:
        Ogre::SceneNode *mVisibleobj;
        btTransform mPosl;
    public:
        ObjectMotionState(const btTransform &initial_pos, Ogre::SceneNode *node)
        {
            mVisibleobj = node;
            mPosl = initialpos;
        }

        virtual ~ObjectMotionState()
        {}

        void setNode(Ogre::SceneNode *node) const
        {
            mVisibleobj = node
        }

        virtual void getWorldTransform(btTransform &worldTrans) const
        {
            worldTrans = mPosl;
        }

        virtual void setWorldTransform(const btTransform &worldTrans)
        {
            if(mVisibleobj == NULL)
                return; //return before we set a node
            btQuaternion rot = worldTrans.getRotation();
            mVisibleobj->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
            btVector3 pos = worldTrans.getOrigin();
            mVisibleobj->setPosition(pos.x(), pos.y(), pos.z());
        }

};
#endif
