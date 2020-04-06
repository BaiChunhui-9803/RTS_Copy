#include <iostream>
#include "GameMap.h" 
#include "Population.h"
#include "GameRule.h" 


int Gudge_ArmType(const int arm) {
    switch (arm) {
    case 1:case 4:case 7:
        return 1;
    case 2:case 5:case 8:
        return 2;
    case 0:case 3:case 6:case 9:
        return 3;
    default: return 0;
    }

}


int Gudge_Xpos(const int xpos) {
	if (xpos >= 0 && xpos <= X_len)return 1;
	else return 0;
}

int Gudge_Ypos(const int ypos) {
	if (ypos >= 0 && ypos <= Y_len)return 1;
	else return 0;
}

int Gudge_State(const int state) {
    switch (state) {
    case 1:
        return 1;
    case 2:
        return 2;
    default:
        return 0;
    }
}

int Gudge_Arm(const int arm) {
    if (arm >= 0 && arm <= 9)
        return 1;
    else
        return 0;
}

int Correct_Unit(Unit* p_unit) {
    if (Gudge_Xpos(p_unit->m_Xpos) && Gudge_Ypos(p_unit->m_Ypos) && Gudge_State(p_unit->m_State) && Gudge_Arm(p_unit->m_Arm)) {
        return 1;
    }
    else
        return 0;
}



bool Unit1_Over(vector<Unit>* p_u1) {
    int over_num = 0;
    for (int i = 0; i < Unit_Size; i++) {
        if (p_u1->at(i).m_Endurance < 0) {
            over_num++;
        }
    }
    
    if (over_num == Unit_Size) {
        return 1;
    }
    else
        return 0;
}

bool Unit2_Over(vector<Unit>* p_u2) {
    int over_num = 0;
    for (int i = 0; i < Unit_Size; i++) {
        if (p_u2->at(i).m_Endurance < 0) {
            over_num++;
        }
    }

    if (over_num == Unit_Size) {
        return 1;
    }
    else
        return 0;
}