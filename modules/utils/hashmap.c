#include "cJSON.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "./utils.h"
#include "../../constants/constants.h"

// Hash table for categories
Category *hash_table[TABLE_SIZE] = {NULL};

// Hash function for indexing
int hash_function(char *key)
{
    int sum = 0;

    for (int i = 0; key[i] != '\0'; i++)
    {
        sum += key[i];
    }

    return sum % TABLE_SIZE;
}

// Function to calculate number of symptoms in a category
int calculate_num_of_symptoms(Category *category)
{
    Symptom *symptoms = category->symptoms;

    if (symptoms == NULL)
        return 0;

    int count = 1;
    Symptom *current = symptoms->next;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

// Function to calculate number of related symptoms in a symptom
int calculate_num_of_related_symptoms(Symptom *symptom)
{
    Symptom *related_symptom = symptom->related_symptom;

    if (related_symptom == NULL)
        return 0;

    int count = 1;
    Symptom *current = related_symptom->next;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

// Function to create a new category node
Category *create_category_node(int id, char *name)
{
    Category *node = malloc(sizeof(Category));

    node->id = id;
    node->next = NULL;
    node->symptoms = NULL;
    strcpy(node->name, name);

    return node;
}

// Function to create a new symptom node
Symptom *create_symptom_node(int id, char *name)
{
    Symptom *node = malloc(sizeof(Symptom));

    node->id = id;
    node->next = NULL;
    strcpy(node->name, name);
    node->related_symptom = NULL;

    return node;
}

// Function to find a category by name
Category *find_category_by_name(char *name)
{
    int index = hash_function(name);
    Category *current = hash_table[index];

    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

// Function to find a category by ID
Category *find_category_by_id(int id)
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Category *current = hash_table[i];

        while (current != NULL)
        {
            if (current->id == id)
            {
                return current;
            }

            current = current->next;
        }
    }

    return NULL;
}

// Function to compare category IDs
int compare_categories_by_id(const void *cat_1, const void *cat_2)
{
    Category *category_1 = *(Category **)cat_1;
    Category *category_2 = *(Category **)cat_2;

    return (category_1->id - category_2->id);
}

// Function to display all categories
void display_categories()
{
    int count = 0;
    Category *categories[TABLE_SIZE];

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        Category *current = hash_table[i];

        while (current != NULL)
        {
            categories[count++] = current;
            current = current->next;
        }
    }

    int midpoint = count / 2 + count % 2;
    qsort(categories, count, sizeof(Category *), compare_categories_by_id);

    printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "List of Categories" BLUE_COLOR BOLD "                                  |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);

    for (int i = 0; i < midpoint; i++)
    {
        if (i != 0)
            printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);

        printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s", categories[i]->id, categories[i]->name);

        if (i + midpoint < count)
        {
            printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s" BLUE_COLOR BOLD "|\n", categories[i + midpoint]->id, categories[i + midpoint]->name);
        }
        else
        {
            printf(BLUE_COLOR BOLD "    | %-22s" BLUE_COLOR BOLD " |\n", " ");
        }
    }

    printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
}

// Function to display symptoms for a selected category
void display_symptoms_for_category(int category_id)
{
    clear_screen();
    Category *category = find_category_by_id(category_id);

    if (category == NULL)
    {
        printf(YELLOW_COLOR "    Category with ID %d not found.\n\n" RESET_COLOR, category_id);

        display_categories();

        printf("\n    Enter the Category ID to view symptoms: ");
        scanf("%d", &category_id);

        display_symptoms_for_category(category_id);
        return;
    }

    printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Symptoms for Category: %-20s" BLUE_COLOR BOLD "         |\n" RESET_COLOR, category->name);

    Symptom *symptom = category->symptoms;

    if (symptom == NULL)
    {
        clear_screen();
        printf(YELLOW_COLOR "    No symptoms found for this category.\n\n" RESET_COLOR);

        display_categories();

        printf("\n    Enter the Category ID to view symptoms: ");
        scanf("%d", &category_id);

        display_symptoms_for_category(category_id);
        return;
    }
    else
    {
        int count = calculate_num_of_symptoms(category);

        Symptom *symptoms[count];
        int midpoint = count / 2 + count % 2;

        for (int i = 0; i < count; i++)
        {
            symptoms[i] = symptom;
            symptom = symptom->next;
        }

        for (int i = 0; i < midpoint; i++)
        {

            printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
            printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s", symptoms[i]->id, symptoms[i]->name);

            if (i + midpoint < count)
            {
                printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s" BLUE_COLOR BOLD "|\n", symptoms[i + midpoint]->id, symptoms[i + midpoint]->name);
            }
            else
            {
                printf(BLUE_COLOR BOLD "    | %-22s" BLUE_COLOR BOLD " |\n", " ");
            }
        }

        printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
    }
}

