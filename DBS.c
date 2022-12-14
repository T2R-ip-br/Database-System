#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include "ShellAPI.h"

/* Отвечает за продолжение работы системы базыданных
 * Пока пользователь не введёт команду exit work равняется 1
 * и система ожидает следующей команды */
bool work = 1;

// Размер команды
#define SIZECOMAND 255

// Хранение имени используемой базы данных
static char used_database[81] = ":";

// Отображение приветствия
void salute(void);
// чтение команды пользователя
void reading_command(void);
// обработка команды пользователя
void processing_command(char comand[]);
// Форматирование команды пользователя
void formatting_command(char comand[]);
// Команда отображение справочной информации по командам
void comand_help(void);
// Команда завершения работы
void comand_exit(void);
// Отображение таблицы
void drawing_table(void);
// Возвращает часть строки в заданных пределах (от start до  end)
char * substring(char str[], int start, int end);
// Переводит все символы в прописные
void to_upper(char* str);
// Возвращает индекс следующего пробела или символа точка с запятой
int before_space(char *comand, int i);
// Отображение сообщения о том что команда не найдена
void command_not_found(void);
// Отображение служебного сообщения
void print_service_message(char * message);
// Отображение служебного сообщения белым текстом
void print_service_message_white(char * message);
// Отображение списка таблиц используемой базы данных
void comand_show_tables(void);
// Получить список таблиц используемой базы данных
char ** get_list_tables(void);
// Проверка на то, что в данный момент используется какая-либо база данных
bool checking_database_used(void);
// Получить количество таблиц в базе данных
int get_number_of_tables(void);
// Отображение списка всех баз данных
void comand_show_database(void);
// Команда перехода к работе с указанной базой данных
void comand_use_db(char * db_name);
// Команда удаления базы данных
void comand_drop_db(char * db_name);
// Команда создания базы данных
void comand_create_db(char * db_name);
// Команда создания таблицы
void comand_create_table(char * table_name, char * comand);
// Возвращает количество раз сколько символ встречается в строке
int count_chars(char * str, char ch);

/*---------------------------------------------------------------------------------------------------------------*/

    /* 
     * TODO: Реализовать полноценнй функционал команды [ CREATE TABLE * * ]
     * TODO: Чтение и формирование собственной структуры при чтении таблиц и их обработки попробовать использовать malloc 
     * предварительно подсчитав сколько может занимать одна запись в таблице в соответствии с типами данных выделить 
     * нужный объём памяти для работы выше указанно функцией, смотри стр 592 или использовать united;
     */


/*---------------------------------------------------------------------------------------------------------------*/
// Главная функция

int main() {

    // Отображение приветствия
    salute();

    while(work) {
        // чтение, форматирование и обработка команды пользователя
        reading_command();
    }

    printf("\n");
    return 0;
}

/*---------------------------------------------------------------------------------------------------------------*/
// Отображение приветствия

