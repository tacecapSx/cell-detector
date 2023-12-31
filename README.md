# cell-detector
A solution to assignment 1 of Computer Systems (02132) made by Martin Handest [s224755] ([@tacecapSx](https://github.com/tacecapSx)) and Oskar Holland [s224768] ([@pandekagee](https://github.com/pandekagee)) in C.

## Instructions
### Cell detection
To compile the main program, simply write:

``gcc cbmp.c main.c -o main.exe -std=c99``


...And to run the program, write:

``./main.exe <input filename> <output filename>``

### Automated tests
If you wish to run the program on all sample images automatically, compile the perform_all program with:

``gcc perform_all.c -o perform_all.exe -std=c99``

...And run it with:

``./perform_all.exe`` (make sure a main.exe file exists!)

## Algorithm Progress
- [x] Load input image
- [x] Convert to gray-scale
- [x] Apply binary threshold
- [x] **Perform separation**
- [x] **Fix slicing**
- [x] Erode image
- [x] Detect spots (cells)
- [x] Generate output image (mark cells)
- [x] Save output image
- [x] Print results

## Original Assignment Readme:
This folder contains code and material for the Assignment 1.

To help you get started, we provide you with the library 'cbmp.h' + 'cbmp.c' to load and save images to bmp files. The description of the functions offered by this library is provided in the assignment document (in the Algorithm Step 1 and Step 7 descriptions).

In addition, we also provide an example code (main.c) that loads an image from a file (example.bmp), inverts the value of the color channels (i.e. creates the negative of the image), and saves it to a file. As a starting point, make sure you can run this code without error. Examine the code, and use it as starting point for implementing the algorithm.

If you use the terminal, compile and run 'main.c' as follows: 

Linux/Mac:
- To compile: gcc cbmp.c main.c -o main.out -std=c99
- To run: ./main.out example.bmp example_inv.bmp

Windows:
- To compile: gcc cbmp.c main.c -o main.exe -std=c99
- To run: main.exe example.bmp example_inv.bmp

The folder 'results_example' provides you with some example images obtained by running the algorithm. 

The folder 'samples' provides you with sample images for the 3 different levels of detection difficulty: easy, medium, and hard. Here you can also find the level 'impossible'. This level of difficulty is not part of the assignment. It is here just in case you really want to challenge your algorithm ;-)