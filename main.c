//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

// Declaring variables
  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
  unsigned char gray_image[BMP_WIDTH][BMP_HEIGHT];
  unsigned char new_gray_image[BMP_WIDTH][BMP_HEIGHT];

  #define EROSION_MASK_WIDTH 3
  #define EROSION_MASK_HEIGHT 3

  int erosion_mask[EROSION_MASK_WIDTH][EROSION_MASK_HEIGHT] = {
    {0, 1, 0},
    {1, 1, 1},
    {0, 1, 0}
  };

void set_pixel(int x, int y, unsigned char val, unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]){
  input_image[x][y] = val;
}

unsigned char get_pixel(int x, int y, unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]){
  return input_image[x][y];
}

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

void apply_threshold(unsigned char threshold, unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]) {
  for(int x = 0; x < BMP_WIDTH; x++)
  {
    for(int y = 0; y < BMP_HEIGHT; y++)
    {
      input_image[x][y] = (input_image[x][y] >= threshold) * 255;
    }
  }
}

void apply_erosion(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT], unsigned char output_image[BMP_WIDTH][BMP_HEIGHT], int erosion_mask[EROSION_MASK_WIDTH][EROSION_MASK_HEIGHT]) {
  for(int x = 0; x < BMP_WIDTH; x++)
  {
    for(int y = 0; y < BMP_HEIGHT; y++)
    {
      for(int x_offset = 0; x_offset < EROSION_MASK_WIDTH; x_offset++)
      {
        for(int y_offset = 0; y_offset < EROSION_MASK_HEIGHT; y_offset++)
        {
          if(erosion_mask[x_offset][y_offset] == 1 && input_image[x - 1 + x_offset][y - 1 + y_offset] == 0) {
            output_image[x][y] = 0;
            goto exit;
          }

          output_image[x][y] = 255;
        }
      }
      exit:
      continue;
    }
  }
}

void save_grayscale_image(char file_name[10], unsigned char gray_image[BMP_WIDTH][BMP_HEIGHT], unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS]) {
  // Convert back to RGB
  convert_rgb(gray_image, rgb_image);

  //Save image to file
  write_bitmap(rgb_image, file_name);
}

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
  write_bitmap(rgb_image, "Step1.bmp");

  // Convert to grayscale
  convert_grayscale(rgb_image,gray_image);
  save_grayscale_image("Step2.bmp",gray_image, rgb_image);

  // Set pixel test
  set_pixel(1,1,255,gray_image);

  // apply threshold
  apply_threshold(100, gray_image);
  save_grayscale_image("Step3.bmp",gray_image, rgb_image);

  char file_name[11];
  // Erode from step 4 onwards...
  for(int s = 4; s < 14; s++) {
    sprintf(file_name,"Step%d.bmp", s);
    printf("%d\n", s);

    if(s % 2 == 1) {
      apply_erosion(new_gray_image, gray_image, erosion_mask);
      save_grayscale_image(file_name,gray_image, rgb_image);
    }
    else {
      apply_erosion(gray_image, new_gray_image, erosion_mask);
      save_grayscale_image(file_name,new_gray_image, rgb_image);
    }
  }

  printf("Done!\n");
  return 0;
}
