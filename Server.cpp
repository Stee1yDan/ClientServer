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
#define COLS 2        // number of columns in matrix

int ROWS;

sem_t* clientSemaphore = sem_open("/my_semaphore1", O_CREAT, 0666, 0);
sem_t* serverSemaphore = sem_open("/my_semaphore2", O_CREAT, 0666, 1);

int shmid = shmget(SHM_KEY, BUF_SIZE, IPC_CREAT | 0666);
    
int* shmaddr = (int*) shmat(shmid, NULL, 0);

void readFromMemory()
{
    sem_wait(clientSemaphore); // Wait for client
    sem_wait(serverSemaphore); // Block access to memory for client

    ROWS = *(shmaddr + BUF_SIZE) + 1; // ??????????

    int resultMatrix[ROWS][COLS];

    for (int row = 0; row < ROWS; row++) //Read from memory 
    {
        for (int col = 0; col < COLS; col++) 
        {
            resultMatrix[row][col] = *(shmaddr + row * COLS + col);
            std::cout << resultMatrix[row][col] << " ";
        }

        std::cout << "\n";
    }

    double area = 0;

    for(int i = 0; i < ROWS; i++) 
    {
        int j = (i + 1) % ROWS;
        area += resultMatrix[i][0] * resultMatrix[j][1] - resultMatrix[j][0] * resultMatrix[i][1];
    }

    area = abs(area)/2;
    std::cout << "The area is: " << area << "\n";

    sem_post(serverSemaphore); // Unlock access to memory memory for client
}

int main(int argc, char const *argv[])
{
    for (size_t i = 0; i < 10; i++)
    {   
        readFromMemory();
    }   
    
    return 0;
}
