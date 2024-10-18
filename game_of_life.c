#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define ROW 25
#define COL 80
#define START_SPEED 500
#define MAX_SPEED 100
#define MIN_SPEED 1000
#define STEP 100

void print_field(int matrix[ROW][COL]);
void print_speed(int speed);
void print_control_information();
void initialisation_matrix(int matrix[ROW][COL]);
int development_of_life(int matrix[ROW][COL]);
int get_count_of_neighbours(int matrix[ROW][COL], int x, int y);
void get_matrix_from_stdin(int matrix[ROW][COL]);
void set_speed_game(char user_input, int *game_speed);

int main() {
  int game_speed = START_SPEED;
  static int matrix[ROW][COL];
  initialisation_matrix(matrix);
  get_matrix_from_stdin(matrix);
  initscr();
  freopen("/dev/tty", "r", stdin);
  char user_input = ' ';
  int change_marker = 1;
  while (user_input != 'q' && change_marker != 0) {
    keypad(stdscr, true);
    print_field(matrix);
    change_marker = development_of_life(matrix);
    print_speed(game_speed);
    print_control_information();
    user_input = getch();
    set_speed_game(user_input, &game_speed);
    timeout(game_speed);
    erase();
  }
  endwin();
  return 1;
}

void set_speed_game(char user_input, int *game_speed) {
  if (user_input == '-' && *game_speed < MIN_SPEED)
    *game_speed += STEP;
  else if (user_input == '+' && *game_speed > MAX_SPEED)
    *game_speed -= STEP;
}

void initialisation_matrix(int matrix[ROW][COL]) {
  for (int i = 0; i < ROW; i++) {
    for (size_t j = 0; j < COL; j++) {
      matrix[i][j] = 0;
    }
  }
}

void print_field(int matrix[ROW][COL]) {
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      if (matrix[i][j] == 1)
        printw("*");
      else
        printw(".");
    }
    printw("\n");
  }
}

void print_speed(int speed) {
  printw("SPEED: ");
  for (int i = 0; i <= MIN_SPEED - speed; i += STEP) {
    printw("=>");
  }
  printw("\n");
}

int development_of_life(int matrix[ROW][COL]) {
  int cout_of_change = 0;
  static int tmp[ROW][COL];
  for (int i = 0; i < ROW; i++)
    for (int j = 0; j < COL; j++) tmp[i][j] = matrix[i][j];
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      int count_of_neighbours = get_count_of_neighbours(tmp, j, i);
      if (tmp[i][j] == 1) {
        if (count_of_neighbours >= 2 && count_of_neighbours <= 3)
          ;
        else {
          matrix[i][j] = 0;
          cout_of_change++;
        }
      } else {
        if (count_of_neighbours == 3) {
          matrix[i][j] = 1;
          cout_of_change++;
        }
      }
    }
  }
  return cout_of_change;
}

int get_count_of_neighbours(int matrix[ROW][COL], int x, int y) {
  int result = 0;
  if (y == 0 && x == 0)
    result = matrix[ROW - 1][COL - 1] + matrix[ROW - 1][x] +
             matrix[ROW - 1][x + 1] + matrix[y][COL - 1] + matrix[y][x + 1] +
             matrix[y + 1][COL - 1] + matrix[y + 1][x] + matrix[y + 1][x + 1];
  else if (y == 0 && x == COL - 1)
    result = matrix[ROW - 1][x - 1] + matrix[ROW - 1][x] + matrix[ROW - 1][0] +
             matrix[y][x - 1] + matrix[0][0] + matrix[y - 1][x - 1] +
             matrix[y - 1][x] + matrix[y - 1][0];
  else if (y == ROW - 1 && x == 0)
    result = matrix[0][COL - 1] + matrix[0][x] + matrix[0][1] +
             matrix[y][x + 1] + matrix[y][COL - 1] + matrix[ROW - 2][COL - 1] +
             matrix[y - 1][x] + matrix[y - 1][x + 1];
  else if (y == ROW - 1 && x == COL - 1)
    result = matrix[y - 1][x - 1] + matrix[y - 1][x] + matrix[y - 1][0] +
             matrix[y][x - 1] + matrix[y][0] + matrix[0][x - 1] + matrix[0][x] +
             matrix[0][0];
  else
    result = matrix[y + 1][x - 1] + matrix[y + 1][x] + matrix[y + 1][x + 1] +
             matrix[y][x - 1] + matrix[y][x + 1] + matrix[y - 1][x - 1] +
             matrix[y - 1][x] + matrix[y - 1][x + 1];
  return result;
}

void get_matrix_from_stdin(int matrix[ROW][COL]) {
  char ch = ' ';
  for (int i = 0; i < ROW; i++) {
    for (int j = 0; j < COL; j++) {
      ch = getchar();
      if (ch == '1')
        matrix[i][j] = 1;
      else if (ch == '0')
        matrix[i][j] = 0;
    }
  }
}

void print_control_information() {
  printw("press 'q'for exit\n");
  printw("use '+/-' for change speed\n");
}