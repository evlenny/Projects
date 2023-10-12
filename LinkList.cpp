//========================================================
// Evan Lenahan
// April 2022
// listdriver.cpp
// This file tests the List class.
//========================================================

template <class T>
List<T>::List (void)
{
    head = NULL; 
}

template <class T>
List<T>::List(const List<T> &mylist)
{
    head = NULL; 
    Node * currentNode = mylist.head;
    while (currentNode != NULL)
    {
        append(currentNode->item); // Append each item in mylist to the new list
        currentNode = currentNode->next; // Move to the next node in mylist
    }
}

template <class T>
List<T>::~List ( void )
{
    clear();
}

template <class T>
List<T> List<T>::operator=(const List<T> &mylist)
{
    clear(); // Call the clear() function to remove all items in the list
    Node *currentNode = mylist.head;
    while (currentNode != NULL)
    {
        append(currentNode->item); // Append each item in mylist to the new list
        currentNode = currentNode->next; // Move to the next node in mylist
    }
    return *this; // Return a reference to the modified list
}

template <class T>
string List<T>::to_string(void) const
{
    string str;
    if (head == NULL)
    {
        str = "0";
        return str;
    }else
    {
        Node *currentNode = head;
        while (currentNode != NULL){
            str = str + std::to_string(currentNode->item) + " "; // Convert the item to a string and append it to the result string
            currentNode = currentNode->next; // Move to the next node in the list
        }
        return str;
    }
}

template <class T>
void List<T>::append(const T &item)
{
    Node *newNode = new Node; 
    newNode->item = item; // Set the item value of the new node
    newNode->next = NULL; 

    if (head == NULL)
    {
        head = newNode; // If the list is empty, set the head to the new node
    }else{
        Node *currentNode = head;
        while (currentNode->next != NULL)
        {
            currentNode = currentNode->next; // Move to the last node in the list
        }
        currentNode->next = newNode; // Set the next pointer of the last node to the new node
    }

}

template <class T>
T &List<T>::operator[](int index)
{
    Node *currentNode = head;
    for (int i = 0; i < index; i++)
    {
        currentNode = currentNode->next; // Move to the node at the specified index
    }
    return currentNode->item; // Return a reference to the item value of the node at the specified index
}

template <class T>
void List<T>::insert(const T &item, int index)
{
    Node *currentNode = head; // Initialize currentNode as head
    if (index == 0)
    {
        Node *newNode = new Node; // Create a new node
        newNode->item = item; // Set the item of the new node to the input item
        newNode->next = head; // Set the next pointer of the new node to the previous head
        head = newNode; // Update the head pointer to point to the new node
    }else
    {
        for (int i = 0; i < index - 1; i++)
        {
            currentNode = currentNode->next; // Traverse to the node before the insertion point
        }
        Node *newNode = new Node; // Create a new node
        newNode->item = item; // Set the item of the new node to the input item
        newNode->next = currentNode->next; // Set the next pointer of the new node to the node that was originally after currentNode
        currentNode->next = newNode; // Update the next pointer of currentNode to point to the new node
    }
}

template <class T>
void List<T>::remove(int index)
{
    if (head == NULL) // Check if the list is empty
    {
        return;
    }
    
    Node *currentNode = head; // Initialize currentNode as head

    if (index == 0)
    {
        Node *tempNode = head; // Create a temporary node that points to the head node
        head = head->next; // Update the head pointer to point to the next node
        delete tempNode; 
    }else
    {
        Node *currentNode = head; // Initialize currentNode as head
        for (int i = 0; i < index - 1; i++)
        {
            currentNode = currentNode->next; // Traverse to the node before the node to be removed
        }
        Node *tempNode = currentNode->next; // Create a temporary node that points to the node to be removed
        currentNode->next = tempNode->next; 
        delete tempNode; 
    }
}

template <class T>
List<T> List<T>::operator+(const List<T> &mylist) const
{
    List<T> newList = *this; 
    Node *currentNode = mylist.head; // Initialize a currentNode to the head of the input list
    while (currentNode != NULL) // Go through the input list and add each node to the new list
    {
        newList.append(currentNode->item);
        currentNode = currentNode->next;
    }
    return newList; 
}

template <class T>
int List<T>::length(void) const
{
    int length = 0;
    Node *currentNode = head; // Initialize currentNode as head
    while (currentNode != NULL) // Go through the list and increment length for each node
    {
        currentNode = currentNode->next;
        length++;
    }
    return length;
}

template <class T>
bool List<T>::isEmpty(void) const
{
    return (head == NULL); // Check if the head pointer is NULL and return true if it is, false otherwise
}

template <class T>
void List<T>::clear(void)
{
    Node *tempNode; 
    Node *currentNode = head; 
    
    // loop through each node in the list until there are no more nodes
    while (currentNode != NULL)
    {
        tempNode = currentNode; // set the tempNode pointer to the current node to be deleted
        currentNode = currentNode->next; // set the currentNode pointer to the next node in the list
        delete tempNode; // delete the node pointed to by tempNode
    }
    
    head = NULL;
}