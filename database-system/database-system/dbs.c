#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

/* �������� �� ����������� ������ ������� ����������
 * ���� ������������ �� ����� ������� exit work ��������� 1
 * � ������� ������� ��������� ������� */
bool work = 1;

// ������ �������
#define SIZECOMAND 255


// ����������� �����������
void salute(void);
// ������ ������� ������������
void reading_command(void);
// ��������� ������� ������������
void processing_command(char comand[]);
// ��������� ������� ������������ ������ 2
void processing_command_v2(char comand[]);
// �������������� ������� ������������
void formatting_command(char comand[]);
// ������� ����������� ���������� ���������� �� ��������
void comand_help(void);
// ����������� �������
void drawing_table(void);
// ���������� ����� ������ � �������� �������� (�� start ��  end)
char* substring(char str[], int start, int end);
// ��������� ��� ������� � ���������
void to_upper(char* str);

/*---------------------------------------------------------------------------------------------------------------*/

    /*
    * TODO: ����������� ������� ��� ����������� �������� ������ � ��������� ���� � ����������� �� ��� ���������
    */

    /*
    * TODO: ����������� ������� ���� � �������
    */


    /*---------------------------------------------------------------------------------------------------------------*/
    // ������� �������

int main() {

    // ����������� �����������
    salute();

    while (work) {
        // ������, �������������� � ��������� ������� ������������
        reading_command();
    }

    printf("\n");
    return 0;
}
/*---------------------------------------------------------------------------------------------------------------*/
// ����������� �����������

void salute(void) {
    printf("\n");
    printf("    _ _ _     _                      _            _     _       _                              _             \n");
    printf("   | | | |___| |___ ___ _____ ___   | |_ ___    _| | __| |_  __| |_  __ ___ ___    ___ _ _ ___| |_ ___ _____ \n");
    printf("   | | | | -_| |  _| . |     | -_|  |  _| . |  | . ||. |  _||. | . ||. |_ -| -_|  |_ -| | |_ -|  _| -_|     |\n");
    printf("   |_____|___|_|___|___|_|_|_|___|  | | |___|  |___|___| | |___|___|___|___|___|  |___|_  |___| | |___|_|_|_|\n");
    printf("                                    |__|               |__|                           |___|   |__|           \n");
    printf("\n");
    printf("\n");
    /* ������ ���� ������ ������� */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    printf(" Enter the command:\n");
}
/*---------------------------------------------------------------------------------------------------------------*/
// ������, �������������� � ��������� ������� ������������

void reading_command(void) {

    char ch;
    char comand[SIZECOMAND];
    int i = 0;

    /* ������ ���� ������ ������� */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    printf(">> ");

    // ������ �������
    while ((ch = getchar()) != ';') {
        comand[i++] = ch;
    }

    comand[i++] = ';';
    comand[i++] = '\0';

    // �������������� ������� �������
    formatting_command(comand);

    // ��������� ������� �������
    processing_command_v2(comand);

    // ������� ������
    while (getchar() != '\n') continue;

    printf("\n");
}
/*---------------------------------------------------------------------------------------------------------------*/
// �������������� ������� ������������

void formatting_command(char comand[]) {

    char copy_comand[SIZECOMAND];
    strcpy(copy_comand, comand);
    char* p_copy_comand = copy_comand;
    char past_char = comand[0];
    int i = 0;

    // ���������� ��� ������ �������
    while (isspace(*p_copy_comand)) {
        p_copy_comand++;
    }

    // ���������� ��� ������������� �������, ��������� � �������� �����
    while (*p_copy_comand != ';') {

        if (isspace(past_char) && isspace(*p_copy_comand)) {
            p_copy_comand++;
            comand[i - 1] = ' ';

        }
        else if (*p_copy_comand == '\n') {
            if (comand[i - 1] == ' ') {
                p_copy_comand++;

            }
            else {
                comand[i] = ' ';
                past_char = comand[i];
                p_copy_comand++;
                i++;
            }

        }
        else {
            comand[i] = *p_copy_comand;
            past_char = comand[i];
            p_copy_comand++;
            i++;
        }
    }

    // ���������� ������ ����� ������ � �������
    if (isspace(comand[i - 1])) {
        comand[i - 1] = ';';

    }
    else {
        comand[i++] = ';';
    }

    comand[i] = '\0';
}
/*---------------------------------------------------------------------------------------------------------------*/
// ��������� ������� ������������

