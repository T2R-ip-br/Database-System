#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

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
// обработка команды пользователя версия 1
void processing_command(char comand[]);
// Обработка команды пользователя версия 2
void processing_command_v2(char comand[]);
// Обработка команды пользователя версия 3
void processing_command_v3(char comand[]);
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

/*---------------------------------------------------------------------------------------------------------------*/

    /*
     * TODO: реализовать команду создания таблиц 
     * TODO: Чтение и формирование собственной структуры при чтении таблиц и их обработки попробовать использовать malloc 
     * предварительно подсчитав сколько может занимать одна запись в таблице в соответствии с типами данных выделить 
     * нужный объём памяти для работы выше указанно функцией, смотри стр 592
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
    processing_command_v3(comand);

    // Очистка буфера
    while(getchar() != '\n') continue;

    printf("\n");
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
// Обработка команды пользователя версия 1

void processing_command(char comand[]) {

    int strl = strlen(comand);
    char ch;
    char word_comand[SIZECOMAND];
    int t = 0;

    for(int i = 0; i < strl; i++) {

        if(isspace(comand[i]) || comand[i] == ';') {
            word_comand[t] = '\0';

            // Проверка чтения команды
            // printf("%s$ \n", word_comand);

            if(strcmp(word_comand, "HELP") == 0 && comand[i] == ';') {
                comand_help();
                break;

            } else if(strcmp(word_comand, "EXIT") == 0) {
                comand_exit();
                break;

            } else if(strcmp(word_comand, "CREATE") == 0) {
                word_comand[t++] = ' ';

            } else if(strcmp(word_comand, "CREATE TABLE") == 0) {
                word_comand[t++] = ' ';

            } else if(strcmp(substring(word_comand, 0, 13), "CREATE TABLE ") == 0 && isalpha(comand[13])) {
                printf("OK:200");

                // TODO: Реализовать захват имени таблицы прочитав строку до символа ';' через цикл while
                // TODO: Реализовать создание таблицы (файла) с указанным именем

                break;

            } else {
                /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                printf("<< Command not found! Check it out: \'help;\'.");
                break;
            }

        } else {
            word_comand[t++] = toupper(comand[i]);
        }

        if(i == strl -1) {
            /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            printf("<< Command not found! Check it out: \'help;\'.");
        }
    }
}

/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ HELP ] отображение справочной информации по командам

void comand_help(void) {
    /* Белый цвет текста консоли*/ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
    printf("<< Information on comands:\n\n");
    //  help - отображает все имеющиеся в системе команды
    printf("\t help; - displays all commands available in the system.\n\n");
    //  exit - завершить работу с системой базы данных.
    printf("\t exit; - exit the database system.\n\n");
    //  create - ключевое слово участвующее в создании базы данных или таблицы.
    printf("\t create * - keyword involved in creating a database or table.\n\n");
    //  create table - создание таблицы в используемой базе данных.
    printf("\t create table *; - creating a table in the database being used.\n\n");
    //  create database - создание базы данных.
    printf("\t create database *; - creating a database.");
}

/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ EXIT ]отвечающая за завершение работы системы

