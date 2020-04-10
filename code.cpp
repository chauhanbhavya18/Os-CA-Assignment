#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool checkIODevices = false;
double num_of_hard_disks = 0;
double num_of_printers = 0;
void commandRequestHardDisk(int num);  
void commandRequestPrinter(int num);  
void commandShowIO();
int current_process_in_CPU_priority = -1;
int current_process_in_CPU_pid = -1; 
void addProcessToReadyQueue(int priority, int gpid);
void commandShowReadyQueueAndCPU(); 
void removeProcessfromCPU();



struct Node 
{ 
  int data; 
  struct Node *next; 
}; 


void push_back(struct Node** head_ref, int new_data) 
{ 
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node)); 
    struct Node *last = *head_ref; 
    new_node->data  = new_data; 
    new_node->next = NULL; 
    if (*head_ref == NULL) 
    { 
       *head_ref = new_node; 
       return; 
    } 
    while (last->next != NULL) 
        last = last->next; 
    last->next = new_node; 
    return; 
}   



struct Node* pop_front(struct Node* head) 
{ 
    if (head == NULL) 
        return NULL; 
    struct Node* temp = head; 
    head = head->next; 
  
    delete temp; 
  
    return head; 
} 



int front(struct Node* head)  
{  
    struct Node* current = head;    
        return(current->data);    
}  




int end(struct Node* head)  
{   
    struct Node* current = head;  
    while (current != NULL)  
    {    
        current = current->next;  
    }  
    return current->data;  
}


struct Node* empty(struct Node* head)  
{   
    if (head == NULL) 
        return NULL;   
} 



int size(struct Node* head)  
{  
    int count = 0;  
    struct Node* current = head;   
    while (current != NULL)  
    {  
        count++;  
        current = current->next;  
    }  
    return count;  
}
  



int next(int a,int b){
	return(a+b);
}



struct PCB {
        int pid;
        int priority;
        int begin;
        int end;
        struct PCB* next;
        struct PCB* prev;
};
    

struct PCB* head = NULL;
struct PCB* tail = NULL;
    
int gpid = 0;
    
double memorysize = 0;
    
void commandAdd(int priority, double size);
void commandTerminate(); 
void commandFinishWorkHardDisk(int num); 
void commandFinishWorkPrint(int num); 
void commandShowMemory();
    
void removeProcessFromMem();

bool valid(double input);
void userCommands(char userInput);

int main() {
    
    char userInput;
    bool userWantToContinue = true;
    printf("enter the number of bytes of memory you would like to use. It has to be the number between 0 to 5 thousands \n");
    scanf("%lf",&memorysize);
    
    printf("enter the number of Hard Disks you want to use. It should be positive and less than 1000 \n");
    scanf("%lf", &num_of_hard_disks);
    
    printf("Please enter the number of Printers you want to use. It should be positive and less than 50\n");
    scanf("%lf",&num_of_printers);
    
    printf("You system have::\n");
    printf("RAM memory: %lf bytes\n",memorysize);
    printf("Printers: %lf bytes\n",num_of_printers);
    printf("Hard Disks: %lf bytes\n",num_of_hard_disks);
    while(userWantToContinue){
        printf("enter command ('c' for details of commands, 'e' to exit): ");
        scanf("%s",&userInput);
        if(userInput == 'e'){
            userWantToContinue = false;
        }
        else{
            userCommands(userInput);
        }
    }
    return 0;
}

