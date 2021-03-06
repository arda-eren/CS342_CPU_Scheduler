#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "scheduler_defs.h"


//NODE DEFINITIONS AND METHODS

//Definition for the structure of a node in a queue
//build like a doubly linked list
typedef struct node{
    PCB process_data;
    struct node* next;
    struct node* prev;
}node;

//Method: create a new node
//Parameter: PCB
//Return: node pointer
node* create_node(PCB process_data){
    node* node = (struct node*) malloc(sizeof(struct node));
    node->next = NULL;
    node->prev = NULL;
    node->process_data = process_data;
    return node;
}

//Method: print a node to console
//Parameters: node pointer
//Return: void
void print_node(node* node){
    printf("%d\n", node->process_data.pid);
}


//QUEUE DEFINITIONS AND METHODS

//Definition to hold dront and the back of the queue
//build like a doubly linked list
typedef struct queue{
    struct node* front;
    struct node* back;
}queue;

//Method: check if queue is empty
//Parameters: queue pointer
//Return: boolean
int isEmpty(struct queue* queue){
    return queue->front == NULL;
}

//Method: create a new queue
//Parameters: -
//Return: queue pointer
queue* create_queue(){
    queue* queue = (struct queue*) malloc(sizeof(struct queue));
    queue->front = NULL;
    queue->back = NULL;
    return queue;
}

//Method: enqueue according to the sjf algorithm
//Parameters: queue pointer, PCB pointer
//Return: void
void sjf_enqueue(struct queue* queue, struct PCB* process_data){
        node* temp = queue->front;
        struct node *added_node = (struct node *)malloc(sizeof(struct node));
        added_node->prev = NULL;
        added_node->next = NULL;
        added_node->process_data = *process_data;

        if(!isEmpty(queue)){
            while (temp && temp->process_data.remaining_CPU_burst_length < added_node->process_data.remaining_CPU_burst_length){
                temp = temp->next;
            }if(!temp){
                queue->back->next = added_node;
                added_node->prev = queue->back;
                queue->back = added_node;
            }else if(temp == queue->front){
                added_node->next = queue->front;
                queue->front->prev = added_node;
                queue->front = added_node;
            }else{
                added_node->next = temp;
                temp->prev->next = added_node;
                added_node->prev = temp->prev;
                queue->back = added_node;
            }
        }else{
            queue->back = added_node;
            queue->front = added_node;
        }   
}

//Method: enqueue according to the fcs and rr algorithms
//Parameters: queue pointer, PCB pointer
//Return: void
void rr_fcfs_enqueue(struct queue* queue, struct PCB* process_data){
    node* temp = queue->back;
    struct node *added_node = (struct node *)malloc(sizeof(struct node));
    added_node->process_data = *process_data;
    added_node->next = NULL;
    added_node->prev = NULL;
    if (isEmpty(queue)){
        queue->front = added_node;
        queue->back  = added_node;
    }else{
        temp->next = added_node;
        added_node->prev = queue->back;
        queue->back = added_node;
    }
}   

//Method: enqueue according to the selected algorithm
//Parameters: queue pointer, PCB pointer, scheduling_algorithm
//Return: void
void enqueue(struct queue* queue, struct PCB* process_data, enum scheduling_algorithm algorithm){
    if(algorithm == SJF){
        sjf_enqueue(queue, process_data);      
    }else{
        rr_fcfs_enqueue(queue, process_data);
    }
}

//Method: dequeue a node
//Parameters: node pointer
//Return: node pointer
node* dequeue(struct queue* queue){
    if(isEmpty(queue)){
        printf("Queue is empty\n");
        return NULL;
    }else{
        struct node *holder = queue->front;
        queue->front = queue->front->next;
        return holder;
    }
}

//Method: freeing the node memory
//Parameters: node pointer
//Return: void
void destroy_node(struct node* node){
    free(node);
}

//Method: freeing the queue memory
//Parameters: queue pointer
//Return: void
void destroy_queue(struct queue* queue){
    while(!isEmpty(queue)){
        destroy_node(dequeue(queue));
    }
    free(queue);
}

//Method: print a queue
//Parameters: queue pointer
//Return: void
void print_queue(queue* queue){
    if (isEmpty(queue)){
        printf("Queue is empty\n");
    } else {
        struct node* holder = queue->front;
        while(holder){
            printf("%d\n", holder->process_data.pid);
            holder = holder->next;
        }
    }
}


//Method: printing the data of the ready queue
//Parameters: queue pointer
//Return: void
void print_queue_data(struct queue *q){
    if (!isEmpty(q)){
        struct node *temp = q->front;
        printf("pid\tarv\tdept\tcpu\twaitr\tturna\tn-bursts\tn-d1\tn-d2\n");
        while (temp != NULL){
            printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n", temp->process_data.pid,
               temp->process_data.arr_time, temp->process_data.finish_time, temp->process_data.total_time_in_cpu,
               (temp->process_data.finish_time - temp->process_data.arr_time - temp->process_data.total_time_in_cpu),
               temp->process_data.finish_time - temp->process_data.arr_time,
               temp->process_data.burst_count, temp->process_data.device1_io_counter, temp->process_data.device2_io_counter);
            temp = temp->next;
        }
        printf("\n");
    }else{
        printf("Queue is empty\n");
        return;
    }
}






