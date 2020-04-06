#ifndef GAMECARULE_H_
#define GAMECARULE_H_

#include <iostream>

int Gudge_ArmType(const int arm);
int Gudge_Xpos(const int xpos);
int Gudge_Ypos(const int ypos);
int Gudge_State(const int state);
int Gudge_Arm(const int arm);
int Correct_Unit(Unit* p_unit);
bool Unit1_Over(vector<Unit>* p_u1);
bool Unit2_Over(vector<Unit>* p_u2);

#endif