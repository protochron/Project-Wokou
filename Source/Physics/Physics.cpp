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

    worldTransform_ = btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0));

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
