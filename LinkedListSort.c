/*
 * Name: Balram Panwar
 * Email id: balram.panwar@gmail.com
 * Name of the file: LinkedListSort.c
 * Platform: Linux
 * Sort the Singly LinkedList using merge sort,
 */

#include <iostream>

struct Node {
    int data;
    Node* next;
    
    Node(int val) : data(val), next(nullptr) {}
};

// Function to split the linked list into two halves
void splitList(Node* head, Node** firstHalf, Node** secondHalf) {
    if (!head || !head->next) {
        *firstHalf = head;
        *secondHalf = nullptr;
        return;
    }

    Node* slow = head;
    Node* fast = head;
    Node* prev = nullptr;

    // Move fast by 2 and slow by 1
    while (fast && fast->next) {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    // Split the list into two halves
    prev->next = nullptr;
    *firstHalf = head;
    *secondHalf = slow;
}

// Function to merge two sorted linked lists
Node* mergeSortedLists(Node* first, Node* second) {
    if (!first) return second;
    if (!second) return first;

    if (first->data < second->data) {
        first->next = mergeSortedLists(first->next, second);
        return first;
    } else {
        second->next = mergeSortedLists(first, second->next);
        return second;
    }
}

// Function to perform merge sort on the linked list
Node* mergeSort(Node* head) {
    if (!head || !head->next) return head;

    Node* firstHalf = nullptr;
    Node* secondHalf = nullptr;

    splitList(head, &firstHalf, &secondHalf);

    firstHalf = mergeSort(firstHalf);
    secondHalf = mergeSort(secondHalf);

    return mergeSortedLists(firstHalf, secondHalf);
}

// Helper function to print the linked list
void printList(Node* head) {
    Node* temp = head;
    while (temp) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << std::endl;
}

// Main sort function
void sort(Node** head, int count) {
    *head = mergeSort(*head);
}

int main() {
    // Creating a linked list: 4 -> 2 -> 1 -> 3
    Node* head = new Node(4);
    head->next = new Node(2);
    head->next->next = new Node(1);
    head->next->next->next = new Node(3);

    std::cout << "Original List: ";
    printList(head);

    // Sorting the list
    sort(&head, 4);

    std::cout << "Sorted List: ";
    printList(head);

    return 0;
}
