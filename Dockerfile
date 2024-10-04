# Use the latest GCC image as the base
FROM gcc:latest

# Set the working directory
WORKDIR /app

# Install required libraries (libcurl, libcjson)
RUN mkdir -p /app && cd /app && apt-get update && apt-get install -y libcurl4-openssl-dev libcjson-dev && apt-get clean

# Set the TERM environment variable
ENV TERM=xterm

# Copy the entire source code directory to the container
COPY . .

# Compile the application with necessary source files and library links
RUN gcc main.c components/splash_screen/splash_screen.c components/exit_screen/exit_screen.c components/authentication/authentication.c components/home/home.c modules/authentication/authentication.c modules/home/home.c modules/utils/utils.c modules/utils/hashmap.c -I/usr/include/curl -I/usr/include/cjson -L/usr/lib/x86_64-linux-gnu -lcurl -lcjson -o MediDiagnose

# Set the default command to execute the compiled application
CMD ["./MediDiagnose"]
