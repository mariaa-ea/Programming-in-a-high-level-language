#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

void input_coord(double& x, double& y); 
bool square_area(double x, double y);
bool circular_area(double x, double y);
void print_result(double x, double y);
void read_coord(double* x, double* y);
void print_to_file(double& x, double& y);
void closed_program();

#endif
