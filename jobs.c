#include <stdio.h>
#include "main.h"
void jobs() {
    for (int i = 0; i < no_of_backgroundprocess; i++) {
        if (background_pids[i] != 0) {
            printf("[%d] Running %s [%d]\n", i + 1, background_process[i],
                   stopped_pids[i]);
        } else {
            printf("[%d] Stopped %s [%d]\n", i + 1, background_process[i],
                   stopped_pids[i]);
        }
    }
}