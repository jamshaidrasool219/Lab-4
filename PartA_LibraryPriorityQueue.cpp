#include <iostream>
#include <string>
#include <limits>
using namespace std;

// ==========================================
// Book Request Node
// ==========================================
struct BookRequestNode
{
    int studentID;
    string studentName;
    int bookID;
    string bookTitle;
    int priority;
    BookRequestNode* next;
};

// ==========================================
// Priority Queue Class (logic unchanged)
// ==========================================
class PriorityQueue
{
private:
    BookRequestNode* front;
    BookRequestNode* rear;
    int count;

public:
    PriorityQueue()
    {
        front = nullptr;
        rear = nullptr;
        count = 0;
    }

    // Add request according to priority
    void Add(BookRequestNode* request)
    {
        request->next = nullptr;

        // Case 1: Queue is empty
        if (front == nullptr)
        {
            front = request;
            rear = request;
        }
        // Case 2: New request has higher priority than front
        else if (request->priority < front->priority)
        {
            request->next = front;
            front = request;
        }
        // Case 3: Insert somewhere after front
        else
        {
            BookRequestNode* current = front;
            while (current->next != nullptr &&
                   current->next->priority <= request->priority)
            {
                current = current->next;
            }
            request->next = current->next;
            current->next = request;

            if (request->next == nullptr)
            {
                rear = request;
            }
        }
        count++;
    }

    // Remove highest-priority request
    BookRequestNode* Remove()
    {
        if (front == nullptr)
        {
            return nullptr;
        }
        BookRequestNode* request = front;
        front = front->next;
        if (front == nullptr)
        {
            rear = nullptr;
        }
        request->next = nullptr;
        count--;
        return request;
    }

    bool IsEmpty()
    {
        return front == nullptr;
    }

    bool IsNotEmpty()
    {
        return front != nullptr;
    }

    int GetCount()
    {
        return count;
    }

    void PrintQueue()
    {
        BookRequestNode* current = front;
        while (current != nullptr)
        {
            cout << "Student ID: " << current->studentID << endl;
            cout << "Student Name: " << current->studentName << endl;
            cout << "Book ID: " << current->bookID << endl;
            cout << "Book Title: " << current->bookTitle << endl;
            cout << "Priority: " << current->priority << endl;
            cout << "-----------------------------" << endl;
            current = current->next;
        }
    }

    // Copy Constructor
    PriorityQueue(const PriorityQueue& other)
    {
        front = nullptr;
        rear = nullptr;
        count = 0;

        BookRequestNode* current = other.front;
        while (current != nullptr)
        {
            BookRequestNode* newNode = new BookRequestNode;
            newNode->studentID = current->studentID;
            newNode->studentName = current->studentName;
            newNode->bookID = current->bookID;
            newNode->bookTitle = current->bookTitle;
            newNode->priority = current->priority;
            newNode->next = nullptr;
            Add(newNode);
            current = current->next;
        }
    }

    // Assignment Operator
    PriorityQueue& operator=(const PriorityQueue& other)
    {
        if (this == &other)
        {
            return *this;
        }

        while (front != nullptr)
        {
            BookRequestNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
        count = 0;

        BookRequestNode* current = other.front;
        while (current != nullptr)
        {
            BookRequestNode* newNode = new BookRequestNode;
            newNode->studentID = current->studentID;
            newNode->studentName = current->studentName;
            newNode->bookID = current->bookID;
            newNode->bookTitle = current->bookTitle;
            newNode->priority = current->priority;
            newNode->next = nullptr;
            Add(newNode);
            current = current->next;
        }
        return *this;
    }

    // Destructor: releases all remaining nodes
    ~PriorityQueue()
    {
        while (front != nullptr)
        {
            BookRequestNode* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }
};

// ==========================================
// Helper functions
// ==========================================
BookRequestNode* CreateRequest(int studentID, string studentName,
                               int bookID, string bookTitle, int priority)
{
    BookRequestNode* request = new BookRequestNode;
    request->studentID = studentID;
    request->studentName = studentName;
    request->bookID = bookID;
    request->bookTitle = bookTitle;
    request->priority = priority;
    request->next = nullptr;
    return request;
}

// Reads an int safely (re-asks on invalid input)
int ReadInt(const string& message)
{
    int value;
    while (true)
    {
        cout << message;
        if (cin >> value)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number.\n";
    }
}

string ReadLine(const string& message)
{
    string text;
    cout << message;
    getline(cin, text);
    return text;
}

void ShowMenu()
{
    cout << "\n==============================\n";
    cout << "  Library Book Request System\n";
    cout << "==============================\n";
    cout << "1. Add Book Request\n";
    cout << "2. Process Highest Priority Request\n";
    cout << "3. Display All Requests\n";
    cout << "4. Check Queue Status\n";
    cout << "5. Exit\n";
}

// ==========================================
// Main
// ==========================================
int main()
{
    PriorityQueue requests;
    int choice;

    do
    {
        ShowMenu();
        choice = ReadInt("Enter your choice (1-5): ");

        switch (choice)
        {
        case 1:
        {
            int studentID = ReadInt("Enter Student ID: ");
            string studentName = ReadLine("Enter Student Name: ");
            int bookID = ReadInt("Enter Book ID: ");
            string bookTitle = ReadLine("Enter Book Title: ");

            int priority;
            do
            {
                priority = ReadInt("Enter Priority (1 = Exam/Urgent, 2 = Assignment/Project, 3 = Normal Reading): ");
                if (priority < 1 || priority > 3)
                {
                    cout << "Priority must be 1, 2 or 3.\n";
                }
            } while (priority < 1 || priority > 3);

            BookRequestNode* request =
                CreateRequest(studentID, studentName, bookID, bookTitle, priority);
            requests.Add(request);
            cout << "\nRequest added successfully.\n";
            break;
        }

        case 2:
        {
            BookRequestNode* processed = requests.Remove();
            if (processed == nullptr)
            {
                cout << "\nNo requests to process. Queue is empty.\n";
            }
            else
            {
                cout << "\n===== Processing Request =====\n";
                cout << "Student ID: " << processed->studentID << endl;
                cout << "Student Name: " << processed->studentName << endl;
                cout << "Book ID: " << processed->bookID << endl;
                cout << "Book Title: " << processed->bookTitle << endl;
                cout << "Priority: " << processed->priority << endl;
                delete processed;
            }
            break;
        }

        case 3:
            if (requests.IsEmpty())
            {
                cout << "\nNo pending requests.\n";
            }
            else
            {
                cout << "\n===== Pending Requests (Highest to Lowest Priority) =====\n\n";
                requests.PrintQueue();
            }
            break;

        case 4:
            if (requests.IsEmpty())
            {
                cout << "\nQueue Status: EMPTY (no pending requests).\n";
            }
            else if (requests.IsNotEmpty())
            {
                cout << "\nQueue Status: NOT EMPTY. Pending requests: "
                     << requests.GetCount() << endl;
            }
            break;

        case 5:
            cout << "\nExiting... releasing memory. Goodbye!\n";
            break;

        default:
            cout << "\nInvalid choice. Please select 1-5.\n";
        }
    } while (choice != 5);

    return 0; // PriorityQueue destructor frees all remaining nodes
}
