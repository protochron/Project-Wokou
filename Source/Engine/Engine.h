#ifndef __PIRATE_ENGINE_H__
#define __PIRATE_ENGINE_H__

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>

class Engine {
 public:
  static Engine* instance();
  
 protected:
  Engine();
  
 private:
  static boost::shared_ptr<Engine> instance_;
  
};

#endif
