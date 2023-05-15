Sure, I can add that. Here's the updated README.md:

---

# Banker's Algorithm Multithreaded Program

This program implements a multithreaded version of the Banker's Algorithm which is used to avoid deadlock in a computing system. It simulates a system with a number of resources and customers (threads) that continually request and release resources.

## Features
- Multithreaded simulation of customers requesting and releasing resources.
- Deadlock avoidance using the Banker's Algorithm.
- Mutex locks preventing race conditions.
- Reads maximum resource requirements from a file.

## Requirements
- GCC compiler for compiling the C program.

## Input File
The program requires an input file named `max_requests.txt` in the same directory as the executable. This file should contain the maximum resources each customer can request. Each line in the file corresponds to a customer, and the resources are separated by commas.

Example:

```
6,4,7,3
4,2,3,2
2,5,3,3
6,3,3,2
5,6,7,5
```

In this example, the first customer can request a maximum of 3 of resource 1, 3 of resource 2, 2 of resource 3, and 2 of resource 4.

## Compilation
To compile the program, navigate to the directory containing the .c file and run the following command in your terminal:

```bash
gcc -o bankers_algorithm bankers_algorithm.c -lpthread
```

This command compiles the program and creates an executable named `bankers_algorithm`.

## Usage
Run the program using the following command:

```bash
./bankers_algorithm <resource_1> <resource_2> <resource_3> <resource_4>
```
Example  
```bash
./bankers_algorithm 10 5 7 8
```

Replace `<resource_1>`, `<resource_2>`, `<resource_3>`, and `<resource_4>` with the number of resources of each type available in the system.

The program will start and the customers will start requesting and releasing resources. When a customer makes a request, the Banker's algorithm will determine if the request can be granted safely. If it can, the request is granted and the state of the system is updated. If it can't, the request is denied.

The program will print the status of each request (granted or denied) whenever a request is made. If a request's status is the same as the previous request's status, the status will not be printed.