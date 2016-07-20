#include <iostream>
#include <cstring>
#include "Leap.h"

using namespace Leap;

class SampleListener : public Listener {
  public:

    virtual void onFrame(const Controller&);


  private:
      Driver dvr;
};



void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();

  HandList hands = frame.hands();
  Leap::Hand firstHand = frame.hands()[0];

    }
  }
