#pragma once
#include <iostream>
#include <Eigen/Dense>  
#include <vector>
#include <stdexcept> 


#define path_res 10 // Set path resolution, eg number of points between the start and end. 

// Data type aliases
typedef Eigen::Vector3d point3D; //[x,y,z] Stores 3D points 
typedef Eigen::Vector3d vector3D; //(x,y,z) Stores 3D vectors


class Leg
{
public:
	point3D path_[path_res]; // Store coordinates of EF step path.
	int path_res_ = path_res;


	// Methods
	std::vector<point3D> Gen_path_line(point3D P_start, vector3D V_direction, int res);

	void Mod_path(std::vector<point3D> path, int i_start);

	void display_path();

};

