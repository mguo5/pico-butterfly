#include <iostream>
#include "butterfly.h"

const int limit = 1;

using namespace std;

int main() {

    ButterflySimulation simulation;
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
