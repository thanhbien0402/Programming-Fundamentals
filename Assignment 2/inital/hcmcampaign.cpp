#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

Unit::Unit(int quantity, int weight, Position pos)
    : quantity(quantity), weight(weight), pos(pos) 
{

}

Unit::~Unit() {}

Position Unit::getCurrentPosition() const 
{
    return pos;
}

Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) 
{

}

int Vehicle::getAttackScore() 
{
    int typeValue = static_cast<int>(vehicleType);
    int score = 0;
    score = ceil((typeValue * 304 + quantity * weight) / 30.0);
    return score; // Default implementation, should be overridden in derived classes
}

string Vehicle::getVehicleTypetoString(int typeValue) const
{
    switch (typeValue) {
        case 0: return "TRUCK";
        case 1: return "MORTAR";
        case 2: return "ANTIAIRCRAFT";
        case 3: return "ARMOREDCAR";
        case 4: return "APC";
        case 5: return "ARTILLERY";
        case 6: return "TANK";
        default: return "UNKNOWN";
    }
}

string Vehicle::str() const 
{
    string result = "Vehicle[vehicleType=" + getVehicleTypetoString(static_cast<int>(vehicleType)) + 
                    ", quantity=" + to_string(quantity) + 
                    ", weight=" + to_string(weight) + 
                    ", position=" + pos.str() + "]";
    return result;
}

VehicleType Vehicle::getType() const 
{
    return vehicleType;
}

//////////////////////////////////

Infantry::Infantry(int quantity, int weight, Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) 
{

}

bool Infantry::isPerfectSquare(int n) 
{
    int r = static_cast<int>(sqrt(n));
    return r * r == n;
}

int Infantry::digitSum(int x) 
{
    int s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

int Infantry::getAttackScore() 
{
    int typeValue = static_cast<int>(infantryType);
    int score = typeValue * 56 + quantity * weight;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight)) {
        score += 75;
    }
    
    int ds = digitSum(score) + digitSum(1975);
    while (ds >= 10) {
        ds = digitSum(ds);
    }
    
    if (ds > 7) {
        quantity += static_cast<int>(ceil(0.2 * quantity));
    } else if (ds < 3) {
        quantity -= static_cast<int>(ceil(0.1 * quantity));
    }
    
    score = typeValue * 56 + quantity * weight;
    if (infantryType == SPECIALFORCES && isPerfectSquare(weight)) {
        score += 75;
    }
    return score;
}

string Infantry::str() const 
{
    string result = "Infantry[infantryType=" + getInfantryTypetoString(static_cast<int>(infantryType)) + 
                    ", quantity=" + to_string(quantity) + 
                    ", weight=" + to_string(weight) + 
                    ", position=" + pos.str() + "]";
    return result;
}

InfantryType Infantry::getType() const 
{
    return infantryType;   
}

////////////////////////////////////////////////

Army::Army(const Unit **unitArray, int size, string name, BattleField *battleField)
    : name(name), battleField(battleField), LF(0), EXP(0) 
{
    unitList = new UnitList(size);

    for (int i = 0; i < size; i++) {
        const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unitArray[i]);
        if (vehicle != nullptr) {
            Vehicle* vehicleCopy = new Vehicle(*vehicle); // Create a copy
            if (unitList->insert(vehicleCopy)) {
                LF += vehicleCopy->getAttackScore();
            }
        } else {
            const Infantry* infantry = dynamic_cast<const Infantry*>(unitArray[i]);
            if (infantry != nullptr) {
                Infantry* infantryCopy = new Infantry(*infantry); // Create a copy
                if (unitList->insert(infantryCopy)) {
                    EXP += infantryCopy->getAttackScore();
                }
            }
        }
    }

    if (LF > 1000) {
        LF = 1000;
    }
    if (EXP > 500) {
        EXP = 500;
    }
}

Army::~Army() 
{
    if (unitList != nullptr) 
    {
        delete unitList;
        unitList = nullptr;
    }
}

