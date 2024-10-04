#include <time.h>
#include "utils.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../../constants/constants.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>

void restore_buffering(struct termios *oldt)
{
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

void disable_buffering(struct termios *oldt, struct termios *newt)
{
    tcgetattr(STDIN_FILENO, oldt);
    *newt = *oldt;
    newt->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, newt);
}
#endif

// Function to clear the input buffer
void clear_input_buffer()
{
    while (getchar() != '\n')
        ;
}

// Function to clear the screen
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Function to pause the execution of the program
void pause_execution(int time)
{
#ifdef _WIN32
    Sleep(time);
#else
    usleep(time * 1000);
#endif
}

// Function to get a character
int get_character()
{
#ifdef _WIN32
    return getch();
#else
    struct termios oldt, newt;
    disable_buffering(&oldt, &newt);
    int ch = getchar();
    restore_buffering(&oldt);

    return ch;
#endif
}

// Function to get current date string
void get_date_string(char *date_str, size_t size)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(date_str, size, "%B %d, %Y", &tm);
}

// Function to print data received from the server response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t real_size = size * nmemb;
    Memory *memory = (Memory *)userp;
    char *ptr = realloc(memory->memory, memory->size + real_size + 1);

    memory->memory = ptr;
    memcpy(&(memory->memory[memory->size]), contents, real_size);
    memory->size += real_size;
    memory->memory[memory->size] = 0;

    return real_size;
}

// Function to display the loading animation
void loading_animation(const char *label)
{
    const int max_loading = 100;

    for (int i = 0; i <= max_loading; i++)
    {
        printf("\033[H");
        printf(GREEN_COLOR "    %s: " RESET_COLOR, label);
        printf("[");

        for (int j = 0; j < 50; j++)
        {
            if (j < (i / 2))
            {
                printf(BLUE_COLOR "#");
            }
            else
            {
                printf(RESET_COLOR " ");
            }
        }

        printf(RESET_COLOR "] " YELLOW_COLOR "%d%%" RESET_COLOR "\n", i);

        pause_execution(100);
        printf("\033[K");
    }
}

// Function to display the loading spinner
void loading_spinner(const char *label)
{
    int i = 0;
    struct timeval start_time, current_time;

    gettimeofday(&start_time, NULL);
    const char spinner[] = {'|', '/', '-', '\\'};

    while (1)
    {
        printf("\r    %s %c", label, spinner[i % 4]);
        fflush(stdout);
        i++;

        pause_execution(100);

        gettimeofday(&current_time, NULL);
        long elapsed_time = (current_time.tv_sec - start_time.tv_sec) * 1000 +
                            (current_time.tv_usec - start_time.tv_usec) / 1000;

        if (elapsed_time >= 5000)
        {
            break;
        }
    }
}
