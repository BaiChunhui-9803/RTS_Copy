#include <iostream>
#include "GameMap.h" 
#include "Population.h"
#include "GameRule.h" 

vector<Unit_Arm_Inf> Arm1 = { {150,10,2,100} };//����1�����Ͷ�150/������10/��������2/�������100
vector<Unit_Arm_Inf> Arm2 = { {120,15,2,120} };//����2�����Ͷ�120/������15/��������2/�������120
vector<Unit_Arm_Inf> Arm3 = { {100,12,5,150} };//����3�����Ͷ�100/������12/��������5/�������150

//�Զ����Unit�����еı�����Ϣ
void Unit::Auto_Add_UnitInf(const int arm_gudge) {
    switch (arm_gudge) {
    case 1:case 4:case 7:
        this->m_Endurance = Arm1.at(0).m_Arm_Endurance;
        this->m_Attack_Damage = Arm1.at(0).m_Arm_Attack_Damage;
        this->m_Attack_Distance = Arm1.at(0).m_Arm_Attack_Distance;
        this->m_Interval = Arm1.at(0).m_Arm_Interval;
        break;
    case 2:case 5:case 8:
        this->m_Endurance = Arm2.at(0).m_Arm_Endurance;
        this->m_Attack_Damage = Arm2.at(0).m_Arm_Attack_Damage;
        this->m_Attack_Distance = Arm2.at(0).m_Arm_Attack_Distance;
        this->m_Interval = Arm2.at(0).m_Arm_Interval;
        break;
    case 0:case 3:case 6:case 9:
        this->m_Endurance = Arm3.at(0).m_Arm_Endurance;
        this->m_Attack_Damage = Arm3.at(0).m_Arm_Attack_Damage;
        this->m_Attack_Distance = Arm3.at(0).m_Arm_Attack_Distance;
        this->m_Interval = Arm3.at(0).m_Arm_Interval;
        break;
    }
}


vector<vector<Unit>> unit1_2dmap;//��λ��ά����
vector<Unit> unit1;//�ҷ���λ����
vector<Unit> unit2;//�з���λ����
vector<Unit> HQ_unit;//���ʵ�Ԫ
vector<Unit> HQ_indivi;//���ʸ���
vector<Unit> HQ_lib;//���ʿ�

//Unit���캯��
Unit::Unit(int xpos, int ypos, int state, int arm) {
    this->m_Xpos = xpos;
    this->m_Ypos = ypos;
    this->m_State = state;
    this->m_Arm = arm;

    if (Correct_Unit(this)) {
        Auto_Add_UnitInf(Gudge_ArmType(arm));
    }
    else {
        std::cerr << "�Ա���ȡֵ������Ҫ��" << endl;
        exit(1);//ֹͣ�����һ�����������ķ�ʽ�����Ա�����ֵ(����ֵ)������˵������ֵ���ڹ涨��Χ��
    }
}

//�������Unit��Ԫ����ʱ����unit1
void Auto_Rand_Unit() {
    do {
        int x_rand = rand() % X_len;
        int y_rand = rand() % Y_len;
        int state = 1;
        int arm_rand = rand() % 10;
        if (CanAddUnit(x_rand, y_rand)) {
            AddUnit(x_rand, y_rand, 1);
            Unit U1_rand(x_rand, y_rand, state, arm_rand);
            unit1.push_back(U1_rand);
            break;
        }
    } while (1);
}

//�������Unit��Ԫ������unit2
void Auto_Rand_Unit_2() {
    do {
        int x_rand = rand() % X_len;
        int y_rand = rand() % Y_len;
        int state = 2;
        int arm_rand = rand() % 10;
        if (CanAddUnit(x_rand, y_rand)) {
            AddUnit(x_rand, y_rand, 2);
            Unit U2_rand(x_rand, y_rand, state, arm_rand);
            unit2.push_back(U2_rand);
            break;
        }
    } while (1);
}

vector<Individual> nowpopulation;//P(t)��Ⱥ
vector<Individual> midpopulation;//�м���Ⱥ���������ѡ�����������
vector<Individual> zeropopulation;//��ʼ��Ⱥ

