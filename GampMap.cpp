#include <iostream>
#include "GameMap.h" 

int map[X_len][Y_len] = {};

int Game_map::map_Numstate_ob() {
    int i, j;
    int num_ob = 0;
    for (i = 0; i < m_xlen; i++)
        for (j = 0; j < m_ylen; j++) 
            if (map[i][j] == obstacle)num_ob++;
    return num_ob;
}

int Game_map::map_Numstate_sp() {
    int i, j;
    int num_sp = 0;
    for (i = 0; i < m_xlen; i++)
        for (j = 0; j < m_ylen; j++)
            if (map[i][j] == space)num_sp++;
    return num_sp;
}

int Game_map::map_Numstate_u1() {
    int i, j;
    int num_u1 = 0;
    for (i = 0; i < m_xlen; i++)
        for (j = 0; j < m_ylen; j++)
            if (map[i][j] == unit1)num_u1++;
    return num_u1;
}

int Game_map::map_Numstate_u2() {
    int i, j;
    int num_u2 = 0;
    for (i = 0; i < m_xlen; i++)
        for (j = 0; j < m_ylen; j++)
            if (map[i][j] == unit2)num_u2++;
    return num_u2;
}


void Game_map::map_numstate_HowMany() {
    num_obstacle = map_Numstate_ob();
    num_space = map_Numstate_sp();
    num_unit1 = map_Numstate_u1();
    num_unit2 = map_Numstate_u2();
}

int Game_map::map_Numstate_All_Unit() {
    map_numstate_HowMany();
    return (num_unit1 + num_unit2);
}

void Clear_Map_unit1() {
    for (int i = 0; i < X_len; i++) {
        for (int j = 0; j < Y_len; j++) {
            if (map[i][j] == 1)
                map[i][j] = 0;
        }
    }
}

void AddUnit(const int x_pos, const int y_pos, const int state) {
    map[x_pos][y_pos] = state;
};

int CanAddUnit(const int x_pos, const int y_pos) {
    if (!map[x_pos][y_pos])
        return 1;
    else
        return 0;
}

void InitMap()//³õÊ¼»¯µØÍ¼
{
    Game_map Map(X_len, Y_len);

    Map.map_numstate_HowMany();
}

