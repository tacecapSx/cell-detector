//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//Function to invert pixels of an image (negative)
void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGHT; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
      output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
}

void convert_grayscale(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGHT]) {
  for(int x = 0; x < BMP_WIDTH; x++)
  {
    for(int y = 0; y < BMP_HEIGHT; y++)
    {
      unsigned char value = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;

      output_image[x][y] = value;
    }
  }
}

void convert_rgb(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT], unsigned char output_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS]) {
  for(int x = 0; x < BMP_WIDTH; x++)
  {
    for(int y = 0; y < BMP_HEIGHT; y++)
    {
      for(int c = 0; c < BMP_CHANNELS; c++)
      {
        output_image[x][y][c] = input_image[x][y];
      }
    }
  }
}

  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
  unsigned char gray_image[BMP_WIDTH][BMP_HEIGHT];

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  printf("Example program - 02132 - A1\n");

  //Load image from file
  read_bitmap(argv[1], rgb_image);

  //Run inversion
  convert_grayscale(rgb_image,gray_image);

  convert_rgb(gray_image, rgb_image);

  //Save image to file
  write_bitmap(rgb_image, argv[2]);

  printf("Done!\n");
  return 0;
}
