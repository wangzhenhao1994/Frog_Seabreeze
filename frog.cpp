#include "spectrometer.cpp"
#include "piezo_PI.cpp"
using namespace std;

int frog(){
  double step_length=10.0;
  PI_Stage stage(&step_length);
  stage.piezo_initializer();
  stage.move_onestep();
  stage.exit();
  Spectrometer spec;
  spec.spec_initializer();
  spec.readSpec();
  spec.spec_destructor();
  return 0;
}
