#include "udp_file_exchange_wrapper.h"

// Define a structure to hold file packet data
typedef struct {
    int totalPackets;   // Total number of packets
    int currentPacket;  // Current packet number
    char fileData[PACKETSIZE]; // Data of the packet
} FilePacket;

// Function to send a file over a socket
void send_file_w(int sockfd, const char *path, struct sockaddr *addr, socklen_t addrlen) {
    int nbytes;
    FilePacket packet;
    FILE *file = fopen(path, "rb");
       if (file == NULL) {
        perror("packet open error");
        exit(1);
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Calculate the total number of packets needed
    int totalPackets = fileSize / PACKETSIZE;
    if (fileSize % PACKETSIZE != 0) {
        totalPackets++;
    }

    int currentPacket = 1;
    // Read the file and send it in chunks
    while ((nbytes = fread(packet.fileData, 1, PACKETSIZE, file)) > 0) {
        packet.totalPackets = totalPackets;
        packet.currentPacket = currentPacket;
        if (sendto(sockfd, &packet, sizeof(FilePacket), 0, addr, addrlen) < 0) {
            perror("file send error");
            exit(1);
        }
        usleep(100);  // Add a small delay between sends
        currentPacket++;
    }

    fclose(file);
}

// Function to receive a file over a socket
void recv_file_w(int sockfd, const char *path, struct sockaddr *addr, socklen_t addrlen) {
    FilePacket packet;
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("packet open error");
        exit(1);
    }

    FilePacket *packetArray = NULL;  // Array to store packets
    int expectedPackets = -1;  // Total number of expected packets

    // Set the timeout for receiving data
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    while (1) {
        // Timeout logic
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);

        int maxfd = sockfd + 1;
        int ready = select(maxfd, &fds, NULL, NULL, &timeout);

        if (ready < 0) {
            perror("select error");
            fclose(file);
            free(packetArray);
            exit(1);
        } else if (ready == 0) {
            break;  // Exit the loop on timeout
        }

        // Receive a packet
        ssize_t nrecv = recvfrom(sockfd, &packet, sizeof(FilePacket), 0, addr, &addrlen);
        if (nrecv < 0) {
            perror("file receive error");
            fclose(file);
            free(packetArray);
            exit(1);
        }

        // Initialize expectedPackets and packetArray on receiving the first packet
        if (expectedPackets == -1) {
            expectedPackets = packet.totalPackets;
            packetArray = calloc(expectedPackets, sizeof(FilePacket));
            if (packetArray == NULL) {
                perror("memory allocation error");
                fclose(file);
                exit(1);
            }
        }

        // Store the packet in the array if it hasn't been received before
        if (packet.currentPacket > 0 && packet.currentPacket <= expectedPackets) {
            int index = packet.currentPacket - 1;
            if (packetArray[index].currentPacket == 0) {
                packetArray[index] = packet;
            }
        }

        // Check if all packets have been received
        bool allReceived = true;
        for (int i = 0; i < expectedPackets; i++) {
            if (packetArray[i].currentPacket == 0) {
                allReceived = false;
                break;
            }
        }
        if (allReceived) {
            break;  // Exit the loop if all packets are received
        }
    }

    // Write the packets to the file in the correct order
    for (int i = 0; i < expectedPackets; i++) {
        fwrite(packetArray[i].fileData, 1, PACKETSIZE, file);
    }

    free(packetArray);
    fclose(file);
}
