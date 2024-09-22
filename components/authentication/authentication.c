#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include "../home/home.h"
#include "../../constants/constants.h"
#include "../../modules/utils/utils.h"
#include "../../components/exit_screen/exit_screen.h"
#include "../../modules/authentication/authentication.h"

void login_screen(bool is_error, User *user)
{
    clear_screen();

    if (is_error)
        printf(YELLOW_COLOR "    Login failed: Invalid credentials.\n\n" RESET_COLOR);

    int i;
    char email[255];
    char password[255];

    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Login" BLUE_COLOR BOLD "                                   |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);

    printf(BLUE_COLOR BOLD "    | " GREEN_COLOR " Email: ");
    scanf("%s", email);

    printf(BLUE_COLOR BOLD "    | " GREEN_COLOR " Password: ");

    for (i = 0; i < 255 - 1; i++)
    {
        char ch = _getch();

        if (ch == '\r' || ch == '\n')
        {
            break;
        }
        if (ch == '\b')
        {
            if (i > 0)
            {
                i -= 2;
                printf("\b \b");
            }
        }
        else
        {
            printf("*");
            password[i] = ch;
        }
    }

    password[i] = '\0';
    printf(BLUE_COLOR BOLD "\n    +-----------------------------------------+\n\n" RESET_COLOR);

    char hex_str[9];
    uint32_t hash = hash_password(password);
    hash_to_hex(hash, hex_str);

    bool login_successful = authenticate_user(email, hex_str, user);

    loading_spinner("Authenticating your account");

    clear_screen();

    if (login_successful)
    {
        home_screen(user);
    }
    else
    {
        login_screen(true, user);
    }
}

void registration_screen(bool is_error, User *user)
{
    clear_screen();

    if (is_error)
        printf(YELLOW_COLOR "    Registration failed: Please try again.\n\n" RESET_COLOR);

    int i;
    char name[255];
    char email[255];
    char password[255];

    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Register" BLUE_COLOR BOLD "                                |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);

    printf(BLUE_COLOR BOLD "    | " GREEN_COLOR " Email: ");
    scanf("%s", email);

    getchar();

    printf(BLUE_COLOR BOLD "    | " GREEN_COLOR " Name: ");
    fgets(name, sizeof(name), stdin);

    if ((strlen(name) > 0) && (name[strlen(name) - 1] == '\n'))
    {
        name[strlen(name) - 1] = '\0';
    }

    printf(BLUE_COLOR BOLD "    | " GREEN_COLOR " Password: ");

    for (i = 0; i < 255 - 1; i++)
    {
        char ch = _getch();

        if (ch == '\r' || ch == '\n')
        {
            break;
        }
        if (ch == '\b')
        {
            if (i > 0)
            {
                i -= 2;
                printf("\b \b");
            }
        }
        else
        {
            printf("*");
            password[i] = ch;
        }
    }

    password[i] = '\0';
    printf(BLUE_COLOR BOLD "\n    +-----------------------------------------+\n\n" RESET_COLOR);

    char hex_str[9];
    uint32_t hash = hash_password(password);
    hash_to_hex(hash, hex_str);

    bool registration_successful = register_user(name, email, hex_str, user);

    loading_spinner("Creating your account, please wait");

    clear_screen();

    if (registration_successful)
    {
        home_screen(user);
    }
    else
    {
        registration_screen(true, user);
    }
}

void authentication_screen(User *user)
{
    clear_screen();
    bool is_active = true;

    printf(BLUE_COLOR BOLD "    +-----------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Welcome to MediDiagnose" BLUE_COLOR BOLD "     |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +-----------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "1. " GREEN_COLOR BOLD "Login" RESET_COLOR BLUE_COLOR BOLD "                    |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "2. " YELLOW_COLOR BOLD "Register" RESET_COLOR BLUE_COLOR BOLD "                 |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "3. " RED_COLOR BOLD "Exit" RESET_COLOR BLUE_COLOR BOLD "                     |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +-----------------------------+\n" RESET_COLOR);

    while (is_active)
    {
        int choice;
        printf("\n    Please select an option: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            is_active = false;
            login_screen(false, user);
            break;
        case 2:
            is_active = false;
            registration_screen(false, user);
            break;
        case 3:
            is_active = false;
            exit_screen();
            break;
        default:
            printf(YELLOW_COLOR "    Invalid option. Please try again.\n" RESET_COLOR);
            break;
        }
    }

    clear_screen();
}