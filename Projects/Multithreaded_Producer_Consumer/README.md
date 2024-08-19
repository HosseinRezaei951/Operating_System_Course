# Producer-Consumer Multithreaded Program

This repository contains a C++ program designed for an Operating System course. The main goal of this code is to demonstrate the producer-consumer problem using multithreading, synchronization, and inter-thread communication mechanisms in C++.

## Purpose

The primary objectives of this program are:

- **Illustrate Multithreading**: Show how multiple threads can be created and managed in C++.
- **Demonstrate Synchronization**: Use mutexes and condition variables to synchronize access to shared resources.
- **Simulate Producer-Consumer Problem**: Implement the classic producer-consumer scenario where producer threads generate data and consumer threads process the data.
- **Show Inter-Thread Communication**: Utilize condition variables to handle communication between producer and consumer threads.
- **Provide Status Reporting**: Implement a master thread that monitors and reports the status of all producer and consumer threads.

This program serves as an educational tool for understanding fundamental concepts of concurrent programming and synchronization in an operating system context.

## Key Features

- **Multithreading**: Utilizes C++11 threading capabilities to create and manage multiple threads, simulating concurrent execution.
- **Synchronization Mechanisms**: Implements mutexes for mutual exclusion and condition variables for signaling between threads, ensuring safe access to shared resources.
- **Producer-Consumer Simulation**: Models the producer-consumer problem with multiple producer and consumer threads, showcasing data production and consumption.
- **Scheduling Algorithms**: Includes Shortest Job First (SJF) and Round Robin (RR) scheduling algorithms to manage the execution order of threads based on different criteria.
- **Dynamic Task Management**: Uses a request queue to dynamically manage task allocation to threads, supporting different scheduling strategies.
- **Status Monitoring**: A master thread continuously monitors and reports the status of all producer and consumer threads, providing insights into the system's state and performance.
- **Randomized Behavior**: Introduces randomness in consumer thread actions to simulate real-world unpredictability in task handling.
- **Comprehensive Output**: Produces detailed output logs that trace the activities and interactions of all threads, aiding in understanding the flow and coordination of the program.
