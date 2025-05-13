#include "raspi5.h"
#include <unistd.h>  // Para sleep()

int main() {
    ControlPin led(6, true);  // GPIO 5 as Output (true = output, false = input)
    ControlPin led2(5, true);  // GPIO 5 as Output (true = output, false = input)

    for (int i = 0; i < 3; ++i) {
        led.setValue(1);
        sleep(1);
        led.setValue(0);
        sleep(1);
    }

    for (int i = 0; i < 3; ++i) {
        led2.toggle();
        sleep(1);
    }

    return 0;
}