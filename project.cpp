/*
 * CONTACT MANAGEMENT SYSTEM
 *
 * How it all works together:
 * 1. User adds contact -> Goes into BST
 * 2. Searching -> BST makes it fast
 * 3. Deleting -> Contact goes to Stack first, then removed from BST
 * 4. Undo -> Pops from Stack, back into BST
 * 5. Batch search -> Queue processes multiple searches in order
 */

#include <iostream>
#include <string>
#include <chrono>

using namespace std;

// Contact class to store contact details
class Contacts {
public:
    string Name;
    string phone_number;

    Contacts() {}
    Contacts(string name, string phoneNumber) : Name(name), phone_number(phoneNumber) {}
};

// Node class for Linked List implementation
class Node {
public:
    Contacts data;
    Node* left;
    Node* right;
    Node* next;

    Node(Contacts contact) : data(contact), left(nullptr), right(nullptr), next(nullptr) {}
};

/*
 * Stack class for recently deleted contacts
 * LIFO (Last In First Out) implementation
*/
class Stack {
private:
    Node* top;

public:
    Stack() : top(nullptr) {}

    void push(Contacts contact) {
        Node* newNode = new Node(contact);
        newNode->next = top;
        top = newNode;
    }

    Contacts pop() {
        if (!top) {
            cout << "Stack is empty! No contacts to undo.\n";
            return Contacts();
        }
        Node* temp = top;
        top = top->next;
        Contacts contact = temp->data;
        delete temp;
        return contact;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    void display() {
        if (!top) {
            cout << "No recently deleted contacts.\n";
            return;
        }
        Node* temp = top;
        while (temp) {
            cout << "Name: " << temp->data.Name << ", Phone: " << temp->data.phone_number << endl;
            temp = temp->next;
        }
    }
};

// Queue class for batch search
// FIFO (First In First Out) implementation
class Queue {
private:
    Node* front;
    Node* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Contacts contact) {
        Node* newNode = new Node(contact);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    Contacts dequeue() {
        if (!front) {
            cout << "Queue is empty! No contacts to search.\n";
            return Contacts();
        }
        Node* temp = front;
        front = front->next;
        if (!front) {
            rear = nullptr;
        }
        Contacts contact = temp->data;
        delete temp;
        return contact;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    void display() {
        if (!front) {
            cout << "Queue is empty!\n";
            return;
        }
        Node* temp = front;
        while (temp) {
            cout << "Name: " << temp->data.Name << ", Phone: " << temp->data.phone_number << endl;
            temp = temp->next;
        }
    }
};

// Binary Search Tree class for managing contacts
class BinarySt {
private:
    Node* root;

    // insert a contact into the BST
    Node* insert(Node* node, Contacts contact) {
        if (!node) {
            return new Node(contact);
        }
        if (contact.phone_number < node->data.phone_number) {
            node->left = insert(node->left, contact);
        } else if (contact.phone_number > node->data.phone_number) {
            node->right = insert(node->right, contact);
        } else {
            cout << "Duplicate contact ignored: " << contact.phone_number << endl;
        }
        return node;
    }

    //in-order traversal function
    void inorder(Node* node) {
        if (node) {
            inorder(node->left);
            cout << "Name: " << node->data.Name << ", Phone: " << node->data.phone_number << endl;
            inorder(node->right);
        }
    }

    //search a contact in the BST
    Node* search(Node* node, string phoneNumber) {
        if (!node || node->data.phone_number == phoneNumber) {
            return node;
        }
        if (phoneNumber < node->data.phone_number) {
            return search(node->left, phoneNumber);
        }
        return search(node->right, phoneNumber);
    }

    //FIND THE in-order successor
    Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    //function to delete a node.
    Node* deleteNode(Node* node, string phoneNumber, Stack& undoStack) {
        if (!node) {
            return node;
        }
        if (phoneNumber < node->data.phone_number) {
            node->left = deleteNode(node->left, phoneNumber, undoStack);
        } else if (phoneNumber > node->data.phone_number) {
            node->right = deleteNode(node->right, phoneNumber, undoStack);
        } else {

            // Store the contact before deleting
            Contacts contactToDelete = node->data;

            // Node with only one child or no child
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                undoStack.push(contactToDelete);
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                undoStack.push(contactToDelete);
                return temp;
            }

            //Node with two children: in-order successor
            Node* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.phone_number, undoStack);

            // also push the original contact that was deleted
            if (contactToDelete.phone_number == phoneNumber) {
                undoStack.push(contactToDelete);
            }
            return node;
        }
        return node;
    }

public:
    BinarySt() : root(nullptr) {}

