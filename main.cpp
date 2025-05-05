#include <raspi5.h>
#include <unistd.h>

int main() {
    ControlPin pin13(13, true);
    pin13.setValue(1);
    sleep(1);
    pin13.setValue(0);
    sleep(1);

    pin13.toggle();
    sleep(1);
    pin13.toggle();
    sleep(1);

    return 0;
}