#include <stdio.h>
#include <stdlib.h>
#include "../../modules/utils/utils.h"
#include "../../constants/constants.h"

// Function to display the splash screen
void exit_screen()
{
    clear_screen();

    printf("\n\n\n\n");
    printf("    %s __  __          _ _ ____  _%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s|  \\/  | ___  __| (_)  _ \\(_) __ _  __ _ _ __   ___  ___  ___ %s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s| |\\/| |/ _ \\/ _` | | | | | |/ _` |/ _` | '_ \\ / _ \\/ __|/ _ \\%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s| |  | |  __/ (_| | | |_| | | (_| | (_| | | | | (_) \\__ \\  __/%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s|_|  |_|\\___|\\__,_|_|____/|_|\\__,_|\\__, |_| |_|\\___/|___/\\___|%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s                                   |___/                      %s\n", GREEN_COLOR, RESET_COLOR);

    loading_animation("Shutting Down");

    clear_screen();
    exit(0);
}
