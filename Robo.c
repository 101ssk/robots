#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ROBOTS 5
#define HIGHT 20
#define LENGTH 40
#define BIAS 10

#define FALSE 0
#define TRUE 1

#define NEXT 2
#define CLEAR 7

#define NONE 0
#define WALL -1
#define ROBO 2
#define SCRAP 4
#define PLAYER 8

void game_start(void);
void board_state_init(int board[][LENGTH],int state[][LENGTH]);
void board_state_set(int board[][LENGTH],int state[][LENGTH],int *p_hight,int *p_length,int robo_x[ROBOTS],int robo_y[ROBOTS],int level);
void move_player(int board[][LENGTH],int state[][LENGTH],int *p_hight,int *p_length);
void move_robot(int board[][LENGTH],int state[][LENGTH],int *p_hight,int *p_length,int robo_x[ROBOTS],int robo_y[ROBOTS],int *flag,int *score);
void scrap_robot(int board[][LENGTH],int state[][LENGTH],int robo_x[ROBOTS],int robo_y[ROBOTS],int *flag,int *score);
void game_over(int board[][LENGTH]);
int check(int state[][LENGTH]);
void disp(int board[][LENGTH]);

int main(void)
{
  srand((unsigned)time(NULL));

  game_start();

  return 0;
}

void game_start(void) //ゲームの処理を行う関数
{
  static int level = 1;
  static int score = 0;
  int board[HIGHT][LENGTH],state[HIGHT][LENGTH];
  int p_hight,p_length,flag = FALSE;
  int robo_x[ROBOTS*level],robo_y[ROBOTS*level];

  board_state_init(board,state); //ボードと状態の配列の初期化
  board_state_set(board,state,&p_hight,&p_length,robo_x,robo_y,level); //プレイヤーとロボの配置
  disp(board); //表示
  while(flag == FALSE){
    printf("(LEVEL:%d SCORE:%d):",level,score);
    move_player(board,state,&p_hight,&p_length); //プレイヤーの移動
    move_robot(board,state,&p_hight,&p_length,robo_x,robo_y,&flag,&score); //ロボットの移動
    scrap_robot(board,state,robo_x,robo_y,&flag,&score); //ロボットのスクラップ状態の配置
    if(flag == TRUE) game_over(board); //ゲームオーバー表示
    else if(flag == NEXT){
      flag = check(state); //ロボットが全滅したか確認
      if(flag == CLEAR){ //次のレベルへ
        score += level*BIAS;
        level++;
        game_start();
      }
      disp(board);
    } else disp(board);
  }
}

void board_state_init(int board[][LENGTH],int state[][LENGTH]) //ボードと状態の配列の初期化
{
  int x,y;

  for(y=0;y<HIGHT;y++){
    for(x=0;x<LENGTH;x++){
      board[y][x] = ' ';
      state[y][x] = NONE;
      if(y==0 || y==HIGHT-1){
        board[y][x] = '#';
        state[y][x] = WALL;
      }
      if(x==0 || x==LENGTH-1){
        board[y][x] = '#';
        state[y][x] = WALL;
      }
      if(y==0 && x==0 || y==0 && x==LENGTH-1 || y==HIGHT-1 && x==0 || y==HIGHT-1 && x==LENGTH-1){
        board[y][x] = '#';
        state[y][x] = WALL;
      }
    }
  }
}

