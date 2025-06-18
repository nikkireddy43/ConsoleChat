#include <iostream>         // For console I/O
#include <thread>           // For handling receiving in a background thread
#include <winsock2.h>       // Windows socket API
#include <ws2tcpip.h>       // For inet_pton and extended TCP/IP support

#pragma comment(lib, "Ws2_32.lib") // Link against Winsock library
#define PORT 8080 // Port number to connect to

// Function to continuously receive messages from the server
void receive(SOCKET sock) {
    char buffer[1024];
    while (true) {
        int bytes = recv(sock, buffer, sizeof(buffer), 0); // Receive data
        if (bytes <= 0) break; // Connection closed or error
        buffer[bytes] = '\0';  // Null-terminate the message
        std::cout << "\n" << buffer << "\n> "; // Display message
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); // Initialize Winsock

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP socket

    sockaddr_in serv;
    serv.sin_family = AF_INET;                // Use IPv4
    serv.sin_port = htons(PORT);              // Convert port to network byte order
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr); // Convert IP to binary form (localhost)

    connect(sock, (sockaddr*)&serv, sizeof(serv)); // Connect to server

    // Prompt user for username and send it to the server
    std::string name;
    std::cout << "Enter your username: ";
    std::getline(std::cin, name);
    send(sock, name.c_str(), name.size(), 0);

    // Welcome message and usage instructions
    std::cout << "\n Welcome, " << name << "! Type messages to broadcast.\n";
    std::cout << " To send private: @username message\n";
    std::cout << " To exit: /exit\n\n";

    // Start a background thread to receive incoming messages
    std::thread(receive, sock).detach();

    // Main loop to send messages
    std::string msg;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, msg); // Read user input
        send(sock, msg.c_str(), msg.size(), 0); // Send to server
        if (msg == "/exit") break; // Exit command
    }

    // Cleanup on exit
    closesocket(sock); // Close client socket
    WSACleanup();      // Cleanup Winsock
    return 0;
}
