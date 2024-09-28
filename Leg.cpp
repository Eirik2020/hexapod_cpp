#include "Leg.h"






std::vector<point3D> Leg::Gen_path_line(point3D P_start, vector3D V_direction, int res) {
    /*
    Generates a path of 3D points in a line from the starting location along the direction vector.
    Returnes a std::vector. 
    */

    // Normalize the direction vector and calculate the step size per increment
    vector3D V_path_step = V_direction.normalized() * (V_direction.norm() / (res - 1));

    // Generate vector containing line of 3D points
    std::vector<point3D> line;
    for (int i = 0; i < res; i++) {
        line.push_back(P_start + V_path_step * i);
    }
    return line;
}


void Leg::Mod_path(std::vector<point3D> new_path, int i_start) {
    /*
    Modifies the coordinate values stored in the path array in the Leg class. 
    */
    int new_path_size = new_path.size();
    int max_size = path_res;
    int i_end = i_start + new_path_size - 1;
    
    // Check if the input causes the pointer to fall off the array.
    if (i_start >= 0 && i_end < max_size) {
        // If input is valid
        for (int i = 0; i < new_path_size; i++) {
            path_[i_start + i] = new_path[i];
        }
    }

    else if ( i_start < 0){
        throw std::runtime_error("Starting index (i_start) must be greater than zero (i_start>0)!");
    }
    else if (new_path_size > max_size) {
        throw std::runtime_error("Input std::vector too large!");
    }
    else if ( (new_path_size + i_start) > max_size) {
        throw std::runtime_error("Input std::vector size and start index combination causes out of bounds!");
    }
    else {
        throw std::runtime_error("Input failed test, cause unknown.");
    }
}

void Leg::display_path() {
    /*
    Displays the 3D points in the EF's path. 
    */
    for (int i = 0; i < path_res_; ++i) {
        std::cout 
            << "Point " << i + 1 << ":" 
            << " (" 
            << path_[i].x() << ", " 
            << path_[i].y() << ", " 
            << path_[i].z() 
            << ") " << std::endl;
    }
}
