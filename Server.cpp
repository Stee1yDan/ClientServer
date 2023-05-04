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

sem_t* clientSemaphore = sem_open("/my_semaphore1", O_CREAT, 0666, 0);
sem_t* serverSemaphore = sem_open("/my_semaphore2", O_CREAT, 0666, 1);

int shmid = shmget(SHM_KEY, BUF_SIZE, IPC_CREAT | 0666);
    
int* shmaddr = (int*) shmat(shmid, NULL, 0);

double calculateArea(int points[ROWS][COLS])
{
    double area = 0;

    for(int i = 0; i < ROWS; i++) 
    {
        int j = (i + 1) % ROWS;
        area += points[i][0] * points[j][1] - points[j][0] * points[i][1];

        std::cout << points[i][0] << " " << points[j][0] << "\n";
        std::cout << points[i][1] << " " << points[j][1] << "\n" << "\n";
    }


    return abs(area)/2;
}

void readFromMemory()
{
    sem_wait(clientSemaphore); // Wait for client
    sem_wait(serverSemaphore); // Block access to memory for client

    // std::cout << "Hello from server " << &semaphore << "\n";

    int resultMatrix[ROWS][COLS];

    for (int row = 0; row < ROWS; row++) 
    {
        for (int col = 0; col < COLS; col++) 
        {
            resultMatrix[row][col] = *(shmaddr + row * COLS + col);
        }
    }

    double area = calculateArea(resultMatrix);

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
