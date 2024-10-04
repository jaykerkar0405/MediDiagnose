![Banner](https://res.cloudinary.com/bytewise0405/image/upload/v1726917049/MediDiagnose/banner_lhvhjm.png)

## Badges

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://github.com/jaykerkar0405/MediDiagnose/blob/main/LICENSE)

# 🩺 MediDiagnose

MediDiagnose is an AI-powered symptom checker that helps users receive potential disease diagnoses and treatment suggestions based on selected symptoms.

## ✨ Features

- 🤖 AI-driven analysis for precise symptom interpretation
- 💊 Personalized treatment suggestions based on matched conditions
- 🧾 Symptom-based disease detection powered by advanced algorithms

## 🛠️ Prerequisites

Before compiling and running the application, ensure you have the following prerequisites:

- **libcurl**
- **cJSON**
- **GCC compiler**

## ⚙️ Compilation

Compile MediDiagnose with the following command:

```bash
gcc main.c ./components/splash_screen/splash_screen.c ./components/exit_screen/exit_screen.c ./components/authentication/authentication.c ./components/home/home.c ./modules/authentication/authentication.c ./modules/home/home.c ./modules/utils/utils.c ./modules/utils/hashmap.c -I<path_to_libcurl_include> -I<path_to_cjson_include> -L<path_to_libcurl_lib> -L<path_to_cjson_lib> -lcurl -lcjson -o MediDiagnose
```

## ▶️ Running the Application

Once compiled, run MediDiagnose with:

```bash
./MediDiagnose
```

## 🐋 Docker

You can pull the MediDiagnose Docker image with the following command:

```bash
docker pull bytewise0405/medi-diagnose:latest
```

## 🎥 Demo Video

Check out our demo video to see MediDiagnose in action:

[MediDiagnose Demonstration](https://res.cloudinary.com/bytewise0405/video/upload/v1726921150/MediDiagnose/MediDiagnose_Demonstration_ehwqsx.mp4)

## Authors

- Afnan Kazi | [@Afnankazi](https://github.com/Afnankazi)
- Jay Kerkar | [@jaykerkar0405](https://github.com/jaykerkar0405)
- Chaitanya Kelkar | [@CHAITANYASKELKAR](https://github.com/CHAITANYASKELKAR)

## 🍴 How to Contribute

Contributions are welcome! Follow these steps:

1. Fork this repository by clicking the "Fork" button on the top right of the repository page.
2. Clone your forked repository:
   ```bash
   git clone https://github.com/jaykerkar0405/MediDiagnose.git
   ```
3. Make your changes and test them thoroughly.
4. Submit a pull request with a description of your changes.

## 🛠️ Tech Stack

- **Libraries**: libcurl, cJSON
- **Programming Language**: C
- **Frameworks**: Custom AI logic, Data Structures

## 📚 References

- [AI Documentation](https://research.facebook.com/file/1574548786327032/LLaMA--Open-and-Efficient-Foundation-Language-Models.pdf)
- [libcurl Documentation](https://curl.se/libcurl/)
- [cJSON Documentation](https://github.com/DaveGamble/cJSON)

## Feedback

If you have any feedback, please reach out to us at bytewise0405@gmail.com
