#include "Hexapod.h"

// Robot origin
point3D ORIGIN_BOT(0, 0, 0);
std::chrono::milliseconds delay(1); // Delay between points

// Constructor: dynamically allocate the legs and assign ids
Hexapod::Hexapod() :
    // Variable initalization
    maestro(COM_PORT, BAUD_RATE)  // Initialize serial communication
{
    std::cout << "Hexapod serial initialized with Maestro on " << COM_PORT << std::endl;

    // Leg initalization
    legs = new Leg * [n_LEGS]; // Allocate array of Leg pointers
    for (int i = 0; i < n_LEGS; ++i) {
        point3D origin = Find_joint_origin(LEG_SPACING * i);
        legs[i] = new Leg(i, origin); // Dynamically create each Leg object with id
    }
}

// Destructor: free the allocated memory
Hexapod::~Hexapod() {
    for (int i = 0; i < n_LEGS; ++i) {
        delete legs[i]; // Delete each Leg object
    }
    delete[] legs; // Delete the array of pointers
}


// Methods
std::vector<double> Hexapod::inv_kin(const point3D& P_EF) {
    // Inline the point J2 instead of creating a separate object
    const point3D P_J2(LINK[1], 0, 0);

    // Calculate vector L
    const vector3D V_L = P_EF - P_J2;

    // Calculate the squared magnitude of V_L
    const double L_sq = (V_L[0] * V_L[0]) + (V_L[2] * V_L[2]);

    // Precompute L (square root of L_sq) to avoid computing sqrt(L_sq) twice
    const double L = std::sqrt(L_sq);

    // Solve for angle J3
    const double ang_J3 = -std::acos(
        ((LINK[2] * LINK[2]) + (LINK[3] * LINK[3]) - L_sq) / (2 * LINK[2] * LINK[3]));

    // Calculate angle B
    const double ang_B = std::acos(
        (L_sq + (LINK[2] * LINK[2]) - (LINK[3] * LINK[3])) / (2 * L * LINK[2]));

    // Calculate angle A
    const double ang_A = std::atan2(V_L[2], V_L[0]);

    // Calculate angle of joint J2
    const double ang_J2 = ang_B + ang_A;

    // Calculate angle of joint J1
    const double ang_J1 = std::atan2(P_EF[1], P_EF[0]);

    // Reserve space for the vector to avoid reallocations
    std::vector<double> angles;
    angles.reserve(3);  // Reserve space for 3 elements

    // Store angles in vector and return
    angles = { ang_J1, ang_J2, ang_J3 };
    return angles;
}

point3D Hexapod::Find_joint_origin(double angle_leg) {
    double x = ORIGIN_BOT[0] + ( LINK[0] * cos(angle_leg) );
    double y = ORIGIN_BOT[1] + ( LINK[0] * sin(angle_leg) );
    double z = ORIGIN_BOT[2];
    point3D joint_origin(x, y, z);
    return joint_origin;
}

vector3D Hexapod::Rot_vec_z(const vector3D& vec, double angle) {
    double cos_theta = std::cos(angle);
    double sin_theta = std::sin(angle);

    // Directly calculate the rotated vector components
    double new_x = cos_theta * vec.x() - sin_theta * vec.y();
    double new_y = sin_theta * vec.x() + cos_theta * vec.y();
    double new_z = vec.z();  // Z remains unchanged

    return vector3D(new_x, new_y, new_z);
}

vector3D Hexapod::To_local_move_vector(const vector3D V_D, const vector3D V_T, int id, double move_mag) {
    // Rotate direction vector to local frame
    vector3D V_D_local = Rot_vec_z(V_D, (id * LEG_SPACING));

    // Normalize vectors and add up vectors.
    V_D_local.normalize();
    vector3D V_T_norm = V_T;
    V_T_norm.normalize();
    vector3D V_move = V_D_local + V_T_norm;

    // Normalize and multiply by the movement magnitude to set exact vector magnitude. 
    V_move.normalize();
    return (V_move * move_mag);
}

void Hexapod::Init_walk(vector3D V_D, vector3D V_T) {
    // ========== Initalize legs ==========
    for (int i = 0; i < n_LEGS; i++) {
        vector3D V_D_leg = To_local_move_vector(V_D, V_T, i, MOVE_MAG);

        // Generate walk path
        legs[i]->Gen_first_move(P_DEF, V_D_leg, H);
        legs[i]->Gen_move(P_DEF, V_D_leg, H);
    }
}

point3D Hexapod::Get_point_from_leg(const int move_size, const int index_path, const int index_leg) {
    switch (move_size) { // Determine if this is half move or full move
    case PATH_SIZE_QUART: // Half move
        return legs[index_leg]->Get_path_first(index_path);
        break;

    case PATH_SIZE_HALF: // Full move
        return legs[index_leg]->Get_path(index_path);
        break;

    default: // Invalid argrument
        std::cerr << "Invalid input. Please enter 10 or 100." << std::endl;
        point3D errorPoint(999, 999, 999);
        return errorPoint;
    }
}

int Hexapod::Rad2bit(double radians) {
    // Map radians from -pi to pi onto a 0-255 scale
    int bitValue = static_cast<int>(128 + (radians / (PI/2)) * 127);

    // Ensure the result is clamped between 0 and 255
    if (bitValue < 0) {
        std::cout << bitValue << std::endl;
        std::cout << "BIT CLAMPED UP TO 0!" << std::endl;
        bitValue = 0;
    }
    if (bitValue > 255) {
        std::cout << "BIT CLAMPED DOWN TO 255!" << std::endl;
        bitValue = 255;
    }
    bitValue = 255 - bitValue;
    return bitValue;
}

