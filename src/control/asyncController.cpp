#include "main.h"

ControlAsync::ControlAsync() {

}

void ControlAsync::run(void* args) {

  reset();

  while(true) {

  }
}

void ControlAsync::reset() {
  rack(0);
  arm(0);
}

void ControlAsync::rackAsync(Vector2 target) {

}

void ControlAsync::armAsync(Vector2 target) {
  
}
