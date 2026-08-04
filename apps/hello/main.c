#include <stdio.h>

int main(int argc, char **argv) {
    printf("Hello from WaxL!\n");
    
    if (argc > 1) {
        printf("Args passed:\n");
        for (int i = 1; i < argc; i++) {
            printf("  argv[%d]: %s\n", i, argv[i]);
        }
    }

    return 0;
}
