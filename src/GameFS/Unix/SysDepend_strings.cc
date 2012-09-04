#include <termios.h>
#include "../strings.h"
void PassInput(char *input) {
  struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  for (uint8_t i = 0; i < 16; i++)
    input[i] = getchar(), fprintf(stderr, "*");
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  input[16] = '\0';
  fprintf(stderr, "\n");

  return;
}
