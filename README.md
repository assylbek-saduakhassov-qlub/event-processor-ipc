# Event Processor IPC

## Overview

`event-processor-ipc` is a C-based project demonstrating inter-process communication (IPC) between a parent process and multiple child processes using unnamed pipes. It simulates the processing of various system events in parallel, mimicking an operating system's event handling mechanism. Each child process is responsible for a specific type of event, such as process creation, memory allocation, file operations, user logins, or system boots.

## Features

- **Inter-Process Communication (IPC)**: Demonstrates IPC using unnamed pipes between parent and child processes.
- **Parallel Event Handling**: Each child process handles a different type of event, enabling parallel processing.
- **Event Processing**: Processes simulated events such as `PROCESS_CREATE`, `MEMORY_ALLOC`, `FILE_OPEN`, `USER_LOGIN`, and `SYSTEM_BOOT`.
- **Modular Design**: Each event type is processed by a dedicated handler, making the codebase easy to extend and maintain.


## Event Log Format

The `events_log.txt` file simulates a log of system events. Each line in the log corresponds to a specific event type and contains relevant information such as process IDs, memory size, file paths, or user details.
