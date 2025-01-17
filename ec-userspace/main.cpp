#include <fcntl.h>   // For file control options
#include <unistd.h>  // For POSIX APIs
#include <termios.h> // For configuring the serial port
#include <iostream>
#include <cstring>   // For memset
#include <thread>    // For multithreading (optional)
#include <chrono>    // For delays

double voltage_remote;
bool succesful_communication = false;

std::string removeStringWithEqualSignAtTheEnd(const std::string toRemove, std::string str)
{
    size_t pos = str.find(toRemove);
    str.erase(pos, toRemove.length() + 1);

    // cout << str << "\n";
    return str;
}

double getValueFromString(const std::string toRemove, std::string str) {
    double value;

    try {
        value = stoi(removeStringWithEqualSignAtTheEnd(toRemove, str));
    }
    catch (std::invalid_argument const& ex) {
        std::cout << "this did an oopsie: " << ex.what() << '\n';
    }

    return value;
}

void writeToSerial(int serialPort, const char *data) {
        ssize_t bytesWritten = write(serialPort, data, strlen(data));
        if (bytesWritten == -1) {
            std::cerr << "Error: Failed to write to serial port.\n";
        } else {
            std::cout << "Sent: " << data << "\n";
        }
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
                    if (line.compare("YES_I_AM_ALIVE") == 0) {
                        std::cout << "Succesful communication with Atmega8!" << "\n";
                        succesful_communication = true;
                    }

                    if (succesful_communication) {
                        // prints everything received
                        std::cout << "Received: " << line << "\n";

                        if (line.compare("PC_POWER_OFF") == 0) {
                            std::cout << "Host: Turning off the computer" << "\n";
                            system("sudo poweroff");
                        }

                        if (line.rfind("VOLTAGE_REMOTE", 0) == 0) {
                            voltage_remote = getValueFromString("VOLTAGE_REMOTE", line);
                            std::cout << "voltage_remote: " << voltage_remote << "\n";
                        }
                    }
                }
            }
        } else if (bytesRead == -1) {
            std::cerr << "Error: Failed to read from serial port.\n";
            break;
        }

        // Add a short delay to avoid CPU overload
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}


int main() {
    std::cout << "Welcome!" << "\n";
    const char *portName = "/dev/ttyUSB0"; // Replace with your serial port

    // Open the serial port
    int serialPort = open(portName, O_RDWR | O_NOCTTY | O_SYNC);
    while (serialPort == -1) {
        std::cerr << "Error: Unable to open port, retrying " << portName << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        serialPort = open(portName, O_RDWR | O_NOCTTY | O_SYNC);
    }

    // Configure the serial port
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(serialPort, &tty) != 0) {
        std::cerr << "Error: Unable to get serial attributes.\n";
        close(serialPort);
        return 1;
    }

    cfsetospeed(&tty, B4800);  // Set baud rate to 4800 (output speed)
    cfsetispeed(&tty, B4800);  // Set baud rate to 4800 (input speed)

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
    //         writeToSerial(serialPort, "GET_VOLTAGE_REMOTE\n");
    //         std::this_thread::sleep_for(std::chrono::seconds(2)); // Send every second
    // });

    std::thread reader(readFromSerial, serialPort);

    std::thread everythingElseThread([&]{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        while (!succesful_communication) {
            writeToSerial(serialPort, "ARE_YOU_ALIVE\n");

            // hol'up
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    // Join threads to main thread
    // writer.join();
    reader.join();
    everythingElseThread.join();


    // Close the serial port
    close(serialPort);
    return 0;
}