void Hexapod::Execute_move(const int move_size, const bool inv, const bool interrupt) {
    // Generate default target list
    std::vector<uint8_t> targets(18, 128);
    std::vector<double> joint_angles{ 0, 0, 0 };
    point3D point;

    // Itterate through half the movement path
    for (int i = 0; i < move_size; i++) {

        // =================== Itterate through each leg ===================
        for (int j = 0; j < n_LEGS; j++) {
            // ---------- Even legs execute step, odd execute kick ----------
            if (j % 2 == 0 && inv == false) { // Even legs
                point = Get_point_from_leg(move_size, i, j);
            }
            else if (j % 2 != 0 && inv == false) { // Odd legs
                point = Get_point_from_leg(move_size, (i + move_size), j);
            }
            // ---------- Odd legs execute step, even execute kick ----------
            else if (j % 2 == 0 && inv == true) { // Even legs
                point = Get_point_from_leg(move_size, (i + move_size), j);
            }
            else if (j % 2 != 0 && inv == true) { // Odd legs
                point = Get_point_from_leg(move_size, i, j);
            }
            // Calculate joint angles for leg j
            // Convert to 8 bit centered on 128, and store in target list.
            joint_angles = inv_kin(point);
            targets[0 + 3 * j] = Rad2bit(joint_angles[0]) - OFFSET[0];
            targets[1 + 3 * j] = Rad2bit(joint_angles[1]) - OFFSET[1];
            targets[2 + 3 * j] = Rad2bit(joint_angles[2]) - OFFSET[2];
        }//===================================================================

       // Send targets over serial
        maestro.setMultipleTargets(0, targets);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void Hexapod::Walk(vector3D V_D, vector3D V_T, int n_steps) {
    Init_walk(V_D, V_T);
    Execute_move(PATH_SIZE_QUART, false, false); // Execute half move

    for (int i = 0; i < n_steps; i++) {
        Execute_move(PATH_SIZE_HALF, true, false); // Exectue full move, odd legs step
        Execute_move(PATH_SIZE_HALF, false, false); // Execute full move, even legs step
    }
}


void Hexapod::Move_dammit(const int move_size, const bool inv){
    // Generate default target list, joint angles and point.
    std::vector<uint8_t> targets(18, 128);
    std::vector<double> joint_angles{ 0, 0, 0 };
    point3D point = P_DEF;

    // =================== Itterate through move path array loop =================== //
    for (int i = 0; i < move_size; i++) {
        // ------------------ Itterate through leg array loop ------------------ //  
        for (int leg_num = 0; leg_num < 6; leg_num++) {
            // Even legs, not inverted --> Get step path
            if ( (inv == false) && (leg_num % 2 == 0)) { 
                point = legs[leg_num]->Get_path(i);
            } // Even legs, inverted --> Get kick path
            else if ((inv == true) && (leg_num % 2 == 0)) { 
                point = legs[leg_num]->Get_path(i + move_size);
            } // Odd legs, not inverted --> Get kick path
            else if ((inv == false) && (leg_num % 2 != 0)) { 
                point = legs[leg_num]->Get_path(i + move_size);
            } // Odd legs, inverted --> Get step path
            else if ((inv == true) && (leg_num % 2 != 0)) {
                point = legs[leg_num]->Get_path(i);
            } // Invalid combination:
            else {
                std::cout << "Failed to get PATH!" << std::endl;
            }
            // Calculate joint angles for leg j
            joint_angles = inv_kin(point);

            // Convert to 8 bit centered on 128, and store in target list.
            targets[0 + 3 * leg_num] = Rad2bit(joint_angles[0]) - OFFSET[0];
            targets[1 + 3 * leg_num] = Rad2bit(joint_angles[1]) - OFFSET[1];
            targets[2 + 3 * leg_num] = Rad2bit(joint_angles[2]) - OFFSET[2];

            // Send targets over serial
            //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            maestro.setMultipleTargets(0, targets);
            
        } // -------------------------- End of leg loop -------------------------- //
    } // ========================== End of move path loop =========================== //
}

void Hexapod::WALK_dammit(vector3D V_D, vector3D V_T, int n_steps) {
    // Initalize legs
    Init_walk(V_D, V_T);

    for (int i = 0; i < n_steps; i++) {
        Move_dammit(PATH_SIZE_HALF, false); // Normal move
        Move_dammit(PATH_SIZE_HALF, true); // Inverted move
    }
}








void Hexapod::Leg_move(const bool inv, const int leg_id) {
    /*
    TEST PROGRAM, NOT FOR ACTUAL USE
    */
    int move_size = PATH_SIZE_HALF;
    // Generate default target list
    std::vector<uint8_t> targets(18, 128);
    std::vector<double> joint_angles{ 0, 0, 0 };
    point3D point;

    // Itterate through half the movement path
    for (int i = 0; i < move_size; i++) {
        if (inv == true) {
            point = legs[leg_id]->Get_path(i+move_size);
        } else {
            point = legs[leg_id]->Get_path(i);
        }
            // Calculate joint angles for leg j
            // Convert to 8 bit centered on 128, and store in target list.
            joint_angles = inv_kin(point);
            targets[0 + 3 * leg_id] = Rad2bit(joint_angles[0]) - OFFSET[0];
            targets[1 + 3 * leg_id] = Rad2bit(joint_angles[1]) - OFFSET[1];
            targets[2 + 3 * leg_id] = Rad2bit(joint_angles[2]) - OFFSET[2];

       //===================================================================
       // Send targets over serial
        int target_servo = 3 * leg_id;
        maestro.setMultipleTargets(0, targets);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}