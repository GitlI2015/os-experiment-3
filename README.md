#  os-experiment-3

In this experiment, I write three IPC demos. They are **Semaphores**
**Sockets**, and **FIFOs** respectively.  I will describe what do these demos
do and how they work.

Main directory has the following structure, and each demo is in its
corresponding directory (together with Makefile)

    .
    ├── FIFO
    ├── semaphore
    └── Socket

## Semaphores 

A semaphore is a variable or abstract data type used to control access to a
common resource by multiple processes in a concurrent system. I implement a
simple multi-writer single reader system, in which writer will fulfill a buffer
with random numbers, and reader need to sum up all numbers in the buffer every
10 milliseconds. writer threads and reader threads are created by pthread, and
they will *join* after reader cycles 10 times.

usage:
```sh
$ make & ./counter
```
