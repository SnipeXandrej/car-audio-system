#include <fcntl.h>   // For file control options
#include <unistd.h>  // For POSIX APIs
#include <termios.h> // For configuring the serial port
#include <iostream>
#include <cstring>   // For memset
#include <thread>    // For multithreading (optional)
#include <chrono>    // For delays

void writeToSerial(int serialPort, const char *data) {
    // const char *data = "Hello, Arduino!"; // Data to send
    // while (true) {
        ssize_t bytesWritten = write(serialPort, data, strlen(data));
        if (bytesWritten == -1) {
            std::cerr << "Error: Failed to write to serial port.\n";
            // break;
        } else {
            std::cout << "Sent: " << data << "\n";
        }
        // std::this_thread::sleep_for(std::chrono::seconds(2)); // Send every second
    // }
}

void readFromSerial(int serialPort) {
    char buffer[256];            // Buffer for incoming data
    memset(buffer, 0, sizeof(buffer)); // Initialize buffer
    std::string receivedData;    // String to store received lines

    while (true) {
        ssize_t bytesRead = read(serialPort, buffer, sizeof(buffer) - 1); // Read data
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the received data

            // Append to the string and check for complete lines
            receivedData.append(buffer);

            // Process each complete line
            size_t newlinePos;
            while ((newlinePos = receivedData.find('\n')) != std::string::npos) {
                std::string line = receivedData.substr(0, newlinePos);
                receivedData.erase(0, newlinePos + 1); // Remove processed line

                // Remove any trailing '\r' (carriage return)
                if (!line.empty() && line.back() == '\r') {
                    line.pop_back();
                }

                // Print the processed line
                if (!line.empty()) {
                    std::cout << "Received: " << line << "\n";

                    if (line.compare("Hello, Arduino!") == 0) {
                        std::cout << "string comparison true!!!" << "\n\n";
                    }

                    if (line.compare("PC_POWER_OFF") == 0) {
                        std::cout << "Host: Turning off the computer" << "\n\n";
                        // system("poweroff");
                    }
                }
            }
        } else if (bytesRead == -1) {
            std::cerr << "Error: Failed to read from serial port.\n";
            break;
        }

        // Add a short delay to avoid CPU overload
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main() {
    const char *portName = "/dev/ttyUSB0"; // Replace with your serial port

    // Open the serial port
    int serialPort = open(portName, O_RDWR | O_NOCTTY | O_SYNC);
    if (serialPort == -1) {
        std::cerr << "Error: Unable to open port " << portName << "\n";
        return 1;
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Error: Unable to get serial attributes.\n";
        close(serialPort);
        return 1;
    }

    cfsetospeed(&tty, B4800);  // Set baud rate to 9600 (output speed)
    cfsetispeed(&tty, B4800);  // Set baud rate to 9600 (input speed)

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 data bits
    tty.c_iflag &= ~IGNBRK;                     // Ignore break processing
    tty.c_lflag = 0;                            // No signaling chars, no echo
    tty.c_oflag = 0;                            // No remapping, no delays
    tty.c_cc[VMIN] = 1;                         // Read at least 1 character
    tty.c_cc[VTIME] = 1;                        // Timeout in deciseconds

    if (tcsetattr(serialPort, TCSANOW, &tty) != 0) {
        std::cerr << "Error: Unable to set serial attributes.\n";
        close(serialPort);
        return 1;
    }

    // Start writing and reading using threads
    // std::thread writer([&]{
    //     for (int i = 0; i < 5; i++) {
    //         writeToSerial(serialPort, "GetVoltageRemote\n");
    //         std::this_thread::sleep_for(std::chrono::seconds(1)); // Send every second
    //     }
    // });

    std::thread reader(readFromSerial, serialPort);

    // Join threads to main thread
    // writer.join();
    reader.join();

    // Close the serial port
    close(serialPort);
    return 0;
}
