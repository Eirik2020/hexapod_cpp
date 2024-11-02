#include "Leg.h"


// Constructor
Leg::Leg(int id, point3D origin) : 
    id(id),
    origin_(origin),
    P_EF_(P_DEF)
{}


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

void Leg::Mod_path(const std::vector<point3D>& new_path, int i_start) {
    /*
    Modifies the coordinate values stored in the path array in the Leg class.
    */
    size_t new_path_size = new_path.size();
    size_t max_size = PATH_SIZE;
    size_t i_end = i_start + new_path_size - 1;

    // Check if the input causes the pointer to fall off the array.
    if (i_start >= 0 && i_end < max_size) {
        // If input is valid
        for (size_t i = 0; i < new_path_size; i++) {
            path_[i_start + i] = new_path[i];
        }
    }
    else if (i_start < 0) {
        throw std::runtime_error("Starting index (i_start) must be greater than zero (i_start > 0)!");
    }
    else if (new_path_size > max_size) {
        throw std::runtime_error("Input std::vector too large!");
    }
    else if ((new_path_size + i_start) > max_size) {
        throw std::runtime_error("Input std::vector size and start index combination causes out of bounds!");
    }
    else {
        throw std::runtime_error("Input failed test, cause unknown.");
    }
}

void Leg::Mod_path_first(const std::vector<point3D>& new_path, int i_start) {
    /*
    Modifies the coordinate values stored in the first_step_ array in the Leg class.
    */
    size_t new_path_size = new_path.size();
    size_t max_size = PATH_SIZE_HALF;
    size_t i_end = i_start + new_path_size - 1;

    // Check if the input causes the pointer to fall off the array.
    if (i_start >= 0 && i_end < max_size) {
        // If input is valid
        for (size_t i = 0; i < new_path_size; i++) {
            path_first_[i_start + i] = new_path[i];
        }
    }
    else if (i_start < 0) {
        throw std::runtime_error("Starting index (i_start) must be greater than zero (i_start > 0)!");
    }
    else if (new_path_size > max_size) {
        throw std::runtime_error("Input std::vector too large!");
    }
    else if ((new_path_size + i_start) > max_size) {
        throw std::runtime_error("Input std::vector size and start index combination causes out of bounds!");
    }
    else {
        throw std::runtime_error("Input failed test, cause unknown.");
    }
}

void Leg::display_path() {
    /*
    Displays the 3D points in the EF's path as whole numbers.
    */
    for (int i = 0; i < PATH_SIZE; ++i) {
        std::cout
            << "Point " << i + 1 << ":"
            << " ("
            << static_cast<int>(path_[i].x()) << ", "  // Cast x-coordinate to int
            << static_cast<int>(path_[i].y()) << ", "  // Cast y-coordinate to int
            << static_cast<int>(path_[i].z())          // Cast z-coordinate to int
            << ") " << std::endl;
    }
}

void Leg::display_path_first() {
    /*
    Displays the 3D points in the EF's path as whole numbers.
    */
    for (int i = 0; i < PATH_SIZE_HALF; ++i) {
        std::cout
            << "Point " << i + 1 << ":"
            << " ("
            << static_cast<int>(path_first_[i].x()) << ", "  // Cast x-coordinate to int
            << static_cast<int>(path_first_[i].y()) << ", "  // Cast y-coordinate to int
            << static_cast<int>(path_first_[i].z())          // Cast z-coordinate to int
            << ") " << std::endl;
    }
}

void Leg::Gen_move(const point3D& P_def, const vector3D& V_D, double h) {
    // Calculate half direction vector
    vector3D V_D_half = V_D / 2;

    // Generate step
    point3D P_start = P_def - V_D_half;
    std::vector<point3D> path_step = Gen_path_3PA(P_start, V_D, h, PATH_SIZE_HALF);
    Mod_path(path_step, 0);

    // Generate half kick
    point3D P_end = P_def + V_D_half;
    std::vector<point3D> path_kick = Gen_path_line(P_end, -V_D, PATH_SIZE_HALF);
    Mod_path(path_kick, PATH_SIZE_HALF);
}

void Leg::Gen_first_move(const point3D& P_def, const vector3D& V_D, double h) {
    // Calculate quater direction vector.
    vector3D V_D_half = V_D / 2;
    int array_size = PATH_SIZE_QUART;

    // Generate half step
    std::vector<point3D> path_step = Gen_path_3PA(P_def, V_D_half, h, array_size);
    Mod_path_first(path_step, 0);

    // Generate half kick
    std::vector<point3D> path_kick = Gen_path_line(P_def, -V_D_half, array_size);
    Mod_path_first(path_kick, array_size);
}


