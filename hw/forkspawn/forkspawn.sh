#!/bin/bash

# Compile required files
gcc spawn_first_second.c -o spawn_first_second
gcc first.c -o first
gcc second.c -o second

 ./spawn_first_second &