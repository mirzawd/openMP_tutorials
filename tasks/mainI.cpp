#include <iostream>
using namespace std;
#ifdef _OPENMP
  #include <omp.h>
#endif


 
// A linked list node
struct Node
{
   int data;
   struct Node *next;
};
//insert a new node in front of the list
void push(struct Node** head, int node_data)
{
   /* 1. create and allocate node */
   struct Node* newNode = new Node;
 
   /* 2. assign data to node */
   newNode->data = node_data;
 
   /* 3. set next of new node as head */
   newNode->next = (*head);
 
   /* 4. move the head to point to the new node */
   (*head) = newNode;
}
 
//insert new node after a given node
void insertAfter(struct Node* prev_node, int node_data)
{
  /*1. check if the given prev_node is NULL */
  if (prev_node == NULL)
  {
   cout<<"the given previous node is required,cannot be NULL"; return; } 
 
   /* 2. create and allocate new node */
   struct Node* newNode =new Node; 
 
   /* 3. assign data to the node */
   newNode->data = node_data;
 
   /* 4. Make next of new node as next of prev_node */
   newNode->next = prev_node->next;
 
    /* 5. move the next of prev_node as new_node */
    prev_node->next = newNode;
}
 
/* insert new node at the end of the linked list */
void append(struct Node** head, int node_data)
{
/* 1. create and allocate node */
struct Node* newNode = new Node;
 
struct Node *last = *head; /* used in step 5*/
 
/* 2. assign data to the node */
newNode->data = node_data;
 
/* 3. set next pointer of new node to null as its the last node*/
newNode->next = NULL;
 
/* 4. if list is empty, new node becomes first node */
if (*head == NULL)
{
*head = newNode;
return;
}
 
/* 5. Else traverse till the last node */
while (last->next != NULL)
    last = last->next;
 
/* 6. Change the next of last node */
last->next = newNode;
return;
}
 
// display linked list contents
void displayList(struct Node *node)
{
   //traverse the list to display each node
   while (node != NULL)
   {
      cout<<node->data<<"-->";
      node = node->next;
   }                                          
 
   if(node== NULL)
	cout<<"null"; 
} 

void changeDataList(struct Node** head,  int value)
{

   (*head)->data=value;
   //cout<<omp_get_thread_num()<<endl;
}


/* main program for linked list*/
int main() 
{ 
    /* empty list */
    struct Node* head = NULL; 
    struct Node* root = NULL; 

    //struct Node* headI = NULL; 


   // Insert 10 at the end
   append(&head, 10);

   // Insert the following data at the beginning. 
   push(&head, 20);  
   push(&head, 30); 
   push(&head, 40);
   push(&head, 50);
   push(&head, 60); 
   push(&head, 70); 
   push(&head, 80); 
   push(&head, 90); 
   push(&head, 100); 

  // The following bock of the code assigns different threads in parallel to changes the values to curent value + increment 
   int increment = 0;
   root  = head;
   #pragma omp parallel
   {
      #pragma omp single nowait
      {
   	while (head != NULL)
   	{ 
       	    #pragma omp task firstprivate(head)
            {	
       	        changeDataList(&head,increment+100);
            }
            increment++;
            head = head->next;
        }                                          
      }
   }


//   cout<<"Final linked list: "<<endl;
   displayList(root);
   cout<<endl; 
   return 0;
}
