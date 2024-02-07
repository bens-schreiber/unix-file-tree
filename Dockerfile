# Use an official C runtime as a parent image
FROM gcc:latest

# Set the working directory in the container to /app
WORKDIR /app

# Add the current directory contents into the container at /app
ADD . /app

# Install CMake
RUN apt-get update && apt-get install -y cmake

# Build the project
RUN cd build && cmake CMakeLists.txt && make all

# Run the output program
CMD ["./build/a"]