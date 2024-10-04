#include "home.h"
#include "cJSON.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "../utils/utils.h"
#include "../utils/hashmap.h"
#include "../../constants/secrets.h"
#include "../../constants/constants.h"
#include "../../components/home/home.h"

// Function to convert a list of symptoms into a comma-separated string
void symptoms_to_string(Symptom *head, char *buffer)
{
    buffer[0] = '\0';
    Symptom *current = head;

    while (current != NULL)
    {
        if (strlen(buffer) > 0)
        {
            strcat(buffer, ", ");
        }

        strcat(buffer, current->name);
        current = current->next;
    }
}

// Function to send the email report
bool send_email_report(User *user, DiagnosisReport *diagnosis_report)
{
    CURL *curl;
    Memory chunk;
    CURLcode response;

    chunk.size = 0;
    chunk.memory = malloc(1);

    char *name = user->name;
    char *email = user->email;
    char *date = diagnosis_report->date;
    char *severity = diagnosis_report->severity;
    char *symptoms = diagnosis_report->symptoms;
    char *treatment = diagnosis_report->treatment;
    char *diagnosis = diagnosis_report->diagnosis;
    char *confidence_level = diagnosis_report->confidence_level;

    char post_data[1024];
    snprintf(post_data, sizeof(post_data),
             "{\n"
             "    \"templateId\": 2,\n"
             "    \"to\": [\n"
             "        {\n"
             "            \"name\": \"%s\",\n"
             "            \"email\": \"%s\"\n"
             "        }\n"
             "    ],\n"
             "    \"params\": {\n"
             "        \"name\": \"%s\",\n"
             "        \"severity\": \"%s\",\n"
             "        \"confidence_level\": \"%s\",\n"
             "        \"date\": \"%s\",\n"
             "        \"diagnosis\": \"%s\",\n"
             "        \"symptoms\": \"%s\",\n"
             "        \"treatment\": \"%s\"\n"
             "    }\n"
             "}",
             name, email, name, severity, confidence_level, date, diagnosis, symptoms, treatment);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, BREVO_API_URL);

        headers = curl_slist_append(headers, "api-key: " BREVO_API_KEY);
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        response = curl_easy_perform(curl);

        if (response != CURLE_OK)
        {
            return false;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return true;
}

// Function to parse the diagnosis report and store it
void process_diagnosis_response(cJSON *content_root, DiagnosisReport *diagnosis_report, char *symptoms)
{
    cJSON *severity = cJSON_GetObjectItemCaseSensitive(content_root, "severity");
    cJSON *diagnosis = cJSON_GetObjectItemCaseSensitive(content_root, "diagnosis");
    cJSON *treatment = cJSON_GetObjectItemCaseSensitive(content_root, "treatment");
    cJSON *confidence_level = cJSON_GetObjectItemCaseSensitive(content_root, "confidence_level");

    char date_str[50];
    get_date_string(date_str, sizeof(date_str));

    diagnosis_report->date = strdup(date_str);
    diagnosis_report->symptoms = strdup(symptoms);

    if (severity)
        diagnosis_report->severity = strdup(severity->valuestring);
    if (diagnosis)
        diagnosis_report->diagnosis = strdup(diagnosis->valuestring);
    if (treatment)
        diagnosis_report->treatment = strdup(treatment->valuestring);

    if (confidence_level)
    {
        char confidence_level_str[5];
        sprintf(confidence_level_str, "%d", confidence_level->valueint);
        diagnosis_report->confidence_level = strdup(confidence_level_str);
    }
    else
    {
        diagnosis_report->confidence_level = strdup("N/A");
    }
}

// Function to display the diagnosis report
void display_diagnosis_report(User *user, DiagnosisReport *diagnosis_report)
{
    printf("\n\n\n\n");
    printf("    %s __  __          _ _ ____  _%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s|  \\/  | ___  __| (_)  _ \\(_) __ _  __ _ _ __   ___  ___  ___ %s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s| |\\/| |/ _ \\/ _` | | | | | |/ _` |/ _` | '_ \\ / _ \\/ __|/ _ \\%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s| |  | |  __/ (_| | | |_| | | (_| | (_| | | | | (_) \\__ \\  __/%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s|_|  |_|\\___|\\__,_|_|____/|_|\\__,_|\\__, |_| |_|\\___/|___/\\___|%s\n", GREEN_COLOR, RESET_COLOR);
    printf("    %s                                   |___/                      %s\n", GREEN_COLOR, RESET_COLOR);

    loading_animation("Generating your report, please wait");

    clear_screen();

    printf(BLUE_COLOR BOLD "    +---------------------------------------------------------------------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Diagnosis Report" BLUE_COLOR BOLD "                                                                                              |\n" RESET_COLOR, user->name);
    printf(BLUE_COLOR BOLD "    +---------------------------------------------------------------------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "1. " RESET_COLOR BOLD "Date: %-100s" RESET_COLOR BLUE_COLOR BOLD " |\n" RESET_COLOR, diagnosis_report->date);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "2. " RESET_COLOR BOLD "Severity: %-96s" RESET_COLOR BLUE_COLOR BOLD " |\n" RESET_COLOR, diagnosis_report->severity);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "3. " RESET_COLOR BOLD "Confidence Level: %-88s" RESET_COLOR BLUE_COLOR BOLD " |\n" RESET_COLOR, diagnosis_report->confidence_level);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "4. " RESET_COLOR BOLD "Diagnosis: %-95s" RESET_COLOR BLUE_COLOR BOLD " |\n" RESET_COLOR, diagnosis_report->diagnosis);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "5. " RESET_COLOR BOLD "Treatment: %-95s" RESET_COLOR BLUE_COLOR BOLD " |\n" RESET_COLOR, diagnosis_report->treatment);
    printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "6. " RESET_COLOR BOLD "Symptoms: %-96s" RESET_COLOR BLUE_COLOR BOLD " |\n" RESET_COLOR, diagnosis_report->symptoms);
    printf(BLUE_COLOR BOLD "    +---------------------------------------------------------------------------------------------------------------+\n" RESET_COLOR);

    printf("\n    Press enter key to continue: ");
    get_character();
    clear_input_buffer();

    prompt_email_report(user, diagnosis_report);
    clear_screen();
}

