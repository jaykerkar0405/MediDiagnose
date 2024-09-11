#include <stdlib.h>
#include "./modules/utils/utils.h"
#include "./components/exit_screen/exit_screen.h"
#include "./components/splash_screen/splash_screen.h"
#include "./components/authentication/authentication.h"

int main()
{
    User *user = (User *)malloc(sizeof(User));

    splash_screen();
    authentication_screen(user);

    return 0;
}
