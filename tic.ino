#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif


#define YP A3  
#define XM A2  
#define YM 9   
#define XP 8   
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define PURPLE  0xA01F
#define SIZE 3
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

int count = 0;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
char player = 'X';
char comp = 'O';
char board[SIZE][SIZE] = {{' ', ' ', ' '},
                          {' ', ' ', ' '},
                          {' ', ' ', ' '}};

void setup() 
{
  Serial.begin(9600);
  Serial.println(F("Tic tac toe !"));
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
      board[i][j] = ' ';
    }
  }
  loadingScreen();
  displayBoard();
  ticTacToe();
}

void loadingScreen()
{
  tft.fillScreen(BLACK);
  tft.drawRect(5,5,235,315,MAGENTA);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(60,50);
  tft.setRotation(3);
  tft.print("TIC-TAC-TOE");
  tft.setCursor(130,90);
  tft.setTextSize(2);
  tft.setTextColor(CYAN);
  tft.println("Choose");
  tft.fillRoundRect(110, 190,100, 20,3, WHITE);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(130,193);
  tft.print("START");
  tft.setCursor(120,130);
  tft.setTextSize(4);
  tft.setTextColor(MAGENTA);
  tft.print("X  O");
  tft.drawRect(110,120,40,45,WHITE);
  //ft.drawRect(183,120,40,45,WHITE);
  while(true)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    if (p.z <= ts.pressureThreshhold) 
    {
      continue;
    }
    if (p.x > 40 && p.x < 60 && p.y > 90 && p.y < 155)
    {      
      break;
    }
    if(p.x > 100 && p.x < 150 && p.y > 90 && p.y < 110)
    {
      tft.drawRect(110,120,40,45,WHITE);
      tft.drawRect(183,120,40,45,BLACK);
      player = 'X';
      comp = 'O';
    }
    if(p.x > 110 && p.x < 155 && p.y > 145 && p.y < 165)
    {
      tft.drawRect(183,120,40,45,WHITE);
      tft.drawRect(110,120,40,45,BLACK);
      player = 'O';
      comp = 'X';
    }    
  } // End of while loop
} // End of loadingScreen

void displayBoard()
{
  tft.fillScreen(BLACK);  
  tft.drawRect(5, 5, 315, 235, MAGENTA);
  tft.drawRect(20, 14, 190, 212, WHITE);
  tft.drawFastVLine(85, 15, 210, WHITE);
  tft.drawFastVLine(145,15, 210, WHITE);
  tft.drawFastHLine(20, 85, 190, WHITE);
  tft.drawFastHLine(20, 155, 190, WHITE);

  tft.setTextColor(WHITE);
  tft.setTextSize(5);
}

void draw()
{
  tft.setTextSize(5);
  tft.setTextColor(CYAN);
  tft.setCursor(250,30);
  tft.print("D");
  tft.setCursor(250,80);
  tft.print("R");
  tft.setCursor(250,130);
  tft.print("A");
  tft.setCursor(250,180);
  tft.print("W");
}

void youwin()
{
  tft.setTextSize(3);
  tft.setTextColor(CYAN);
  tft.setCursor(235,40);
  tft.print("YOU");
  tft.setTextSize(5);
  tft.setCursor(250,80);
  tft.print("W");
  tft.setCursor(250,130);
  tft.print("I");
  tft.setCursor(250,180);
  tft.print("N");
}

void youlose()
{
  tft.setTextSize(3);
  tft.setTextColor(CYAN);
  tft.setCursor(235,35);
  tft.print("YOU");
  tft.setTextSize(4);
  tft.setCursor(250,65);
  tft.print("L");
  tft.setCursor(250,105);
  tft.print("O");
  tft.setCursor(250,145);
  tft.print("S");
  tft.setCursor(250,185);
  tft.print("E");
}

void printXorO(int r, int c, char ch)
{
  int x = 0;
  int y = 0;
  if (r == 0)
  {
    y = 35;
  }
  else if (r == 1)
  {
    y = 105;
  }
  else
  {
    y = 175;
  }

  if (c == 0)
  {
    x = 40;
  }
  else if (c == 1)
  {
    x = 105;
  }
  else
  {
    x = 165;
  }
  tft.setCursor(x,y);
  tft.print(ch);
}

int isWinner(char player) 
{
    int i = 0;

    // Check rows and columns
    for (; i < SIZE; i++) 
    {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) 
        {
            return 1; // Player wins
        }
    }

    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) 
    {
        return 1; // Player wins
    }

    return 0; // No winner yet
}
int isBoardFull() 
{
    int i = 0, j = 0;
    for (; i < SIZE; i++) 
    {
        for (j = 0; j < SIZE; j++) 
        {
            if (board[i][j] == ' ') 
            {
                return 0; // Board is not full
            }
        }
    }
    return 1; // Board is full
}


// Function to evaluate the score of the current board
int evaluate() 
{
    if (isWinner(comp)) 
    {
        return 10; // Computer wins
    } 
    else if (isWinner(player)) 
    {
        return -10; // Player wins
    } 
    else 
    {
        return 0; // It's a draw
    }
}