void salute(void) {
    printf("\n");
    printf("    _ _ _     _                      _            _     _       _                              _             \n");
    printf("   | | | |___| |___ ___ _____ ___   | |_ ___    _| | __| |_  __| |_  __ ___ ___    ___ _ _ ___| |_ ___ _____ \n");
    printf("   | | | | -_| |  _| . |     | -_|  |  _| . |  | . ||. |  _||. | . ||. |_ -| -_|  |_ -| | |_ -|  _| -_|     |\n");
    printf("   |_____|___|_|___|___|_|_|_|___|  | | |___|  |___|___| | |___|___|___|___|___|  |___|_  |___| | |___|_|_|_|\n");
    printf("                                    |__|               |__|                           |___|   |__|           \n");
    printf("\n");
    printf("\n");
    /* Зелёный цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    printf(" Enter the command:\n");
}

/*---------------------------------------------------------------------------------------------------------------*/
// чтение команды пользователя

void reading_command(void) {

    char ch;
    char comand[SIZECOMAND];
    int i = 0;

    /* Зелёный цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
    printf("%s: ", used_database);

    // Чтение команды
    while((ch = getchar()) != ';') {
        comand[i++] = ch;
    }

    comand[i++] = ';';
    comand[i++] = '\0';

    // Форматирование введёной команды
    formatting_command(comand);

    // Обработка введёной команды
    processing_command(comand);

    // Очистка буфера
    while(getchar() != '\n') continue;

    //printf("\n");
}

/*---------------------------------------------------------------------------------------------------------------*/
// Форматирование команды пользователя 

void formatting_command(char comand[]) {

    char copy_comand[SIZECOMAND];
    strcpy(copy_comand, comand);
    char *p_copy_comand = copy_comand;
    char past_char = comand[0];
    int i = 0;

    // Пропускает все первые пробелы
    while (isspace(*p_copy_comand)) {
        p_copy_comand++;
    }

    // Пропускаем все дублированные пробелы, табуляции и переносы строк
    while (*p_copy_comand != ';') {

        if (isspace(past_char) && isspace(*p_copy_comand)) {
            p_copy_comand++;
            comand[i-1] = ' ';

        } else if (*p_copy_comand == '\n') {
            if (comand[i - 1] == ' ') {
                p_copy_comand++;

            } else {
                comand[i] = ' ';
                past_char = comand[i];
                p_copy_comand++;
                i++;
            }

        } else {
            comand[i] = *p_copy_comand;
            past_char = comand[i];
            p_copy_comand++;
            i++;
        }
    }

    // Пропускаем пробел перед точкой с запятой
    if (isspace(comand[i - 1])) {
        comand[i-1] = ';';

    } else {
        comand[i++] = ';';
    }

    comand[i] = '\0';
}

/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ HELP ] отображение справочной информации по командам

void comand_help(void) {
    /* Белый цвет текста консоли*/ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
    printf("\n");
    //  help - отображает все имеющиеся в системе команды
    printf(" %-20s- displays all commands available in the system.\n", "help;");
    //  exit - завершить работу с системой базы данных.
    printf(" %-20s- exit the database system.\n", "exit;");
    //  create - ключевое слово участвующее в создании базы данных или таблицы.
    printf(" %-20s- keyword involved in creating a database or table.\n", "create * *;");
    //  create database %-20s- создание базы данных.
    printf(" %-20s- creating a database.\n", "create database *;");
    //  create table %-20s- создание таблицы в используемой базе данных.
    printf(" %-20s- creating a table in the database being used.\n", "create table *;");
    //  use * - использование указанной базы данных.
    printf(" %-20s- using the specified database.\n", "use *;");
    //  show tables - отображение списка таблиц используемой базы данных.
    printf(" %-20s- displaying a list of tables in the database being used.\n", "show tables;");
    //  show database - отображение списка баз данных.
    printf(" %-20s- displaying a list of databases.\n", "show database;");
    //  drop database * - удаление базы данных.
    printf(" %-20s- deleting a database.\n", "drop database *;");
    printf("\n");
}

/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ EXIT ]отвечающая за завершение работы системы

