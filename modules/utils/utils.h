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
    char *email;
    char *created_at;
} User;

// Function to clear the screen
void clear_screen();

// Function to set text color
void set_text_color(int color);

// Function to display the loading spinner
void loading_spinner(const char *label);

// Function to extract username from email
char *extract_username(const char *email);

// Function to display the loading animation
void loading_animation(const char *label);

// Function to print data received from the server response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

#endif
