#pragma once
#include <iostream>
#include <Eigen/Dense>  
#include <vector>
#include <stdexcept> 
#include <cmath>
#include <cstdbool>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <string>
#include <boost/system/error_code.hpp>

// Data type aliases
typedef Eigen::Vector3d point3D; //[x,y,z] Stores 3D points 
typedef Eigen::Vector3d vector3D; //(x,y,z) Stores 3D vectors


// Math constants
const double PI = 3.14; // Define pi

// Communication configuration
const std::string COM_PORT = "COM3";
const unsigned int BAUD_RATE = 115200;

// Robot configuration
extern point3D ORIGIN_BOT;
const int n_LEGS = 6;


// Leg configuration
const point3D P_DEF(125.00, 0.00, -65);
const int n_LINK = 4;
const double LINK[n_LINK] = { 72.75,  54.00, 71.00, 73.50 };
const double H = 30;
const double MOVE_MAG = 60;
const int OFFSET[3] = { 0, 0, 128 };
const int INVERT_SERVO[3] = {0, 0, 0};

// Leg path resolution
const int SET_PATH_SIZE = 60; // Configure the number of points in the movment path of the leg. 


// Calculate resultant values
const double LEG_SPACING = (2*PI) / n_LEGS; // Find leg spacing in radiance.
const int PATH_SIZE_HALF = SET_PATH_SIZE / 2; // Find half size of full move.
const int PATH_SIZE_QUART = PATH_SIZE_HALF / 2; // Find quarter size of full move.
const int PATH_SIZE = PATH_SIZE_QUART * 4; // Ensure full move is double that to avoid missmatch.


