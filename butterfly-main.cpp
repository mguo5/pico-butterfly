#include <iostream>
#include "butterfly.h"

const int limit = 10;

using namespace std;

int main() {

    Breadboard simulation;
    clock_t currentClock = clock();

    int i = 0;
    simulation._initialize();

    while(i < limit) {

        currentClock += 0.1 * CLOCKS_PER_SEC; // Make the simulation advance 100 ms.
        simulation._update(currentClock);
        i++;
    }

    return 0;
}
