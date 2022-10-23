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

/*---------------------------------------------------------------------------------------------------------------*/

    /*
     * TODO: реализовать команду отображения таблиц в базе данных;
     * TODO: Попробовать заменить strcat на sprintf;
     * TODO: сделать проверку на существование уже таблицы с таким именем при поптке её создания (можно хранить в служебном файле 
     * количество таблиц и список их имён, и при попытке создания таблицы сравнивать имя создаваемой таблицы с уже имеющимися);
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
    printf("%s: Information on comands:\n\n", used_database);
    //  help - отображает все имеющиеся в системе команды
    printf("\t help; - displays all commands available in the system.\n\n");
    //  exit - завершить работу с системой базы данных.
    printf("\t exit; - exit the database system.\n\n");
    //  create - ключевое слово участвующее в создании базы данных или таблицы.
    printf("\t create * - keyword involved in creating a database or table.\n\n");
    //  create database - создание базы данных.
    printf("\t create database *; - creating a database.\n\n");
    //  create table - создание таблицы в используемой базе данных.
    printf("\t create table *; - creating a table in the database being used.\n\n");
    //  use * - использование указанной базы данных.
    printf("\t use *; - using the specified database.\n");
}

/*---------------------------------------------------------------------------------------------------------------*/
// Команда [ EXIT ]отвечающая за завершение работы системы

void comand_exit(void) {
    work = 0;
    print_service_message_white("System operation is completed");
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
    char path[175] = ".\\data\\";

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

                        sprintf(path, ".\\data\\%s\\%s.dat", word_comand, word_comand);
                        
                        // Создание служебного файла в базе данных
                        if((fp = fopen(path, "wb")) == NULL) {
                            print_service_message("Failed to create a database");
                            break;
                        }
                        // сохранение в служебном файле имя базы данных
                        unsigned int number_of_tables = 0; // начальное значение количества таблиц в базе данных
                        fprintf(fp, "name: %s", word_comand);
                        fprintf(fp, "\nnumber_of_tables: ");
                        fwrite(&number_of_tables, sizeof(unsigned int), 1, fp);
                        fprintf(fp, "\ntables:\n");
		                fclose(fp);

                        print_service_message_white("The database has been created");

                    } else {
                        print_service_message("Failed to create a database");
                    }
                    
                // Создание таблицы
                } else if(strcmp("TABLE", word_comand) == 0) {                    

                    // Проверка на то, что в данный момент используется какая-либо база данных
                    if(strcmp(used_database, ":") == 0) {
                        print_service_message("No database selected");
                        break;
                    }
                    
                    word_comand = substring(comand, pointer, before_space(comand, pointer));
                    
                    // Проверка на то, чтобы имя таблицы не совпадало с именем базы данных
                    if(strcmp(used_database, word_comand) == 0) {
                        print_service_message("You cannot create a table with a database name");
                        break;
                    }

                    sprintf(path, ".\\data\\%s\\%s.dat", used_database, word_comand);

                    // Создание файла таблицы в базе данных
                    if((fp = fopen(path, "a")) == NULL) {
                        print_service_message("Failed to create a table");
                        break;
                    }

                    sprintf(path, ".\\data\\%s\\%s.dat", used_database, used_database);
                    if((fp = fopen(path, "r+")) == NULL) {
                        print_service_message("Failed to create a table");
                        break;
                    }

                    // Получение значения количества таблиц в базе данных из служебного файла
                    unsigned int number_of_tables;
                    fseek(fp, (long)(25 + strlen(used_database)), SEEK_SET);
                    fread(&number_of_tables, sizeof(unsigned int), 1, fp);

                    // Увеличение количества таблиц на 1 и запись нового значения в служебный файл
                    number_of_tables++;
                    fseek(fp, (long)(25 + strlen(used_database)), SEEK_SET);
                    fwrite(&number_of_tables, sizeof(unsigned int), 1, fp);

                    // Сохранение имени таблицы в служебном файле
                    char name_table[81];
                    sprintf(name_table, "%s", word_comand);
                    fseek(fp, 0, SEEK_END);
                    fwrite(name_table, sizeof(char), 80, fp);

                    print_service_message_white("The table has been created");

                    fclose(fp);

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

            // Проверка первого ключевого слова USE
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
                    print_service_message("The database could not be opened or there is no such database");
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
    printf("%s: Command not found! Check it out: \'help;\'.\n", used_database);
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
    /* Синий цвет текста консоли */ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    printf("%s: %s.\n", used_database, message);
}