void userCommands(char userInput){
    
    if(userInput== 'A'){
        char a;
        int priority = -1;
        double size = -1;
        
        scanf("%c %d %lf",&a,&priority,&size);
        
        if(priority == -1 || size == -1){ 
            printf("Please enter both priority and size positive\n");
            return;
        }
        
        else if(priority < 0 || 4 < priority){
            printf("Please enter the priority from 0 to 4.\n");
            return;
        }
        else if(size < -1 || size > memorysize){
            printf("Please enter the size of the memory positive and less than the size of the memory you created.\n");
            return;
        }
        else{
            commandAdd(priority, size);
        }
        
    }
    else if(userInput == 't'){ 
        commandTerminate();
        
    }
    else if(userInput== 'd'){
        char d;
        int num = -1;
        
        scanf("%c %d",&d,&num);
        
        if(num == -1){
            printf("Mention positive and valid Hard Disk Number.\n");
            return;
        }
        else if(num < 1 || num > num_of_hard_disks){
            printf("Please enter hard idsk number greater than 1 and smaller than the number of hard disks you created.\n");
            return;
        }
        
        commandRequestHardDisk(num);
        
    }
    else if(userInput == 'p'){
        char p;
        int num = -1;
        
        scanf("%c %d",&p,&num);
        
        if(num == -1){
            printf("Mention valid Printer Number.\n");
            return;
        }
        else if(num < 1 || num > num_of_printers){
            printf("Please enter printer number greater than 1 and smaller than the number of printers you created.\n");
            return;
        }
        
        commandRequestPrinter(num); 
        
    }
    else if(userInput == 'D'){
        char e;
        int num = -1;
        
    	scanf("%c %d",&e,&num);
        if(num == -1){
            printf("*Invalid command; Please mention Hard Disk Number.\n");
            return;
        }
        
        commandFinishWorkHardDisk(num);     
        
    }
    else if(userInput== 'P'){
        char q;
        int num = -1;
        scanf("%c %d",&q,&num);
        
        if(num == -1){
            printf("*Invalid command; Please mention Printer Number.\n");
            return;
        }
        
        commandFinishWorkPrint(num);
        
    }
    else if(userInput== 'S'){
        char s;
        char t;
        scanf("%c %c",&s,&t);
        
        if(t == 'r'){
            commandShowReadyQueueAndCPU();   
        }
        else if(t == 'i'){
            commandShowIO(); 
        }
        else if (t == 'm'){
            commandShowMemory();   
        }
        else{
            printf("*Invalid command.\n");
            return;
        }
    }
    else if(userInput == 'c'){
    	char c[1000];
    FILE *fptr;
    if ((fptr = fopen("c.txt", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    fscanf(fptr, "%[^\n]", c);
    printf("\n%s", c);
    fclose(fptr);
        
    }
    else{
        printf( "*Invalid command.\n");
    }
}




//I/O functions


void commandRequestHardDisk(int num){
    struct Node* hardDisks[num-1];
    if(current_process_in_CPU_pid != -1){
        push_back(&hardDisks[num-1],current_process_in_CPU_pid);
        removeProcessfromCPU();
    }
    else{ 
        printf("*Error; There is no process in CPU.\n");
        return;
    }
    
    
}

void commandRequestPrinter(int num){
	struct Node* printers[num-1];
    if(current_process_in_CPU_pid != -1){
        push_back(&printers[num-1],current_process_in_CPU_pid);
        
        removeProcessfromCPU();
    }
    else{
        printf("*Error; There is no process in CPU.");
        return;
    }
    
}

void commandShowIO(){
    for(int i = 0; i < num_of_hard_disks; i++){
    	struct Node* hardDisks[i];
        if(!empty(hardDisks[i])){
            checkIODevices = true;
            printf("Hard Disk  %d : P  %d",i+1, front(hardDisks[i]));
            if(size(hardDisks[i]) > 1){
                printf( " ( ");
                for(int it= next(front(hardDisks[i]), 1); it !=end(hardDisks[i]); ++it){
                    printf("P %d", it);
                }
                printf(")");
            }
            printf("\n\n");
        }
    }
    for(int j = 0; j < num_of_printers; j++){
    	struct Node* printers[j];
        if(!empty(printers[j])){
            checkIODevices = true;
            printf("Printer  %d : P  %d",j+1,front(printers[j]));
            if(size(printers[j]) > 1){
                printf( " ( ");
                for(int it= next(front(printers[j]), 1); it !=end(printers[j]); ++it){
                    printf("P %d", it);
                }
                printf( ")");
            }
            printf("\n\n");
        }
    }
    
    if(!checkIODevices){
        printf("There is no process in I/O devices.\n");
    }
    else
        printf("*The processes in ( ) are in its Ready Queue.\n");
    
}



//Memory function





struct PCB* findHole(double size){
    struct PCB* current;
    current = head;
    
    while(current != tail){
        
        if(current -> next -> begin - current -> end > size)
            return current;
        else
            current = current -> next;
    }
    return NULL;
}


struct PCB* findPidInMem(int wantPid){
    struct PCB* check;
    check = head;
    
    while(check != tail){
        if(check->pid == wantPid)
            return check;
        else
            check = check -> next;
    }
    return NULL;
}


void commandAdd(int priority, double size){
    struct PCB* firstNode = NULL;
    struct PCB* newNode;
    struct PCB* nextNode;
    struct PCB* limitNode;
    
    if(head == NULL){
        firstNode = (struct PCB*)malloc(sizeof(struct PCB));
        firstNode -> end = -1;
        head = firstNode;
    }
    
    newNode = (struct PCB*)malloc( sizeof(struct PCB));
    newNode -> pid = ++gpid;
    newNode -> priority = priority;
    
    addProcessToReadyQueue(priority, gpid);
    
    if(gpid == 1){
        newNode -> begin = 0;
        newNode -> end = size -1;
        newNode -> prev = firstNode;
        firstNode -> next = newNode;
        
        limitNode = (struct PCB*)malloc( sizeof(struct PCB));
        limitNode -> begin = memorysize;
        newNode -> next = limitNode;
        limitNode -> prev = newNode;
        tail = limitNode;
    }
    else{
        struct PCB* node = findHole(size);
        if(node == NULL){ 
            printf("*Error; There is no enough space in Memory!\n");
        }
        else{
            nextNode = node -> next;
            newNode -> next = nextNode;
            newNode -> prev = node;
            node -> next = newNode;
            nextNode -> prev = newNode;
            newNode -> begin = newNode -> prev -> end +1;
            newNode -> end = newNode -> begin + size -1;
        }
        
    }
}


void commandTerminate(){
    
    removeProcessFromMem();
    removeProcessfromCPU();
    
}

void removeProcessFromMem(){
    struct PCB* checkPid;
    struct PCB* sucNode;
    struct PCB* preNode;
    
    checkPid = head;
    
    while(checkPid -> pid != current_process_in_CPU_pid){
        checkPid = checkPid -> next;
    }
    sucNode = checkPid -> next;
    preNode = checkPid -> prev;
    
    sucNode -> prev = preNode;
    if(preNode != 0)
        preNode -> next = sucNode;
    if(checkPid == head)
        head = sucNode;
    
}

void commandFinishWorkHardDisk(int num){
    struct PCB* temp;
    struct Node* hardDisks[num-1];
    if(!empty(hardDisks[num-1])){
        temp = findPidInMem(front(hardDisks[num-1]));
        if(temp == NULL){
            printf("*Error; didn't find the pid in Memory\n");
        }
        else{
            addProcessToReadyQueue(temp -> pid, front(hardDisks[num-1]));
            pop_front(hardDisks[num-1]);
        }
    }
    
    if(empty(hardDisks[num-1]))
        checkIODevices = false;
}

void commandFinishWorkPrint(int num){
    struct PCB* temp;
    struct Node* printers[num-1];
    if(!empty(printers[num-1])){
        temp = findPidInMem(front(printers[num-1]));
        if(temp == NULL){
            printf("Error; didn't find the pid in memory\n");
        }
        else{
            addProcessToReadyQueue(temp -> pid, front(printers[num-1]));
            pop_front(printers[num-1]);
        }
    }
    
    if(empty(printers[num-1]))
        checkIODevices = false;
}


void commandShowMemory(){
    struct PCB* p;
    p = head -> next;
    while(p != tail){
        printf("P %d : %d - %d",  p -> pid,p -> begin,p -> end);
        p = p -> next;
    }
}




// CPU fuction



struct Node* readyQueuePrior4=NULL;
struct Node* readyQueuePrior3=NULL;
struct Node* readyQueuePrior2=NULL;
struct Node* readyQueuePrior1=NULL;
struct Node* readyQueuePrior0=NULL;

void addProcessToReadyQueue(int priority, int gpid){
    if(priority > current_process_in_CPU_priority){
        if(current_process_in_CPU_priority == -1){  
            current_process_in_CPU_priority = priority;
            current_process_in_CPU_pid = gpid;
        }
        else{  
            if(current_process_in_CPU_priority == 3)
                push_back(&readyQueuePrior3,current_process_in_CPU_pid);
            else if(current_process_in_CPU_priority == 2)
                push_back(&readyQueuePrior2,current_process_in_CPU_pid);
            else if(current_process_in_CPU_priority == 1)
                push_back(&readyQueuePrior1,current_process_in_CPU_pid);
            else if(current_process_in_CPU_priority == 0)
                push_back(&readyQueuePrior0,current_process_in_CPU_pid);
            else
                printf("*Error; Cannot push back the process that was running in CPU!\n");
            
            current_process_in_CPU_priority = priority;
            current_process_in_CPU_pid = gpid;
        }
    }
    else{
        if(priority == 4)
            push_back(&readyQueuePrior4,gpid);
        else if(priority == 3)
            push_back(&readyQueuePrior3,gpid);
        else if(priority == 2)
            push_back(&readyQueuePrior2,gpid);
        else if(priority == 1)
            push_back(&readyQueuePrior1,gpid);
        else if(priority == 0)
            push_back(&readyQueuePrior0,gpid);
        else{
            printf("*Error; Cannot add process to Ready Queue.\n");
            return;
        }
    }
    
}

void removeProcessfromCPU(){
    current_process_in_CPU_priority = -1;
    current_process_in_CPU_pid = -1;
    if(!empty(readyQueuePrior4)){
        current_process_in_CPU_priority = 4;
        current_process_in_CPU_pid = front(readyQueuePrior4);
        pop_front(readyQueuePrior4);
    }
    else if(!empty(readyQueuePrior3)){
        current_process_in_CPU_priority = 3;
        current_process_in_CPU_pid = front(readyQueuePrior3);
        pop_front(readyQueuePrior3);
    }
    else if(!empty(readyQueuePrior2)){
        current_process_in_CPU_priority = 2;
        current_process_in_CPU_pid = front(readyQueuePrior2);
        pop_front(readyQueuePrior2);
    }
    else if(!empty(readyQueuePrior1)){
        current_process_in_CPU_priority = 1;
        current_process_in_CPU_pid = front(readyQueuePrior1);
        pop_front(readyQueuePrior1);
    }
    else if(!empty(readyQueuePrior0)){
        current_process_in_CPU_priority = 0;
        current_process_in_CPU_pid = front(readyQueuePrior0);
        pop_front(readyQueuePrior0);
    }
    
}

void commandShowReadyQueueAndCPU(){
    if(current_process_in_CPU_pid == -1)
        printf("No process in CPU\n");
    else
        printf("In CPU: P   %d\n",current_process_in_CPU_pid);
    
    if(!empty(readyQueuePrior4)){
        printf("Ready Queue Priority 4: ");
        for (int it = front(readyQueuePrior4); it !=end(readyQueuePrior4); ++it)
            printf("P  %d\n",it);
    }
    if(!empty(readyQueuePrior3)){
        printf("Ready Queue Priority 3: ");
        for (int it = front(readyQueuePrior3); it !=end(readyQueuePrior3); ++it)
            printf("P  %d\n",it);
    }
    if(!empty(readyQueuePrior2)){
        printf("Ready Queue Priority 2: ");
        for (int it = front(readyQueuePrior2); it !=end(readyQueuePrior2); ++it)
            printf("P  %d\n",it);
    }
    if(!empty(readyQueuePrior1)){
        printf("Ready Queue Priority 1: ");
        for (int it = front(readyQueuePrior1); it !=end(readyQueuePrior1); ++it)
            printf("P  %d\n",it);
    }
    if(!empty(readyQueuePrior0)){
        printf("Ready Queue Priority 0: ");
        for (int it = front(readyQueuePrior0); it !=end(readyQueuePrior0); ++it)
            printf("P  %d\n",it);
    }
    
}
