#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#include<thread>
#include<iostream>
#include<fcntl.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x4444
#define ROWS 5         // number of rows in matrix
#define COLS 2        // number of columns in matrix

sem_t* clientSemaphore = sem_open("/my_semaphore1", 0);
sem_t* serverSemaphore = sem_open("/my_semaphore2", 0);

int shmid = shmget(SHM_KEY, BUF_SIZE, IPC_CREAT | 0666);
    
int* shmaddr = (int*) shmat(shmid, NULL, 0);

void writeToMemory()
{
    int value;
    sem_getvalue(serverSemaphore, &value);
    std::cout << value << "\n";

    sem_wait(serverSemaphore); // Wait for memory access

    // std::cout << "Hello from client " << &semaphore << "\n";

    int matrix[ROWS][COLS] = {{1, 6}, {3, 1}, {7, 2}, {4, 4}, {8, 5}};
    
    // write matrix data to shared memory segment
    for (int row = 0; row < ROWS; row++) 
    {
        for (int col = 0; col < COLS; col++) 
        {
            *(shmaddr + row * COLS + col) = matrix[row][col];
        }
    }

    sem_post(clientSemaphore); // Set client to 1 and allow server to read info
    sem_post(serverSemaphore); // Unlock access to memory memory for client
}

int main(int argc, char const *argv[])
{
    writeToMemory();

    return 0;
}
