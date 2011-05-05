/*
 * =====================================================================================
 *
 *       Filename:  Physics.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/28/2011 12:48:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Daniel Norris (), dnorris10@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "Physics.h"

boost::shared_ptr<GamePhysics> GamePhysics::instance_;

//Mostly sets up defaults
GamePhysics::GamePhysics() : gravity_(btVector3(0, -10, 0)), worldSize_(10.0), shipMass_(10)
{
    broadphase_ = new btDbvtBroadphase();
    collisionConfig_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collisionConfig_); 
    solver_ = new btSequentialImpulseConstraintSolver();
    dynamicsWorld_ = new btDiscreteDynamicsWorld(dispatcher_, broadphase_, solver_, collisionConfig_);
    dynamicsWorld_->setGravity(gravity_);

    worldTransform_ = btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)); //shared world transform that all motion states will need

    //Sets up a ground
    // May get rid of this later
    ground_ = new btStaticPlaneShape(btVector3(0, 0, 0), 0);
    groundMotionState_ = new btDefaultMotionState(worldTransform_);
    btRigidBody::btRigidBodyConstructionInfo
        groundRigidBodyCI(0, groundMotionState_, ground_, btVector3(0,0,0));
   groundBody_ = new btRigidBody(groundRigidBodyCI);
   dynamicsWorld_->addRigidBody(groundBody_);
}

//Delete all allocated space.
//Bullet forces us to clean up after ourselves so this is absolutely necessary.
GamePhysics::~GamePhysics()
{
    dynamicsWorld_->removeRigidBody(groundBody_);
    delete groundBody_->getMotionState();
    delete groundBody_;

    delete ground_;
    delete dynamicsWorld_;
    delete solver_;
    delete dispatcher_;
    delete collisionConfig_;
    delete broadphase_;
}

//Return the pointer to the object unless it doesn't exist already
GamePhysics* GamePhysics::instance()
{
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new GamePhysics);
    return instance_.get();
  }
}

void GamePhysics::setupRigidDynamicsBody(Ogre::SceneNode *node)
{
    ObjectMotionState::ObjectMotionState *state = new ObjectMotionState(worldTransform_, node); 
    btVector3 localInertia = btVector3(0,0,0);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(shipMass_, state, ground_, localInertia);
    btRigidBody *body = new btRigidBody(rbInfo);

    //dynamicsWorld_->debugDrawObject(worldTransform_, OGRE_NEW btCylinderShape(btVector3(0.5f, 15.0f, 0.0f)), btVector3(1.0f, 1.0f, 1.0f));
    dynamicsWorld_->addRigidBody(body);
}

void GamePhysics::performCollisionDetection()
{
    int numManifolds = dynamicsWorld_->getDispatcher()->getNumManifolds();

    for(int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold *contactManifold = dynamicsWorld_->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject *a = static_cast<btCollisionObject*>(contactManifold->getBody0());
        btCollisionObject *b = static_cast<btCollisionObject*>(contactManifold->getBody1());

        int numContacts = contactManifold->getNumContacts();

        for(int j = 0; j < numContacts; j++)
        {
            btManifoldPoint &pt = contactManifold->getContactPoint(j);
            if(pt.getDistance() < 0.f)
            {
                const btVector3 &ptA = pt.getPositionWorldOnA();
                const btVector3 &ptB = pt.getPositionWorldOnB();
                const btVector3 &normalOnB = pt.m_normalWorldOnB;
            }
        }

    }
}