// Function to display related symptoms for a selected symptom
Symptom *display_related_symptoms(int symptom_id, int category_id)
{
    clear_screen();
    Category *category = find_category_by_id(category_id);
    Symptom *symptom = category->symptoms;

    for (int i = 0; i < symptom_id - 1; i++)
    {

        symptom = symptom->next;

        if (symptom == NULL)
            break;
    }

    if (symptom == NULL)
    {
        clear_screen();
        printf(YELLOW_COLOR "    No related symptoms found for this symptom.\n\n" RESET_COLOR);

        display_symptoms_for_category(category_id);

        printf("\n    Enter the Symptom ID to view related symptoms: ");
        scanf("%d", &symptom_id);

        display_related_symptoms(symptom_id, category_id);
        return NULL;
    }
    else
    {
        printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
        printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Related Symptoms for %-22s" BLUE_COLOR BOLD "         |\n" RESET_COLOR, symptom->name);

        int count = calculate_num_of_related_symptoms(category->symptoms);

        Symptom *related_symptoms[count];
        int midpoint = count / 2 + count % 2;
        Symptom *related_symptom = symptom->related_symptom;

        for (int i = 0; i < count; i++)
        {
            related_symptoms[i] = related_symptom;
            related_symptom = related_symptom->next;
        }

        for (int i = 0; i < midpoint; i++)
        {

            printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);
            printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s", related_symptoms[i]->id, related_symptoms[i]->name);

            if (i + midpoint < count)
            {
                printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s" BLUE_COLOR BOLD "|\n", related_symptoms[i + midpoint]->id, related_symptoms[i + midpoint]->name);
            }
            else
            {
                printf(BLUE_COLOR BOLD "    | %-22s" BLUE_COLOR BOLD " |\n", " ");
            }
        }

        printf(BLUE_COLOR BOLD "    +-----------------------------------------------------+\n" RESET_COLOR);

        int choice, slots_left = count, selected_symptom_id = 1;
        Symptom *selected_symptoms = create_symptom_node(selected_symptom_id++, category->name);

        Symptom *current = selected_symptoms;
        current->next = create_symptom_node(selected_symptom_id++, symptom->name);
        current = current->next;

        while (selected_symptom_id - 3 < count)
        {
            printf("\n    (%d slot%s left). Please enter the id of a related symptom or press '0' to save and continue: ", slots_left--, slots_left == 1 ? "" : "s");
            scanf("%d", &choice);

            if (choice <= 0)
            {
                break;
            }
            else if (choice > count)
            {
                printf(YELLOW_COLOR "    Invalid option. Please try again.\n" RESET_COLOR);
                slots_left++;
            }
            else
            {
                current->next = create_symptom_node(selected_symptom_id++, related_symptoms[choice - 1]->name);
                current = current->next;
            }
        }

        return selected_symptoms;
    }

    return NULL;
}

// Function to display selected symptoms
void display_selected_symptoms(Symptom *selected_symptoms)
{
    clear_screen();
    Symptom *current = selected_symptoms;

    printf(BLUE_COLOR BOLD "    +---------------------------------------+\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    | " RESET_COLOR "Selected Symptoms" BLUE_COLOR BOLD "                     |\n" RESET_COLOR);
    printf(BLUE_COLOR BOLD "    +---------------------------------------+\n" RESET_COLOR);

    while (current != NULL)
    {
        printf(BLUE_COLOR BOLD "    | " YELLOW_COLOR BOLD "%d." RESET_COLOR BOLD " %-20s" BLUE_COLOR BOLD "               |\n", current->id, current->name);
        current = current->next;
    }

    printf(BLUE_COLOR BOLD "    +---------------------------------------+\n" RESET_COLOR);

    printf("\n    Press any key to continue: ");
    getch();
}

