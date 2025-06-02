/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 *
 * This header defines the classes and interfaces for simulating
 * the 1975 Ho Chi Minh Campaign.  Students must implement the bodies
 * in the corresponding .cpp file according to the assignment spec.
 */

// Prevent inclusion of any other libraries
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any function signatures.
////////////////////////////////////////////////////////////////////////

namespace {
    // Flag used to guard terrain‐effects application
    static thread_local bool _terrain_applying = false;
}

// Forward declarations for all core types
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

// Enumerations for unit subtypes
enum VehicleType {
    TRUCK, MORTAR, ANTIAIRCRAFT,
    ARMOREDCAR, APC, ARTILLERY, TANK
};

enum InfantryType {
    SNIPER, ANTIAIRCRAFTSQUAD, MORTARSQUAD,
    ENGINEER, SPECIALFORCES, REGULARINFANTRY
};


/*------------------------------------------------ Position ---------*/
/// Represents a grid coordinate; supports parsing, stringifying,
/// and Euclidean distance.
class Position {
private:
    int r_;    // row
    int c_;    // column

public:
    // Default and (r,c) constructors
    Position(int r = 0, int c = 0);
    explicit Position(const std::string& s);

    // Accessors
    int getRow() const;
    int getCol() const;

    // Mutators
    void setRow(int r);
    void setCol(int c);

    // "(r,c)" formatting
    std::string str() const;

    // Euclidean distance between two positions
    double dist(const Position& p) const;
};


/*------------------------------------------------ Unit & Vehicle / Infantry ---------*/
/// Abstract base for any military unit.
class Unit {
    friend class UnitList;

protected:
    int      quantity;  // number of elements
    int      weight;    // power weight
    Position pos;       // map location

public:
    Unit(int q, int w, const Position& p);
    virtual ~Unit();

    // Must be implemented by subclasses
    virtual int         getAttackScore() const = 0;
    virtual bool        isVehicle()       const = 0;
    virtual std::string str()             const = 0;

    // Getters
    Position    getPos()      const;
    int         getQuantity() const;

    // Scale helpers: multiply quantity/weight by f, round up
    void scaleQuantity(double f);
    void scaleWeight  (double f);
};

/// Concrete Vehicle unit (e.g. Tank, APC)
class Vehicle : public Unit {
public:
    Vehicle(int q, int w, const Position& p, VehicleType t);
    ~Vehicle() override = default;

    bool        isVehicle()      const override;
    VehicleType getType()        const;
    int         getAttackScore() const override;
    std::string str()            const override;

private:
    VehicleType type;
};

/// Concrete Infantry unit (e.g. Sniper, Engineer)
class Infantry : public Unit {
public:
    Infantry(int q, int w, const Position& p, InfantryType t);
    ~Infantry() override = default;

    bool         isVehicle()      const override;
    InfantryType getType()        const;
    int          getAttackScore() const override;
    std::string  str()            const override;

private:
    InfantryType type;

    // Applies special rules (commandos, personal number)
    void applyRule();
};


/*------------------------------------------------ UnitList ---------*/
/// Singly‐linked list of Unit* with merge/insert/remove logic.
class UnitList {
private:
    struct Node {
        Unit* u;
        Node* next;
    };

    Node* head;
    Node* tail;
    int   vCnt;   // number of vehicle nodes
    int   iCnt;   // number of infantry nodes
    int   cap;    // maximum capacity

public:
    explicit UnitList(int capacity);
    ~UnitList();

    // Always merges if possible, else appends
    bool append(Unit* u);

    // Infantry go to front, vehicles to back
    bool insert(Unit* u);

    // Contains‐by‐type queries
    bool isContain(VehicleType vt)   const;
    bool isContain(InfantryType it)  const;

    // Iterate all units with a lambda
    template<typename Func>
    void forEach(Func f) const {
        for (Node* cur = head; cur; cur = cur->next)
            f(cur->u);
    }

    // Return all units matching predicate
    template<typename Pred>
    std::vector<Unit*> subset(Pred pred) const {
        std::vector<Unit*> out;
        for (Node* cur = head; cur; cur = cur->next)
            if (pred(cur->u))
                out.push_back(cur->u);
        return out;
    }

