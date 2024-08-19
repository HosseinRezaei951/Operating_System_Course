# Producer-Consumer Multithreaded Program with Scheduling

This repository contains a C++ program designed for an Operating System course. The program simulates the producer-consumer problem using multithreading and demonstrates two different scheduling algorithms: Shortest Job First (SJF) and Round Robin (RR).

## Purpose

The primary objectives of this code are:

- **Illustrate Multithreading**: Demonstrate how to create and manage multiple threads in C++.
- **Showcase Synchronization**: Use mutexes and condition variables to handle synchronization between threads.
- **Simulate Producer-Consumer Problem**: Implement a producer-consumer scenario where producer threads generate data and consumer threads process the data.
- **Implement Scheduling Algorithms**: Demonstrate the use of SJF and RR scheduling algorithms to manage thread execution.

## Key Features

- **Multithreading**: Utilizes C++11 threading capabilities to create and manage multiple threads.
- **Synchronization**: Implements mutexes and condition variables to ensure proper coordination between threads.
- **Scheduling Algorithms**: Includes implementations of SJF and RR to manage the execution order of threads.
- **Status Reporting**: A master thread monitors and reports the status of all producer and consumer threads.