void board_state_set(int board[][LENGTH],int state[][LENGTH],int *p_hight,int *p_length,int robo_x[ROBOTS],int robo_y[ROBOTS],int level) //プレイヤーとロボの配置
{
  int p_y,p_x;
  int i = 0;

  p_y = 1+rand()%(HIGHT-2);
  p_x = 1+rand()%(LENGTH-2);

  board[p_y][p_x] = '@';
  state[p_y][p_x] = PLAYER;

  *p_hight = p_x;
  *p_length = p_y;

  while(i<ROBOTS*level){
    robo_y[i] = 1+rand()%(HIGHT-2);
    robo_x[i] = 1+rand()%(LENGTH-2);
    if(state[robo_y[i]][robo_x[i]]==NONE){
      board[robo_y[i]][robo_x[i]] = '+';
      state[robo_y[i]][robo_x[i]] = ROBO;
      i++;
      if(i == LENGTH) break;
    }
  }
}
void move_player(int board[][LENGTH],int state[][LENGTH],int *p_hight,int *p_length) //プレイヤーの移動
{
  int i,num,p_y,p_x,x,y;

  p_y = *p_length;
  p_x = *p_hight;
  scanf("%d",&num);
  board[p_y][p_x] = ' ';
  state[p_y][p_x] = NONE;

  switch(num){
    case 0: y = 1+rand()%(HIGHT-2),x = 1+rand()%(LENGTH-2);
    while(state[y][x]!=NONE) {
      y = 1+rand()%(HIGHT-2),x = 1+rand()%(LENGTH-2);
    }
    p_y = y,p_x = x; break;
    case 1: p_y = p_y+1,p_x = p_x-1; break;
    case 2: p_y = p_y+1,p_x = p_x; break;
    case 3: p_y = p_y+1,p_x = p_x+1; break;
    case 4: p_y = p_y,p_x = p_x-1; break;
    case 5: p_y = p_y,p_x = p_x; break;
    case 6: p_y = p_y,p_x = p_x+1; break;
    case 7: p_y = p_y-1,p_x = p_x-1; break;
    case 8: p_y = p_y-1,p_x = p_x; break;
    case 9: p_y = p_y-1,p_x = p_x+1; break;
  }
  if(p_y!=0 && p_x!=0 && p_y!=HIGHT-1 && p_x!=LENGTH-1){
    if(state[p_y][p_x]==NONE){
      board[p_y][p_x] = '@';
      state[p_y][p_x] = PLAYER;
      *p_length = p_y;
      *p_hight = p_x;
    }
  }
}
void move_robot(int board[][LENGTH],int state[][LENGTH],int *p_hight,int *p_length,int robo_x[ROBOTS],int robo_y[ROBOTS],int *flag,int *score) //ロボットの移動
{
  int i;

  for(i=0;i<ROBOTS;i++){
    if(state[robo_y[i]][robo_x[i]] == ROBO){
      board[robo_y[i]][robo_x[i]] = ' ';
      state[robo_y[i]][robo_x[i]] = NONE;
      if(robo_x[i] < *p_hight) robo_x[i] += 1;
      if(robo_x[i] > *p_hight) robo_x[i] -= 1;
      if(robo_y[i] < *p_length) robo_y[i] += 1;
      if(robo_y[i] > *p_length) robo_y[i] -= 1;
      if(state[robo_y[i]][robo_x[i]] == PLAYER) *flag = 1;
      if(state[robo_y[i]][robo_x[i]] == SCRAP){
        *flag = NEXT;
        *score += 1;
      }else{
        board[robo_y[i]][robo_x[i]] = '+';
        state[robo_y[i]][robo_x[i]] = ROBO;
      }
    }
  }
}
void scrap_robot(int board[][LENGTH],int state[][LENGTH],int robo_x[ROBOTS],int robo_y[ROBOTS],int *flag,int *score) //ロボットのスクラップ状態の配置
{
  int i,j;

  for(i=0;i<ROBOTS;i++){
    for(j=i+1;j<ROBOTS;j++){
      if(robo_x[i] == robo_x[j] && robo_y[i] == robo_y[j]){
        if(state[robo_y[i]][robo_x[i]] == ROBO) *score += 2;
        board[robo_y[i]][robo_x[i]] = '!';
        state[robo_y[i]][robo_x[i]] = SCRAP;
        *flag = NEXT;
      }
    }
  }
}

int check(int state[][LENGTH]) //ロボットが全滅したか確認
{
  int i,j;
  int count = 0;

  for(i=0;i<HIGHT;i++){
    for(j=0;j<LENGTH;j++){
      if(state[i][j] == ROBO) count++;
    }
  }
  if(count == 0) return CLEAR;

  return FALSE;
}

void disp(int board[][LENGTH]) //画面表示
{
  int i,j;

  printf("\n");

  for(i=0;i<HIGHT;i++){
    for(j=0;j<LENGTH;j++){
      printf("%c",board[i][j]);
    }
    printf("\n");
  }
}

void game_over(int board[][LENGTH]) //ゲームオーバー表示
{
  printf("GAME OVER\n");
}