void comand_exit(void) {
    work = 0;
    print_service_message_white("System operation is completed\n");
    /* Белый цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
}

/*---------------------------------------------------------------------------------------------------------------*/
// Отображение таблицы

void drawing_table(void) {

}

/*---------------------------------------------------------------------------------------------------------------*/
// Возвращает часть строки в заданных пределах (от start до  end)

char * substring(char str[], int start, int end) {

    char * copy_string;
    copy_string = malloc((end - start + 1) * sizeof(char));
    int i;

    for(i = 0; start < end; i++, start++)
        copy_string[i] = str[start];

    copy_string[i] = '\0';

    return copy_string;
}

/*---------------------------------------------------------------------------------------------------------------*/
// Переводит все символы в прописные

void to_upper(char* str) {

    while (*str) {
        *str = toupper(*str);
        str++;
    }
}

/*---------------------------------------------------------------------------------------------------------------*/
// Обработка команды пользователя

void processing_command(char comand[]) { 

    char first_char = toupper(comand[0]);
    char *word_comand;
    int pointer = 0;

    switch (first_char)
    {
        case 'C':
            
            // Чтение первого слова. Преобразование его в прописной вид. Сохранение индекса начала следующего слова команды
            word_comand = substring(comand, 0, before_space(comand, pointer));
            to_upper(word_comand);
            pointer = before_space(comand, pointer) + 1;

            // Проверка первого ключевого слова CREATE
            if(strcmp("CREATE", word_comand) == 0) {
                
                word_comand = substring(comand, pointer, before_space(comand, pointer));
                to_upper(word_comand);
                pointer = before_space(comand, pointer) + 1;


                // Создание базы данных
                if(strcmp("DATABASE", word_comand) == 0) {

                    word_comand = substring(comand, pointer, before_space(comand, pointer));
                    // Команда создания базы данных
                    comand_create_db(word_comand);
                    
                // Создание таблицы
                } else if(strcmp("TABLE", word_comand) == 0) {                    
                    
                    int end_space = before_space(comand, pointer);
                    int end;
                    char * p_begin_bracket = strchr(comand, '(');
                    if(p_begin_bracket != 0) {
                        end = p_begin_bracket - comand; 
                        end = (end < end_space) ? end : end_space; // определение что первое встречается после имени таблицы, пробел или кругла скобка, предотвращение сохранения описания таблицы в имя таблицы
                    } else {
                        print_service_message("Error in the syntax of the description of table properties");
                        break;
                    }

                    word_comand = substring(comand, pointer, end);
                    // Команда создания таблицы
                    comand_create_table(word_comand, comand);

                } else {
                    command_not_found();
                }

            } else {
                command_not_found();
            }

            break;

        case 'D':

            // Чтение первого слова. Преобразование его в прописной вид. Сохранение индекса начала следующего слова команды
            word_comand = substring(comand, 0, before_space(comand, pointer));
            to_upper(word_comand);
            pointer = before_space(comand, pointer) + 1;

            // Проверка первого ключевого слова CREATE
            if(strcmp("DROP", word_comand) == 0) {

                word_comand = substring(comand, pointer, before_space(comand, pointer));
                to_upper(word_comand);
                pointer = before_space(comand, pointer) + 1;

                // Удаление базы данных
                if(strcmp("DATABASE", word_comand) == 0) {
                    
                    word_comand = substring(comand, pointer, before_space(comand, pointer));
                    // Команда удаления базы данных
                    comand_drop_db(word_comand);

                } else {
                    command_not_found();
                }

            } else {
                command_not_found();
            }

            break;

        case 'E':

            to_upper(comand);

            if (strcmp(comand, "EXIT;") == 0) {
                // Команда завершения работы
                comand_exit();
            } else {
                command_not_found();
            }
            break;

        case 'H':

            to_upper(comand);

            if (strcmp(comand, "HELP;") == 0) {
                // Команда отображение справочной информации по командам
                comand_help();
            } else {
                command_not_found();
            }
            break;
        
        case 'S':

            to_upper(comand);

            if (strcmp(comand, "SHOW DATABASE;") == 0) {
                // Отображение списка всех баз данных
                comand_show_database();

            } else if (strcmp(comand, "SHOW TABLES;") == 0) {
                // Отображение списка таблиц используемой базы данных
                comand_show_tables();
            } else {
                command_not_found();
            }
            break;

        case 'U':

            // Чтение первого слова. Преобразование его в прописной вид. Сохранение индекса начала следующего слова команды
            word_comand = substring(comand, 0, before_space(comand, pointer));
            to_upper(word_comand);
            pointer = before_space(comand, pointer) + 1;

            // Проверка первого ключевого слова USE
            if(strcmp("USE", word_comand) == 0) { 
                
                word_comand = substring(comand, pointer, before_space(comand, pointer));
                // Команда перехода к работе с указанной базой данных
                comand_use_db(word_comand);

            } else {
                command_not_found();
            }
            break;
        
        default:
            command_not_found();
            break;
    }
}

/*---------------------------------------------------------------------------------------------------------------*/
// Возвращает индекс следующего пробела (поиск начинается с указанного индекса) или символа точка с запятой

int before_space(char *comand, int i) {

    while(comand[i] != ' ' && comand[i] != ';')
        i++;

    return i;
}

/*---------------------------------------------------------------------------------------------------------------*/
// Отображение сообщения о том что команда не найдена

void command_not_found(void) {
    /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    printf("%s: Command not found! Check it out: help;\n", used_database);
}

/*---------------------------------------------------------------------------------------------------------------*/
// Отображение служебного сообщения

void print_service_message(char * message) {
    /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    printf("%s: %s.\n", used_database, message);
}
/*---------------------------------------------------------------------------------------------------------------*/
// Отображение служебного сообщения белым текстом

void print_service_message_white(char * message) {
    /* Белый цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    printf("%s: %s", used_database, message);
}
/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ SHOW TABLES ] отображение списка таблиц используемой базы данных

void comand_show_tables(void) {

    char (* list_tables)[81];

    // Проверка на то, что в данный момент используется какая-либо база данных
    if(checking_database_used()) {
        return;
    }

    int number_of_tables = get_number_of_tables();

    list_tables = get_list_tables();
    
    if(number_of_tables == 0) {
        print_service_message_white("There are no tables in the database\n");

    } else if(number_of_tables == -1) {
        print_service_message("Error");

    } else {
        /* Белый цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        printf("\n");
        for(int i = 0; i < number_of_tables; i++) {
            printf(" %s\n", list_tables[i]);
        }
        printf("\n");
    }
    free(list_tables);
}
/*---------------------------------------------------------------------------------------------------------------*/
// Получить список таблиц используемой базы данных

char ** get_list_tables(void) {

    char path[175];
    char (* list_tables)[81];
    FILE *fp;

    // Открытие служебного файла
    sprintf(path, ".\\data\\%s\\%s.dat", used_database, used_database);
    if((fp = fopen(path, "r+")) == NULL) {
        print_service_message("Failed to create a table");

    } else {

        // Получение значения количества таблиц в базе данных из служебного файла
        unsigned int number_of_tables;
        fseek(fp, (long)(25 + strlen(used_database)), SEEK_SET);
        fread(&number_of_tables, sizeof(unsigned int), 1, fp);

        // Получение списка всех таблиц в базе данных
        list_tables = (char (*)[81] ) malloc(sizeof(char [81]) * number_of_tables);
        fseek(fp, (long)(34 + strlen(used_database) + sizeof(unsigned int)), SEEK_SET);
        for(int i = 0; i < number_of_tables; i++) {
            fread(list_tables[i], sizeof(char [80]), 1, fp);
        }
    }
    fclose(fp);
    return list_tables;
}
/*---------------------------------------------------------------------------------------------------------------*/
// Проверка на то, что в данный момент используется какая-либо база данных

bool checking_database_used(void) {
    if(strcmp(used_database, ":") == 0) {
        print_service_message("No database selected");
        return 1;
    } else
        return 0;
}
/*---------------------------------------------------------------------------------------------------------------*/
// Получить количество таблиц в базе данных

int get_number_of_tables(void) {
    
    char path[175];
    FILE *fp;

    // Открытие служебного файла
    sprintf(path, ".\\data\\%s\\%s.dat", used_database, used_database);
    if((fp = fopen(path, "r+")) == NULL) {
        print_service_message("Failed to create a table");
        fclose(fp);
        return -1;

    } else {

        // Получение значения количества таблиц в базе данных из служебного файла
        unsigned int number_of_tables;
        fseek(fp, (long)(25 + strlen(used_database)), SEEK_SET);
        fread(&number_of_tables, sizeof(unsigned int), 1, fp);
        fclose(fp);
        return (int) number_of_tables;
    }

}
/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ SHOW DATABASE ] отображение списка баз данных

void comand_show_database(void) {

    // Выполнение команды в командной строке по отображению списка содержимого папки .\data и сохранение результата в файл .\data\info.txt
    ShellExecute(NULL, "open", "cmd.exe", "/C dir /d .\\data > .\\data\\info.txt", NULL,SW_SHOWNORMAL);
    
    // Открытие файла .\data\info.txt и отображение всех папок являющихся базами данных
    char path[175];
    sprintf(path, ".\\data\\info.txt");

    FILE * fp = NULL;

    while(fp == NULL) {
        fp = fopen(path, "r");
        Sleep(1);
    }

    /* Белый цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    fseek(fp, 0, SEEK_SET);
    char ch;
    char name_database[81];
    bool confirmation = 0;
    int i = 0;
    printf("\n");
    while((ch = getc(fp)) != EOF) {
        
        if(confirmation && ch != ']') {
            name_database[i] = ch;
            i++;
        }

        if(ch == '[') {
            confirmation = 1;
        } else if(ch == ']') {
            confirmation = 0;
            name_database[i] = '\0';
            i = 0;
            
            if(strcmp(name_database, ".") == 0 || strcmp(name_database, "..") == 0) {
                // . и .. это служебные файлы в любой папке имеются по умолчанию
            } else {
                printf(" %s\n", name_database);
            }
        }
    }
    printf("\n");

    fclose(fp);
    if(remove(path) != 0) {
        print_service_message("Error");
    }
 }
/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ SHOW DATABASE ] перехода к работе с указанной базой данных

void comand_use_db(char * db_name) {

    FILE * fp;
    char path[175];
    sprintf(path, ".\\data\\%s\\%s.dat", db_name, db_name);

    /* Проверка на существование базы данных (наличие директории, папки, каталога)
     * Проверка идёт на наличие служебного файла в базе данных, что докажет существование как самого 
     * служебного файла, так и базы данных в нужном месте. */
    if((fp = fopen(path, "rb")) == NULL) {
        print_service_message("The database could not be opened or there is no such database");
    } else {
        strncpy(used_database, db_name, 80);
    }
    fclose(fp);
}
/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ DROP DATABASE * ] удаления базы данных

