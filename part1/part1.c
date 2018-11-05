#include <unistd.h>

int main() {
    int i;

    for (i = 0; i < 10; i++) {
        access("/home", F_OK);
    }

    return 0;
}