// Function to add symptoms to a category
void add_symptoms_to_category(int category_id, Symptom *symptoms)
{
    Category *category = find_category_by_id(category_id);

    if (category == NULL)
    {
        return;
    }

    Symptom *existing_symptoms = category->symptoms;

    if (existing_symptoms == NULL)
    {
        category->symptoms = symptoms;
    }
    else
    {
        Symptom *current = existing_symptoms;

        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = symptoms;
    }
}

// Function to insert a category into the hash table
void insert_category(int id, char *name)
{
    if (find_category_by_name(name) != NULL)
    {
        return;
    }

    int index = hash_function(name);
    Category *node = create_category_node(id, name);

    if (hash_table[index] == NULL)
    {
        hash_table[index] = node;
    }
    else
    {
        Category *prev = NULL;
        Category *current = hash_table[index];

        while (current != NULL && strcmp(current->name, name) < 0)
        {
            prev = current;
            current = current->next;
        }

        if (prev == NULL)
        {
            node->next = hash_table[index];
            hash_table[index] = node;
        }
        else
        {
            node->next = prev->next;
            prev->next = node;
        }
    }
}

// Function to parse JSON and populate the hash table
void parse_JSON(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Unable to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json = malloc(length);
    fread(json, 1, length, file);
    fclose(file);

    cJSON *root = cJSON_Parse(json);

    if (root == NULL)
    {
        printf("Error parsing JSON.\n");
        free(json);
        return;
    }

    cJSON *item = NULL;
    int symptom_id = 1, category_id = 1;

    cJSON_ArrayForEach(item, root)
    {
        cJSON *symptom = cJSON_GetObjectItemCaseSensitive(item, "symptom");
        cJSON *category = cJSON_GetObjectItemCaseSensitive(item, "category");
        cJSON *related_symptoms = cJSON_GetObjectItemCaseSensitive(item, "related_symptoms");

        if (cJSON_IsString(symptom) && (symptom->valuestring != NULL) &&
            cJSON_IsString(category) && (category->valuestring != NULL) &&
            cJSON_IsArray(related_symptoms))
        {
            char category_name[50];
            strcpy(category_name, category->valuestring);

            int category_exists = 0;
            int index = hash_function(category_name);
            Category *category_node = hash_table[index];

            while (category_node != NULL)
            {
                if (strcmp(category_node->name, category_name) == 0)
                {
                    category_exists = 1;
                    break;
                }

                category_node = category_node->next;
            }

            if (!category_exists)
            {
                insert_category(category_id, category_name);
                category_node = hash_table[index];
                category_id++;
            }

            char symptom_name[50];
            strcpy(symptom_name, symptom->valuestring);
            int num_of_symptoms = calculate_num_of_symptoms(category_node);

            Symptom *symptom = create_symptom_node(++num_of_symptoms, symptom_name);

            Symptom *related_symptom_list = NULL;
            Symptom **last = &related_symptom_list;
            int related_symptom_id = 1, num_of_related_symptoms = cJSON_GetArraySize(related_symptoms);

            for (int i = 0; i < num_of_related_symptoms; i++)
            {
                cJSON *symptom_item = cJSON_GetArrayItem(related_symptoms, i);

                if (cJSON_IsString(symptom_item) && (symptom_item->valuestring != NULL))
                {
                    Symptom *new_symptom = create_symptom_node(related_symptom_id++, symptom_item->valuestring);
                    *last = new_symptom;
                    last = &new_symptom->next;
                }
            }

            symptom->related_symptom = related_symptom_list;
            add_symptoms_to_category(category_node->id, symptom);
        }
    }

    free(json);
    cJSON_Delete(root);
}
