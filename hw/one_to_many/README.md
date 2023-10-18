# One-to-Many Communication Model with Python

This README addresses a tricky problem related to the one-to-many communication model, similar to the publish/subscribe pattern. We will extend upon the concepts presented in the first and second examples. Imagine you have two second processes, both reading data from the first process. The operator decides which second process will process the data by explicitly specifying the first number in the input. Data is broadcasted to both second processes through the same unique FIFO channel.

## Problem Description

The challenge is to devise a solution to this problem. You may find it helpful to review the following slides on pipes to gain a better understanding of the underlying concepts.

## Realistic Code Implementation

To provide a realistic code example, we will create a common "father" process that executes the three child processes. The "father" process will wait until all child processes terminate and then terminate itself after notifying completion.

### Code Overview

Here's an overview of the steps we'll take:

1. Create a "father" process that spawns three child processes.
2. Implement a communication mechanism using FIFO (named pipes) to broadcast data to the two second processes.
3. The operator specifies which second process should process the data by indicating it as the first number in the input.
4. Ensure that all processes terminate gracefully and notify the "father" process upon completion.

### Running the Code

To run the code, follow these steps:

1. Compile and execute the "father" process, which will start the child processes.
2. Input data along with the specified first number to determine the processing second process.

### Expected Behavior

The code should demonstrate the one-to-many communication model, where the "father" process manages the child processes and data distribution to the second processes based on operator input.

Please refer to the code files for a detailed implementation of this problem.