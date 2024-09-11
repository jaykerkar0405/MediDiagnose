# MediDiagnose

MediDiagnose is an AI-powered symptom checker that allows users to select symptoms and receive potential disease diagnoses along with treatment suggestions.

## Features

- AI-driven analysis
- Symptom-based disease diagnosis
- Treatment suggestions based on symptoms

## Prerequisites

Before compiling, ensure you have the following libraries installed:

- [libcurl](__https://curl.se/libcurl/__)
- [cJSON](__https://github.com/DaveGamble/cJSON__)

## Compile Command

```bash
gcc main.c ./components/splash_screen/splash_screen.c ./modules/utils/utils.c ./components/exit_screen/exit_screen.c ./components/authentication/authentication.c ./modules/authentication/authentication.c ./components/home/home.c -o medi_diagnose.exe -I<path_to_libcurl_include> -L<path_to_libcurl_lib> -I<path_to_cjson_include> -L<path_to_cjson_lib> -lcurl -lcjson
```


## Run Command

```bash
./medi_diagnose.exe
```