void format_error_to_response(const char *error_string, char **formatted_response)
{
    const char *json_start = strstr(error_string, "{");

    if (json_start == NULL)
    {
        loading_spinner("Internal Server Error. Redirecting to the main menu");
        return;
    }

    size_t json_length = strlen(json_start);

    if (json_start[json_length - 1] != '}')
    {
        json_length++;
    }

    *formatted_response = malloc(json_length + 1);

    if (*formatted_response == NULL)
    {
        loading_spinner("Internal Server Error. Redirecting to the main menu");
        return;
    }

    snprintf(*formatted_response, json_length + 1, "%s", json_start);

    if (json_start[json_length - 1] != '}')
    {
        strcat(*formatted_response, "}");
    }
}

// Function to generate report
void generate_report(User *user, Symptom *selected_symptoms)
{
    clear_screen();

    CURL *curl;
    Memory chunk;
    CURLcode response;

    chunk.size = 0;
    chunk.memory = malloc(1);

    if (chunk.memory == NULL)
    {
        loading_spinner("Memory allocation failed. Redirecting to the main menu");
        return;
    }

    char symptoms[1024];
    symptoms_to_string(selected_symptoms, symptoms);

    DiagnosisReport *diagnosis_report = (DiagnosisReport *)malloc(sizeof(DiagnosisReport));

    if (diagnosis_report == NULL)
    {
        loading_spinner("Memory allocation failed. Redirecting to the main menu");
        free(chunk.memory);
        return;
    }

    char post_data[2048];
    snprintf(post_data, sizeof(post_data),
             "{\n"
             "  \"messages\": [\n"
             "    {\n"
             "      \"role\": \"user\",\n"
             "      \"content\": \"Based on the symptoms provided, return a JSON object containing the following fields:\\n1. \\\"diagnosis\\\": Provide a possible medical diagnosis.\\n2. \\\"treatment\\\": Provide a single string describing the treatment recommendations without any nested fields.\\n3. \\\"severity\\\": Choose one of the following values: \\\"Mild\\\", \\\"Moderate\\\", \\\"Critical\\\".\\n4. \\\"confidence_level\\\": Indicate your confidence in the response as a percentage value between 0 and 100.\\n\\nPlease return only a valid JSON object with no additional text.\\n\\nSymptoms: %s\\n\"\n"
             "    }\n"
             "  ],\n"
             "  \"model\": \"llama3-8b-8192\",\n"
             "  \"temperature\": 0.7,\n"
             "  \"max_tokens\": 1024,\n"
             "  \"top_p\": 0.9,\n"
             "  \"stream\": false,\n"
             "  \"response_format\": {\n"
             "    \"type\": \"json_object\"\n"
             "  },\n"
             "  \"stop\": \"}\\n\"\n"
             "}",
             symptoms);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, GROQ_API_URL);

        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer " GROQ_API_KEY);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        response = curl_easy_perform(curl);

        if (response != CURLE_OK)
        {
            loading_spinner("Internal Server Error. Redirecting to the main menu");
        }
        else
        {
            cJSON *root = cJSON_Parse(chunk.memory);

            if (root == NULL)
            {
                loading_spinner("Failed to parse response. Internal Server Error. Redirecting to the main menu");
                free(chunk.memory);

                curl_slist_free_all(headers);
                curl_easy_cleanup(curl);
                return;
            }

            cJSON *error = cJSON_GetObjectItemCaseSensitive(root, "error");

            if (error)
            {
                cJSON *failed_generation = cJSON_GetObjectItemCaseSensitive(error, "failed_generation");

                if (failed_generation && cJSON_IsString(failed_generation))
                {
                    char *correct_response = NULL;
                    format_error_to_response(failed_generation->valuestring, &correct_response);

                    cJSON *content_root = cJSON_Parse(correct_response);
                    process_diagnosis_response(content_root, diagnosis_report, symptoms);
                }
                else
                {
                    loading_spinner("Unexpected error format. Redirecting to the main menu");
                }
            }
            else
            {
                cJSON *choices = cJSON_GetObjectItemCaseSensitive(root, "choices");

                if (!cJSON_IsArray(choices) || cJSON_GetArraySize(choices) == 0)
                {
                    loading_spinner("No choices found. Internal Server Error. Redirecting to the main menu");
                }
                else
                {
                    cJSON *first_choice = cJSON_GetArrayItem(choices, 0);
                    cJSON *message = cJSON_GetObjectItemCaseSensitive(first_choice, "message");
                    cJSON *content = cJSON_GetObjectItemCaseSensitive(message, "content");

                    cJSON *content_root = cJSON_Parse(content->valuestring);

                    if (content_root == NULL)
                    {
                        loading_spinner("Failed to parse content. Internal Server Error. Redirecting to the main menu");
                    }
                    else
                    {
                        process_diagnosis_response(content_root, diagnosis_report, symptoms);
                        cJSON_Delete(content_root);
                    }
                }
            }

            cJSON_Delete(root);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    free(chunk.memory);

    display_diagnosis_report(user, diagnosis_report);
}