// Function to find the best move using the Min-Max algorithm
void findBestMove() 
{
    int bestVal = -1000;
    int bestMoveRow = -1;
    int bestMoveCol = -1;
    int i = 0, j = 0, moveVal = 0;

    // Traverse all cells
    for (; i < SIZE; i++) 
    {
        for (j = 0; j < SIZE; j++) 
        {
            // Check if cell is empty
            if (board[i][j] == ' ') 
            {
                // Make the move
                board[i][j] = comp;
                
                
				        if (evaluate() > 0)
				        {
							printXorO(i,j,comp);
					        return;
				        }
				
                // Compute the evaluation function for this move
                moveVal = minimax(0);

                // Undo the move
                board[i][j] = ' ';

                // Update the best move if needed
                if (moveVal > bestVal) 
                {
                    bestMoveRow = i;
                    bestMoveCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    printXorO(bestMoveRow,bestMoveCol,comp);
    board[bestMoveRow][bestMoveCol] = comp;
}

// Min-Max function to find the best move
int minimax(int isMax) 
{
    int score = evaluate();
    int minimaxScore = 0;
    int i = 0, j = 0, best;

    count++;
    // If the game is over, return the score
    if (score != 0) 
    {
        return score;
    }

    // If the board is full, it's a draw
    if (isBoardFull()) 
    {
        return 0;
    }

    // If it's the computer's turn
    if (isMax) 
    {
        best = -1000;

        // Traverse all cells
        for (i = 0; i < SIZE; i++) 
        {
            for (j = 0; j < SIZE; j++) 
            {
                // Check if cell is empty
                if (board[i][j] == ' ') 
                {
                    // Make the move
                    board[i][j] = comp;

                    minimaxScore = minimax(!isMax);

                    // Recur and get the maximum score
                    best = max(best, minimaxScore);

                    // Undo the move
                    board[i][j] = ' ';

                    if (minimaxScore > 0)
                        return best;                    
                }
            }
        }
        return best;
    }
    // If it's the player's turn
    else 
    {
        best = 1000;

        // Traverse all cells
        for (i = 0; i < SIZE; i++) 
        {
            for (j = 0; j < SIZE; j++) 
            {
                // Check if cell is empty
                if (board[i][j] == ' ') 
                {
                    // Make the move
                    board[i][j] = player;

                    minimaxScore = minimax(!isMax);                   

                    // Recur and get the minimum score
                    best = min(best, minimaxScore);

                    // Undo the move
                    board[i][j] = ' ';

                    if (minimaxScore < 0)
                        return best;
                }
            }
        }
        return best;
    }
}
void ticTacToe()
{
  int pmr = -1;	//player move row
  int pmc = -1;	//player move column

  while(true)
  {
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);

    if (p.z <= ts.pressureThreshhold)
    {
      continue;
    }
    
    if (p.x > 30 && p.x < 95 && p.y > 25 && p.y < 60)
    {
      pmr = 2;
      pmc = 0;
    }
    else if (p.x > 120 && p.x < 200 && p.y > 25 && p.y < 60)
    {
      pmr = 1;
      pmc = 0;
    }
    else if (p.x > 220 && p.x < 290 && p.y > 25 && p.y < 60)
    {
      pmr = 0;
      pmc = 0;
    }
    else if (p.x > 30 && p.x < 95 && p.y > 70 && p.y < 100)
    {
      pmr = 2;
      pmc = 1;
    }
    else if (p.x > 120 && p.x < 200 && p.y > 70 && p.y < 100)
    {
      pmr = 1;
      pmc = 1;
    }
    else if (p.x > 220 && p.x < 290 && p.y > 70 && p.y < 100)
    {
      pmr = 0;
      pmc = 1;
    }
    else if (p.x > 30 && p.x < 95 && p.y > 110 && p.y < 150)
    {
      pmr = 2;
      pmc = 2;
    }
    else if (p.x > 120 && p.x < 200 && p.y > 110 && p.y < 150)
    {
      pmr = 1;
      pmc = 2;
    }
    else if (p.x > 220 && p.x < 290 && p.y > 110 && p.y < 150)
    {
      pmr = 0;
      pmc = 2;
    }        
    else
    {
      pmr = -1;
      pmc = -1; 
      continue;     
    }
    if (board[pmr][pmc] == ' ')
    {
      board[pmr][pmc] = player;
      printXorO(pmr,pmc,player);
    }
    else
    {
      continue;
    }
    if (isWinner(player)) 
    {
      youwin();  //player win
      break;
    }
    if (isBoardFull()) 
    {
      draw();    //draw
      break;
    }
    findBestMove();
    if (isWinner(comp)) 
    {
      youlose();    //comp win
      break;
    }
    if (isBoardFull()) 
    {
      draw();    //draw
      break;
    }
  } // End of while loop  
} // End of ticTacToe function

void loop()
{

}