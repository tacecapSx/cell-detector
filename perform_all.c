// compile (win): gcc perform_all.c -o perform_all.exe -std=c99
// run (win): perform_all.exe

#include <stdio.h>
#include <stdlib.h>

char paths[35][37] = {
    "easy/1EASY.BMP",
    "easy/2EASY.BMP",
    "easy/3EASY.BMP",
    "easy/4EASY.BMP",
    "easy/5EASY.BMP",
    "easy/6EASY.BMP",
    "easy/7EASY.BMP",
    "easy/8EASY.BMP",
    "easy/9EASY.BMP",
    "easy/10EASY.BMP",
    "medium/1MEDIUM.BMP",
    "medium/2MEDIUM.BMP",
    "medium/3MEDIUM.BMP",
    "medium/4MEDIUM.BMP",
    "medium/5MEDIUM.BMP",
    "medium/6MEDIUM.BMP",
    "medium/7MEDIUM.BMP",
    "medium/8MEDIUM.BMP",
    "medium/9MEDIUM.BMP",
    "medium/10MEDIUM.BMP",
    "hard/1HARD.BMP",
    "hard/2HARD.BMP",
    "hard/3HARD.BMP",
    "hard/4HARD.BMP",
    "hard/5HARD.BMP",
    "hard/6HARD.BMP",
    "hard/7HARD.BMP",
    "hard/8HARD.BMP",
    "hard/9HARD.BMP",
    "hard/10HARD.BMP",
    "impossible/1IMPOSSIBLE.BMP",
    "impossible/2IMPOSSIBLE.BMP",
    "impossible/3IMPOSSIBLE.BMP",
    "impossible/4IMPOSSIBLE.BMP",
    "impossible/5IMPOSSIBLE.BMP",
};

char outputs[35][35] = {
    "easy/output_1EASY.BMP",
    "easy/output_2EASY.BMP",
    "easy/output_3EASY.BMP",
    "easy/output_4EASY.BMP",
    "easy/output_5EASY.BMP",
    "easy/output_6EASY.BMP",
    "easy/output_7EASY.BMP",
    "easy/output_8EASY.BMP",
    "easy/output_9EASY.BMP",
    "easy/output_10EASY.BMP",
    "medium/output_1MEDIUM.BMP",
    "medium/output_2MEDIUM.BMP",
    "medium/output_3MEDIUM.BMP",
    "medium/output_4MEDIUM.BMP",
    "medium/output_5MEDIUM.BMP",
    "medium/output_6MEDIUM.BMP",
    "medium/output_7MEDIUM.BMP",
    "medium/output_8MEDIUM.BMP",
    "medium/output_9MEDIUM.BMP",
    "medium/output_10MEDIUM.BMP",
    "hard/output_1HARD.BMP",
    "hard/output_2HARD.BMP",
    "hard/output_3HARD.BMP",
    "hard/output_4HARD.BMP",
    "hard/output_5HARD.BMP",
    "hard/output_6HARD.BMP",
    "hard/output_7HARD.BMP",
    "hard/output_8HARD.BMP",
    "hard/output_9HARD.BMP",
    "hard/output_10HARD.BMP",
    "impossible/output_1IMPOSSIBLE.BMP",
    "impossible/output_2IMPOSSIBLE.BMP",
    "impossible/output_3IMPOSSIBLE.BMP",
    "impossible/output_4IMPOSSIBLE.BMP",
    "impossible/output_5IMPOSSIBLE.BMP",
};

int main() {
    for(int i = 0; i < 35; i++) {
        char command[125];

        sprintf(command,"main.exe ./samples/%s outputs/%s\n",paths[i],outputs[i]);
        printf("%s",command);

        system(command);
    }

    return 0;
}
