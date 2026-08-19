#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <common.h>

static const char *binder_nodes[] = {
    "/dev/binderfs/stdbinder",
    "/dev/binderfs/corebinder",
    "/dev/binderfs/appbinder"
};

int main(int argc, char *argv[]) {
    size_t num_nodes = sizeof(binder_nodes) / sizeof(binder_nodes[0]);

    NL_INFO("Starting %zu orchestrator instances...", num_nodes);

    for (size_t i = 0; i < num_nodes; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            NL_ERROR("fork failed");
            exit(1);
        }

        if (pid == 0) {
            execl("/bin/worch", "/bin/worch", binder_nodes[i], NULL);

            NL_ERROR("execl failed");
            exit(1);
        } else {
            // NL_INFO("Spawned orchestrator for %s (PID: %d)", binder_nodes[i], pid);
        }
    }

    int status;
    while (wait(&status) > 0);

    return 0;
}
