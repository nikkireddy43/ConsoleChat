
# 🔐 Secure Terminal-Based Chat Application (C++ & Python)

A multithreaded client-server chat application built using C++ sockets that supports:

- 🧑‍🤝‍🧑 Personal messaging using `@username`
- 🔐 Encrypted per-user chat logging (XOR-based AES logic)
- 🧪 Log decryption and viewing using Python script
- 📥 Client exit using `/exit`
- 💬 Welcome message and usage instructions

---

## 📁 Project Structure

```

chat\_app/
├── client.cpp         # Client-side logic with personal messaging
├── server.cpp         # Server-side logic with multiple clients and logging
├── aes.hpp            # Custom XOR-based AES-like encryption
├── log_viewer.py      # Python script to decrypt and view logs
├── logs/              # Stores encrypted chat logs per user
└── README.md          # This file

````

---

## 🚀 Features

### ✅ Messaging
- Supports **broadcast** and **personal messages** with `@username`
- Handles invalid usernames gracefully with user-friendly error messages

### ✅ Logging
- Creates a `logs/<username>.json` file with encrypted logs
- Logs every message with timestamp, type (broadcast/private), and content

### ✅ Encryption
- Uses simple XOR-based encryption logic (`aes.hpp`) to secure logs
- Only decrypted via provided Python script

### ✅ Other Utilities
- `/exit` command for client-side graceful shutdown
- Server logs show connect/disconnect events for each client

---

## 🧪 How to Run

### 1. Compile Server and Client (C++11+)
```bash
g++ -std=c++11 server.cpp -o server -lws2_32
g++ -std=c++11 client.cpp -o client -lws2_32
````

### 2. Run Server

```bash
./server
```

### 3. Run Client

```bash
./client
```

> 💡 Client will show a welcome message and instructions on how to send personal messages.

---

## 🔍 Log Decryption

To view any user's encrypted logs:

```bash
python log_viewer.py <username>
```

### 📦 Example Output

```
Decrypted logs for user 'alice':
[2025-06-13 15:30:12] [BROADCAST] Hello everyone!
[2025-06-13 15:31:00] [PRIVATE] To bob: Hi Bob!
```

---

## 📌 Notes

* Make sure `logs/` directory exists before running the server
* Uses `winsock2.h`, tested on Windows (can be ported to Linux with small changes)
* Encryption is basic XOR logic — for demonstration purposes

---

## 🧠 Skills Demonstrated

* Socket Programming (TCP, C++)
* Multithreading (std::thread)
* File I/O and encryption
* Error handling and user input parsing
* C++ and Python interoperability
* Logging and debugging

---

