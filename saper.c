#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_STR 100
#define MAX_COL 100
#define MINE_SYM '*'
#define SHOT_SYM '.'
#define FIELD_SYM 'o'

int get_game_settings(int *str, int *col, int *mines);
void get_rand_mines_coord(int mines_arr[MAX_STR][2], int mines, int str, int col);
void set_mines(int matrix[MAX_STR][MAX_COL], int str, int col, int mines_arr[MAX_STR][2], int mines);
void init_field(int matrix[MAX_STR][MAX_COL], int str, int col);
void get_user_coord(int *i, int *j);
int check_pos(int matrix[MAX_STR][MAX_COL], int str, int col, int user_i, int user_j);
void update_matrix(int matrix[MAX_STR][MAX_COL], int str, int col, int user_i, int user_j);
int check_neighbours(int matrix[MAX_STR][MAX_COL], int str, int col, int user_i, int user_j);

int main()
{
    // int str = -1, col = -1, mines = -1;
    int str = 10, col = 10, mines = 5; // DEBUG
    get_game_settings(&str, &col, &mines);

    int matrix[MAX_STR][MAX_COL] = {0};

    int mines_arr[MAX_STR][2] = {0};
    get_rand_mines_coord(mines_arr, mines, str, col);
    set_mines(matrix, str, col, mines_arr, mines);

    init_field(matrix, str, col);

    int user_i = -1, user_j = -1;
    get_user_coord(&user_i, &user_j);

    while (!check_pos(matrix, str, col, user_i, user_j))
    {
        get_user_coord(&user_i, &user_j);
    }

    // todo логика обезвреживания
    // todo логика выигрыша
    // todo счетчик
    // todo сброс поля
    // todo выход

    return 0;
}

int get_game_settings(int *str, int *col, int *mines)
{
    printf("Enter mode:\nb - begginer\na - amateur\np - pro\ns - special\n");

    unsigned char mode;

    while (scanf("%c", &mode) == 1 && (mode != 'b' && mode != 'a' && mode != 'p' && mode != 's')) // проверить условие!!!!!!!!!!!!!!!!!!!!!!!
    {

        if (mode == 'b')
        {
            *str = 9, *col = 9;
            *mines = 10;
        }
        else if (mode == 'a')
        {
            *str = 16, *col = 16;
            *mines = 40;
        }
        else if (mode == 'p')
        {
            *str = 16, *col = 30;
            *mines = 99;
        }
        else if (mode == 's')
        {
            printf("Set str col (example: 5 5)"); // todo убрать дублирование?????
            scanf("%d %d", str, col);
            while (*str <= 0 || *col <= 0 || *str >= MAX_STR || *col >= MAX_COL)
            {
                printf("Set str col (example: 5 5)");
                scanf("%d %d", str, col);
            }

            printf("Set mines count"); // todo убрать дублирование?????
            scanf("%d", mines);
            while (*mines <= 0 || *mines > *str * *col - 1)
            {
                printf("Set mines count");
                scanf("%d", mines);
            }
        }
        else
        {
            printf("Wrong mode\n");
            printf("Enter mode:\nb - begginer\na - amateur\np - pro\ns - special\n");
        }
    }
}

void get_rand_mines_coord(int mines_arr[MAX_STR][2], int mines, int str, int col)
{

    srand(time(NULL));
    int mine_i = 0, mine_j = 0;
    for (int i = 0; i < mines; ++i)
    {
        mine_i = rand() % str;
        mine_j = rand() % col;
        mines_arr[i][0] = mine_i;
        mines_arr[i][1] = mine_j;

        printf("%d %d\n", mine_i, mine_j); // DEBUG
        for (int x = 0; x < i; ++x)
        {
            if (mines_arr[x][0] == mine_i && mines_arr[x][1] == mine_j)
            {
                i--;
                continue;
            }
        }
    }
}

