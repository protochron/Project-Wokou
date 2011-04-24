#include "Engine.h"

boost::shared_ptr<Engine> Engine::instance_;

Engine::Engine(){
  
}

Engine* Engine::instance(){
  if (instance_)
    return instance_.get();
  else {
    instance_.reset(new Engine);
    return instance_.get();
  }
}