//�Ŵ��㷨��׼������
void Initialize()//�����ʼ����Ⱥ���õ���һ����Ⱥ
{
    Initial_Proba_Matrix();
    srand((unsigned)time(NULL));
    for (int i = 0; i < Unit_Size; i++) {
        Auto_Rand_Unit_2();
    }
    std::cout << "Unit2 is generated." << endl;
    for (int j = 0; j < Unit_Size; j++)
    {
        std::cout << '\t' << "Unit2[" << j << "]:" << std::endl;
        std::cout << '\t' << unit2.at(j).m_Xpos;
        std::cout << '\t' << unit2.at(j).m_Ypos;
        std::cout << '\t' << unit2.at(j).m_Arm << std::endl;
    }
    //���ɵз�����

    for (int i = 0; i < Po_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {

            Auto_Rand_Unit();//�ڶ�ά�������������Po_Size*De_Variable����Ԫ
        }
        unit1_2dmap.push_back(unit1);
        unit1.clear();
        Clear_Map_unit1();
    }
    //ѭ������ʱ�����������Ԫ�ͱ�����unit1_2dmap��

    //���ɶ���Ⱦɫ�壩�����뵽��ʼ��Ⱥ��
    for (int i = 0; i < Po_Size; i++)
    {
        for (int j = 0; j < Unit_Size; j++)
        {
            unit1.push_back(unit1_2dmap.at(i).at(j));
        }
        Unit* p_unit = &(unit1.at(0));
        Individual Indivi(p_unit);//����һ������Ⱦɫ�壩
        nowpopulation.push_back(Indivi);//���뵽��Ⱥpopulation��
    }
    //ѭ������ʱ����ʼ��Ⱥ������nowpopulation��
    for (int i = 0; i < Po_Size; i++) {
        zeropopulation.push_back(nowpopulation.at(i));
    }
}

//Individual��ʵ��
Individual::Individual(Unit* p_unit)//���캯��
{
    for (int j = 0; j < Unit_Size; j++)//��forѭ���Ա��������ֵ
    {
        if (Correct_Unit(p_unit))//����Ҫ�����Ա���ȡֵ��Χ�ж� 
        {
            p_unit->Auto_Add_UnitInf(p_unit->m_Arm);
            Chrom.push_back(*p_unit++);
        }
        else//������Ҫ���򷢳������沢����
        {
            cerr << "�Ա���ȡֵ������Ҫ��" << endl;
            exit(1);//ֹͣ�����һ�����������ķ�ʽ�����Ա�����ֵ(����ֵ)������˵������ֵ���ڹ涨��Χ��
        }
    }
    unit1.clear();
    //��ʼ��ʱĬ����Ӧֵ��ֵΪ0
    this->Fitness = 0;
    this->Fitness_Proba = 0;
}



double Individual::GetFitness()const//��ȡ��Ӧֵ
{
    return Fitness;
}
double Individual::GetFitnessProba()const //��ȡ��Ӧֵ����
{
    return Fitness_Proba;
}
void Individual::ChaFitness(const double m_fitness)//�޸���Ӧֵ
{
    this->Fitness = m_fitness;
}
void Individual::ChaFitnessProba(const double m_ReFitness)//�޸���Ӧֵ����
{
    this->Fitness_Proba = m_ReFitness;
}




Fitness_Map_Unit Cacula[Unit_Size][Unit_Size] = {};//�����ҷ������ڵ�Ԫ��Ϣ���Ա������Ӧֵ
int Can_ATK_Num[Unit_Size] = {};//�����ҷ�����ɹ����������Ա������Ӧֵ

void Clear_Fitness_Map_Unit() {
    for (int i = 0; i < Unit_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            Cacula[i][j].Attack_Proba = 0;
        }
    }
}

bool No_Unit_In_Distance() {
    int num = 0;
    for (int i = 0; i < Unit_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            if (Cacula[i][j].Attack_Proba == 0) {
                num++;
            }
        }
    }

    if (num == Unit_Size * Unit_Size) {
        return 1;
    }
    else
        return 0;
}


//���㵥λ�����
void Cacula_Unit_Distance(const int indivi_pos) {
    for (int i = 0; i < Unit_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            double u1_xpos = nowpopulation.at(indivi_pos).Chrom.at(i).m_Xpos;
            double u1_ypos = nowpopulation.at(indivi_pos).Chrom.at(i).m_Ypos;
            double u2_xpos = unit2.at(j).m_Xpos;
            double u2_ypos = unit2.at(j).m_Ypos;
            Cacula[i][j].Unit_Distance = sqrt(pow(abs(u1_xpos - u2_xpos), 2) + pow(abs(u1_ypos - u2_ypos), 2));
        }
    }
}

