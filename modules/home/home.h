#include <stdbool.h>
#define AUTHENTICATION_H
#include "../../modules/utils/utils.h"
#include "../../modules/utils/hashmap.h"

// Function to display report
void display_report(User *user, Symptom *selected_symptoms);

// Function to send the email report
bool send_email_report(User *user, DiagnosisReport *diagnosis_report);
