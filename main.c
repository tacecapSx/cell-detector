//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "cbmp.h"

// Declaring variables
  //Runtime analysis
  int time_elapsed;
  #define START_TIMER clock_t start_time = clock()
  #define END_TIMER clock_t end_time = clock()
  #define CALCULATE_TIME ((int)(end_time - start_time))

  //Declaring the arrays to store the images (unsigned char = unsigned 8 bit)
  unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
  unsigned char final_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS];
  unsigned char gray_image[BMP_WIDTH][BMP_HEIGHT];
  unsigned char new_gray_image[BMP_WIDTH][BMP_HEIGHT];

  //Counting cells
  #define CELL_MAX 500
  int cell_amount = 0;
  int cell_x[CELL_MAX];
  int cell_y[CELL_MAX];

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

void separate_cells(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]){
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

void fix_slicing(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT]){
  for(int i = 1; i < BMP_HEIGHT - 1; i++){
    for(int j = 1; j < BMP_WIDTH - 1; j++){
      if(input_image[i][j] == 0 && input_image[i][j - 1] == 255 && input_image[i][j + 1] == 255) {
        input_image[i][j] = 255;
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

int count_cells(unsigned char input_image[BMP_WIDTH][BMP_HEIGHT], unsigned char rgb_image[BMP_WIDTH][BMP_HEIGHT][BMP_CHANNELS], int cell_x[CELL_MAX], int cell_y[CELL_MAX], int cell_amount){
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
        cell_x[cell_amount + count] = i + 1;
        cell_y[cell_amount + count] = j + 1;

        for(int k = 0; k < 6 && i + k + check_width/2 < BMP_WIDTH; k ++){
          for(int l = 0; l < 6 && j + l + check_height/2 < BMP_HEIGHT; l ++){
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
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)

  // Start timer (to time execution of the entire program in this case)
  START_TIMER;

  //Checking that 2 arguments are passed
  if (argc != 2)
  {
      fprintf(stderr, "Usage: %s <input file path>\n", argv[0]);
      exit(1);
  }

  // 1: Load image from file
  read_bitmap(argv[1], rgb_image);
  read_bitmap(argv[1], final_image);
  write_bitmap(rgb_image, "step_0.bmp");

  // Convert to grayscale
  convert_grayscale(rgb_image,gray_image); // 3 ms

  // 2: Apply binary threshold
  apply_threshold(95, gray_image); // 1 ms
  save_grayscale_image("step_1.bmp",gray_image, rgb_image); // 22 ms

  // 3: Separate cells
  separate_cells(gray_image); // 2 ms
  save_grayscale_image("step_2.bmp",gray_image, rgb_image); // 22 ms

  // 4: Fix slicing
  fix_slicing(gray_image); // 1 ms
  save_grayscale_image("step_3.bmp",gray_image, rgb_image); // 22 ms

  // 5: Apply initial, aggressive erosion to clean up separation
  apply_erosion(gray_image, new_gray_image, 21, 5, 5); // 97 ms
  save_grayscale_image("step_4.bmp",new_gray_image, rgb_image); // 22 ms

  char file_name[12];
  // Erode from step 5 onwards... (maxing out at 20 - 5 = 15 erosion steps, less will likely be needed.)
  for(int s = 5; s < 20; s++) {
    sprintf(file_name,"step_%d.bmp", s);
    printf("Erosion Step: %d\n", s - 4);

    unsigned char (*input_image)[BMP_HEIGHT] = s % 2 ? new_gray_image : gray_image;
    unsigned char (*output_image)[BMP_HEIGHT] = s % 2 ? gray_image : new_gray_image;

    if(apply_erosion(input_image, output_image, 7, 3, 3)) { // 34 ms
      break; // image is completely eroded
    }
    cell_amount += count_cells(output_image, final_image, cell_x, cell_y, cell_amount); // 2 ms
    save_grayscale_image(file_name, output_image, rgb_image); // 22 ms
  }

  write_bitmap(final_image, "output_image.bmp");

  printf("\nCell Amount is: %i\n\nFound cells at these coordinates:\n", cell_amount);
  
  int i = 0;
  while(cell_x[i] != 0) {
    printf("[%u, %u], ", cell_x[i], cell_y[i]);

    i++;
    if(i % 6 == 0) {
      printf("\n");
    }
  }

  printf("\n\nDone!\n");

  END_TIMER;
  time_elapsed = CALCULATE_TIME;
  printf("Total program time: %d ms\n", time_elapsed);
  return 0;
}
