#pragma once
#include <iostream>
#include <Eigen/Dense>  
#include <vector>
#include <stdexcept> 
#include <cmath>


#define path_res 10 // Set path resolution, eg number of points between the start and end. 

// Data type aliases
typedef Eigen::Vector3d point3D; //[x,y,z] Stores 3D points 
typedef Eigen::Vector3d vector3D; //(x,y,z) Stores 3D vectors


class Leg
{
public:
	point3D path_[path_res]; // Store coordinates of EF step path.
	int path_res_ = path_res;
	int id = 1;
	double spacing = (3.14 * 2) / 6;


	// Methods
	vector3D Rot_vec_z(const vector3D& vec, double angle);

	std::vector<point3D> Gen_path_line(const point3D& P_start, const vector3D& V_direction, int n);

	std::vector<point3D> Gen_path_3PA(point3D P_start, vector3D V_direction, double h, int res);

	void Mod_path(std::vector<point3D> path, int i_start);

	void display_path();

	vector3D To_local_move_vector(const vector3D V_D, const vector3D V_T, double move_mag);


};

