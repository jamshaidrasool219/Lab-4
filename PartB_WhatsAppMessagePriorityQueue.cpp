/*
 * Part B - Individual Project: WhatsApp Auto-Reply AI Agent
 * Priority Queue for incoming customer messages
 *
 * 1. Real-world problem
 *    My WhatsApp Auto-Reply AI Agent receives many customer messages at the
 *    same time. The agent can only reply to one message at a time (API calls
 *    take time and have rate limits). If messages are handled only in arrival
 *    order, an urgent message (e.g. "payment failed", "order not received")
 *    can wait behind many casual messages ("hi", "what are your timings?").
 *
 * 2. Why a Priority Queue?
 *    A normal Queue is FIFO and ignores urgency. A Priority Queue always
 *    serves the most important message first, and messages with the same
 *    priority are still served in arrival order (fair).
 *
 * 3. Priority levels (smaller number = higher priority)
 *    1 -> Urgent    (payment problem, complaint, order issue)
 *    2 -> Important (order status, booking, product inquiry)
 *    3 -> Normal    (general questions, greetings, feedback)
 *
 * 4. Node design -> MessageNode (below)
 */

#include <iostream>
#include <string>
using namespace std;

// ==========================================
// Node: one incoming WhatsApp message
// ==========================================
struct MessageNode
{
    int messageID;
    string customerName;
    string phoneNumber;
    string messageText;
    int priority;        // 1 = Urgent, 2 = Important, 3 = Normal
    MessageNode* next;
};

// ==========================================
// Priority Queue class
// ==========================================
class MessagePriorityQueue
{
private:
    MessageNode* front;   // highest-priority message
    MessageNode* rear;    // last message in the queue
    int count;

    static string PriorityName(int p)
    {
        if (p == 1) return "Urgent";
        if (p == 2) return "Important";
        return "Normal";
    }

    // Deep copy helper used by copy constructor and assignment operator
    void CopyFrom(const MessagePriorityQueue& other)
    {
        MessageNode* current = other.front;
        while (current != nullptr)
        {
            MessageNode* newNode = new MessageNode;
            newNode->messageID = current->messageID;
            newNode->customerName = current->customerName;
            newNode->phoneNumber = current->phoneNumber;
            newNode->messageText = current->messageText;
            newNode->priority = current->priority;
            newNode->next = nullptr;
            Add(newNode);
            current = current->next;
        }
    }

    void Clear()
    {
        while (front != nullptr)
        {
            MessageNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        count = 0;
    }

public:
    MessagePriorityQueue()
    {
        front = nullptr;
        rear = nullptr;
        count = 0;
    }

    // 6. Add() according to priority
    //    Smaller priority number goes first. Same priority -> after existing
    //    ones (so arrival order is kept).
    void Add(MessageNode* message)
    {
        message->next = nullptr;

        if (front == nullptr)                        // Case 1: empty queue
        {
            front = message;
            rear = message;
        }
        else if (message->priority < front->priority) // Case 2: new front
        {
            message->next = front;
            front = message;
        }
        else                                          // Case 3: middle / end
        {
            MessageNode* current = front;
            while (current->next != nullptr &&
                   current->next->priority <= message->priority)
            {
                current = current->next;
            }
            message->next = current->next;
            current->next = message;

            if (message->next == nullptr)
            {
                rear = message;
            }
        }
        count++;
    }

    // 7. Remove() the highest-priority message
    MessageNode* Remove()
    {
        if (front == nullptr)
        {
            return nullptr;
        }
        MessageNode* message = front;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr;
        }
        message->next = nullptr;
        count--;
        return message;
    }

    // 8. Supporting operations
    bool IsEmpty()    { return front == nullptr; }
    bool IsNotEmpty() { return front != nullptr; }
    int  GetCount()   { return count; }

    // Look at the next message without removing it
    MessageNode* Peek() { return front; }

    void PrintQueue()
    {
        if (front == nullptr)
        {
            cout << "  (no pending messages)\n";
            return;
        }
        MessageNode* current = front;
        while (current != nullptr)
        {
            cout << "  [P" << current->priority << " - "
                 << PriorityName(current->priority) << "] "
                 << "ID " << current->messageID << " | "
                 << current->customerName << " (" << current->phoneNumber << ")"
                 << "\n      \"" << current->messageText << "\"\n";
            current = current->next;
        }
    }