    // Remove specific pointers
    void remove(const std::vector<Unit*>& drop);

    // Extract all pointers, clearing the list
    std::vector<Unit*> extractAll();

    int vehicles()   const { return vCnt; }
    int infantries() const { return iCnt; }

    // Serialize list to string
    std::string str() const;

private:
    // Internal node management and merge routines
    Node* createNode(Unit* u);
    bool  pointerExists(Unit* u) const;
    void  deleteFirstMatching(Unit* target);
    void  clear();
    bool  merge(Unit* u);
    bool  mergeVehicle(  Vehicle*  existing, Vehicle*  incoming);
    bool  mergeInfantry(Infantry* existing, Infantry* incoming);
};


/*------------------------------------------------ Terrain ----------*/
/// Base for map terrain elements that modify an Army.
class TerrainElement {
protected:
    Position pos;

public:
    explicit TerrainElement(const Position& p): pos(p) {}
    virtual ~TerrainElement() {}
    virtual void getEffect(Army* a) = 0;  // apply to army
    Position getPos() const { return pos; }
};

class Road         : public TerrainElement { public: Road(const Position& p): TerrainElement(p) {}  void getEffect(Army*) override {} };
class Mountain     : public TerrainElement { public: Mountain(const Position& p): TerrainElement(p) {}  void getEffect(Army*) override; };
class River        : public TerrainElement { public: River(const Position& p): TerrainElement(p) {}      void getEffect(Army*) override; };
class Urban        : public TerrainElement { public: Urban(const Position& p): TerrainElement(p) {}      void getEffect(Army*) override; };
class Fortification: public TerrainElement { public: Fortification(const Position& p): TerrainElement(p) {} void getEffect(Army*) override; };
class SpecialZone  : public TerrainElement { public: SpecialZone(const Position& p): TerrainElement(p) {}   void getEffect(Army*) override; };


/*------------------------------------------------ Army & BattleField ---------*/
/// Abstract Army with combat‐index tracking and battle interface.
class Army {
protected:
    int           LF;          // vehicle combat strength [0..1000]
    int           EXP;         // infantry combat strength [0..500]
    std::string   name;
    UnitList*     unitList;
    BattleField*  battleField;

public:
    explicit Army(const std::string& n);
    virtual ~Army();

    // Access & clamp indices
    int getLF()  const;
    int getEXP() const;
    void setLF(int x);
    void setEXP(int x);

    // Access underlying units
    UnitList* units() const;

    // Recompute LF/EXP from the units
    void update();

    // Battle must be defined by subclasses
    virtual void        fight(Army* enemy, bool defense) = 0;
    virtual bool        isLiberation()           const = 0;
    virtual std::string str()                    const = 0;

private:
    // Recursive helper for update()
    void sumUnits(const std::vector<Unit*>& v, std::size_t idx, int& lf, int& ex);
};

/// 2D map of terrain; applies all terrain effects to an Army.
class BattleField {
private:
    int R, C;  // rows, cols
    std::vector<TerrainElement*> elems;

public:
    BattleField(int r, int c,
               const std::vector<Position*>& f,
               const std::vector<Position*>& rv,
               const std::vector<Position*>& fo,
               const std::vector<Position*>& ub,
               const std::vector<Position*>& sp);
    ~BattleField();

    // Apply all terrain elements in sequence
    void apply(Army* a);

    // "(n_rows,n_cols)"
    std::string str() const;

    // Query element at (r,c), or nullptr
    TerrainElement* getElement(int row, int col) const;

private:
    // Recursively add each type, fill with Roads, etc.
    template<typename T>
    void addTerrains(const std::vector<Position*>& v, std::size_t idx);
    void fillRoads(int x, int y);
    bool terrainAt(int row, int col, std::size_t idx) const;
    void deleteElems(std::size_t idx);
    void applyRec(Army* a, std::size_t idx);
    TerrainElement* findElem(int row, int col, std::size_t idx) const;
};


/*------------------------------------------------ Army Implementations ---------*/
/// LiberationArmy: offensive/defensive multipliers, combination attacks.
class LiberationArmy : public Army {
public:
    LiberationArmy(Unit** arr, int sz, BattleField* b);
    LiberationArmy(Unit** arr, int sz, const std::string& name, BattleField* b);

