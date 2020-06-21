#include <Stepper.h>

#define STEPS 2038 // the number of steps in one revolution of your motor (28BYJ-48)

Stepper stepper(STEPS, 8, 10, 9, 11);

void setup() {
    // nothing to do
    stepper.setSpeed(10); // 10 rpm
}

void loop() {
    stepper.step(-200); // keep stepping. Bump # up if adding more into loop
}