    // Copy constructor
    MessagePriorityQueue(const MessagePriorityQueue& other)
    {
        front = nullptr;
        rear = nullptr;
        count = 0;
        CopyFrom(other);
    }

    // Assignment operator
    MessagePriorityQueue& operator=(const MessagePriorityQueue& other)
    {
        if (this == &other)
        {
            return *this;
        }
        Clear();
        CopyFrom(other);
        return *this;
    }

    // Destructor
    ~MessagePriorityQueue()
    {
        Clear();
    }
};

// ==========================================
// Helper: create a message node dynamically
// ==========================================
MessageNode* CreateMessage(int id, string name, string phone,
                           string text, int priority)
{
    MessageNode* message = new MessageNode;
    message->messageID = id;
    message->customerName = name;
    message->phoneNumber = phone;
    message->messageText = text;
    message->priority = priority;
    message->next = nullptr;
    return message;
}

// ==========================================
// 9. Demonstration with the project's own data
// ==========================================
int main()
{
    MessagePriorityQueue inbox;

    cout << "===== Incoming WhatsApp Messages (arrival order) =====\n";
    cout << "1. Sara   -> Hi, what are your shop timings?        (P3)\n";
    cout << "2. Usman  -> My payment was deducted but no order!   (P1)\n";
    cout << "3. Hina   -> Where is my order #4521?                (P2)\n";
    cout << "4. Zain   -> I received a damaged product, refund!   (P1)\n";
    cout << "5. Areeba -> Do you have this in blue colour?        (P2)\n";
    cout << "6. Talha  -> Thanks, great service!                  (P3)\n";

    inbox.Add(CreateMessage(1, "Sara",   "0300-1111111", "Hi, what are your shop timings?", 3));
    inbox.Add(CreateMessage(2, "Usman",  "0301-2222222", "My payment was deducted but no order!", 1));
    inbox.Add(CreateMessage(3, "Hina",   "0302-3333333", "Where is my order #4521?", 2));
    inbox.Add(CreateMessage(4, "Zain",   "0303-4444444", "I received a damaged product, refund!", 1));
    inbox.Add(CreateMessage(5, "Areeba", "0304-5555555", "Do you have this in blue colour?", 2));
    inbox.Add(CreateMessage(6, "Talha",  "0305-6666666", "Thanks, great service!", 3));

    cout << "\n===== Queue after priority arrangement ("
         << inbox.GetCount() << " messages) =====\n";
    inbox.PrintQueue();

    cout << "\n===== AI Agent replying in priority order =====\n";
    cout << "Next to be served: " << inbox.Peek()->customerName << "\n\n";

    // Serve the first three messages
    for (int i = 0; i < 3 && inbox.IsNotEmpty(); i++)
    {
        MessageNode* m = inbox.Remove();
        cout << "Replying to " << m->customerName << " (P" << m->priority
             << "): \"" << m->messageText << "\"\n";
        delete m;
    }

    // A new urgent message arrives while others are still waiting
    cout << "\n>>> New urgent message arrives: Kamran -> \"Account hacked!\" (P1)\n";
    inbox.Add(CreateMessage(7, "Kamran", "0306-7777777", "My account got hacked, help!", 1));

    cout << "\n===== Remaining queue (" << inbox.GetCount() << " messages) =====\n";
    inbox.PrintQueue();

    // Test copy constructor
    MessagePriorityQueue backup = inbox;
    cout << "\nBackup copy created with " << backup.GetCount() << " messages.\n";

    cout << "\n===== Serving everything left =====\n";
    while (inbox.IsNotEmpty())
    {
        MessageNode* m = inbox.Remove();
        cout << "Replying to " << m->customerName << " (P" << m->priority << ")\n";
        delete m;
    }

    cout << "\nOriginal inbox empty? " << (inbox.IsEmpty() ? "Yes" : "No") << "\n";
    cout << "Backup still has " << backup.GetCount() << " messages (deep copy works).\n";

    return 0;   // destructors free remaining memory
}