void comand_drop_db(char * db_name) {

    int ch;
    char path[175];
    while ((ch = getchar()) != '\n' && ch != EOF) { }

    print_service_message_white("Are you sure [ Y(yes) / N(no) ]? ");
    ch = getchar();
    ch = toupper(ch);
    if(ch == 'Y') {
        
        sprintf(path, "/C RD /S /Q .\\data\\%s", db_name);
        // Выполнение команды в командной строке по отображению списка содержимого папки .\data и сохранение результата в файл .\data\info.txt
        ShellExecute(NULL, "open", "cmd.exe", path, NULL, SW_SHOWNORMAL);
    }
}
/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ CREATE DATABASE * ] создания базы данных

void comand_create_db(char * db_name) {
    
    char path[175];
    FILE * fp;

    sprintf(path, ".\\data\\%s", db_name);

    if (_mkdir(path) == 0) {

        sprintf(path, ".\\data\\%s\\%s.dat", db_name, db_name);
        
        // Создание служебного файла в базе данных
        if((fp = fopen(path, "wb")) == NULL) {
            print_service_message("Failed to create a database");
            return;
        }
        // сохранение в служебном файле имя базы данных
        unsigned int number_of_tables = 0; // начальное значение количества таблиц в базе данных
        fprintf(fp, "name: %s", db_name);
        fprintf(fp, "\nnumber_of_tables: ");
        fwrite(&number_of_tables, sizeof(unsigned int), 1, fp);
        fprintf(fp, "\ntables:\n");
        fclose(fp);

        print_service_message_white("The database has been created\n");

    } else {
        print_service_message("Failed to create a database");
    }
}
/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ CREATE TABLE * ] создания таблицы