    bool        isLiberation() const override;
    void        fight(Army* enemy, bool defense = false) override;
    std::string str()               const override;

private:
    BattleField* bf;

    void init(Unit** arr, int sz);  // shared constructor logic
    std::string formatStr(const std::string& label) const;
    static std::pair<int,std::vector<Unit*>> bestCombo(const std::vector<Unit*>& units, int need);
};

/// ARVN: fixed combat indices, desertion/weight penalties
class ARVN : public Army {
public:
    ARVN(Unit** arr, int sz, BattleField* b);
    ARVN(Unit** arr, int sz, const std::string& name, BattleField* b);

    bool        isLiberation() const override;
    void        fight(Army* enemy, bool defense = false) override;
    std::string str()               const override;

private:
    BattleField* bf;
    void init(Unit** arr, int sz);  // shared constructor logic
};


/*------------------------------------------------ Configuration ---------*/
/// Parses a config file defining map size, terrain arrays, unit list, event code.
class Configuration {
public:
    explicit Configuration(const std::string& path);
    ~Configuration();

    // Terrain arrays
    const std::vector<Position*>& getForestPositions()      const;
    const std::vector<Position*>& getRiverPositions()       const;
    const std::vector<Position*>& getFortificationPositions() const;
    const std::vector<Position*>& getUrbanPositions()       const;
    const std::vector<Position*>& getSpecialZonePositions() const;

    // Extract unit pointers (ownership transferred)
    std::vector<Unit*> stealLiberation();
    std::vector<Unit*> stealARVN();

    // Map dimensions & event code
    int getNumRows()   const;
    int getNumCols()   const;
    int getEventCode() const;

    // Debug dump of entire config
    std::string str() const;

private:
    int num_rows;
    int num_cols;
    int eventCode;

    std::vector<Position*> arrayForest;
    std::vector<Position*> arrayRiver;
    std::vector<Position*> arrayFortification;
    std::vector<Position*> arrayUrban;
    std::vector<Position*> arraySpecialZone;
    std::vector<Unit*>    liberationUnits;
    std::vector<Unit*>    ARVNUnits;

    // Helpers
    template<typename T> static void cleanupVector(std::vector<T*>& v);
    static std::vector<Unit*> stealUnits(std::vector<Unit*>& src);
    static std::string trim(const std::string& s);
    static std::string vecPosStr (const std::vector<Position*>& v);
    static std::string vecUnitStr(const std::vector<Unit*>&    v);

    // Parsing internals
    void parsePosArray  (const std::string& raw, std::vector<Position*>& dst);
    Unit* makeUnit      (const std::string& rawToken);
    void readArrayLines (const std::string& firstLine, std::string& collected, std::ifstream& fin);
    void finishToken    (const std::string& tok);
    void parseFile      (const std::string& path);
};


/*---------------- Campaign driver ---------------------------------*/
/// Orchestrates reading config, building armies & battlefield, running battle.
class HCMCampaign {
private:
    Configuration*   cfg;
    BattleField*     bf;
    LiberationArmy*  lib;
    ARVN*            arvn;

    // Convert vector<Unit*> → Unit** for constructors
    static Unit** makeUnitArray(const std::vector<Unit*>& vec);
    static void   purgeArmy(Army* army);

public:
    explicit HCMCampaign(const std::string& path);
    ~HCMCampaign();

    // Apply terrain, execute fight(s), then purge low‐score units
    void        run();

    // "LIBERATIONARMY[LF=...,EXP=...]-ARVN[LF=...,EXP=...]"
    std::string printResult() const;
};
// In hcmcampaign.h, after class BattleField { … };
template<typename T>
void BattleField::addTerrains(const std::vector<Position*>& v, std::size_t idx) {
    if (idx >= v.size()) return;
    elems.push_back(new T(*v[idx]));
    addTerrains<T>(v, idx + 1);
}
// In hcmcampaign.h, after class Configuration { … };
template<typename T>
void Configuration::cleanupVector(std::vector<T*>& vec) {
    for (T* p : vec) {
        delete p;
    }
    vec.clear();
}

#endif // _H_HCM_CAMPAIGN_H_