////////////////////////////////////////////////////////

LiberationArmy::LiberationArmy(const Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField)
{
    // Initialize LF and EXP based on the units in the unitArray
    for (int i = 0; i < size; i++) {
        const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unitArray[i]);
        if (vehicle != nullptr) {
            Vehicle* vehicleCopy = new Vehicle(*vehicle); // Create a copy
            if (unitList->insert(vehicleCopy)) {
                LF += vehicleCopy->getAttackScore();
            }
        } else {
            const Infantry* infantry = dynamic_cast<const Infantry*>(unitArray[i]);
            if (infantry != nullptr) {
                Infantry* infantryCopy = new Infantry(*infantry); // Create a copy
                if (unitList->insert(infantryCopy)) {
                    EXP += infantryCopy->getAttackScore();
                }
            }
        }
    }

    if (LF > 1000) {
        LF = 1000;
    }
    if (EXP > 500) {
        EXP = 500;
    }
}

LiberationArmy::~LiberationArmy() 
{
    if (unitList != nullptr) 
    {
        delete unitList;
        unitList = nullptr;
    }
}

vector<Unit*> LiberationArmy::findInfantryCombination(int threshold)
{

}

vector<Unit*> LiberationArmy::findVehicleCombination(int threshold)
{

}

void LiberationArmy::removeUnit(const vector<Unit*>& unitsToRemove)
{

}

void LiberationArmy::updateIndices()
{
    LF = 0;
    EXP = 0;
    // vector<Unit*> units = unitList->getUnits();
}

void LiberationArmy::applyDefensiveLoss()
{

}

void LiberationArmy::reinforceUnits()
{

}

int LiberationArmy::nearestFibonacci(int n)
{

} 

void LiberationArmy::fight(Army *enemy, bool defense) 
{
    // Implement the fight logic here
}

string LiberationArmy::str() const 
{
    string result = "LiberationArmy[name=" + name + 
                    ",LF=" + to_string(LF) + 
                    ",EXP=" + to_string(EXP) + 
                    ",unitList=" + unitList->str() + 
                    ",battleField=" + battleField->str() + "]";
    return result;
}

/////////////////////////////////////////////////////////////

UnitList::UnitList(int capacity) 
    : capacity(capacity), count(0), head(nullptr) 
{

}

UnitList::~UnitList() 
{
    Node *current = head;
    while (current != nullptr) 
    {
        Node *next = current->next;
        // delete current->unit; // Assuming unit is dynamically allocated
        delete current;
        current = next;
    }
    head = nullptr;
}

bool UnitList::insert(Unit *unit) 
{
    if (count >= capacity)
        return false;
    
    if (dynamic_cast<Infantry*>(unit) != nullptr) {
        Node* newNode = new Node(unit);
        newNode->next = head;
        head = newNode;
    }
    else if (dynamic_cast<Vehicle*>(unit) != nullptr) {
        Node* newNode = new Node(unit);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while(current->next != nullptr)
                current = current->next;
            current->next = newNode;
        }
    }
    else {
        Node* newNode = new Node(unit);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while(current->next != nullptr)
                current = current->next;
            current->next = newNode;
        }
    }
    
    count++;
    return true;
}

