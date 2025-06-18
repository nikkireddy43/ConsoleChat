# log_viewer.py
# A simple log viewer to decrypt and display user chat logs using XOR decryption

import os  # For file and path handling

# Function to decrypt a string using XOR cipher with the given key
def xor_decrypt(text, key):
    # Apply XOR on each character using the key cyclically
    return ''.join(chr(ord(c) ^ ord(key[i % len(key)])) for i, c in enumerate(text))

# Function to locate and decrypt log entries for a given username
def view_logs(username, key="1234"):
    path = os.path.join("logs", f"{username}.json")  # Construct the file path
    if not os.path.exists(path):  # Check if the log file exists
        print(f"No logs found for {username}")
        return

    # Open and read the file line-by-line, decrypt, and print
    with open(path, "r") as f:
        for line in f:
            decrypted = xor_decrypt(line.strip(), key)  # Decrypt each line
            print(decrypted)  # Print the decrypted log entry

# Entry point for the script
if __name__ == "__main__":
    user = input("Enter username to view logs: ")  # Prompt for username
    view_logs(user)  # Display the logs
