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
  unsigned char final_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
  unsigned char gray_image[BMP_WIDTH][BMP_HEIGHT];
  unsigned char new_gray_image[BMP_WIDTH][BMP_HEIGHT];

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

void seperate_cells(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]){
  int check_width = 20;
  int check_height = 20;
  int found = 0;
  int offset = 8;

  for(int i = 0; i < BMP_WIDTH-check_width; i ++){
    for(int j = 0; j < BMP_HEIGHT-check_height; j ++){
      found = 0;
      if (input_image[i+offset][j+offset] == 255 && input_image[i+check_width-offset][j+check_height-offset] == 255
      && input_image[i+(check_width-1)/2][j+(check_height-1)/2] == 255 && input_image[i+check_width-offset][j+offset] == 255
      && input_image[i+offset][j+check_height-offset] == 255){
        found = 1;
      }

      if (found == 1){
        for(int k = 0; k < check_width; k++){
          input_image[i+k][j] = 0;
          input_image[i+k][j+check_height] = 0;
        }

        for(int k = 0; k < check_height; k++){
          input_image[i][j+k] = 0;
          input_image[i+check_width][j+k] = 0;
        }
      }
    }
  }
}

int clamp(int val, int min, int max) {
  if(val < min) {
    return min;
  }

  if(val > max) {
    return max;
  }

  return val;
}

int apply_erosion(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT], unsigned char output_image[BMP_WIDTH][BMP_HEIGHT], int min_neighbors, int width, int height) {
  int total_dark = 0;
  for(int x = 0; x < BMP_WIDTH; x++)
  {
    for(int y = 0; y < BMP_HEIGHT; y++)
    {
      int counter = 0;
      for(int x_offset = 0; x_offset < width; x_offset++)
      {
        for(int y_offset = 0; y_offset < height; y_offset++)
        {
          if(input_image[clamp(x-(width-1) / 2+x_offset,0,BMP_WIDTH-1)][clamp(y-(height-1) / 2+y_offset,0,BMP_HEIGHT-1)]) {
            counter++;
          }
        }
      }

      if(input_image[x][y] == 255 && counter > min_neighbors) {
        output_image[x][y] = 255;
      }
      else {
        output_image[x][y] = 0;
        total_dark++;
      }
    }
  }

  return total_dark == BMP_WIDTH * BMP_HEIGHT;
}

unsigned char get_average_color(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]) {
  int sum = 0;

  for(int x = 0; x < BMP_WIDTH; x++)
  {
    for(int y = 0; y < BMP_HEIGHT; y++)
    {
      sum += (int)input_image[x][y];
    }
  }

  return sum / (BMP_WIDTH * BMP_HEIGHT);
}

void save_grayscale_image(char file_name[11], unsigned char gray_image[BMP_WIDTH][BMP_HEIGHT], unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS]) {
  // Convert back to RGB
  convert_rgb(gray_image, rgb_image);

  //Save image to file
  write_bitmap(rgb_image, file_name);
}

int count_cells(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT], unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS]){
  int check_width = 8;
  int check_height = 8;
  int found = 0;

  int count = 0;
  
  //loop through grid and area
  for(int i = -1; i < BMP_WIDTH+1; i ++){
    for(int j = -1; j < BMP_HEIGHT+1; j ++){
      if (input_image[i + (check_width-1)/2][j+(check_height-1)/2] == 255){
        found = 1;
      }

      if (found == 1){
        for(int k = 0; k < check_width; k++){
          if ((input_image[i + k][j] == 255 || input_image[i + k][j + check_height-1] == 255)){
            goto end;
          }
        }

        for(int k = 0; k < check_height; k++){
          if ((input_image[i][j + k] == 255 || input_image[i + check_width-1][j + k] == 255)){
            goto end;
          }
        }
      }

      if (found == 1){
        for(int k = 0; k < 6; k ++){
          for(int l = 0; l < 6; l ++){
            rgb_image[i+k + check_width/2][j + l + check_height / 2][0] = 255;
            rgb_image[i+k + check_width/2][j + l + check_height / 2][1] = 0;
            rgb_image[i+k + check_width/2][j + l + check_height / 2][2] = 0;
          }
        }

        count++;
        for(int k = 1; k < check_width - 1; k ++){
          for(int l = 1; l < check_height - 1; l ++){
            input_image[i + k][j + l] = 0;
          }
        }
        
      }

      end:
      found = 0;
    }
  }

  return count;
}

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)

  int cell_amount = 0;

  //Checking that 2 arguments are passed
  if (argc != 2)
  {
      fprintf(stderr, "Usage: %s <input file path>\n", argv[0]);
      exit(1);
  }

  //Load image from file
  read_bitmap(argv[1], rgb_image);
  read_bitmap(argv[1], final_image);
  write_bitmap(rgb_image, "Step1.bmp");

  // Convert to grayscale
  convert_grayscale(rgb_image,gray_image);

  // apply threshold
  apply_threshold(95, gray_image);
  save_grayscale_image("Step2.bmp",gray_image, rgb_image);

  // separate cells
  seperate_cells(gray_image);
  save_grayscale_image("Step3.bmp",gray_image, rgb_image);

  // apply initial, aggressive erotion
  apply_erosion(gray_image, new_gray_image, 21, 5, 5);
  save_grayscale_image("Step4.bmp",new_gray_image, rgb_image);

  char file_name[11];
  // Erode from step 5 onwards... (maxing out at 20 - 5 = 15 erosion steps, less will be needed.)
  for(int s = 5; s < 20; s++) {
    sprintf(file_name,"Step%d.bmp", s);
    printf("Erosion step: %d\n", s - 4);

    if(s % 2) {
      if(apply_erosion(new_gray_image, gray_image, 7, 3, 3)) {
        break;
      }
      cell_amount += count_cells(gray_image, final_image);
      save_grayscale_image(file_name,gray_image, rgb_image);
    }
    else {
      if(apply_erosion(gray_image, new_gray_image, 7, 3, 3)) {
        break;
      }
      cell_amount += count_cells(new_gray_image, final_image);
      save_grayscale_image(file_name,new_gray_image, rgb_image);
    }
  }

  write_bitmap(final_image, "output.bmp");

  printf("Cell Amount is: %i\n", cell_amount);

  printf("Done!\n");
  return 0;
}