void comand_create_table(char * table_name, char * comand) {

    char path[175];
    FILE * fp;

    // Проверка на то, что в данный момент используется какая-либо база данных
    if(checking_database_used())
        return;
    
    // Проверка на то, чтобы имя таблицы не совпадало с именем базы данных
    if(strcmp(used_database, table_name) == 0) {
        print_service_message("You cannot create a table with a database name");
        return;
    }

    // Открытие служебного файла
    sprintf(path, ".\\data\\%s\\%s.dat", used_database, used_database);
    if((fp = fopen(path, "r+")) == NULL) {
        print_service_message("Failed to create a table");
        return;
    }

    // Получение значения количества таблиц в базе данных из служебного файла
    unsigned int number_of_tables;
    fseek(fp, (long)(25 + strlen(used_database)), SEEK_SET);
    fread(&number_of_tables, sizeof(unsigned int), 1, fp);

    // Получение списка всех таблиц в базе данных и проверка имени на оригинальность
    char (* list_tables)[81];
    list_tables = get_list_tables(); 
    for(int i = 0; i < number_of_tables; i++) {
        // Проверка на оригинальность имени создаваемой таблицы
        if(strcmp(list_tables[i], table_name) == 0) {
            print_service_message("A table with this name already exists");
            return;
        }
    }
    free(list_tables);

    // Увеличение количества таблиц на 1 и запись нового значения в служебный файл
    number_of_tables++;
    fseek(fp, (long)(25 + strlen(used_database)), SEEK_SET);
    fwrite(&number_of_tables, sizeof(unsigned int), 1, fp);

    // Сохранение имени таблицы в служебном файле
    char name_table[81];
    sprintf(name_table, "%s", table_name);
    fseek(fp, 0, SEEK_END);
    fwrite(name_table, sizeof(char), 80, fp);

    // Закрытие служебного файла
    fclose(fp);

    sprintf(path, ".\\data\\%s\\%s.dat", used_database, table_name);

    // Создание файла таблицы в базе данных
    if((fp = fopen(path, "a")) == NULL) {
        print_service_message("Failed to create a table");
        return;
    }

    // Создание служебной записи, содержащей имена столбцов и их тип данных
    // Получаем указатель на открывающуюся скобку
    char * p_begin_bracket = strchr(comand, '(');
    char * p_end_bracket   = strchr(comand, ')');
    int begin = p_begin_bracket - comand + 1;
    int end   = p_end_bracket - comand;

    // Ошибка синтаксиса описания таблицы если 
    if(p_begin_bracket == 0 || p_end_bracket == 0 || (end - begin) < 4) {
        print_service_message("Error in the syntax of the description of table properties");
        return;
    }

    // Избавляемся от возможного пробела между скобкой и первым словом 
    if(comand[begin] == ' ') {
        begin++;
    }

    // Описание таблицы (...)
    char *table_description = substring(comand, begin, end);
    // Длина описания
    int description_length  = strlen(table_description);
    // Строка имени столбцов
    char * column_name;
    // Строка параметров столбца
    char column_param[81];

    //printf("%s, %d\n", table_description, description_length);
    /*// Чтение первого слова. Преобразование его в прописной вид. Сохранение индекса начала следующего слова команды
            
            word_comand = substring(comand, 0, before_space(comand, pointer));
            to_upper(word_comand);
            pointer = before_space(comand, pointer) + 1; 
    */

    // Получаем количество параметров в описании создаваемой таблицы, каждый параметр должен отделяться запятой
    int number_of_parameters = 1 + count_chars(table_description, ',');

    /* Если проверка на синтаксис и ликвидность имён типов данных пройдёт успешно, то можно сохранить исходную строку описания столбцов таблицы в файле */
    int counter, pointer, index;
    counter = pointer = index = 0;
    for(int i = 0; i < number_of_parameters; i++) {
        pointer = before_space(table_description, pointer);
        column_name = substring(table_description, counter, pointer);
        pointer++;

        while(table_description[pointer] != ',' && table_description[pointer] != '\0') {
            column_param[index] = table_description[pointer];
            pointer++;
            index++;
        }

        /* Произвести проверку на ликвидность типа данных создаваемой колонки */

        index = 0;
        pointer+=2;
        counter = pointer;
    }

    print_service_message_white("The table has been created\n");

    // закрытие файла таблицы
    fclose(fp);
}
/*---------------------------------------------------------------------------------------------------------------*/
// Возвращает количество раз сколько символ встречается в строке

int count_chars(char * str, char ch) {

    int count = 0;
    int length = strlen(str);

    for(int i = 0; i < length; i++) {
        if(str[i] == ch)
            count++;
    }

    return count;
}


















