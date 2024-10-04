#include <cJSON.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "../utils/utils.h"
#include "../../constants/secrets.h"
#include "../../constants/constants.h"

// Function to hash a password using FNV-1a
uint32_t hash_password(const char *password)
{
    uint32_t hash = 2166136261u;

    while (*password)
    {
        hash ^= (uint8_t)*password++;
        hash *= 16777619u;
    }

    return hash;
}

// Function to convert hash to hexadecimal string
void hash_to_hex(uint32_t hash, char *hex_str)
{
    snprintf(hex_str, 9, "%08x", hash);
}

// Function to authenticate the user
bool authenticate_user(const char *email, const char *password, User *user)
{
    CURL *curl;
    Memory chunk;
    CURLcode response;

    chunk.size = 0;
    chunk.memory = malloc(1);

    char url[256];
    snprintf(url, sizeof(url), "%s" FETCH_USER_ENDPOINT, SUPABASE_API_URL, email);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, url);

        headers = curl_slist_append(headers, "apikey: " SUPABASE_API_KEY);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        response = curl_easy_perform(curl);

        if (response != CURLE_OK)
        {
            return false;
        }
        else
        {
            cJSON *json = cJSON_Parse(chunk.memory);

            if (json == NULL)
            {
                curl_slist_free_all(headers);
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return false;
            }

            if (cJSON_IsArray(json))
            {
                cJSON *object = cJSON_GetArrayItem(json, 0);

                if (object != NULL)
                {
                    cJSON *id = cJSON_GetObjectItem(object, "id");
                    cJSON *name = cJSON_GetObjectItem(object, "name");
                    cJSON *created_at = cJSON_GetObjectItem(object, "created_at");
                    cJSON *fetched_password = cJSON_GetObjectItem(object, "password");

                    if (cJSON_IsString(fetched_password))
                    {
                        if (strcmp((const char *)password, (const char *)fetched_password->valuestring) == 0)
                        {
                            user->id = id->valuestring;
                            user->email = (char *)email;
                            user->name = name->valuestring;
                            user->created_at = created_at->valuestring;

                            return true;
                        }
                    }
                }
            }

            cJSON_Delete(json);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return false;
}

// Function to register the user
bool register_user(const char *name, const char *email, const char *password, User *user)
{
    CURL *curl;
    Memory chunk;
    CURLcode response;

    chunk.size = 0;
    chunk.memory = malloc(1);

    char url[256];
    snprintf(url, sizeof(url), "%s" CREATE_USER_ENDPOINT, SUPABASE_API_URL);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        char post_fields[512];
        struct curl_slist *headers = NULL;

        snprintf(post_fields, sizeof(post_fields),
                 "{\"password\": \"%s\", \"email\": \"%s\", \"name\": \"%s\"}",
                 password, email, name);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        headers = curl_slist_append(headers, "apikey: " SUPABASE_API_KEY);
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        response = curl_easy_perform(curl);

        if (response != CURLE_OK)
        {
            return false;
        }

        user->id = "Not Assigned";
        user->name = (char *)name;
        user->email = (char *)email;
        user->created_at = "To Be Set";

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    else
    {
        return false;
    }

    curl_global_cleanup();
    return true;
}