//���㵥λ�������ȼ�
void Cacula_Attack_Proba(const int indivi_pos) {

    for (int i = 0; i < Unit_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            if (double(nowpopulation.at(indivi_pos).Chrom.at(i).m_Attack_Distance) >= Cacula[i][j].Unit_Distance) {
                Cacula[i][j].Attack_Proba++;
                Can_ATK_Num[j]++;
            }
        }
    }
}

//����unit1[i]���ȹ����ĸ���λ
int Pick_Which_To_ATK(const int unit_pos) {
    int Max_CanATKNum = 0;
    for (int i = 0; i < Unit_Size; i++) {
        if (Can_ATK_Num[i] > Max_CanATKNum)
            Max_CanATKNum = Can_ATK_Num[i];
    }

    for (int i = 0; i < Unit_Size; i++) {
        if (Cacula[unit_pos][i].Attack_Proba > 0 && Can_ATK_Num[i] == Max_CanATKNum)
            return i;
    }

    return -1;
}

Proba_Matrix_Struct Proba_Matrix[X_len][Y_len];//���ʾ��󣬷�Ӧ��Ԫ������
Proba_Matrix_Struct Proba_Matrix_Wit[X_len][Y_len];//���ʾ���ĸ���
Proba_Matrix_Struct Proba_Matrix_Sum[X_len][Y_len];//���ʾ�����ۼӸ���

//��ʼ�����ʾ���
void Initial_Proba_Matrix() {

    for (int i = 0; i < X_len; i++) {
        for (int j = 0; j < Y_len; j++) {
            Proba_Matrix[i][j].Arm1_Weight = 0;
            Proba_Matrix[i][j].Arm2_Weight = 0;
            Proba_Matrix[i][j].Arm3_Weight = 0;
        }
    }
}

//��Ӹ��ʾ�����ĳ�������Ȩֵ����ֵ���������ۼ�����˺��й�
void Add_Proba_Matrix(Unit* p_unit) {
    switch (p_unit->m_Arm) {
    case 1:case 4:case 7:
        Proba_Matrix[p_unit->m_Xpos][p_unit->m_Ypos].Arm1_Weight += p_unit->m_Attack_Damage;
        break;
    case 2:case 5:case 8:
        Proba_Matrix[p_unit->m_Xpos][p_unit->m_Ypos].Arm2_Weight += p_unit->m_Attack_Damage;
        break;
    case 0:case 3:case 6:case 9:
        Proba_Matrix[p_unit->m_Xpos][p_unit->m_Ypos].Arm3_Weight += p_unit->m_Attack_Damage;
        break;
    }

}

//���¸��ʾ���ĸ���
void Update_PM_Wit()
{
    double sum = 0;//�����ۼ���

    for (int i = 0; i < X_len; i++)//������ʾ���ĸ���֮��
    {
        for (int j = 0; j < Y_len; j++)
        {
            Proba_Matrix_Struct* p_pm = &Proba_Matrix[i][j];
            sum += double(p_pm->Arm1_Weight) + double(p_pm->Arm2_Weight) + double(p_pm->Arm3_Weight);
        }
    }

    for (int i = 0; i < X_len; i++)
    {
        for (int j = 0; j < Y_len; j++)
        {
            Proba_Matrix_Struct* p_pm = &Proba_Matrix[i][j];
            Proba_Matrix_Struct* p_pm_w = &Proba_Matrix_Wit[i][j];

            p_pm_w->Arm1_Weight = p_pm->Arm1_Weight / sum;
            p_pm_w->Arm2_Weight = p_pm->Arm2_Weight / sum;
            p_pm_w->Arm3_Weight = p_pm->Arm3_Weight / sum;
        }
    }
}

