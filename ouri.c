#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
#include "ouri.h"


int board[2][7];
int jogada = 0;
bool endGame = false;



int main(int argc, char *argv[])
{
    for(int i = 0; i < argc; i++) {
        printf("Paramtero %d: %s\n", i, argv[i]);
    }


    if(argc > 1)
    {
        read_File(argv[1]);
    }
    else {
        resetBoard();
    }
    while(!endGame)
    {
        print_board();
        playerMove(jogada % 2);
        CheckWinners(jogada % 2);
        endGame = IsEndOfGame();
        jogada++;
        
    }
}

void print_board()
{
    printf("jogada\n");
    printf("|---------   B   ---------|\n");
    printf("|---| %d| %d| %d| %d| %d| %d|---|\n", 6, 5, 4 ,3, 2, 1);
    printf("|-------------------------|\n");
    printf("|---|--|--|--|--|--|--|---|\n");
    printf("|   |");
    for(int i = 6; i > 0; i--) {
        printf(" %d|", board[1][i]);
    }
    printf("   |\n");
    printf("|  %d|-----------------|  %d|\n",board[1][0],board[0][0]);
    printf("|   |");
    for(int i = 1; i < 7; i++) {
        printf(" %d|", board[0][i]);
    }
    printf("   |\n");
    printf("|---|--|--|--|--|--|--|---|\n");
    printf("|-------------------------|\n");
    printf("|---| %d| %d| %d| %d| %d| %d|---|\n",1 , 2, 3, 4, 5,6);
    printf("|---------   A   ---------|\n");

}

void resetBoard()
{
    printf("Reseting board...");
    board[0][0] =  0;
    board[1][0] =  0;
    for(int i = 0; i < 2;i++)
    {
        for(int j = 1;j < 7;j++)
        {
            board[i][j] = 4;
        }
    }
}

void playerMove(int player)
{
    int move_pos;
    int current_board = player;
    printf("Jogador %c: Escolhe onde queres tirar as bolas (1-6): ", player + 'A');
    scanf("%d",&move_pos);

    if(move_pos == 0)
    {
        save_File(jogada % 2);
        exit(0);
    }
    int peaces = board[current_board][move_pos];
    bool valid = Valid_Play(move_pos,peaces);


    if(valid == 1)
    {
        if(peaces != 0)
        {
            board[current_board][move_pos] = 0;
            while(peaces > 0)
            {
                if(move_pos == 6)
                {
                    if(current_board == 0)
                    {
                        current_board = 1;
                    }
                    else{
                        current_board = 0;
                    }
                    move_pos = 1;
                    CheckWinners();
                }
                else
                {
                    move_pos++;
                }
                board[current_board][move_pos]++;
                peaces--;
            }
            if(current_board != player)
            {
                if(board[current_board][move_pos] == 2 || board[current_board][move_pos] == 3)
                {
                    for(int i = move_pos; i > 0; i--)
                    {
                        if(board[current_board][i] == 2 || board[current_board][i] == 3){
                            //printf("i = %d -> numero de peças -> %d\n",i,board[current_board][i]);
                            board[player][0] += board[current_board][i];
                            //printf("numero de paças do jogador: %d\n",board[player][0]);
                            board[current_board][i] = 0;
                        }
                        else{
                            break;
                        }
                    }
                }
            }
        }
    }
    else
    {
        jogada--;
        printf("Jogada invalida\n");
    }

    
}

bool IsEndOfGame()
{
    int a = 0;

    for(int i = 0; i < 2;i++)
    {
        for(int j = 1;j < 7;j++)
        {
            a += board[i][j];
        }
    }
    printf("%d\n",a);
    return a == 0;
}

void CheckWinners(int player)
{
    int soma;
    int current_player = player; 
    if(board[0][0] >= 25)
    {
        printf("Jogador A ganhou\n");
        endGame = true;
    }
    else if(board[1][0] >= 25)
    {
        printf("jogador B ganhou\n");
        endGame = true;
    }   
}

bool Valid_Play(int move_pos, int peaces)
{
    bool valid;
    if(move_pos > 0 && move_pos < 7)
    {
        if(peaces > 0)
        {
            valid = true;
        }
        else{
            valid = false;
        }
    }else{
        valid = false;
    }
    printf("valid -> %d\n",valid);
    return valid;
}

void save_File(int player)
{
    char data[1000];
    FILE * fPtr;

    printf("Em qual ficheiro queres guardar: ");
    scanf("%s",data);
    printf("A escrever no ficheiro %s\n",data);

    fPtr = fopen(data, "w+");

    if(fPtr == NULL)
    {
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    sprintf(data,"%d\n",board[1][0]);
    fputs(data, fPtr);

    for(int i = 6; i > 0; i--)
    {
        sprintf(data,"%d ", board[1][i]);
        fputs(data,fPtr);
    }

    sprintf(data,"\n%d\n",board[0][0]);
    fputs(data, fPtr);

    for(int i = 1; i < 7; i++) {
        sprintf(data,"%d ", board[0][i]);
        fputs(data,fPtr);
    }
    //perguntar ao stor
    //sprintf(data,"\n%d", player);
    //fputs(data,fPtr);

    fclose(fPtr);
}

void read_File(char * file_name)
{
    FILE * fPtr;
    char *data;
    size_t len = 0;
    ssize_t read = 0;
    int i = 0;
    int player = 1;

    fPtr = fopen(file_name, "r");

    if(fPtr == NULL)
    {
        printf("Unable to read file.\n");
        exit(EXIT_FAILURE);
    }
    printf("reading file %s\n", file_name);
    char c;
    bool new_number = true;
    do 
    {
        c = (char)fgetc(fPtr);
        int c_int = c - '0';

        if(c_int >= 0 && c_int <= 9)
        {
            new_number = false;
            printf("=> %d player: %d i: %i\n", c_int, player, i);

            if(i == 7)
            {
                player = 0;
                i = 0;
            }
            if(player == 1 && i != 0)  {
                board[player][7 - i] = board[player][7 - i] * 10 + c_int;
            }
            else {
                board[player][i] = board[player][i] * 10 + c_int;
            }
        }
        else if(!new_number) {
            i++;
            new_number = true;
        }
    } while(c != EOF);

    printf("------------------------------\n");
    fclose(fPtr);
}