void set_mines(int matrix[MAX_STR][MAX_COL], int str, int col, int mines_arr[MAX_STR][2], int mines)
{
    for (int i = 0; i < mines; ++i)
    {
        int matr_i = mines_arr[i][0];
        int matr_j = mines_arr[i][1];
        matrix[matr_i][matr_j] = -1;
    }
}

void init_field(int matrix[MAX_STR][MAX_COL], int str, int col)
{
    for (int i = 0; i < str; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            printf("%c ", FIELD_SYM);
        }
        printf("\n");
    }
}

void get_user_coord(int *i, int *j)
{
    //    while (*i < 0 || *j < 0 || *i >= MAX_STR || *j >= MAX_COL)       //зацикливание
    {
        printf("Enter coord(example: 2 3)");
        scanf("%d %d", i, j);
    }
}

int check_pos(int matrix[MAX_STR][MAX_COL], int str, int col, int user_i, int user_j)
{
    if (user_i >= str || user_i < 0 || user_j >= col || user_j < 0)
    {
        printf("Out of bounds coordinates\n");
        return 0;
    }
    if (matrix[user_i][user_j] == -1)
    {
        printf("GAME OVER\n");
        for (int i = 0; i < str; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                if (matrix[i][j] == -1)
                    printf("%c ", MINE_SYM);
                else if (matrix[i][j] == -2)
                    printf("%c ", SHOT_SYM);
                else if (matrix[i][j] > 0)
                    printf("%d ", matrix[i][j]);
                else
                    printf("%c ", FIELD_SYM);
            }
            printf("\n");
        }
        return 1;
    }
    else
    {
        update_matrix(matrix, str, col, user_i, user_j);
        return 0;
    }
}

void update_matrix(int matrix[MAX_STR][MAX_COL], int str, int col, int user_i, int user_j)
{
    check_neighbours(matrix, str, col, user_i, user_j);
    for (int i = 0; i < str; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            if (matrix[i][j] == -2)
                printf("%c ", SHOT_SYM);
            else if (matrix[i][j] > 0)
                printf("%d ", matrix[i][j]);
            else
                printf("%c ", FIELD_SYM);
        }
        printf("\n");
    }
}

int check_neighbours(int matrix[MAX_STR][MAX_COL], int str, int col, int user_i, int user_j)
{
    // for (int i = 0; i < str; ++i)
    // {
    //     for (int j = 0; j < col; ++j)
    //     {
    //         if (matrix[i][j] == 0)
    //         {
    //             for (int di = -1; i <= 1; ++i)
    //             {
    //                 for (int dj = 1; j <= 1; ++j)
    //                 {
    //                     int ai = i + di;
    //                     int aj = j + dj;
    //                     if (ai >= 0 && ai <= str && aj >= 0 && ai <= col && matrix[i][j] == -1)
    //                         matrix[i][j] += 1;
    //                 }
    //             }
    //         }
    //     }
    // }
    int count = 0;
    printf("%d %d\n", user_i, user_j); // DEBUG
    for (int i = user_i - 1; i >= 0 && i < str && i <= user_i + 1; ++i)
    {
        for (int j = user_j - 1; j >= 0 && j < col && j <= user_j + 1; ++j)
        {
            if (matrix[i][j] == -1)
            {
                printf("%d %d %d\n", i, j, matrix[i][j]); // DEBUG
                count++;
            }
        }
    }
    matrix[user_i][user_j] += count;
    count = 0;
    if (matrix[user_i][user_j] == 0)
    {
        matrix[user_i][user_j] = -2;
        printf("pass\n");                                                   // DEBUG
        for (int i = user_i - 1; i >= 0 && i < str && i <= user_i + 1; ++i) // todo не работает для краевых значений а-ля 0 0
        {
            for (int j = user_j - 1; j >= 0 && j < col && j <= user_j + 1; ++j)
            {
                check_neighbours(matrix, str, col, i, j);
            }
        }
    }
}
