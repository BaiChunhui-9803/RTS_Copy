#ifndef GAMEMAP_H_
#define GAMEMAP_H_
#include <iostream>

constexpr int X_len = 20;               //length of X axis
constexpr int Y_len = 15;               //length of Y axis

class Game_map
{
private:
    enum m_state { obstacle = -1, space = 0, unit1 = 1, unit2 = 2 };
public:
    int m_xlen = 0;                             //length of X axis
    int m_ylen = 0;                             //length of Y axis
    int num_obstacle = 0;                       //number of obstacle
    int num_space = 0;                          //number of space
    int num_unit1 = 0;                          //number of unit1
    int num_unit2 = 0;                          //number of unit2

public:
    Game_map(int x, int y) :m_xlen(x), m_ylen(y) {};
    ~Game_map() {};
    int map_Numstate_ob();                      //gain number of obstacle
    int map_Numstate_sp();                      //gain number of space
    int map_Numstate_u1();                      //gain number of unit1
    int map_Numstate_u2();                      //gain number of unit2
    void map_numstate_HowMany();                //gain number of all states
    int map_Numstate_All_Unit();                //gain number of all units


    //friend std::ostream& operator<<(std::ostream& os, const Game_map& a);
    //friend std::istream& operator>>(std::istream& is, Game_map& a);
   
};


void Clear_Map_unit1();
void AddUnit(const int x_pos, const int y_pos, const int state);

int CanAddUnit(const int x_pos, const int y_pos);
void InitMap();



#endif