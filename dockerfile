# Use a base image that has Ubuntu or Debian
FROM gcc:14.2.0

RUN wget https://github.com/Kitware/CMake/releases/download/v3.30.2/cmake-3.30.2-linux-x86_64.sh && \
    chmod +x cmake-3.30.2-linux-x86_64.sh && \
    ./cmake-3.30.2-linux-x86_64.sh --skip-license --prefix=/usr/local && \
    rm cmake-3.30.2-linux-x86_64.sh

# Verify the GCC version
RUN g++ --version && cmake --version

# Set up the working directory in the container
WORKDIR /app/build

# Copy the source code into the container
COPY . /app

# Run CMake to configure the project
RUN cmake ..

# Build the project using CMake
RUN cmake --build .

# Specify the command to run the application
CMD ["./my_rest_api"]
