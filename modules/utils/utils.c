#include <time.h>
#include "utils.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include "../../constants/constants.h"

// Function to clear the screen
void clear_screen()
{
    system("cls");
}

// Function to set text color
void set_text_color(int color)
{
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h_console, color);
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
        printf(GREEN_COLOR "%s: " RESET_COLOR, label);
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
        Sleep(100);
        printf("\033[K");
    }
}

// Function to display the loading spinner
void loading_spinner(const char *label)
{
    int i = 0;
    DWORD start_time = GetTickCount();
    const char spinner[] = {'|', '/', '-', '\\'};

    while (1)
    {
        printf("\r    %s %c", label, spinner[i % 4]);

        i++;
        fflush(stdout);
        Sleep(100);

        if (GetTickCount() - start_time >= 5000)
        {
            break;
        }
    }
}