    void insert(string name, string phoneNumber) {
        Contacts contact(name, phoneNumber);
        root = insert(root, contact);
    }

    void display() {
        if (!root) {
            cout << "No contacts found.\n";
            return;
        }
        inorder(root);
    }

    void search(string phoneNumber) {
        Node* result = search(root, phoneNumber);
        if (result) {
            cout << "Contact found: Name: " << result->data.Name
            << ", Phone: " << result->data.phone_number << endl;
        } else {
            cout << "Contact not found.\n";
        }
    }

    void deleteContact(string phoneNumber, Stack& undoStack) {
        root = deleteNode(root, phoneNumber, undoStack);
        cout << "Contact deleted successfully (if it existed).\n";
    }

    void undoDelete(Stack& undoStack) {
        if (undoStack.isEmpty()) {
            cout << "No recently deleted contacts to undo.\n";
            return;
        }
        Contacts lastDeleted = undoStack.pop(); //pop the deleted contact
        insert(lastDeleted.Name, lastDeleted.phone_number); //insert that into the BST
        cout << "Undo successful! Contact restored: "
        << lastDeleted.Name << ", " << lastDeleted.phone_number << endl;
    }

    //Batch Search
    void batchSearch(Queue& searchQueue) {
        while (!searchQueue.isEmpty()) {
            Contacts contact = searchQueue.dequeue();
            search(contact.phone_number);
        }
    }
};

int main() {
    BinarySt contactBook;
    Stack undoStack;
    Queue searchQueue;
    int choice;
    string name, phoneNumber;

    cout << "*********** Contact Management System ***********\n";

    do {
        cout << "\nMenu:\n"
        << "1. Add Contact\n"
        << "2. Display Contacts\n"
        << "3. Search Contact by Phone Number\n"
        << "4. Delete Contact\n"
        << "5. Undo Last Deletion\n"
        << "6. Batch Search\n"
        << "7. Search Contact by Name\n"
        << "8. Exit\n"
        << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Phone Number: ";
                getline(cin, phoneNumber);
                contactBook.insert(name, phoneNumber);
                cout << "Contact added successfully.\n";
                break;
            }
            case 2: {
                cout << "All Contacts:\n";
                contactBook.display();
                break;
            }
            case 3: {
                cout << "Enter Phone Number to Search: ";
                cin.ignore();
                getline(cin, phoneNumber);
                contactBook.search(phoneNumber);
                break;
            }
            case 4: {
                cout << "Enter Phone Number to Delete: ";
                cin.ignore();
                getline(cin, phoneNumber);
                contactBook.deleteContact(phoneNumber, undoStack);
                break;
            }
            case 5: {
                contactBook.undoDelete(undoStack);
                break;
            }
            case 6: {
                cout << "Enter phone numbers to search (type 'done' to finish):\n";
                while (true) {
                    cout << "Phone Number: ";
                    cin >> phoneNumber;
                    if (phoneNumber == "done") {
                        break;
                    }
                    searchQueue.enqueue(Contacts("", phoneNumber));
                }
                contactBook.batchSearch(searchQueue);
                break;
            }
            case 7: {
                cout << "Feature not implemented.\n";
                break;
            }
            case 8: {
                cout << "Exiting Contact Management System. Goodbye!\n";
                break;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    } while (choice != 8);

    return 0;
}
