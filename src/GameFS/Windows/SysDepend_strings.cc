#include "../strings.h"
#include <conio.h>
void PassInput(char *input) {
  for (uint8_t i = 0; i < 16; i++)
    input[i] = getch(), fprintf(stderr, "*");
  input[16] = '\0';
  fprintf(stderr, "\n");
  return;
}