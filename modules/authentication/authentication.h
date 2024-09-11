#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H
#include "../utils/utils.h"

// Function to hash a password using FNV-1a
uint32_t hash_password(const char *password);

// Function to convert hashed password to hexadecimal string
void hash_to_hex(uint32_t hash, char *hex_str);

// Function to register the user
bool register_user(const char *email, const char *password, User *user);

// Function to authenticate the user
bool authenticate_user(const char *email, const char *password, User *user);

#endif