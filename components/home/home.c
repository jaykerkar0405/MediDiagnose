#include "home.h"
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include "../../modules/home/home.h"
#include "../../constants/constants.h"
#include "../../modules/utils/utils.h"
#include "../../modules/utils/hashmap.h"
#include "../../components/exit_screen/exit_screen.h"

void view_profile(User *user)
{
    clear_screen();

    printf(BLUE_COLOR BOLD "    +------------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "View Profile" BLUE_COLOR BOLD "                                         |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +------------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "1. " RESET_COLOR BOLD "ID: %-46s" RESET_COLOR BLUE_COLOR BOLD "|\n" RESET_COLOR, user->id);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "2. " RESET_COLOR BOLD "Name: %-44s" RESET_COLOR BLUE_COLOR BOLD "|\n" RESET_COLOR, user->name);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "2. " RESET_COLOR BOLD "Email: %-43s" RESET_COLOR BLUE_COLOR BOLD "|\n" RESET_COLOR, user->email);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "3. " RESET_COLOR BOLD "Created At: %-38s" RESET_COLOR BLUE_COLOR BOLD "|\n" RESET_COLOR, user->created_at);
    printf(BLUE_COLOR BOLD "    +------------------------------------------------------+\n" RESET_COLOR);

    printf("\n    Press any key to return: ");
    getch();

    clear_screen();
    home_screen(user);
}

void symptom_checker(User *user)
{
    clear_screen();

    int symptom_id, category_id;
    parse_JSON("./assets/symptoms.json");

    display_categories();

    printf("\n    Enter the Category ID to view symptoms: ");
    scanf("%d", &category_id);

    display_symptoms_for_category(category_id);

    printf("\n    Enter the Symptom ID to view related symptoms: ");
    scanf("%d", &symptom_id);

    Symptom *selected_symptoms = display_related_symptoms(symptom_id, category_id);
    display_selected_symptoms(selected_symptoms);

    display_report(user, selected_symptoms);

    clear_screen();
    home_screen(user);
}

void prompt_email_report(User *user, DiagnosisReport *diagnosis_report)
{
    clear_screen();
    bool is_active = true;

    printf("\n    Do you want to receive an email report of your diagnosis?\n");
    printf(GREEN_COLOR BOLD "\n    1. Yes - Send email report" RESET_COLOR);
    printf(RED_COLOR BOLD "\n    2. No - Do not send email report\n" RESET_COLOR);

    while (is_active)
    {
        int choice;
        printf("\n    Please select an option: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            is_active = false;
            bool is_email_report_sent = send_email_report(user, diagnosis_report);

            loading_spinner("Sending your email report, please wait");

            if (is_email_report_sent)
            {
                printf(YELLOW_COLOR "\n    The email report has been sent. Please check your inbox.\n\n" RESET_COLOR);
            }
            else
            {
                printf(YELLOW_COLOR "\n    The email could not be sent. Please try again later.\n\n" RESET_COLOR);
            }

            loading_spinner("Redirecting to the main menu");
            break;
        case 2:
            is_active = false;
            loading_spinner("No email will be sent. Redirecting to the main menu");
            home_screen(user);
            break;
        default:
            printf(YELLOW_COLOR "    Invalid option. Please try again.\n" RESET_COLOR);
            break;
        }
    }

    clear_screen();
    home_screen(user);
}

void home_screen(User *user)
{
    clear_screen();
    bool is_active = true;

    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Welcome %-22s" BLUE_COLOR BOLD "          |\n" RESET_COLOR, user->name);
    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "1. " GREEN_COLOR BOLD "View Profile" RESET_COLOR BLUE_COLOR BOLD "                         |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "2. " YELLOW_COLOR BOLD "Symptom Checker" RESET_COLOR BLUE_COLOR BOLD "                      |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "3. " RED_COLOR BOLD "Exit" RESET_COLOR BLUE_COLOR BOLD "                                 |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +-----------------------------------------+\n" RESET_COLOR);

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
            symptom_checker(user);
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