bool UnitList::isContain(VehicleType vehicleType) 
{
    Node *current = head;
    while (current != nullptr) 
    {
        Vehicle *vehicle = dynamic_cast<Vehicle*>(current->unit);
        if (vehicle && vehicle->getType() == vehicleType) 
        {
            return true;
        }

        current = current->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType) 
{
    Node *current = head;
    while (current != nullptr) 
    {
        Infantry *infantry = dynamic_cast<Infantry*>(current->unit);
        if (infantry && infantry->getType() == infantryType) 
        {
            return true;
        }

        current = current->next;
    }
    return false;
}

string UnitList::str() const 
{
    int count_vehicle = 0;
    int count_infantry = 0;
    Node *current = head;
    bool isFirst = true;
    string unitListStr = "";

    while (current != nullptr) {
        if (dynamic_cast<Vehicle*>(current->unit) != nullptr) {
            count_vehicle++;
        } else if (dynamic_cast<Infantry*>(current->unit) != nullptr) {
            count_infantry++;
        }

        if (!isFirst) {
            unitListStr += ",";
        }
        unitListStr += current->unit->str();
        isFirst = false;
        current = current->next;
    }

    string result = "UnitList[count_vehicle=" +to_string(count_vehicle) + 
                    ";count_infantry=" + to_string(count_infantry) + 
                    ";" + unitListStr + "]";

    return result;
}

////////////////////////////////////////////////

ARVN::ARVN(const Unit **unitArray, int size, string name, BattleField *battleField)
    : Army(unitArray, size, name, battleField) 
{

}

ARVN::~ARVN() 
{
    if (unitList != nullptr) 
    {
        delete unitList;
        unitList = nullptr;
    }
}

void ARVN::fight(Army *enemy, bool defense) 
{
    // Implement the fight logic here
}

string ARVN::str() const 
{
    string result = "ARVN[name=" + name + 
                    ",LF=" + to_string(LF) + 
                    ",EXP=" + to_string(EXP) + 
                    ",unitList=" + unitList->str() + 
                    ",battleField=" + battleField->str() + "]";
    return result;
}

///////////////////////////////////////////////////
// Constructor: reads the configuration file and populates all attributes.
// Configuration::Configuration(const string &filepath)
// {
//     // Initialize numeric attributes.
//     num_rows = 0;
//     num_cols = 0;
//     eventCode = 0;
    
//     // Clear vectors (if not already empty).
//     arrayForest.clear();
//     arrayRiver.clear();
//     arrayFortification.clear();
//     arrayUrban.clear();
//     arraySpecialZone.clear();
//     liberationUnits.clear();
//     ARVNUnits.clear();
    
//     ifstream fin(filepath);
//     if (!fin.is_open())
//         return; // could throw an exception or handle error
    
//     string line;
//     while(getline(fin, line))
//     {
//         // Skip empty lines.
//         if(line.empty()) continue;
        
//         size_t pos = line.find('=');
//         if(pos == string::npos)
//             continue;
//         string key = line.substr(0, pos);
//         string value = line.substr(pos + 1);
        
//         // Remove spaces.
//         key.erase(remove(key.begin(), key.end(), ' '), key.end());
//         value.erase(remove(value.begin(), value.end(), ' '), value.end());
        
//         if(key == "NUM_ROWS")
//         {
//             num_rows = stoi(value);
//         }
//         else if(key == "NUM_COLS")
//         {
//             num_cols = stoi(value);
//         }
//         else if(key == "ARRAY_FOREST")
//         {
//             if(value.front()=='[' && value.back()==']')
//                 value = value.substr(1, value.size()-2);
//             // Split positions by searching for closing parenthesis.
//             size_t start = 0;
//             while(true)
//             {
//                 size_t end = value.find(')', start);
//                 if(end == string::npos)
//                     break;
//                 string posStr = value.substr(start, end - start + 1);
//                 if(!posStr.empty())
//                 {
//                     // Create new Position from string representation (e.g. "(1,2)")
//                     Position* p = new Position(posStr);
//                     arrayForest.push_back(p);
//                 }
//                 start = end + 1;
//             }
//         }
//         else if(key == "ARRAY_RIVER")
//         {
//             if(value.front()=='[' && value.back()==']')
//                 value = value.substr(1, value.size()-2);
//             size_t start = 0;
//             while(true)
//             {
//                 size_t end = value.find(')', start);
//                 if(end == string::npos)
//                     break;
//                 string posStr = value.substr(start, end - start + 1);
//                 if(!posStr.empty())
//                 {
//                     Position* p = new Position(posStr);
//                     arrayRiver.push_back(p);
//                 }
//                 start = end + 1;
//             }
//         }
//         else if(key == "ARRAY_FORTIFICATION")
//         {
//             if(value.front()=='[' && value.back()==']')
//                 value = value.substr(1, value.size()-2);
//             size_t start = 0;
//             while(true)
//             {
//                 size_t end = value.find(')', start);
//                 if(end == string::npos)
//                     break;
//                 string posStr = value.substr(start, end - start + 1);
//                 if(!posStr.empty())
//                 {
//                     Position* p = new Position(posStr);
//                     arrayFortification.push_back(p);
//                 }
//                 start = end + 1;
//             }
//         }
//         else if(key == "ARRAY_URBAN")
//         {
//             if(value.front()=='[' && value.back()==']')
//                 value = value.substr(1, value.size()-2);
//             size_t start = 0;
//             while(true)
//             {
//                 size_t end = value.find(')', start);
//                 if(end == string::npos)
//                     break;
//                 string posStr = value.substr(start, end - start + 1);
//                 if(!posStr.empty())
//                 {
//                     Position* p = new Position(posStr);
//                     arrayUrban.push_back(p);
//                 }
//                 start = end + 1;
//             }
//         }
//         else if(key == "ARRAY_SPECIAL_ZONE")
//         {
//             if(value.front()=='[' && value.back()==']')
//                 value = value.substr(1, value.size()-2);
//             size_t start = 0;
//             while(true)
//             {
//                 size_t end = value.find(')', start);
//                 if(end == string::npos)
//                     break;
//                 string posStr = value.substr(start, end - start + 1);
//                 if(!posStr.empty())
//                 {
//                     Position* p = new Position(posStr);
//                     arraySpecialZone.push_back(p);
//                 }
//                 start = end + 1;
//             }
//         }
//         else if(key == "UNIT_LIST")
//         {
//             // Remove enclosing brackets.
//             if(value.front()=='[' && value.back()==']')
//                 value = value.substr(1, value.size()-2);
            
//             // The units are assumed to be separated by a closing parenthesis.
//             vector<string> unitStrs;
//             string token;
//             for (char ch : value)
//             {
//                 token.push_back(ch);
//                 if(ch == ')')
//                 {
//                     unitStrs.push_back(token);
//                     token.clear();
//                 }
//             }
            
//             for(auto &us : unitStrs)
//             {
//                 // Each unit string is expected as: UNIT_NAME(q,wt,(r,c),armyBelong)
//                 size_t paren = us.find('(');
//                 if(paren == string::npos)
//                     continue;
//                 string unitName = us.substr(0, paren);
//                 string params = us.substr(paren+1, us.size()-paren-2); // remove '(' and ')'
//                 stringstream ss(params);
//                 string part;
//                 vector<string> parts;
//                 while(getline(ss, part, ','))
//                 {
//                     parts.push_back(part);
//                 }
//                 if(parts.size() < 4)
//                     continue;
//                 int q = stoi(parts[0]);
//                 int wt = stoi(parts[1]);
//                 string posStr = parts[2]; // e.g. "(1,2)"
//                 Position pos(posStr);
//                 int armyBelong = stoi(parts[3]);
                
//                 // Create the proper unit.
//                 if(unitName == "TANK" || unitName == "TRUCK" || unitName == "MORTAR" ||
//                    unitName == "ANTIAIRCRAFT" || unitName == "ARMOREDCAR" || unitName == "APC" ||
//                    unitName == "ARTILLERY")
//                 {
//                     VehicleType vt;
//                     if(unitName == "TRUCK")       vt = TRUCK;
//                     else if(unitName == "MORTAR") vt = MORTAR;
//                     else if(unitName == "ANTIAIRCRAFT") vt = ANTIAIRCRAFT;
//                     else if(unitName == "ARMOREDCAR")   vt = ARMOREDCAR;
//                     else if(unitName == "APC")      vt = APC;
//                     else if(unitName == "ARTILLERY") vt = ARTILLERY;
//                     else /*TANK*/                  vt = TANK;
                    
//                     Unit* newUnit = new Vehicle(q, wt, pos, vt);
//                     if(armyBelong == 0)
//                         liberationUnits.push_back(newUnit);
//                     else
//                         ARVNUnits.push_back(newUnit);
//                 }
//                 else  // Assume it's infantry.
//                 {
//                     InfantryType it;
//                     if(unitName == "SNIPER")              it = SNIPER;
//                     else if(unitName == "ANTIAIRCRAFTSQUAD") it = ANTIAIRCRAFTSQUAD;
//                     else if(unitName == "MORTARSQUAD")       it = MORTARSQUAD;
//                     else if(unitName == "ENGINEER")          it = ENGINEER;
//                     else if(unitName == "SPECIALFORCES")     it = SPECIALFORCES;
//                     else /*REGULARINFANTRY*/               it = REGULARINFANTRY;
                    
//                     Unit* newUnit = new Infantry(q, wt, pos, it);
//                     if(armyBelong == 0)
//                         liberationUnits.push_back(newUnit);
//                     else
//                         ARVNUnits.push_back(newUnit);
//                 }
//             }
//         }
//         else if(key == "EVENT_CODE")
//         {
//             int ec = stoi(value);
//             if(ec < 0)
//                 eventCode = 0;
//             else if(ec > 99)
//                 eventCode = ec % 100;
//             else
//                 eventCode = ec;
//         }
//     }
//     fin.close();
// }

// // Destructor: free all dynamically allocated memory.
// Configuration::~Configuration()
// {
//     for(auto p : arrayForest)
//         delete p;
//     arrayForest.clear();
    
//     for(auto p : arrayRiver)
//         delete p;
//     arrayRiver.clear();
    
//     for(auto p : arrayFortification)
//         delete p;
//     arrayFortification.clear();
    
//     for(auto p : arrayUrban)
//         delete p;
//     arrayUrban.clear();
    
//     for(auto p : arraySpecialZone)
//         delete p;
//     arraySpecialZone.clear();
    
//     for(auto u : liberationUnits)
//         delete u;
//     liberationUnits.clear();
    
//     for(auto u : ARVNUnits)
//         delete u;
//     ARVNUnits.clear();
// }

// // Returns a string representation of the configuration in the required format.
// string Configuration::str() const
// {
//     ostringstream oss;
//     oss << "Configuration[" << endl;
//     oss << "num_rows=" << num_rows << "," << endl;
//     oss << "num_cols=" << num_cols << "," << endl;
    
//     oss << "arrayForest=[";
//     for(size_t i = 0; i < arrayForest.size(); i++){
//         oss << arrayForest[i]->str();
//         if(i < arrayForest.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "arrayRiver=[";
//     for(size_t i = 0; i < arrayRiver.size(); i++){
//         oss << arrayRiver[i]->str();
//         if(i < arrayRiver.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "arrayFortification=[";
//     for(size_t i = 0; i < arrayFortification.size(); i++){
//         oss << arrayFortification[i]->str();
//         if(i < arrayFortification.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "arrayUrban=[";
//     for(size_t i = 0; i < arrayUrban.size(); i++){
//         oss << arrayUrban[i]->str();
//         if(i < arrayUrban.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "arraySpecialZone=[";
//     for(size_t i = 0; i < arraySpecialZone.size(); i++){
//         oss << arraySpecialZone[i]->str();
//         if(i < arraySpecialZone.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "liberationUnits=[";
//     for(size_t i = 0; i < liberationUnits.size(); i++){
//         oss << liberationUnits[i]->str();
//         if(i < liberationUnits.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "ARVNUnits=[";
//     for(size_t i = 0; i < ARVNUnits.size(); i++){
//         oss << ARVNUnits[i]->str();
//         if(i < ARVNUnits.size()-1)
//             oss << ",";
//     }
//     oss << "]," << endl;
    
//     oss << "eventCode=" << eventCode << endl;
//     oss << "]";
//     return oss.str();
// }

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

