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

#include <Ogre.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletCollision/btBulletCollisionCommon.h>

class ObjectMotionState : public btMotionState
{
    protected:
        Ogre::SceneNode *visibleObj_;
        btTransform pos_;
    public:
        ObjectMotionState(const btTransform &initial_pos, Ogre::SceneNode *node)
        {
            visibleObj_ = node;
            pos_ = initial_pos;
        }

        virtual ~ObjectMotionState()
        {}

        void setNode(Ogre::SceneNode *node)
        {
            visibleObj_ = node;
        }

        //Modify the worldTrans to reflect our position
        virtual void getWorldTransform(btTransform &worldTrans) const
        {
            worldTrans = pos_;
        }

        virtual void setWorldTransform(const btTransform &worldTrans)
        {
            if(visibleObj_ == NULL)
                return; //return before we set a node
            btQuaternion rot = worldTrans.getRotation();
            visibleObj_->setOrientation(rot.w(), rot.x(), rot.y(), rot.z());
            btVector3 pos = worldTrans.getOrigin();
            visibleObj_->setPosition(pos.x(), pos.y(), pos.z());
        }

};
#endif
