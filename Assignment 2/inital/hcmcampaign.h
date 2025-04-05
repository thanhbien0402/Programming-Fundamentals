/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
};

class Vehicle : public Unit
{
private:
    VehicleType vehicleType;

public:
    Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType);
    string getVehicleTypetoString(int typeValue) const;
    int getAttackScore() override;
    string str() const override;
    VehicleType getType() const;
};

class Infantry : public Unit
{
private:
    InfantryType infantryType;
    // Helper function to check if a number is a perfect square
    bool isPerfectSquare(int n);
    int digitSum(int x); 

public:
    Infantry(int quantity, int weight, Position pos, InfantryType infantryType);
    string getInfantryTypetoString(int typeValue) const;
    int getAttackScore() override;
    string str() const override;
    InfantryType getType() const;
};

class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(const Unit **unitArray, int size, string name, BattleField *battleField);
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
};

class UnitList
{
private:
    int capacity;
    // TODO
    int count;
    struct Node
    {
        Unit *unit;
        Node *next;
        Node(Unit *unit) : unit(unit), next(nullptr) {}
    };
    Node *head;

public:
    UnitList(int capacity);
    ~UnitList();
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    // TODO
};

class LiberationArmy : public Army
{
private:
    vector<Unit*> findInfantryCombination(int threshold);
    vector<Unit*> findVehicleCombination(int threshold);
    void removeUnit(const vector<Unit*>& unitsToRemove);
    void updateIndices();
    void applyDefensiveLoss();
    void reinforceUnits();
    int nearestFibonacci(int n);    

public:
    LiberationArmy(const Unit **unitArray, int size, string name, BattleField *battleField);
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
};

class ARVN : public Army
{
private:
    

public:
    ARVN(const Unit **unitArray, int size, string name, BattleField *battleField);
    ~ARVN();
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};

class TerrainElement
{
public:
    TerrainElement();
    ~TerrainElement();
    virtual void getEffect(Army *army) = 0;
};

class BattleField
{
private:
    int n_rows, n_cols;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    // TODO
    string str() const;
};

class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *ARVN;

public:
    HCMCampaign(const string &config_file_path);
    void run();
    string printResult();
};

#endif