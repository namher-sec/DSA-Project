# DSA-Project
Data Structures and Algorithms Project 
# Contact Management System (Using Binary Search Tree, Stack and Queue)

## Overview
This is a **Contact Management System** that uses a **Binary Search Tree (BST)** to store and manage contact information such as names and phone numbers, a **Queue** to batch search and a **Stack** to undo deletion. The system allows users to add, search, delete, and undo deletions of contacts. It also provides a batch search feature for processing multiple search queries at once.

### Key Features:
- **Add Contact**: Insert new contacts into the system with a name and phone number.
- **Display Contacts**: Display all contacts stored in the system.
- **Search Contact**: Search for a contact by phone number.
- **Delete Contact**: Delete a contact by phone number, with the ability to undo the deletion.
- **Undo Last Deletion**: Restore the last deleted contact.
- **Batch Search**: Perform multiple phone number searches at once using a queue.
- **Search by Name**: Placeholder feature (not fully implemented).

## Technology Stack
- **C++**: The system is written in C++ using object-oriented programming principles.
- **Binary Search Tree (BST)**: Efficient search, insert, and delete operations.
- **Stack**: Used to store recently deleted contacts for undo functionality.
- **Queue**: Used to handle multiple search queries for batch search.

## How to Run

### Prerequisites
- C++ compiler (e.g., GCC, MinGW, Clang)
- C++11 or later

### Running the Code

1. **Clone the repository:**
   ```bash
   git clone https://github.com/namher-sec/DSA-Project.git
   cd DSA-project
   ```
2. **Build the Program:**  
Ensure you have a C++ compiler (e.g., `g++`) installed. Then compile the program:  
```bash  
g++ -o project project.cpp  
```

3. **Run the program:**
```bash
./project
```
