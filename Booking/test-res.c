#!/bin/bash

#printing initial message
echo -e "\nRunning the program for 100 bookings and seed 1000\n"

#compiling the program
gcc -Wall -pthread leitourgika.c -o booking.out

#running the program for 100 bookings and seed=1000
./booking.out 100 1000

#printing a message
echo -e "\nProgram finished, press any key to exit"

#waiting for input
read varkey