//���¸��ʾ�����ۼӸ���
void Update_PM_Sum()
{
    double sum = 0;//�����ۼ���

    for (int i = 0; i < X_len; i++)//������ʾ���ĸ���֮��
    {
        for (int j = 0; j < Y_len; j++)
        {
            Proba_Matrix_Struct* p_pm_w = &Proba_Matrix_Wit[i][j];
            Proba_Matrix_Struct* p_pm_s = &Proba_Matrix_Sum[i][j];
            sum += p_pm_w->Arm1_Weight;
            p_pm_s->Arm1_Weight = sum;
            sum += p_pm_w->Arm2_Weight;
            p_pm_s->Arm2_Weight = sum;
            sum += p_pm_w->Arm3_Weight;
            p_pm_s->Arm3_Weight = sum;
        }
    }
}

int Indivi_Have_HQ_Unit(Unit* p_unit) {
    
    for (auto it = HQ_indivi.begin(); it != HQ_indivi.end(); it++) {
        if (it->m_Xpos == p_unit->m_Xpos && it->m_Ypos == p_unit->m_Ypos && it->m_Arm == p_unit->m_Arm) {
            return 1;
        }
    }
    return 0;
}
//�������ʸ���
void Save_HQ_Unit() {
    double sum1 = 0, sum2 = 0, sum3 = 0;
    double quality_index_1 = 0, quality_index_2 = 0, quality_index_3 = 0;
    for (int i = 0; i < X_len; i++) {
        for (int j = 0; j < Y_len; j++) {
            if (!Proba_Matrix[i][j].Arm1_Weight) sum1 += Proba_Matrix[i][j].Arm1_Weight;
            if (!Proba_Matrix[i][j].Arm2_Weight) sum2 += Proba_Matrix[i][j].Arm2_Weight;
            if (!Proba_Matrix[i][j].Arm3_Weight) sum3 += Proba_Matrix[i][j].Arm3_Weight;
        }
    }
    quality_index_1 = sum1 * 0.5;
    quality_index_2 = sum2 * 0.5;
    quality_index_3 = sum3 * 0.5;

    for (int i = 0; i < X_len; i++) {
        for (int j = 0; j < Y_len; j++) {
            if (Proba_Matrix[i][j].Arm1_Weight >= quality_index_1) {
                Unit U1(i, j, 1, 1);
                HQ_unit.push_back(U1);
            }
            if (Proba_Matrix[i][j].Arm2_Weight >= quality_index_2) {
                Unit U2(i, j, 1, 2);
                HQ_unit.push_back(U2);
            }
            if (Proba_Matrix[i][j].Arm3_Weight >= quality_index_3) {
                Unit U3(i, j, 1, 3);
                HQ_unit.push_back(U3);
            }
        }
    }
}

//�������ʸ���
void Save_HQ_Indivi() {
    double best_fitness=GetBestFitness();
    double fitness_index = best_fitness * 0.95;
    for (int i = 0; i < Po_Size; i++) {
        Individual* p_indivi=&(nowpopulation.at(i));
        if (p_indivi->GetFitness() >= fitness_index) {
            for (int j = 0; j < Unit_Size; j++) {
                Unit* p_unit = &(p_indivi->Chrom.at(j));
                if (!Indivi_Have_HQ_Unit(p_unit)) {
                    Unit U(p_unit->m_Xpos, p_unit->m_Ypos, 1, p_unit->m_Arm);
                    HQ_indivi.push_back(U);
                }
            }
        }
    }
}

//�������ʿ�
void Update_HQ_Lib() {
    
    for (auto itu = HQ_unit.begin(); itu != HQ_unit.end(); itu++) {
        Unit* p_unit = &(*itu);
        if (Indivi_Have_HQ_Unit(p_unit)) {
            Unit U(p_unit->m_Xpos, p_unit->m_Ypos, 1, p_unit->m_Arm);
            HQ_lib.push_back(U);
        }
    }
}



//ģ���ҷ�һ����λ�Ĺ�������
void Do_ATK(const int atk_damage, const int unit2_pos) {
    unit2.at(unit2_pos).m_Endurance -= atk_damage;
    if (unit2.at(unit2_pos).m_Endurance <= 0) {
        Can_ATK_Num[unit2_pos] = 0;
        for (int i = 0; i < Unit_Size; i++) {
            Cacula[i][unit2_pos].Attack_Proba = 0;
        }
    }
}


void Unit_Move() {}


