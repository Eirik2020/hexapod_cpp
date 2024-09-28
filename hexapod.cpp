// hexapod.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "leg.h"


int main()
{
    std::cout << "Hello World!\n";

    Leg leg1;
    point3D P_start(0, 0, 0);
    vector3D V_dir(10, 0, 0);
    int res = 5;
    int array_size = path_res;

    std::vector<point3D> line1 = leg1.Gen_path_line(P_start, V_dir, res);
    std::vector<point3D> line2 = leg1.Gen_path_line(P_start, V_dir, res);

    leg1.Mod_path(line1, 0);
    leg1.Mod_path(line2, 5);

    leg1.display_path();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
