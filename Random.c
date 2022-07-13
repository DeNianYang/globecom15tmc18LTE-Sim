#include <time.h>
#include "Random.h"

void randomize() {
    time_t t;
    srand((unsigned) time(&t));
}