#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include <windows.h>

static const int screen_width = 40;
static const int screen_height = 20;

static bool gameover;

static int px, py; // player position
static int fx, fy; // fruit position
static int tx[100], ty[100]; // tail
static int tnum;
static int score;

typedef enum {
  dir_stop = 0,
  dir_left,
  dir_right,
  dir_up,
  dir_down
} direction_t;

static direction_t pdir; // player direction

static void
do_setup(void)
{
  px = screen_width / 2;
  py = screen_height / 2;

  fx = rand() % screen_width;
  fy = rand() & screen_height;
}

static void
do_draw(void)
{
  system("cls");
  
  for (int i = 0; i < screen_width; i++)
    printf("#");
  printf("\n");
  
  for (int i = 0; i < screen_height; i++) {
    for (int j = 0; j < screen_width; j++) {
      if (j == 0)
	printf("#");
      else if (j == screen_width - 1)
	printf("#");
      else if (i == py && j == px)
	printf("O");
      else if (i == fy && j == fx)
	printf("F");
      else {
	bool printed = false;
	for (int k = 0; k < tnum; k++) {
	  if (tx[k] == j && ty[k] == i) {
	    printf("o");
	    printed = true;
	  }
	}

	if (!printed)
	  printf(" ");
      }
    }

    printf("\n");
  }

  for (int i = 0; i < screen_width; i++)
    printf("#");
  printf("\n");

  printf("SCORE: %d\r\n", score);
}

static void
do_input(void)
{
  if (_kbhit()) {
    switch (_getch()) {
    case 'a':
      pdir = dir_left;
      break;

    case 'd':
      pdir = dir_right;
      break;

    case 'w':
      pdir = dir_up;
      break;

    case 's':
      pdir = dir_down;
      break;

    case 'q':
      gameover = true;
      break;
    }
  }
}

static void
do_logic(void)
{
  int prevx = tx[0];
  int prevy = ty[0];
  int prev2x, prev2y;

  tx[0] = px;
  ty[0] = py;

  for (int i = 1; i < tnum; i++) {
    prev2x = tx[i];
    prev2y = ty[i];

    tx[i] = prevx;
    ty[i] = prevy;

    prevx = prev2x;
    prevy = prev2y;
  }
  
  switch (pdir) {
  case dir_left:
    px--;
    break;

  case dir_right:
    px++;
    break;

  case dir_up:
    py--;
    break;

  case dir_down:
    py++;
    break;
  }

  if (px > screen_width || px < 0)
    gameover = true;
  if (py > screen_height || py < 0)
    gameover = true;

  for (int i = 0; i < tnum; i++) {
    if (tx[i] == px && ty[i] == py)
      gameover = true;
  }

  if (px == fx && py == fy) {
    score += 10;

    fx = rand() % screen_width;
    fy = rand() & screen_height;

    tnum++;
  }
}

int
main(void)
{
  do_setup();

  while(!gameover) {
    do_draw();
    do_input();
    do_logic();
    Sleep(100);
  }

  printf("GAME OVER\n");
  return 0;
}