void comand_exit(void) {
    work = 0;
    printf("<< System operation is completed.");
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
// Обработка команды пользователя версия 2

void processing_command_v2(char comand[]) {

    char first_symbol;
    // указатель для строки команды
    char *p_subcomand;
    // указатель на имя объекта (таблицы, базы данных...)
    char *p_name_object;
    // путь к директории хранения таблиц
    char path_to_tables[80] = ".\\tables\\";
    // Объявление указателя на файл
	FILE * fptr;

    to_upper(comand);
    first_symbol = comand[0];

    // printf("%s.\n", comand);

    switch (first_symbol)
    {
        case 'C':
            p_subcomand = substring(comand, 0, 13);

            if (strcmp(p_subcomand, "CREATE TABLE ") == 0 && isalpha(comand[13])) {
                
                p_name_object = substring(comand, 13, (int) ((char *)strchr(comand, ';') - (char *)comand)/sizeof(char) );

                // добавление к пути имени талицы
                strcat(path_to_tables, p_name_object);
                // добавление расширения файла
                strcat(path_to_tables, ".txt");

                printf("%s\n", path_to_tables);

                fptr = fopen(path_to_tables, "w");

                // Проверка что файл удалось открыть
                if(fptr == 0) {
                    printf("Don't can open file!");
                    exit(1);
                }

                fprintf(fptr, "*** Coordinate ***\n");
                
                //Закрываем файл
		        fclose(fptr);
                free(p_subcomand);
                free(p_name_object);
                break;
            } 

            /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            printf("<< Command not found! Check it out: \'help;\'.");
            
            free(p_subcomand);
            break;
            

        case 'E':
            if (strcmp(comand, "EXIT;") == 0) {
                comand_exit();
            } else {
                /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                printf("<< Command not found! Check it out: \'help;\'.");
            }
            break;

        case 'H':
            if (strcmp(comand, "HELP;") == 0) {
                comand_help();
            } else {
                /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                printf("<< Command not found! Check it out: \'help;\'.");
            }
        break;



        default:
            /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            printf("<< Command not found! Check it out: \'help;\'.");
            break;
    }
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
// Обработка команды пользователя версия 3

void processing_command_v3(char comand[]) { 

    char first_char = toupper(comand[0]);
    char *word_comand;
    char path[171] = ".\\data\\";

    int pointer = 0;

    FILE * fp;

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

                    strncat(path, word_comand, 80);

                    if (_mkdir(path) == 0) {
                        
                        strcat(path, "\\");
                        strncat(path, word_comand, 80);
                        strcat(path, ".dat");

                        // Создание служебного файла в базе данных
                        if((fp = fopen(path, "wb")) == NULL) {
                            /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                            printf("<< Failed to create a database.");
                            break;
                        }
                        // сохранение в служебном файле имя базы данных
                        fwrite(word_comand, sizeof(char), strlen(word_comand), fp);
		                fclose(fp);

                    } else {
                        /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                        printf("<< Failed to create a database.");
                    }
                    
                // Создание таблицы
                } else if(strcmp("TABLE", word_comand) == 0) {                    

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
                comand_exit();
            } else {
                command_not_found();
            }
            break;

        case 'H':

            to_upper(comand);

            if (strcmp(comand, "HELP;") == 0) {
                comand_help();
            } else {
                command_not_found();
            }
            break;
        
        case 'U':

            // Чтение первого слова. Преобразование его в прописной вид. Сохранение индекса начала следующего слова команды
            word_comand = substring(comand, 0, before_space(comand, pointer));
            to_upper(word_comand);
            pointer = before_space(comand, pointer) + 1;

            // Проверка первого ключевого слова CREATE
            if(strcmp("USE", word_comand) == 0) { 
                
                word_comand = substring(comand, pointer, before_space(comand, pointer));
                
                strncat(path, word_comand, 80);
                strcat(path, "\\");
                strncat(path, word_comand, 80);
                strcat(path, ".dat");

                /* Проверка на существование базы данных (наличие директории, папки, каталога)
                 * Проверка идёт на наличие служебного файла в базе данных, что докажет существование как самого 
                 * служебного файла, так и базы данных в нужном месте.
                 */
                if((fp = fopen(path, "rb")) == NULL) {
                    /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
                    fprintf(stderr, "The database could not be opened or there is no such database.");
                } else {
                    strncpy(used_database, word_comand, 80);
                }

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
// Возвращает индекс следующего пробела или символа точка с запятой

int before_space(char *comand, int i) {

    while(comand[i] != ' ' && comand[i] != ';')
        i++;

    return i;
}

/*---------------------------------------------------------------------------------------------------------------*/
// Отображение сообщения о том что команда не найдена

void command_not_found(void) {

    /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
    printf("<< Command not found! Check it out: \'help;\'.");
}