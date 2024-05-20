# Use the official Ubuntu base image
FROM ubuntu:latest

# Update package list and install SQLite3
RUN apt-get update && apt-get install -y sqlite3

# Set the default command to run when starting the container
CMD ["sqlite3"]
