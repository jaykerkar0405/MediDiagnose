#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include "../../constants/constants.h"
#include "../../modules/utils/utils.h"
#include "../../components/exit_screen/exit_screen.h"

void view_profile(User *user)
{
    clear_screen();
    int a;

    printf(BLUE_COLOR BOLD "    +------------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "View Profile" BLUE_COLOR BOLD "                                         |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +------------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "1. " BOLD "ID: %s" RESET_COLOR BLUE_COLOR BOLD "\n" RESET_COLOR, user->id);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "2. " BOLD "Username: %s" RESET_COLOR BLUE_COLOR BOLD "\n" RESET_COLOR, extract_username(user->email));
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "2. " BOLD "Email: %s" RESET_COLOR BLUE_COLOR BOLD "\n" RESET_COLOR, user->email);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "3. " BOLD "Created At: %s" RESET_COLOR BLUE_COLOR BOLD "\n" RESET_COLOR, user->created_at);
    printf(BLUE_COLOR BOLD "    +------------------------------------------------------+\n" RESET_COLOR);

    printf("\n    Press any key to return: ");
    getch();

    clear_screen();

    home_screen(user);
}

void home_screen(User *user)
{
    clear_screen();
    bool is_active = true;

    printf(BLUE_COLOR BOLD "    +-----------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Welcome %s" BLUE_COLOR BOLD "\n" RESET_COLOR, extract_username(user->email));
    printf(BLUE_COLOR BOLD "    +-----------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "1. " GREEN_COLOR BOLD "View Profile" RESET_COLOR BLUE_COLOR BOLD "             |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "2. " YELLOW_COLOR BOLD "Symptom Checker" RESET_COLOR BLUE_COLOR BOLD "          |\n" RESET_COLOR);
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
            view_profile(user);
            break;
        case 2:
            is_active = false;
            view_profile(user);
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