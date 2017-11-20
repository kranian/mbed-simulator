#include "mbed.h"
#include "EthernetInterface.h"

// Network interface
EthernetInterface net;

// Socket demo
int main() {
    // Bring up the ethernet interface
    printf("Ethernet socket example\n");
    net.connect();

    // Show the network address
    const char *ip = net.get_ip_address();
    const char *mac = net.get_mac_address();
    const char *gateway = net.get_gateway();
    printf("IP address: %s\n", ip ? ip : "None");
    printf("MAC address: %s\n", mac ? mac : "None");
    printf("Gateway: %s\n", gateway ? gateway : "None");

    TCPSocket socket;
    socket.open(&net);
    socket.connect("api.ipify.org", 80);

    char *buffer = new char[256];

    // Send an HTTP request
    strcpy(buffer, "GET / HTTP/1.1\r\nHost: api.ipify.org\r\n\r\n");
    int scount = socket.send(buffer, strlen(buffer));
    printf("sent %d [%.*s]\n", scount, strstr(buffer, "\r\n")-buffer, buffer);

    // Recieve an HTTP response and print out the response line
    int rcount = socket.recv(buffer, 256);
    printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

    // The api.ipify.org service also gives us the device's external IP address
    const char *payload = strstr(buffer, "\r\n\r\n")+4;
    printf("External IP address: %.*s\n", rcount-(payload-buffer), payload);

    // Close the socket to return its memory and bring down the network interface
    socket.close();
    delete[] buffer;

    // Bring down the ethernet interface
    net.disconnect();
    printf("Done\n");
}