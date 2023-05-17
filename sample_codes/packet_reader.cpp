#include <stdio.h>
#include <stdlib.h>
#include "frameio.h"

void printFrame(const unsigned char* frame, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%02x ", frame[i]);
        if ((i + 1) % 22 == 0) {
            printf("\n");
            if ((i + 1) % 42 == 0)
                printf("\n");
        }
    }
    printf("\n\n");
}

int main() {
    frameio net;

    if (net.open_net("enp0s3") < 0) {
        fprintf(stderr, "Failed to open network interface.\n");
        return 1;
    }

    while (1) {
        unsigned char frame[1500]; // Adjust the maximum frame size as needed
        int bytesRead = net.recv_frame(frame, sizeof(frame));

        if (bytesRead >= 14) {
            unsigned short etherType = (frame[12] << 8) | frame[13];

            if (etherType == 0x800) { // IPv4 EtherType
                printf("Received an IP packet:\n");
                printFrame(frame, 42);
            } else if (etherType == 0x806) { // ARP EtherType
                printf("Received an ARP packet:\n");
                printFrame(frame, 42);
            }
        }
    }

    return 0;
}
