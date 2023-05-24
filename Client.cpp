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

sem_t* clientSemaphore = sem_open("/my_semaphore1", 0);
sem_t* memorySemaphore = sem_open("/my_semaphore2", 0);

int shmid = shmget(SHM_KEY, BUF_SIZE, IPC_CREAT | 0666);
    
int* shmaddr = (int*) shmat(shmid, NULL, 0);

void writeToMemory()
{
    sem_wait(memorySemaphore); // Wait for memory access

    // int matrix[ROWS][COLS] = {{1, 6}, {3, 1}, {7, 2}, {4, 4}, {8, 5}};
     
    for (int row = 0; row < ROWS; row++) // write matrix data to shared memory segment
    {
        for (int col = 0; col < COLS; col++) 
        {
            int buf = 0;
            while (true)
            {
                std::cout << "Enter element " << row << " " << col << ":";
                std::cin >> buf;
                if (buf > -128 && buf < 127) break;
                std::cout << "ERROR: Invalid value" << "\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
            }

            *(shmaddr + row * COLS + col) = buf;
        }
    }

    sem_post(clientSemaphore); // Set client to 1 and allow server to read info
    sem_post(memorySemaphore); // Unlock access to memory memory for client
}

int main(int argc, char const *argv[])
{
    while (true)
	{
		std::cout << "Enter number of points (3-9) : ";
		std::cin >> ROWS;
		if (ROWS > 2 && ROWS < 10) break;
		std::cout << "ERROR: Invalid value" << "\n";
		std::cin.clear();
		std::cin.ignore(10000, '\n');
	}

    //TODO: Initialize end
    *(shmaddr + BUF_SIZE) = ROWS;
    
    writeToMemory();

    return 0;
}
