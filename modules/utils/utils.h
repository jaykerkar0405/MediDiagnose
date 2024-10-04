#ifndef UTILS_H
#define UTILS_H
#include <stddef.h>

// Struct to hold the response data
typedef struct
{
    size_t size;
    char *memory;
} Memory;

// Structure to store the details of user (Logged IN)
typedef struct
{
    char *id;
    char *name;
    char *email;
    char *created_at;
} User;

// Structure to store the details of diagnosis report
typedef struct
{
    char *date;
    char *symptoms;
    char *severity;
    char *treatment;
    char *diagnosis;
    char *confidence_level;
} DiagnosisReport;

// Function to clear the screen
void clear_screen();

// Function to get a character
int get_character();

// Function to pause the execution of the program
void pause_execution(int time);

// Function to clear the input buffer
void clear_input_buffer();

// Function to display the loading spinner
void loading_spinner(const char *label);

// Function to display the loading animation
void loading_animation(const char *label);

// Function to get current date string
void get_date_string(char *date_str, size_t size);

// Function to print data received from the server response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

#endif