//ģ��һ���������Ϸ����
double Start_Game(const int indivi_pos) {
    int k = 0;
    int t1 = 0;
    int t2 = 0;
    int sum_damage_1 = 0;
    int sum_damage_2 = 0;
    double fitness = 0;
    vector<Unit>* vp_u1 = &(nowpopulation.at(indivi_pos).Chrom);
    vector<Unit>* vp_u2 = &unit2;
    Cacula_Unit_Distance(indivi_pos);
    Cacula_Attack_Proba(indivi_pos);
    while (1) {
        k += 10;
        if (No_Unit_In_Distance() || Unit1_Over(vp_u1) || Unit2_Over(vp_u2)) {
            break;
        }

        for (int i = 0; i < Unit_Size; i++) {
            //�ֵ��ҷ���λi�Ĺ���ʱ��
            if (!(k % vp_u1->at(i).m_Interval) && (vp_u1->at(i).m_Endurance > 0)) {
                int atk_pos = Pick_Which_To_ATK(i);
                if (atk_pos >= 0) {
                    Do_ATK(vp_u1->at(i).m_Attack_Damage, atk_pos);
                    Add_Proba_Matrix(&(vp_u1->at(i)));
                    sum_damage_1 += vp_u1->at(i).m_Attack_Damage;

                }
                else {
                    Unit_Move();
                    t1++;
                }


            }
        }

        for (int j = 0; j < Unit_Size; j++) {
            //�ֵ��з���λi�Ĺ���ʱ��
            if (!(k % vp_u2->at(j).m_Interval) && (vp_u2->at(j).m_Endurance > 0)) {
                while (1) {
                    int atk_pos = rand() % Unit_Size;
                    if (vp_u1->at(atk_pos).m_Endurance > 0 && vp_u1->at(atk_pos).m_Attack_Distance >= Cacula[atk_pos][j].Unit_Distance) {
                        vp_u1->at(atk_pos).m_Endurance -= vp_u2->at(j).m_Attack_Damage;
                        sum_damage_2 += vp_u2->at(j).m_Attack_Damage;
                        if (vp_u1->at(atk_pos).m_Endurance <= 0) {
                            for (int n = 0; n < Unit_Size; n++) {
                                Cacula[atk_pos][n].Attack_Proba = 0;
                            }
                        }
                        break;
                    }

                    else {
                        t2++;
                    }
                    if (t2 > 10) {
                        break;
                    }
                }
            }
        }
    };

    fitness = double(sum_damage_1) - double(sum_damage_2);
    if (fitness > 0) {
        return fitness;
    }
    else
        return 0;

}

void Cacula_Fitness()//����������Ӧֵ
{
    double fitness = 0;//��ʱ��Ӧֵ
    for (int i = 0; i < Po_Size; i++) {
        fitness = Start_Game(i);
        nowpopulation.at(i).ChaFitness(fitness);
        Clear_Fitness_Map_Unit();
        for (int j = 0; j < Unit_Size; j++) {
            unit2.at(j).Auto_Add_UnitInf(Gudge_ArmType(unit2.at(j).m_Arm));
        }
    }
}

void Cacula_FitnessProba()//������Ӧֵ����
{
    double sum = 0;//��Ӧֵ�ۼ���
    double temp = 0;
    for (int i = 0; i < Po_Size; i++)//�������Ӧֵ֮��
    {
        sum += nowpopulation.at(i).GetFitness();
    }
    for (int j = 0; j < Po_Size; j++)
    {
        temp = nowpopulation.at(j).GetFitness() / sum;//�������
        nowpopulation.at(j).ChaFitnessProba(temp);//�޸ĸ������Ӧ�ȸ���
    }
}


void Individual::printout(vector<Individual>::iterator itb, vector<Individual>::iterator ite) {
    int i = 0;
    for (auto it = itb; it != ite; it++, i++) {
        cout << "Indivi[" << i << "]:" << endl;
        cout << '\t' << "fitness=" << it->Fitness;
        cout << '\t' << "fitness_proba=" << it->Fitness_Proba << endl;
    }
}

//��ȡ��Ⱥ�����ŵ���Ӧֵ
double GetBestFitness() {
    double best = 0;
    double get = 0;
    for (int i = 0; i < Po_Size; i++) {
        get = nowpopulation.at(i).GetFitness();
        if (get >= best)best = get;
    }
    return best;
}

