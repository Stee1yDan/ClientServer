# ClientServer
This project contains 2 programs that use shared memory to communicate with each other. Access control implemented through 2 POSIX semaphores.
The purpose of the server program is to calculate the area of an irregular polygon using shoelace formula. The input to the server is provided through the client program. The client gets its input from the user.

Initial state:

clientSemaphore = 0; - block server from reading the critical section

serverSemaphore = 1; - allow client access to the critical section

Client writes info to the critical section:

clientSemaphore = 0; - blocks server from reading the critical section

serverSemaphore = 0; - block access to the critical section

After client finishes writing to the critical section:

clientSemaphore = 1; - allows the server to read from the critical section

serverSemaphore = 1; - allow access to the critical section

Server starts reading from the critical section:

clientSemaphore = 1; - allows the server to read from the critical section

serverSemaphore = 0; - block access to the critical section

After server finishes reading from the critical section:

clientSemaphore = 0; - block server from reading the critical section

serverSemaphore = 1; - allow client access to the critical section




