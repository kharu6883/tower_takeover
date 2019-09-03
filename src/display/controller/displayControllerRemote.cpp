#include "main.h"

#include "config/io.h"
#include "control/displayController.h"
using namespace Display;

static int count = 0;

RemoteDisplay::RemoteDisplay() {

}

void RemoteDisplay::update(void* ignore) {
  while(true) {
    if(!(count % 25)) {
      master.set_text(0, 0, "Hello, World!");
    }

    count++;
    wait(2);
  }
}
