# Elevator Simulation Program for Operating System Course

This repository contains a C++ program designed as a project for an Operating System course. The program simulates the operation of an elevator in a building with 9 floors, modeling elevator movements, people generation, and request handling using multithreading.

## Purpose

The primary objectives of this code are:

- **Simulate Elevator Operation**: Model the behavior of an elevator moving between floors, handling requests, and managing passengers.
- **Illustrate Multithreading**: Demonstrate the use of threads in C++ to manage concurrent tasks such as generating people and processing elevator requests.
- **Showcase Synchronization**: Utilize mutexes and condition variables to ensure thread-safe operations and coordination between different parts of the program.
- **Implement Scheduling Algorithms**: Illustrate elevator scheduling algorithms similar to those used in operating systems to handle processes and requests efficiently.
- **Simulate Realistic Scenarios**: Implement random generation of people with varying weights and destinations to mimic real-world scenarios.

## Key Features

- **Multithreading**: Leverages C++11 threading capabilities to manage the elevator's operations and the generation of people concurrently.
- **Synchronization**: Uses mutexes and condition variables to coordinate between the elevator's movements and people's requests.
- **Dynamic Requests Handling**: Handles both inside and outside requests for the elevator, ensuring efficient movement and passenger management.
- **Random People Generation**: Generates people with random weights, source floors, and destination floors to create realistic elevator usage patterns.
- **Status Reporting**: Provides detailed status reports on elevator movements, requests, and passenger details.

## Understanding Key Concepts

This project helps in understanding the following key concepts in operating systems:

- **Multithreading and Concurrency**: The use of threads to perform multiple tasks simultaneously.
- **Synchronization Mechanisms**: Ensuring thread-safe operations using mutexes and condition variables.
- **Scheduling Algorithms**: Implementing scheduling logic to efficiently handle requests and manage resources.
- **Resource Management**: Handling the capacity of the elevator and ensuring it does not exceed its limits.

## Problem Definition

The problem is to simulate an elevator system in the Faculty of Computer Science and Information Technology at the Graduate University of Zanjan, which has 9 floors. The elevator must have the following features:
1. The elevator's capacity is 500 kilograms.
2. The priority for the elevator's movement is for the people inside the elevator.
3. The starting location of the elevator is the first floor.
4. When the elevator is called from any floor, it will move to that floor and start operating.
5. If there are no requests, the elevator remains idle until the next request.

At each floor, there may be people who need to move between floors. The number of people at any moment can increase. The goal of this project is to write a program to simulate this elevator system, where:
- People are randomly added to floors. (The maximum number of people in the queue for odd floors is 13 and for even floors is 20. The weight of the people is randomly selected between 40 to 110 kilograms.)
- People who arrive first in the queue can use the elevator first. The program should handle the requests efficiently, and the program ends when at least 100 people have been transported between floors.

## Analysis

Based on the problem definition, the elevator is defined as a class (`Elevator`) in the program, with the following attributes considered as private members of the class:
- **Location**: The current location of the elevator (Which floor is the elevator currently on?)
- **Direction**: The direction of the elevator's movement (0: stopped, 1: moving up, 2: moving down)
- **Capacity**: The current total weight of the people inside the elevator
- **Maximum_Work_Can_Do**: The maximum amount of work the elevator can do (How much more can the elevator work?)
- **People_Inside**: A queue representing the people currently inside the elevator
- **OutSide_Requests**: A queue representing the requested floors for people waiting outside the elevator
- **InSide_Requests**: A queue representing the requested floors for people inside the elevator to get off

### Key Methods of the Elevator Class

- **Start**: The main method that starts the elevator's operation after initializing its attributes. This method manages the elevator's direction, stopping, moving, allowing people to get in and out, and continues until `Maximum_Work_Can_Do` is zero.
- **Move**: Moves the elevator by changing its `Location` attribute according to the `Direction`.
- **Which_Direction**: Determines the direction the elevator should move based on the request queues (0: stopped, 1: moving up, 2: moving down).
- **Check_Floors_Requests**: Checks the requests from people on different floors and adds them to the outside request queue (`OutSide_Requests`).
- **People_GetIn**: Manages people getting into the elevator.
- **People_GetOut**: Manages people getting out of the elevator.

### Use of Threads

To implement the program, threads are used:
- One thread for overall management of the elevator (generating and handling necessary tasks).
- One thread for generating people.

Additionally, a `Building` class is defined to encapsulate the elevator, with a main method `People_Generator` to generate people and add them to the request queues.

### Conclusion

One drawback of this elevator management approach (prioritizing internal requests) is that if the density of requests for getting on and off the elevator is higher in certain floors (e.g., lower floors), the elevator tends to stay in those lower floors more, neglecting the requests from higher floors, leading to an unfair distribution. This issue can be mitigated by incorporating a counter or prioritizing floors to ensure the elevator passes through each floor at least once.
