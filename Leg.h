#pragma once
#include "hexapod_config.h"


class Leg
{
public:
	int id;
	point3D origin_;
	point3D P_EF_;
	Leg(int id, point3D orign); // Constructor declaration

private:
	point3D path_[PATH_SIZE];
	point3D path_first_[PATH_SIZE_HALF];


	// Methods
	std::vector<point3D> Gen_path_line(const point3D& P_start, const vector3D& V_direction, int n);

	std::vector<point3D> Gen_path_3PA(point3D P_start, vector3D V_direction, double h, int res);

	void Mod_path(const std::vector<point3D>& new_path, int i_start);

	void Mod_path_first(const std::vector<point3D>& new_path, int i_start);

public:
	void display_path();

	void display_path_first();

	void Gen_move(const point3D& P_def, const vector3D& V_D, double h);

	void Gen_first_move(const point3D& P_def, const vector3D& V_D, double h);

	inline int ID() {
		return id;
	}

	inline point3D Get_path(int index) {
		return path_[index];
	}

	inline point3D Get_path_first(int index) {
		return path_first_[index];
	}
};