//��ȡ��Ⱥ����Ӧֵ�����ֵ
double GetSumFitness() {
    double sum = 0;
    for (int i = 0; i < Po_Size; i++) {
        sum += nowpopulation.at(i).GetFitness();
    }
    return sum;
}

void pp() {
    Update_PM_Wit();
    Update_PM_Sum();
    for (int i = 0; i < X_len; i++) {
        for (int j = 0; j < Y_len; j++) {
            cout << "Proba_Matrix[" << i << "][" << j << "]:" << endl;
            cout << '\t' << "Arm1_Weight=" << left << setw(8) << Proba_Matrix[i][j].Arm1_Weight;
            cout << '\t' << "Arm2_Weight=" << left << setw(8) << Proba_Matrix[i][j].Arm2_Weight;
            cout << '\t' << "Arm3_Weight=" << left << setw(8) << Proba_Matrix[i][j].Arm3_Weight << endl;

            cout << '\t' << "Arm1_WitPro=" << left << setw(8) << Proba_Matrix_Wit[i][j].Arm1_Weight;
            cout << '\t' << "Arm2_WitPro=" << left << setw(8) << Proba_Matrix_Wit[i][j].Arm2_Weight;
            cout << '\t' << "Arm3_WitPro=" << left << setw(8) << Proba_Matrix_Wit[i][j].Arm3_Weight << endl;

            cout << '\t' << "Arm1_WitSum=" << left << setw(8) << Proba_Matrix_Sum[i][j].Arm1_Weight;
            cout << '\t' << "Arm2_WitSum=" << left << setw(8) << Proba_Matrix_Sum[i][j].Arm2_Weight;
            cout << '\t' << "Arm3_Wit_um=" << left << setw(8) << Proba_Matrix_Sum[i][j].Arm3_Weight << endl;
        }
    }
}





void plan_learning() //��Ⱥѧϰ
{
    //���������0��1��С��
    double array[Po_Size][Unit_Size];//������������
    default_random_engine e(time(0));//���棬�����������
    uniform_real_distribution<double> u(0.0, 1.0); //�ֲ�
    for (int i = 0; i < Po_Size; i++)
        for (int j = 0; j < Unit_Size; j++)
            array[i][j] = u(e);

    unit1_2dmap.clear();
    unit1.clear();

    Update_PM_Wit();
    Update_PM_Sum();

    for (int i = 0; i < Po_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {               
            Auto_Rand_Unit();     
        }

        Clear_Map_unit1();
        Unit* p_unit = &(unit1.at(0));
        Individual Indivi(p_unit);
        midpopulation.push_back(Indivi);
        unit1.clear();
    }

    for (int i = 0; i < Po_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            nowpopulation.at(i).Chrom.at(j) = midpopulation.at(i).Chrom.at(j);
        }
    }
    midpopulation.clear();

}

void select() {

    unit1_2dmap.clear();
    unit1.clear();
    nowpopulation.clear();

    for (int i = 0; i < Po_Size; i++) {
        nowpopulation.push_back(zeropopulation.at(i));
    }
    


    for (int i = 0; i < Po_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            int num_lib = HQ_lib.size();
            int rand_pos = rand() % num_lib;
            Unit* p_r = &(HQ_lib.at(rand_pos));
            if (CanAddUnit(p_r->m_Xpos, p_r->m_Ypos)) {
                AddUnit(p_r->m_Xpos, p_r->m_Ypos, 1);
                Unit U1(p_r->m_Xpos, p_r->m_Ypos, 1, p_r->m_Arm);
                unit1.push_back(U1);
            }
            else
                Auto_Rand_Unit();
        }

        Clear_Map_unit1();
        Unit* p_unit = &(unit1.at(0));
        Individual Indivi(p_unit);
        midpopulation.push_back(Indivi);
        unit1.clear();
    }

    for (int i = 0; i < Po_Size; i++) {
        for (int j = 0; j < Unit_Size; j++) {
            nowpopulation.at(i).Chrom.at(j) = midpopulation.at(i).Chrom.at(j);
        }
    }

    midpopulation.clear();
}
double Scand() //�������0��1��С��
{
    int N = rand() % 999;
    return double(N) / 1000.0;;//�������0��1��С��
}