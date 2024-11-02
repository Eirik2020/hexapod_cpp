#pragma once
#include "hexapod_config.h"
#include "Leg.h"
#include "Maestro.h"

/// <summary>
/// NEEEEEEED TO FIX CONSTRUCTOR FOR HEXAPOD CLASS
/// </summary>

class Hexapod
{
public: // Constructor and Destructor
    Hexapod(); 
    ~Hexapod();      

//private:
public:
    Leg** legs; // Pointer to an array of Leg pointers
    Maestro maestro;


private://
public:
	std::vector<double> inv_kin(const point3D& P_EF);

    point3D Find_joint_origin(double angle_leg);

    vector3D Rot_vec_z(const vector3D& vec, double angle);

    vector3D To_local_move_vector(const vector3D V_D, const vector3D V_T, int id, double move_mag);

    void Init_walk(vector3D V_D, vector3D V_T);

    point3D Get_point_from_leg(const int move_size, const int index_path, const int index_leg);

    int Rad2bit(double radians);

    void Execute_move(const int move_size, const bool inv, const bool interrupt);

public:
    void Walk(vector3D V_D, vector3D V_T, int n_steps);

    void Leg_move(const bool inv, const int leg_id);

    void Move_dammit(const int move_size, const bool inv);
    void WALK_dammit(vector3D V_D, vector3D V_T, int n_steps);

















};

