#include "Leg.h"






vector3D Leg::Rot_vec_z(const vector3D& vec, double angle) {
    double cos_theta = std::cos(angle);
    double sin_theta = std::sin(angle);

    // Directly calculate the rotated vector components
    double new_x = cos_theta * vec.x() - sin_theta * vec.y();
    double new_y = sin_theta * vec.x() + cos_theta * vec.y();
    double new_z = vec.z();  // Z remains unchanged

    return vector3D(new_x, new_y, new_z);
}

std::vector<point3D> Leg::Gen_path_line(const point3D& P_start, const vector3D& V_direction, int n) {
    /*
    Generates a path of 3D points in a line from the starting location along the direction vector.
    Returns a std::vector.
    */
    // Preallocate the vector to avoid dynamic resizing
    std::vector<point3D> line;
    line.reserve(n);

    // Calculate the normalized step size
    vector3D V_path_step = V_direction / (n - 1);  // No need to normalize and multiply again

    // Generate line of 3D points
    for (int i = 0; i < n; ++i) {
        line.emplace_back(P_start + V_path_step * i);
    }
    return line;
}

std::vector<point3D> Leg::Gen_path_3PA(point3D P_start, vector3D V_direction, double h, int n) {
    /*
    Generates a path of 3D points in a 3 point arch from the starting location along the direction vector.
    Returnes a std::vector. 
    */
    std::vector<point3D> arch;
    arch.reserve(n);

    point3D P_end = P_start + V_direction;
    point3D P_mid = (P_start + P_end) / 2.0;
    P_mid.z() += (2*h);  // Adjust the height of the midpoint

    // Generate the arch points using a parametric quadratic equation
    for (int i = 0; i < n; ++i) {
        double t = static_cast<double>(i) / (n - 1);  // Parameter t ranges from 0 to 1
        double one_minus_t = 1.0 - t;

        point3D point = 
            one_minus_t * one_minus_t * P_start 
            + 2 * one_minus_t * t * P_mid 
            + t * t * P_end;
        arch.push_back(point);  
    }
    return arch;  // Return the vector of points
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
