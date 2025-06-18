#include <iostream>         // For standard I/O
#include <thread>           // For multithreading (handling multiple clients)
#include <vector>           // Not used here, but typically for dynamic arrays
#include <map>              // To store client usernames and sockets
#include <sstream>          // For stringstream used in logging
#include <fstream>          // For file output
#include <winsock2.h>       // For Windows socket programming
#include <ws2tcpip.h>       // For extended TCP/IP definitions
#include "aes.hpp"          // Custom AES encryption class
#include <algorithm>        // For general STL algorithms (not used here)
#include <mutex>            // For thread safety (mutex locking)
#include <direct.h>         // For creating directories (Windows specific)

#pragma comment(lib, "Ws2_32.lib") // Link against Winsock library

#define PORT 8080 // Server port

// Global structures
std::map<std::string, SOCKET> clients; // Map of connected clients (username -> socket)
std::mutex client_mutex;               // Mutex for synchronizing access to 'clients'
AES aes("1234");                       // AES encryption object with a static key

// Returns current timestamp as a formatted string
std::string timestamp() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return buf;
}

// Logs encrypted data to a file corresponding to the username
void log_data(const std::string& username, const std::string& type, const std::string& content) {
    _mkdir("logs"); // Create 'logs' directory if it doesn't exist (Windows)

    std::string path = "logs/" + username + ".json";
    std::ofstream file(path, std::ios::app); // Append mode

    std::stringstream ss;
    ss << "{\"time\":\"" << timestamp()
       << "\",\"type\":\"" << type
       << "\",\"data\":\"" << content << "\"}\n";

    std::string encrypted = aes.encrypt(ss.str()); // Encrypt log entry
    file << encrypted << "\n";                     // Store encrypted log line
}

// Sends a message to all clients except the sender
void broadcast(const std::string& from, const std::string& msg) {
    std::lock_guard<std::mutex> lock(client_mutex); // Ensure thread safety
    for (auto& pair : clients) {
        if (pair.first != from)
            send(pair.second, (from + ": " + msg).c_str(), 1024, 0);
    }
}

// Handles communication for a single connected client
void handle_client(SOCKET client_socket) {
    char name[64];
    recv(client_socket, name, sizeof(name), 0); // Receive username
    std::string username(name);

    // Register the client
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        clients[username] = client_socket;
        std::cout << username << " connected.\n";
    }
    log_data(username, "connect", "joined the chat");

    char buffer[1024]; // Message buffer

    while (true) {
        int bytes = recv(client_socket, buffer, 1024, 0);
        if (bytes <= 0) break; // Client disconnected or error

        buffer[bytes] = '\0'; // Null-terminate message
        std::string msg(buffer);

        if (msg == "/exit") break; // Client wants to disconnect

        // Personal message handling (starts with '@')
        if (msg.rfind("@", 0) == 0) {
            size_t space = msg.find(' ');
            if (space != std::string::npos) {
                std::string to = msg.substr(1, space - 1);
                std::string content = msg.substr(space + 1);
                std::lock_guard<std::mutex> lock(client_mutex);

                if (clients.count(to)) {
                    send(clients[to], ("(PM from " + username + "): " + content).c_str(), 1024, 0);
                    send(client_socket, "(PM sent)", 1024, 0);
                } else {
                    send(client_socket, "Error: user not found.", 1024, 0);
                }
            } else {
                send(client_socket, "Error: Invalid PM format.", 1024, 0);
            }
        } else {
            // Public broadcast
            broadcast(username, msg);
        }

        log_data(username, "message", msg); // Log message
    }

    // Cleanup after client disconnects
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        clients.erase(username);
        std::cout << username << " disconnected.\n";
    }
    closesocket(client_socket); // Close the socket
    log_data(username, "disconnect", "left the chat");
}

int main() {
    _mkdir("logs"); // Ensure logs directory exists

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa); // Initialize Winsock

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0); // Create server socket

    sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(PORT);

    bind(server_socket, (sockaddr*)&serv, sizeof(serv)); // Bind socket to port
    listen(server_socket, 5);                            // Start listening
    std::cout << "Server listening on port " << PORT << "...\n";

    // Accept client connections in an infinite loop
    while (true) {
        SOCKET client = accept(server_socket, nullptr, nullptr); // Accept client
        std::thread(handle_client, client).detach();             // Handle client in new thread
    }

    closesocket(server_socket); // Cleanup (unreachable in current code)
    WSACleanup();               // Cleanup Winsock
    return 0;
}
