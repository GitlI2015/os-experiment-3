#  os-experiment-3

In this experiment, I write three IPC demos. They are **Semaphores**
**Sockets**, and **SharedMemory** respectively.  I will describe what do these demos
do and how they work.

Main directory has the following structure, and each demo is in its
corresponding directory (together with Makefile)

    .
    ├── SharedMemory
    ├── semaphore
    └── Socket


## SharedMemory

Shared memory can be created to share data between processes. In this demo, I
write a matrix multiplication program that can multiply two matrix in parallel.
Each process will be assigned with certain lines of multiplication tasks.

usage:
```sh
$ make & cat testcase | ./matrix_mul
```

## Semaphores 

A semaphore is a variable or abstract data type used to control access to a
common resource by multiple processes in a concurrent system. I implement a
simple multi-writer single reader system, in which writer will fulfill a buffer
with random numbers, and reader need to sum up all numbers in the buffer every
second. Since writer processes are forked from main process, shared memory is
required to share either buffer or semaphores.

usage:
```sh
$ make & ./counter
```
