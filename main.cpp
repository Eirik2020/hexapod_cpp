#include "Maestro.h"
#include "TCP_server.h"
#include "Hexapod.h"
#include <iostream>
#include <iomanip>    // For std::fixed and std::setprecision

/*
To-Do:
- Find out why all target values are clamped to 255
*/

int main() {
    try {
        // Initalize Robot
        Hexapod robot;
        vector3D V_D = { 0, 0, 0 };
        vector3D V_T = { 0, 0, 0 };

        // Initalize TCP server
        TCP_server server(12345); // Initialize the server on port 12345
        server.start(); // Start the server to listen for incoming connections


        // ================ MAIN PROGRAM LOOP START ================ //
        bool run_loop = true;
        while (server.isClientConnected() && run_loop) {
            // Receive a character from the client
            char receivedChar = server.receiveMessage();
            // Check if we received a disconnection signal
            if (receivedChar == '\0') {
                std::cout << "Client has disconnected." << std::endl;
                break;
            }


            // Process the received character and perform actions
            switch (receivedChar) {
            case 'w': // Walk forward.
                std::cout << "Received 'w': Moving forward..." << std::endl;
                V_D = { 0, 1, 0 }; // Set direction vector to forward.
                robot.Init_walk(V_D, V_T); // Initalize movement path.

                // Execute move
                robot.Move_dammit(PATH_SIZE_HALF, false);
                robot.Move_dammit(PATH_SIZE_HALF, true);
                break;
            case 's':
                std::cout << "Received 's': Moving backwards..." << std::endl;
                V_D = { 0, -1, 0 }; // Set direction vector to backwards.
                robot.Init_walk(V_D, V_T); // Initalize movement path.

                // Execute move
                robot.Move_dammit(PATH_SIZE_HALF, false);
                robot.Move_dammit(PATH_SIZE_HALF, true);
                break;
            case 'a':
                std::cout << "Received 'a': Moving left..." << std::endl;
                V_D = { 1, 0, 0 }; // Set direction vector to left.
                robot.Init_walk(V_D, V_T); // Initalize movement path.

                // Execute move
                robot.Move_dammit(PATH_SIZE_HALF, false);
                robot.Move_dammit(PATH_SIZE_HALF, true);
                break;
            case 'd':
                std::cout << "Received 'd': Moving right..." << std::endl;
                V_D = { -1, 0, 0 }; // Set direction vector to right.
                robot.Init_walk(V_D, V_T); // Initalize movement path.

                // Execute move
                robot.Move_dammit(PATH_SIZE_HALF, false);
                robot.Move_dammit(PATH_SIZE_HALF, true);
                break;
            case 'e': // Exit Program 
                run_loop = false;
                break;
            case 'n': // Do nothing
                break;
            default: // Something weird happened
                std::cout << "Received unknown command: " << receivedChar << std::endl;
                // Handle other cases or invalid inputs
                break;
            }

            // Idle program unless continuesly updated
            receivedChar = 'n'; 
        } // ================ MAIN PROGRAM LOOP END ================ //



       
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}