void processing_command(char comand[]) {

    int strl = strlen(comand);
    char ch;
    char word_comand[SIZECOMAND];
    int t = 0;

    for (int i = 0; i < strl; i++) {

        if (isspace(comand[i]) || comand[i] == ';') {
            word_comand[t] = '\0';

            // �������� ������ �������
            // printf("%s$ \n", word_comand);

            if (strcmp(word_comand, "HELP") == 0 && comand[i] == ';') {
                comand_help();
                break;

            }
            else if (strcmp(word_comand, "EXIT") == 0) {
                comand_exit();
                break;

            }
            else if (strcmp(word_comand, "CREATE") == 0) {
                word_comand[t++] = ' ';

            }
            else if (strcmp(word_comand, "CREATE TABLE") == 0) {
                word_comand[t++] = ' ';

            }
            else if (strcmp(substring(word_comand, 0, 13), "CREATE TABLE ") == 0 && isalpha(comand[13])) {
                printf("OK:200");

                // TODO: ����������� ������ ����� ������� �������� ������ �� ������� ';' ����� ���� while
                // TODO: ����������� �������� ������� (�����) � ��������� ������

                break;

            }
            else {
                /* ����� ���� ������ ������� */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                printf("<< Command not found! Check it out: \'help;\'.");
                break;
            }

        }
        else {
            word_comand[t++] = toupper(comand[i]);
        }

        if (i == strl - 1) {
            /* ����� ���� ������ ������� */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            printf("<< Command not found! Check it out: \'help;\'.");
        }
    }
}
/*---------------------------------------------------------------------------------------------------------------*/
// ������� ����������� ���������� ���������� �� ��������
void comand_help(void) {
    /* ����� ���� ������ �������*/SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    printf("<< Information on comands:\n\n");
    //  help - ���������� ��� ��������� � ������� �������
    printf("\t help; - displays all commands available in the system.\n\n");
    //  exit - ��������� ������ � �������� ���� ������.
    printf("\t exit; - exit the database system.\n\n");
    //  create - �������� ����� ����������� � �������� ���� ������ ��� �������.
    printf("\t create * - keyword involved in creating a database or table.\n\n");
    //  create table - �������� ������� � ������������ ���� ������.
    printf("\t create table *; - creating a table in the database being used.\n\n");
    //  create database - �������� ���� ������.
    printf("\t create database *; - creating a database.");
}
/*---------------------------------------------------------------------------------------------------------------*/
// ������� ���������� �� ���������� ������ �������
void comand_exit(void) {
    work = 0;
    printf("<< System operation is completed.");
    /* ����� ���� ������ ������� */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
/*---------------------------------------------------------------------------------------------------------------*/
// ����������� �������

void drawing_table(void) {

}
/*---------------------------------------------------------------------------------------------------------------*/
// ���������� ����� ������ � �������� �������� (�� start ��  end)

char* substring(char str[], int start, int end) {

    //char copy_string[255];
    char* copy_string;
    copy_string = malloc((end - start + 1) * sizeof(char));
    int i;

    for (i = 0; i < end; i++, start++)
        copy_string[i] = str[start];

    copy_string[i] = '\0';

    return copy_string;
}
/*---------------------------------------------------------------------------------------------------------------*/
// ��������� ������� ������������ ������ 2

void processing_command_v2(char comand[]) {

    char first_symbol;

    to_upper(comand);
    first_symbol = comand[0];

    // printf("%s.\n", comand);

    switch (first_symbol)
    {
    case 'H':
        if (strcmp(comand, "HELP;") == 0) {
            comand_help();
        }
        break;

    case 'E':
        if (strcmp(comand, "EXIT;") == 0) {
            comand_exit();
        }
        break;

    default:
        /* ����� ���� ������ ������� */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
        printf("<< Command not found! Check it out: \'help;\'.");
        break;
    }
}
/*---------------------------------------------------------------------------------------------------------------*/
// ��������� ��� ������� � ���������

void to_upper(char* str) {

    while (*str) {
        *str = toupper(*str);
        str++;
    }
}
