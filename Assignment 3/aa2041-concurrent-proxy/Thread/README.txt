
Name: Ashwin Anand
Class: Network Centric Programming - Spring 2022
Assignment: Assignment 3 - Concurrent Web Proxy - Threaded Version

This implementation of the code only works for http sites.

Sites tested:
http://silverastoundingbrightpoem.neverssl.com/online
http://www.example.com/

Steps to run:

The executable has to made through the command make in the terminal

To run the program set up your proxy settings on your machine

To run the code type in ./port <port number> 

The code will run as it continuously listens to requests from the web browser

The user can close the program by opening up terminal 2. In terminal 2, they obtain the process id by typing in "ps" to the terminal which will list all the process and process id's happening at the same time. To terminate the process then type in kill <processid listed next to 8080>. This will show a zsh: termination message in terminal 1.

The user will be able to take a look at the PLog.txt file as it contains the Date, time, browser ip address, browser url, size



Directory files information:

The makefile, csapp.c and csapp.h were not changed and only proxy.c was changed to write the code in to the main method

The directory contains:

csapp.c - not changed and contains helper functions
csapp.h - not changed and contains wrapper function
csapp.o - not changed and its object code
Makefile - make file used to run the command make to create the proxy executable
proxy - executable made through make from Makefile
proxy.c - changed and contains all the code
proxy.o - object code
ProxyLog.txt - proxy log text file that will be written into
README.txt - read me file that talks about the program