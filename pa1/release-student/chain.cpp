#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{
    clear();
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
    height_ = 0;
    width_ = 0;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{

    Node *newNode = new Node(ndata);
    if (length_ == 0)
    {
        head_ = newNode;
        tail_ = newNode;
        head_->prev = tail_;
        tail_->next = head_;
        length_ += 1;

    }
    else
    {
        tail_->next = newNode;
        newNode->prev = tail_;
        newNode->next = head_;
        head_->prev = newNode;
        tail_ = newNode;
        length_ += 1;

    }
    newNode = NULL;
}

void Chain::swap(int pos1, int pos2) {
    // Make sure pos1 is less than pos2
    // if (pos1 > pos2) {
    //     int temp = pos1;
    //     pos1 = pos2;
    //     pos2 = temp;
    // }
    // Find node1 and node2
    Node* node1 = walk(head_, pos1 - 1);
    Node* node2 = walk(head_, pos2 - 1);

    // Save next and prev nodes
    Node* node1Prev = node1->prev;
    Node* node1Next = node1->next;
    Node* node2Prev = node2->prev;
    Node* node2Next = node2->next;

    // Only 2 case
    if (length_ == 2)
    {
        node1->prev = node2;
        node1->next = node2;
        node2->prev = node1;
        node2->next = node1;
        tail_ = node1;
        head_ = node2;
    }
    // Adjacent case
    else if (node1Next == node2)
    {
        // Left side pointers
        node1Prev->next = node2;
        node1->next = node2->next;
        node1->prev = node2;

        // Right side pointers
        node2Next->prev = node1;
        node2->next = node1;
        node2->prev = node1Prev;

        if (node1 == head_) {
            head_ = node2;
        }
        if (node2 == tail_) {
            tail_ = node1;
        }
    }
    // // node1 is head and node2 is tail 
    // else if (node1 == head_ && node2 == tail_)
    // {
    //     node1->next = node2;
    //     node1->prev = node2Prev;
    //     node1Next->prev = node2;

    //     node2Prev->next = node1;
    //     node2->next = node1Next;
    //     node2->prev = node1;
    //     head_ = node2;
    //     tail_ = node1;
    // }
    // node1 is head
    else if (node1 == head_)
    {
        node1->next = node2Next;
        node1->prev = node2Prev;
        node1Next->prev = node2;
        node1Prev->next = node2;

        node2->next = node1Next;
        node2->prev = node1Prev;
        node2Prev->next = node1;
        node2Next->prev = node1;

        head_ = node2;
    }
    // node2 is tail;
    else if (node2 == tail_)
    {
        node1->next = node2Next;
        node1->prev = node2Prev;
        node1Next->prev = node2;
        node1Prev->next = node2;

        node2->next = node1Next;
        node2->prev = node1Prev;
        node2Prev->next = node1;
        node2Next->prev = node1;

        tail_ = node1;
    }
    else
    {
        // Left side pointers
        node1Prev->next = node2;
        node1->next = node2Next;
        node1->prev = node2Prev;
        node1Next->prev = node2;

        // Right side pointers
        node2Prev->next = node1;
        node2->prev = node1Prev;
        node2->next = node1Next;
        node2Next->prev = node1;
    }
}

/**
 * Mirrors the given chain across its center on the x-axis.
 */
void Chain::mirror()
{
    int left;
    int right;
    if (length_ == 0 || length_ == 1)
    {
        return;
    }
    else if (length_ % 2 == 0)
    {
        left = length_ / 2;
        right = left + 1;
        while (left > 0)
        {
            swap(left, right);
            left--;
            right++;
        }
    }
    else
    {
        left = length_ / 2;
        right = left + 2;
        while (left > 0)
        {
            swap(left, right);
            left--;
            right++;
        }
    }

}

/*
* Modifies the current chain by "rotating" every k nodes.
* Starting with the 0th node, swap with with the kth next node.
* Starting at the position you just swapped into, continue to
* swap nodes until you swap a node with the final node of the chain.
* Some examples with the chain a b c d e:
*   k = 0: a b c d e
*   k = 1: b c d e a
*   k = 2: c b e d a
*   k = 3: d e c b a
* You may assume that the rotation will not loop forever
* (i.e. you **will** swap the final node in the list at some point)
*/
void Chain::rotate(int k)
{
    if (k == 0)
    {
        return;
    }

    
    int pivot = 1;

    while (pivot != length_)
    {

        if ((pivot + k) > length_) 
        {
            int newPos = (pivot+k) % length_;
            swap(newPos, pivot);
            pivot = newPos;
        } 
        else
        {
            swap(pivot, pivot + k);
            pivot += k;
        }
    }

}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */

void Chain::clear()
{
    head_->prev = NULL;
    tail_->next = NULL;

    Node *curr = head_;
    Node *next = curr->next;
    if (length_ == 1) {
        delete curr;
    }
    while(next != NULL) {
        next = curr->next;
        delete curr;
        curr = next;
    }

}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */

void Chain::copy(Chain const &other)
{

    height_ = other.height_;
    width_ = other.width_;
    length_ = other.length_;
    Node *otherCurr = other.head_;
    head_ = new Node(otherCurr->data);
    Node *copyCurr = head_;
    otherCurr = otherCurr->next;

    while (otherCurr != other.head_)
    {
        Node *newNode = new Node(otherCurr->data);
        copyCurr->next = newNode;
        newNode->prev = copyCurr;
        copyCurr = copyCurr->next;
        otherCurr = otherCurr->next;
    }
    tail_ = copyCurr;
    tail_->next = head_;
    head_->prev = tail_;
    copyCurr = NULL;
    otherCurr = NULL;
}
