

#include "hcmcampaign.h"
// ──────────────────────────────────────────────────────────────────────────────
// Position class implementation (verbose style with detailed comments)
// ──────────────────────────────────────────────────────────────────────────────

// Primary constructor: initializes row and column from integer parameters
Position::Position(int r, int c)
    : r_(r),  // assign input row to member r_
      c_(c)  // assign input column to member c_
{
    // Empty body: initialization done in initializer list
}

// String constructor: parses a string of the form "(row,col)" into r_ and c_
Position::Position(const std::string& s)
{
    // Use C-style sscanf to extract two integers from the input string
    int parsedRow = 0;
    int parsedCol = 0;
    std::sscanf(s.c_str(), "(%d,%d)", &parsedRow, &parsedCol);
    r_ = parsedRow;  // set member r_
    c_ = parsedCol;  // set member c_
}

// Getter for row component
int Position::getRow() const
{
    return r_;
}

// Getter for column component
int Position::getCol() const
{
    return c_;
}

// Setter for row component
void Position::setRow(int r)
{
    r_ = r;
}

// Setter for column component
void Position::setCol(int c)
{
    c_ = c;
}

// Convert position to string in format "(row,col)"
std::string Position::str() const
{
    // Build string piece by piece for clarity
    std::string result = "(";
    result += std::to_string(r_);
    result += ",";
    result += std::to_string(c_);
    result += ")";
    return result;
}

// Compute Euclidean distance to another Position
double Position::dist(const Position& p) const
{
    // Calculate difference in rows and columns
    int deltaRow    = r_ - p.r_;
    int deltaCol    = c_ - p.c_;
    // Square and sum
    double sqRow    = static_cast<double>(deltaRow * deltaRow);
    double sqCol    = static_cast<double>(deltaCol * deltaCol);
    double sumOfSq  = sqRow + sqCol;
    // Return square root of the summed squares
    return std::sqrt(sumOfSq);
}


// ──────────────────────────────────────────────────────────────────────────────
// Unit base class implementation (with verbose comments)
// ──────────────────────────────────────────────────────────────────────────────

// Constructor: initialize quantity, weight, and position
Unit::Unit(int q, int w, const Position& p)
    : quantity(q),  // set quantity
      weight(w),    // set weight
      pos(p)        // copy-construct pos from p
{
    // No additional logic in constructor body
}

// Virtual destructor: ensure proper cleanup in derived classes
Unit::~Unit()
{
    // Nothing to delete here; no dynamic members
}

// Return a copy of the unit's position
Position Unit::getPos() const
{
    return pos;
}

// Return the current quantity of units
int Unit::getQuantity() const
{
    return quantity;
}

// Scale quantity by factor f, rounding up and clamping to non-negative
void Unit::scaleQuantity(double f)
{
    // Compute scaled value
    double rawScaled = static_cast<double>(quantity) * f;
    int    newScaled = static_cast<int>(std::ceil(rawScaled));

    // Clamp negative values to zero
    if (newScaled < 0) {
        newScaled = 0;
    }
    quantity = newScaled;
}

// Scale weight by factor f, rounding up and clamping to non-negative
void Unit::scaleWeight(double f)
{
    double rawScaledW = static_cast<double>(weight) * f;
    int    newWeight  = static_cast<int>(std::ceil(rawScaledW));

    // Ensure non-negative weight
    if (newWeight < 0) {
        newWeight = 0;
    }
    weight = newWeight;
}


// ──────────────────────────────────────────────────────────────────────────────
// Vehicle class implementation (inheriting from Unit)
// ──────────────────────────────────────────────────────────────────────────────

// Constructor: initialize base Unit and vehicle-specific type
Vehicle::Vehicle(int q, int w, const Position& p, VehicleType t)
    : Unit(q, w, p),  // invoke Unit constructor
      type(t)         // set vehicle type
{
    // No body required
}

// Indicate that this Unit is a vehicle
bool Vehicle::isVehicle() const
{
    return true;
}

// Return the vehicle's type
VehicleType Vehicle::getType() const
{
    return type;
}

// Compute attack score for vehicle: (typeVal*304 + quantity*weight + 29)/30
int Vehicle::getAttackScore() const
{
    int typeVal   = static_cast<int>(type);
    int rawScore  = typeVal * 304 + (quantity * weight);
    // Perform division by 30 with rounding up: (rawScore + (30-1)) / 30
    int roundedUp = (rawScore + 29) / 30;
    return roundedUp;
}

// Return a descriptive string for the Vehicle
std::string Vehicle::str() const
{
    static const char* names[] = {
        "TRUCK", "MORTAR", "ANTIAIRCRAFT",
        "ARMOREDCAR", "APC", "ARTILLERY", "TANK"
    };

    // Build the output string step by step
    std::ostringstream oss;
    oss << "Vehicle[vehicleType="
        << names[static_cast<int>(type)]
        << ",quantity="    << quantity
        << ",weight="      << weight
        << ",position="    << pos.str()
        << "]";
    return oss.str();
}


// ──────────────────────────────────────────────────────────────────────────────
// Infantry class implementation (inheriting from Unit)
// ──────────────────────────────────────────────────────────────────────────────


// Constructor: initialize base Unit, type, and apply personal-number rule
Infantry::Infantry(int q, int w, const Position& p, InfantryType t)
    : Unit(q, w, p),  // call Unit constructor
      type(t)         // set infantry type
{
    // Apply rule to adjust initial quantity if needed
    applyRule();
}

// Indicate that this Unit is not a vehicle
bool Infantry::isVehicle() const
{
    return false;
}

// Return the infantry's type
InfantryType Infantry::getType() const
{
    return type;
}

// Compute attack score for infantry: typeVal*56 + quantity*weight
int Infantry::getAttackScore() const
{
    int typeVal   = static_cast<int>(type);
    int scoreBase = typeVal * 56 + (quantity * weight);
    return scoreBase;
}

// Return a descriptive string for the Infantry
std::string Infantry::str() const
{
    static const char* names[] = {
        "SNIPER", "ANTIAIRCRAFTSQUAD", "MORTARSQUAD",
        "ENGINEER", "SPECIALFORCES",    "REGULARINFANTRY"
    };
    std::ostringstream oss;
    oss << "Infantry[infantryType="
        << names[static_cast<int>(type)]
        << ",quantity="    << quantity
        << ",weight="      << weight
        << ",position="    << pos.str()
        << "]";
    return oss.str();
}

// Apply personal-number adjustment rules to infantry quantity
void Infantry::applyRule()
{
    // Step 1: compute base score
    int typeVal   = static_cast<int>(type);
    int baseScore = typeVal * 56 + (quantity * weight);

    // Step 2: commando bonus (75) if SPECIALFORCES and weight is a perfect square
    int bonus75 = 0;
    if (type == SPECIALFORCES)
    {
        int nonNegWeight = (weight < 0 ? 0 : weight);
        int rootValue    = static_cast<int>(std::sqrt(nonNegWeight) + 0.5);
        if ((rootValue * rootValue) == nonNegWeight)
        {
            bonus75 = 75;
        }
    }

    // Step 3: combine base score and bonus
    int combinedScore = baseScore + bonus75;

    // Step 4: compute personal number (digit-root of combinedScore + digits of 1975)
    int t = combinedScore;

    // Sum digits of year 1975
    int sumOfYear = 0;
    for (int year = 1975; year > 0; year /= 10)
    {
        sumOfYear += (year % 10);
    }
    t += sumOfYear;

    // Reduce to single-digit by repeated digit sums
    while (t >= 10)
    {
        int temp = t;
        int digitSum = 0;
        while (temp > 0)
        {
            digitSum += (temp % 10);
            temp     /= 10;
        }
        t = digitSum;
    }
    int personalNum = t;

    // Step 5: adjust quantity based on personal number
    if (personalNum > 7)
    {
        double factor = 1.2;
        quantity = static_cast<int>(std::ceil(quantity * factor));
    }
    else if (personalNum < 3)
    {
        double factor = 0.9;
        quantity = static_cast<int>(std::ceil(quantity * factor));
    }

    // Step 6: clamp quantity to non-negative
    if (quantity < 0)
    {
        quantity = 0;
    }
}


UnitList::UnitList(int capacity)
  : head(nullptr), tail(nullptr), vCnt(0), iCnt(0)
{
    cap = (capacity < 8) ? 12 : capacity;
}

UnitList::~UnitList() {
    clear();
}

bool UnitList::append(Unit* u) {
    if (!u) return false;

    if (merge(u)) return true;

    Node* node = createNode(u);
    if (!tail) {
        head = tail = node;
    } else {
        tail->next = node;
        tail       = node;
    }

    u->isVehicle() ? ++vCnt : ++iCnt;

    return true;
}

bool UnitList::insert(Unit* u) {
    if (!u) return false;
    if ((vCnt + iCnt) >= cap) return false;
    if (pointerExists(u))     return false;
    if (merge(u))             return true;

    Node* node = createNode(u);
    if (u->isVehicle()) {
        // append
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            tail       = node;
        }
        ++vCnt;
    } else {
        // prepend
        node->next = head;
        head       = node;
        tail       = (tail ? tail : node);
        ++iCnt;
    }
    return true;
}

bool UnitList::isContain(VehicleType vt) const {
    Node* cur = head;
    if (cur) {
        do {
            if (cur->u->isVehicle() &&
                static_cast<Vehicle*>(cur->u)->getType() == vt)
            {
                return true;
            }
            cur = cur->next;
        } while (cur);
    }
    return false;
}

bool UnitList::isContain(InfantryType it) const {
    Node* cur = head;
    if (cur) {
        do {
            if (!cur->u->isVehicle() &&
                static_cast<Infantry*>(cur->u)->getType() == it)
            {
                return true;
            }
            cur = cur->next;
        } while (cur);
    }
    return false;
}

void UnitList::remove(const std::vector<Unit*>& drop) {
    std::size_t idx = 0;
    std::size_t size = drop.size();
    if (size > 0) {
        do {
            deleteFirstMatching(drop[idx]);
            idx++;
        } while (idx < size);
    }
}

std::vector<Unit*> UnitList::extractAll() {
    std::vector<Unit*> out;
    Node* cur = head;
    if (cur) {
        do {
            out.push_back(cur->u);
            cur = cur->next;
        } while (cur);
    }
    clear();
    return out;
}

std::string UnitList::str() const {
    std::ostringstream oss;
    oss << "UnitList[count_vehicle=" << vCnt
        << ";count_infantry="        << iCnt;
    if (head) {
        oss << ";";
        bool first = true;
        Node* cur = head;
        do {
            if (!first) oss << ",";
            first = false;
            oss << cur->u->str();
            cur = cur->next;
        } while (cur);
    }
    oss << "]";
    return oss.str();
}

// private helpers

UnitList::Node* UnitList::createNode(Unit* u) {
    Node* n = new Node{u, nullptr};
    return n;
}

bool UnitList::pointerExists(Unit* u) const {
    Node* cur = head;
    if (cur) {
        do {
            if (cur->u == u) return true;
            cur = cur->next;
        } while (cur);
    }
    return false;
}

void UnitList::deleteFirstMatching(Unit* target) {
    Node* prev = nullptr;
    Node* cur  = head;
    while (cur) {
        if (cur->u == target) {
            Node* nxt = cur->next;
            prev ? (prev->next = nxt) : (head = nxt);
            tail = (cur == tail) ? prev : tail;
            cur->u->isVehicle() ? --vCnt : --iCnt;
            delete cur;
            return;
        }
        prev = cur;
        cur  = cur->next;
    }
}

void UnitList::clear() {
    Node* cur = head;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = tail = nullptr;
    vCnt = iCnt = 0;
}

bool UnitList::merge(Unit* u) {
    if (!u) return false;
    Node* cur = head;
    if (cur) {
        do {
            if (cur->u->isVehicle() == u->isVehicle()) {
                if (u->isVehicle()) {
                    if (mergeVehicle(
                          static_cast<Vehicle*>(cur->u),
                          static_cast<Vehicle*>(u)))
                    {
                        return true;
                    }
                } else {
                    if (mergeInfantry(
                          static_cast<Infantry*>(cur->u),
                          static_cast<Infantry*>(u)))
                    {
                        return true;
                    }
                }
            }
            cur = cur->next;
        } while (cur);
    }
    return false;
}

bool UnitList::mergeVehicle(Vehicle* existing, Vehicle* incoming) {
    bool sameType = (existing->getType() == incoming->getType());
    if (!sameType) return false;
    int sumQ = existing->getQuantity() + incoming->getQuantity();
    existing->scaleQuantity(double(sumQ) / existing->getQuantity());
    return true;
}

bool UnitList::mergeInfantry(Infantry* existing, Infantry* incoming) {
    bool sameType = (existing->getType() == incoming->getType());
    if (!sameType) return false;
    existing->quantity += incoming->getQuantity();
    existing->weight = (incoming->weight > existing->weight)
                       ? incoming->weight
                       : existing->weight;
    
    // personal-number recalculation
    int sc = existing->getAttackScore();
    int t = sc;
    int y = 1975;
    do {
        t += (y % 10);
        y /= 10;
    } while (y != 0);
    
    do {
        int sum = 0;
        int tmp = t;
        do {
            sum += (tmp % 10);
            tmp /= 10;
        } while (tmp != 0);
        t = sum;
    } while (t >= 10);
    
    int pNo = t;
    existing->quantity = (pNo > 7)
                          ? static_cast<int>(std::ceil(existing->quantity * 1.2))
                          : ((pNo < 3)
                             ? static_cast<int>(std::ceil(existing->quantity * 0.9))
                             : existing->quantity);
    existing->quantity = (existing->quantity < 0) ? 0 : existing->quantity;
    return true;
}


Army::Army(const std::string& n)
    : LF(0),
      EXP(0),
      name(n),
      unitList(nullptr),
      battleField(nullptr)
{
}

Army::~Army()
{
    delete unitList;
}

int Army::getLF() const
{
    return LF;
}

int Army::getEXP() const
{
    return EXP;
}

void Army::setLF(int x)
{
    if (x < 0)
    {
        LF = 0;
    }
    else if (x > 1000)
    {
        LF = 1000;
    }
    else
    {
        LF = x;
    }
}

void Army::setEXP(int x)
{
    if (x < 0)
    {
        EXP = 0;
    }
    else if (x > 500)
    {
        EXP = 500;
    }
    else
    {
        EXP = x;
    }
}

UnitList* Army::units() const
{
    return unitList;
}

void Army::update()
{
    // collect all units
    std::vector<Unit*> allUnits = unitList->subset(
        [](Unit* unitPointer) -> bool
        {
            return true;
        }
    );

    int totalLF = 0;
    int totalEXP = 0;

    sumUnits(allUnits, static_cast<std::size_t>(0), totalLF, totalEXP);

    setLF(totalLF);
    setEXP(totalEXP);
}

void Army::sumUnits(const std::vector<Unit*>& v,
                    std::size_t idx,
                    int& lf,
                    int& ex)
{
    if (idx >= v.size())
    {
        return;
    }

    Unit* currentUnit = v[idx];

    if (currentUnit->isVehicle() == true)
    {
        lf = lf + currentUnit->getAttackScore();
    }
    else
    {
        ex = ex + currentUnit->getAttackScore();
    }

    std::size_t nextIndex = idx + static_cast<std::size_t>(1);
    sumUnits(v, nextIndex, lf, ex);
}

// ------------------- BattleField Implementation -------------------

// Constructor: build terrain elements and default roads
BattleField::BattleField(int r, int c,
                         const std::vector<Position*>& f,
                         const std::vector<Position*>& rv,
                         const std::vector<Position*>& fo,
                         const std::vector<Position*>& ub,
                         const std::vector<Position*>& sp)
    : R(r), C(c)
{
    // 1) Place each specified terrain type
    addTerrains<Mountain>(f,  0);
    addTerrains<River>(rv,    0);
    addTerrains<Fortification>(fo, 0);
    addTerrains<Urban>(ub,    0);
    addTerrains<SpecialZone>(sp,   0);

    // 2) Fill any remaining grid cells with Road objects
    fillRoads(0, 0);
}

// Destructor: delete all allocated TerrainElement pointers
BattleField::~BattleField() {
    deleteElems(0);
}

// Apply all terrain effects in sequence to the given army
void BattleField::apply(Army* a) {
    if (a == nullptr) {
        return;  // no army to affect
    }
    _terrain_applying = true;
    applyRec(a, 0);
    _terrain_applying = false;
}

// Stringify battlefield dimensions
std::string BattleField::str() const {
    std::ostringstream oss;
    oss << "BattleField[n_rows=" << R
        << ",n_cols="  << C
        << "]";
    return oss.str();
}

// Retrieve the terrain element at (row, col), or nullptr if none
TerrainElement* BattleField::getElement(int row, int col) const {
    return findElem(row, col, 0);
}

// Recursively scan grid and insert Road objects where no terrain exists
void BattleField::fillRoads(int x, int y) {
    if (x >= R) {
        return; // reached beyond last row
    }
    if (y >= C) {
        // end of column: wrap to next row
        fillRoads(x + 1, 0);
        return;
    }
    // If no terrain at this cell, create a Road
    if (!terrainAt(x, y, 0)) {
        Position pos(x, y);
        Road* road = new Road(pos);
        elems.push_back(road);
    }
    // Continue to next column
    fillRoads(x, y + 1);
}

bool BattleField::terrainAt(int row, int col, std::size_t idx) const {
    if (idx >= elems.size()) {
        return false;  // no more elements to test
    }
    Position p = elems[idx]->getPos();
    if (p.getRow() == row && p.getCol() == col) {
        return true;
    }
    // Test next element
    return terrainAt(row, col, idx + 1);
}

// Delete all terrain elements recursively
void BattleField::deleteElems(std::size_t idx) {
    if (idx >= elems.size()) {
        return;  // done deleting
    }
    delete elems[idx];
    deleteElems(idx + 1);
}


// Recursively apply getEffect() of each terrain element to the army
void BattleField::applyRec(Army* a, std::size_t idx) {
    if (idx >= elems.size()) {
        return;
    }
    elems[idx]->getEffect(a);
    applyRec(a, idx + 1);
}

// Find and return the element at (row, col) or nullptr if not found
TerrainElement* BattleField::findElem(int row, int col, std::size_t idx) const {
    if (idx >= elems.size()) {
        return nullptr;
    }
    Position p = elems[idx]->getPos();
    if (p.getRow() == row && p.getCol() == col) {
        return elems[idx];
    }
    // Continue searching
    return findElem(row, col, idx + 1);
}

using std::vector;
using std::pair;
using std::make_pair;
using std::ostringstream;

// Primary constructor: builds a LiberationArmy with default name
LiberationArmy::LiberationArmy(Unit** arr, int sz, BattleField* b)
  : Army("LIBERATIONARMY"), bf(b)
{
    init(arr, sz);  // common initialization logic
}

// Named constructor: allows custom army name
LiberationArmy::LiberationArmy(Unit** arr, int sz,
                               const std::string& name,
                               BattleField* b)
  : Army(name), bf(b)
{
    init(arr, sz);
}

// Override to indicate this is the Liberation side
bool LiberationArmy::isLiberation() const {
    return true;
}

// Shared initialization: compute indices, set capacity, and insert units
void LiberationArmy::init(Unit** arr, int sz) {
    // 1) Compute total LF (vehicles) and EXP (infantry)
    int totalLF = 0;
    int totalEXP = 0;
    int idx1 = 0;
    if (sz > 0) do {
        Unit* u = arr[idx1];
        if (u->isVehicle()) {
            totalLF  += u->getAttackScore();  // accumulate vehicle score
        } else {
            totalEXP += u->getAttackScore();  // accumulate infantry score
        }
        idx1++;
    } while (idx1 < sz);

    // 2) Determine if the sum S is a "special" number (capacity 12)
    int S = totalLF + totalEXP;
    bool special = (S == 0);
    if (!special) {
        const int bases[] = {3, 5, 7};
        int idx2 = 0;
        do {
            int base = bases[idx2];
            int n = S;
            bool ok = true;
            // Check representation in base
            do {
                if ((n % base) > 1) ok = false;
                n /= base;
            } while (n > 0 && ok);
            if (ok) {
                special = true;
                break;
            }
            idx2++;
        } while (idx2 < 3 && !special);
    }

    // 3) Allocate UnitList: 12 if special, otherwise 8
    if (special) {
        unitList = new UnitList(12);
    } else {
        unitList = new UnitList(8);
    }

    // 4) Insert each unit pointer into the list
    int idx3 = 0;
    if (sz > 0) do {
        unitList->insert(arr[idx3]);  // merge or append/prepend
        idx3++;
    } while (idx3 < sz);

    // 5) Set the initial clamped indices on the base class
    setLF(totalLF);
    setEXP(totalEXP);
}

// Helper to compose the string representation with a custom label
std::string LiberationArmy::formatStr(const std::string& label) const {
    ostringstream oss;
    oss << label
        << "[LF="        << getLF()
        << ",EXP="       << getEXP()
        << ",unitList="  << unitList->str()
        << ",battleField=";
    if (bf) oss << bf->str();  // include battlefield info if present
    oss << "]";
    return oss.str();
}

// Public str(): always use the fixed "LiberationArmy" label
std::string LiberationArmy::str() const {
    return formatStr("LiberationArmy");
}

// Find the minimal subset of units whose combined score >= need
pair<int, vector<Unit*>> LiberationArmy::bestCombo(
    const vector<Unit*>& units, int need)
{
    int n = static_cast<int>(units.size());
    // If there are no units, return "no solution"
    if (n == 0) {
        return make_pair(INT_MAX, vector<Unit*>());
    }

    int best = INT_MAX;
    vector<Unit*> chosen;
    int maxMask = 1 << n;  // total subsets

    int mask = 1;
    if (maxMask > 1) do {
        int sum = 0;
        vector<Unit*> tmp;
        int idx4 = 0;
        // Iterate bits of mask
        if (n > 0) do {
            if (mask & (1 << idx4)) {
                sum += units[idx4]->getAttackScore();
                tmp.push_back(units[idx4]);
            }
            idx4++;
        } while (idx4 < n);

        // If this subset is a new best, remember it
        if (sum >= need && sum < best) {
            best = sum;
            chosen = tmp;
        }
        mask++;
    } while (mask < maxMask);

    return make_pair(best, chosen);
}


// Primary constructor
ARVN::ARVN(Unit** arr, int sz, BattleField* b)
  : Army("ARVN"), bf(b)
{
    init(arr, sz);
}

// Named constructor
ARVN::ARVN(Unit** arr, int sz,
           const std::string& name,
           BattleField* b)
  : Army(name), bf(b)
{
    init(arr, sz);
}

bool ARVN::isLiberation() const {
    return false;
}

std::string ARVN::str() const {
    auto oss = ostringstream{};
    oss << "ARVN"
        << "[LF="         << getLF()
        << ",EXP="        << getEXP()
        << ",unitList="   << unitList->str()
        << ",battleField=";
    if (bf) oss << bf->str();
    oss << "]";
    return oss.str();
}

void ARVN::init(Unit** arr, int sz) {
    // 1) Compute total LF and EXP
    int totalLF  = 0;
    int totalEXP = 0;
    if (sz > 0) {
        int idx = 0;
        do {
            Unit* u = arr[idx];
            if (u->isVehicle()) totalLF += u->getAttackScore();
            else                totalEXP += u->getAttackScore();
            idx++;
        } while (idx < sz);
    }

    // 2) Determine special-number capacity rule
    int S = totalLF + totalEXP;
    bool special = (S == 0);
    if (!special) {
        static const int bases[] = {3, 5, 7};
        int bIdx = 0;
        do {
            int base = bases[bIdx];
            int n = S;
            bool ok = true;
            do {
                if ((n % base) > 1) { ok = false; }
                n /= base;
            } while (n > 0 && ok);
            if (ok) { special = true; break; }
            bIdx++;
        } while (bIdx < 3);
    }

    // 3) Initialize unitList with correct capacity
    unitList = new UnitList(special ? 12 : 8);

    // 4) Insert units into list
    if (sz > 0) {
        int idx = 0;
        do {
            unitList->insert(arr[idx]);
            idx++;
        } while (idx < sz);
    }

    // 5) Set initial LF and EXP
    setLF(totalLF);
    setEXP(totalEXP);
}


// Helper to check application guard
static inline bool shouldApply(Army* a) {
    return _terrain_applying && a;
}

// Lookup tables for terrain parameters


// Fibonacci helper moved out into file scope:
static int fibUp(int x) {
    int a = 1, b = 1;
    while (b < x) {
        int t = a + b;
        a = b; b = t;
    }
    return b;
}

inline void Mountain::getEffect(Army* a) {
    if (!_terrain_applying || !a) return;

    bool isLib = a->isLiberation();
    // Inline radius and percentages rather than from a map
    double radius = isLib ? 2.0 : 4.0;
    double vehPct  = isLib ? 0.10 : 0.05;
    double infPct  = isLib ? 0.30 : 0.20;

    // Collect units and iterate with do-while
    std::vector<Unit*> units = a->units()->subset([](Unit* u){ return true; });
if (!units.empty()) {
    std::size_t idx = 0;
    do {
        Unit* u = units[idx];
        double dist = u->getPos().dist(pos);
        if (u->isVehicle() && dist <= radius) {
            u->scaleWeight(1.0 - vehPct);
        } else if (!u->isVehicle() && dist <= radius) {
            u->scaleWeight(1.0 + infPct);
        }
        idx++;
    } while (idx < units.size());
}

    a->update();
}

inline void River::getEffect(Army* a) {
    if (!shouldApply(a)) return;
    double radius = 2.0;
    int beforeLF = a->getLF();
    int beforeEXP = a->getEXP();
    int dLF = 0;
    int dEXP = 0;

    std::vector<Unit*> units = a->units()->subset([](Unit* u){ return !u->isVehicle(); });
if (!units.empty()) {
    std::size_t idx = 0;
    do {
        Unit* u = units[idx];
        double dist = u->getPos().dist(pos);
        if (dist <= radius) {
            int sc = u->getAttackScore();
            dEXP -= static_cast<int>(sc * 0.10);
        }
        idx++;
    } while (idx < units.size());
}
    a->setLF(beforeLF + dLF);
    a->setEXP(beforeEXP + dEXP);
}

inline void Urban::getEffect(Army* a) {
    if (!shouldApply(a)) return;
    bool isLib = a->isLiberation();
    double radius = isLib ? 5.0 : 3.0;  // was p[0] or p[1]
    double artRad  = 2.0;               // was p[2]

    int beforeLF = a->getLF();
    int beforeEXP = a->getEXP();
    int dLF = 0;
    int dEXP = 0;

    std::vector<Unit*> units = a->units()->subset([](Unit* u){ return true; });
if (!units.empty()) {
    std::size_t idx = 0;
    do {
        Unit* u = units[idx];
        double dist = u->getPos().dist(pos);
        if (!u->isVehicle() && dist <= radius) {
            Infantry* inf = static_cast<Infantry*>(u);
            InfantryType t = inf->getType();
            bool ok = (isLib ? (t==SPECIALFORCES||t==REGULARINFANTRY) : (t==REGULARINFANTRY));
            if (ok) {
                int sc = u->getAttackScore();
                double factor = (isLib ? 2.0 : 1.5);
                dEXP += static_cast<int>(std::ceil(factor * sc / dist));
            }
        }
        if (isLib && u->isVehicle() && dist <= artRad) {
            Vehicle* v = static_cast<Vehicle*>(u);
            if (v->getType() == ARTILLERY) {
                int sc = u->getAttackScore();
                dLF -= static_cast<int>(std::ceil(0.5 * sc));
            }
        }
        idx++;
    } while (idx < units.size());
}
    a->setLF(beforeLF + dLF);
    a->setEXP(beforeEXP + dEXP);
}

inline void Fortification::getEffect(Army* a) {
    if (!shouldApply(a)) return;
    bool isLib = a->isLiberation();
  double radius = 2.0;    // was terrainParams.at("Fortification")[0]
  double pct    = 0.20;   // was terrainParams.at("Fortification")[1]

    int beforeLF = a->getLF();
    int beforeEXP = a->getEXP();
    int dLF=0, dEXP=0;

    std::vector<Unit*> units = a->units()->subset([](Unit* u){ return true; });
if (!units.empty()) {
    std::size_t idx = 0;
    do {
        Unit* u = units[idx];
        double dist = u->getPos().dist(pos);
        if (dist <= radius) {
            int sc = u->getAttackScore();
            int amt = static_cast<int>(sc * pct);
            if (isLib == u->isVehicle()) dLF -= amt;
            else if (!isLib && !u->isVehicle()) dEXP += amt;
            else if (!isLib && u->isVehicle()) dLF += amt;
            else dEXP -= amt;
        }
        idx++;
    } while (idx < units.size());
}
    a->setLF(beforeLF + dLF);
    a->setEXP(beforeEXP + dEXP);
}

inline void SpecialZone::getEffect(Army* a) {
    if (!shouldApply(a)) return;
  double radius = 1.0;    // was terrainParams.at("SpecialZone")[0]

    int beforeLF  = a->getLF();
    int beforeEXP = a->getEXP();
    int dLF=0, dEXP=0;

    std::vector<Unit*> units = a->units()->subset([](Unit* u){ return true; });
if (!units.empty()) {
    std::size_t idx = 0;
    do {
        Unit* u = units[idx];
        double dist = u->getPos().dist(pos);
        if (dist <= radius) {
            int sc = u->getAttackScore();
            if (u->isVehicle()) dLF -= sc;
            else dEXP -= sc;
        }
        idx++;
    } while (idx < units.size());
}
    a->setLF(beforeLF + dLF);
    a->setEXP(beforeEXP + dEXP);
}

// Refactored fight methods for LiberationArmy and ARVN

inline void LiberationArmy::fight(Army* enemy, bool defense) {
    // 1. Define scaling factors for offensive and defensive modes
    const double factorOff = 1.5;
    const double factorDef = 1.3;
    
    // 2. Map defense flag to corresponding scale factor
    bool modes[2] = { false, true };
    double scaleFactors[2] = { factorOff, factorDef };
    int modeIndex = defense ? 1 : 0;
    double scaleLF = scaleFactors[modeIndex];
    double scaleEXP = scaleLF;

    // 3. Compute boosted indices
    int originalLF = getLF();
    int originalEXP = getEXP();
    int boostedLF = static_cast<int>(std::ceil(originalLF * scaleLF));
    int boostedEXP = static_cast<int>(std::ceil(originalEXP * scaleEXP));

    // 4. Defense sequence (modeIndex == 1)
    if (modeIndex == 1) {
        setLF(boostedLF);
        setEXP(boostedEXP);

        // 4a. Immediate hold if both indices meet or exceed enemy's
        if (boostedLF >= enemy->getLF() && boostedEXP >= enemy->getEXP()) {
            return;
        }

        // 4b. Both below → reinforce quantities to next Fibonacci
        if (boostedLF < enemy->getLF() && boostedEXP < enemy->getEXP()) {
            // Fibonacci helper

            // Scale each unit's quantity
            std::vector<Unit*> units = unitList->subset([](Unit* u){ return true; });
            std::size_t idx = 0;
            do {
                Unit* u = units[idx];
                int q = u->getQuantity();
                int fibVal = fibUp(q);
                double factor = static_cast<double>(fibVal) / q;
                u->scaleQuantity(factor);
                idx++;
            } while (idx < units.size());
        } else {
            // 4c. Mixed case → 10% desertion
            std::vector<Unit*> units = unitList->subset([](Unit* u){ return true; });
            std::size_t idx = 0;
            do {
                units[idx]->scaleQuantity(0.9);
                idx++;
            } while (idx < units.size());
        }

        // Finalize defense
        update();
        return;
    }

    // 5. Offensive sequence (modeIndex == 0)
    // 5a. Partition units into infantry and vehicles
    std::vector<Unit*> infUnits = unitList->subset([](Unit* u){ return !u->isVehicle(); });
    std::vector<Unit*> vehUnits = unitList->subset([](Unit* u){ return u->isVehicle(); });

    // 5b. Determine best combinations
    std::pair<int,std::vector<Unit*>> comboIPair = bestCombo(infUnits, enemy->getEXP());
    std::vector<Unit*> comboI = comboIPair.second;
    std::pair<int,std::vector<Unit*>> comboVPair = bestCombo(vehUnits, enemy->getLF());
    std::vector<Unit*> comboV = comboVPair.second;

    bool gotI = !comboI.empty();
    bool gotV = !comboV.empty();

    // 5c. No valid combos → apply weight penalty
    if (!gotI && !gotV) {
        std::vector<Unit*> units = unitList->subset([](Unit* u){ return true; });
        if (!units.empty()) {
        std::size_t idx = 0;
        do {
            units[idx]->scaleWeight(0.9);
            idx++;
        } while (idx < units.size());
    }
        update();
        return;
    }

    // 5d. Win condition mapping
    struct WinCase { bool cond1; bool cond2; };
    WinCase winCases[3] = {
        { gotI && gotV,          false },
        { gotI && boostedLF > enemy->getLF(), false },
        { gotV && boostedEXP > enemy->getEXP(), false }
    };
    bool win = false;
    std::size_t wi = 0;
    do {
        if (winCases[wi].cond1) {
            win = true;
            break;
        }
        wi++;
    } while (wi < 3);

    if (!win) {
        std::vector<Unit*> units = unitList->subset([](Unit* u){ return true; });
        std::size_t idx = 0;
        do {
            units[idx]->scaleWeight(0.9);
            idx++;
        } while (idx < units.size());
        update();
        return;
    }

    // 5e. Victory: remove used combos
    if (gotI) unitList->remove(comboI);
    if (gotV) unitList->remove(comboV);

    // 5f. Partial cleanup
    if (gotI && !gotV) {
        std::vector<Unit*> dropV = unitList->subset([](Unit* u){ return u->isVehicle(); });
        unitList->remove(dropV);
    }
    if (!gotI && gotV) {
        std::vector<Unit*> dropI = unitList->subset([](Unit* u){ return !u->isVehicle(); });
        unitList->remove(dropI);
    }

    // 5g. Confiscate enemy units in reverse order
auto enemyAll = enemy->units()->extractAll();
if (!enemyAll.empty()) {
    long ri = static_cast<long>(enemyAll.size()) - 1;
    do {
        unitList->insert(enemyAll[ri]);
        ri--;
    } while (ri >= 0);
}


    // 5h. Zero-out enemy and update both
    enemy->setLF(0);
    enemy->setEXP(0);
    enemy->update();
    update();
}
inline void ARVN::fight(Army* enemy, bool defense) {
    // defense==false → modeIndex=0 (attack), true → 1 (defense)
    int modeIndex = defense ? 1 : 0;

    if (modeIndex == 0) {
        // Attack: 20% desertion on each unit
        {
            auto units = unitList->subset([](Unit* u){ return true; });
            if (!units.empty()) {
                std::size_t idx = 0;
                do {
                    units[idx]->scaleQuantity(0.8);
                    idx++;
                } while (idx < units.size());
            }
        }

        // Remove any units with quantity ≤ 1
        auto drop = unitList->subset([](Unit* u){ return u->getQuantity() <= 1; });
        unitList->remove(drop);

        update();
        return;
    }

    // Defense: if both LF and EXP have fallen to zero, apply 20% weight penalty
    if (getLF() == 0 && getEXP() == 0) {
        auto units = unitList->subset([](Unit* u){ return true; });
        if (!units.empty()) {
            std::size_t idx = 0;
            do {
                units[idx]->scaleWeight(0.8);
                idx++;
            } while (idx < units.size());
        }
        update();
    }
    // Otherwise (mixed or non-zero indices), ARVN does not change
}

// Constructor: initializes defaults and triggers file parsing
Configuration::Configuration(const std::string& path)
  : num_rows(0), num_cols(0), eventCode(0)
{
    // Parse configuration file line by line
    parseFile(path);
}

// Destructor: frees all dynamically allocated Position* and Unit* vectors
Configuration::~Configuration() {
    cleanupVector(arrayForest);        // clear forest positions
    cleanupVector(arrayRiver);         // clear river positions
    cleanupVector(arrayFortification); // clear fortification positions
    cleanupVector(arrayUrban);         // clear urban positions
    cleanupVector(arraySpecialZone);   // clear special zone positions
    cleanupVector(liberationUnits);    // delete Liberation Army units
    cleanupVector(ARVNUnits);          // delete ARVN units
}

// ===== Public getters for terrain and grid info =====
const std::vector<Position*>& Configuration::getForestPositions() const {
    return arrayForest;
}
const std::vector<Position*>& Configuration::getRiverPositions() const {
    return arrayRiver;
}
const std::vector<Position*>& Configuration::getFortificationPositions() const {
    return arrayFortification;
}
const std::vector<Position*>& Configuration::getUrbanPositions() const {
    return arrayUrban;
}
const std::vector<Position*>& Configuration::getSpecialZonePositions() const {
    return arraySpecialZone;
}

// Transfer ownership of unit lists to caller, leaving internal vectors empty
std::vector<Unit*> Configuration::stealLiberation() {
    return stealUnits(liberationUnits);
}
std::vector<Unit*> Configuration::stealARVN() {
    return stealUnits(ARVNUnits);
}

// Grid dimensions and event code accessors
int Configuration::getNumRows()   const { return num_rows; }
int Configuration::getNumCols()   const { return num_cols; }
int Configuration::getEventCode() const { return eventCode; }

// Dump full configuration state as a single string (for debugging)
std::string Configuration::str() const {
    std::ostringstream oss;
    oss << "[num_rows=" << num_rows
        << ",num_cols="    << num_cols
        << ",arrayForest="      << vecPosStr(arrayForest)
        << ",arrayRiver="       << vecPosStr(arrayRiver)
        << ",arrayFortification="<< vecPosStr(arrayFortification)
        << ",arrayUrban="       << vecPosStr(arrayUrban)
        << ",arraySpecialZone=" << vecPosStr(arraySpecialZone)
        << ",liberationUnits="  << vecUnitStr(liberationUnits)
        << ",ARVNUnits="        << vecUnitStr(ARVNUnits)
        << ",eventCode="        << eventCode
        << "]";
    return oss.str();
}

// ===== Static Helpers =====

// Swap source vector into a temporary and return it, clearing original
std::vector<Unit*> Configuration::stealUnits(std::vector<Unit*>& src) {
    std::vector<Unit*> tmp;
    tmp.swap(src);
    return tmp;
}

// Remove whitespace from both ends of a string
std::string Configuration::trim(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (char ch : s) {
        if (!std::isspace(static_cast<unsigned char>(ch)))
            out.push_back(ch);
    }
    return out;
}

// Convert vector of Position* to a printable string
std::string Configuration::vecPosStr(const std::vector<Position*>& v) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) oss << ",";
        oss << v[i]->str();
    }
    oss << "]";
    return oss.str();
}

// Convert vector of Unit* to a printable string
std::string Configuration::vecUnitStr(const std::vector<Unit*>& v) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) oss << ",";
        oss << v[i]->str();
    }
    oss << "]";
    return oss.str();
}

// ===== Parsing routines =====

// Extract all "(r,c)" entries from a raw string into Position objects
void Configuration::parsePosArray(const std::string& raw,
                                  std::vector<Position*>& dst)
{
    // Find each '(' and matching ')'
    size_t p = raw.find('(');
    while (p != std::string::npos) {
        size_t q = raw.find(')', p);
        if (q == std::string::npos) break;
        // Substring from '(' to ')' inclusive
        std::string token = raw.substr(p, q - p + 1);
        dst.push_back(new Position(token));
        p = raw.find('(', p + 1);
    }
}

// Create a Unit (Vehicle or Infantry) from a token string
Unit* Configuration::makeUnit(const std::string& rawToken) {
    // Locate parentheses enclosing parameters
    size_t lp = rawToken.find('(');
    size_t rp = rawToken.rfind(')');
    if (lp == std::string::npos || rp == std::string::npos) return nullptr;

    // Name is before '(', params inside
    std::string name  = trim(rawToken.substr(0, lp));
    std::string param = rawToken.substr(lp, rp - lp + 1);

    // Parse five integer values: quantity, weight, row, col, army flag
    std::vector<int> nums;
    nums.reserve(5);
    int value = 0;
    int sign = 1;
    bool inNumber = false;
    for (char ch : param) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            value = value * 10 + (ch - '0');
            inNumber = true;
        } else {
            if (inNumber) {
                nums.push_back(sign * value);
                value = 0;
                sign = 1;
                inNumber = false;
            }
            if (ch == '-') sign = -1;
        }
    }
    // Push last number if ended on digit
    if (inNumber) nums.push_back(sign * value);
    if (nums.size() != 5) return nullptr;

    int q = nums[0], w = nums[1], r = nums[2], c = nums[3];
    int belong = nums[4];

    // Match name against known vehicle types
    static const std::vector<std::string> vehNames = {
        "TRUCK","MORTAR","ANTIAIRCRAFT","ARMOREDCAR","APC","ARTILLERY","TANK"
    };
    for (size_t j = 0; j < vehNames.size(); ++j) {
        if (vehNames[j] == name) {
            return new Vehicle(q, w, Position(r,c), static_cast<VehicleType>(j));
        }
    }

    // Match name against infantry types
    static const std::vector<std::string> infNames = {
        "SNIPER","ANTIAIRCRAFTSQUAD","MORTARSQUAD",
        "ENGINEER","SPECIALFORCES","REGULARINFANTRY"
    };
    for (size_t j = 0; j < infNames.size(); ++j) {
        if (infNames[j] == name) {
            return new Infantry(q, w, Position(r,c), static_cast<InfantryType>(j));
        }
    }

    return nullptr; // Unrecognized token
}

// Read lines until closing ']' is found, concatenating them
void Configuration::readArrayLines(const std::string& firstLine,
                                   std::string& collected,
                                   std::ifstream& fin)
{
    collected = firstLine;
    while (collected.find(']') == std::string::npos) {
        std::string tmp;
        if (!std::getline(fin, tmp)) break;
        collected += tmp;
    }
}

// Process one UNIT_LIST token: decide which army it belongs to and store unit
void Configuration::finishToken(const std::string& tok) {
    std::string t = trim(tok);
    if (t.empty()) return;

    // Last number in parentheses indicates army: 0=liberation, 1=ARVN
    size_t lp = t.rfind('(');
    size_t rp = t.rfind(')');
    int belong = 0;
    if (lp != std::string::npos && rp != std::string::npos && lp < rp) {
        std::string inner = t.substr(lp+1, rp-lp-1);
        size_t comma = inner.rfind(',');
        if (comma != std::string::npos) {
            belong = std::stoi(inner.substr(comma+1));
        }
    }

    Unit* u = makeUnit(t);
    if (!u) return;
    if (belong == 0)
        liberationUnits.push_back(u);
    else
        ARVNUnits.push_back(u);
}

// Main file parsing: reads each line, categorizes by prefix, and dispatches
void Configuration::parseFile(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) return; // unable to open file

    // Enumeration of possible line types
    enum LineType {
        LT_NUM_ROWS, LT_NUM_COLS,
        LT_ARRAY_FOREST, LT_ARRAY_RIVER,
        LT_ARRAY_FORT, LT_ARRAY_URBAN,
        LT_ARRAY_SPECIAL, LT_EVENT_CODE,
        LT_UNIT_LIST,  LT_UNKNOWN
    };

    // Map of string prefixes to the corresponding LineType
    static const std::vector<std::pair<std::string, LineType>> prefixMap = {
        {"NUM_ROWS=",            LT_NUM_ROWS},
        {"NUM_COLS=",            LT_NUM_COLS},
        {"ARRAY_FOREST=[",       LT_ARRAY_FOREST},
        {"ARRAY_RIVER=[",        LT_ARRAY_RIVER},
        {"ARRAY_FORTIFICATION=[",LT_ARRAY_FORT},
        {"ARRAY_URBAN=[",        LT_ARRAY_URBAN},
        {"ARRAY_SPECIAL_ZONE=[", LT_ARRAY_SPECIAL},
        {"EVENT_CODE=",          LT_EVENT_CODE},
        {"UNIT_LIST=[",          LT_UNIT_LIST}
    };

    std::string collected;
    std::string line;

    // Read the configuration file line by line
    while (std::getline(fin, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // skip blanks/comments

        // Identify line type by matching prefixes
        LineType lt = LT_UNKNOWN;
        for (size_t i = 0; i < prefixMap.size(); ++i) {
            const std::pair<std::string, LineType>& entry = prefixMap[i];
            if (line.rfind(entry.first, 0) == 0) {
                lt = entry.second;
                break;
            }
        }

        // Dispatch based on identified type
        switch (lt) {
            case LT_NUM_ROWS:
                num_rows = std::stoi(line.substr(9));
                break;

            case LT_NUM_COLS:
                num_cols = std::stoi(line.substr(9));
                break;

            case LT_ARRAY_FOREST:
                // Multi-line array: read until ']' then parse positions
                readArrayLines(line, collected, fin);
                parsePosArray(collected, arrayForest);
                break;

            case LT_ARRAY_RIVER:
                readArrayLines(line, collected, fin);
                parsePosArray(collected, arrayRiver);
                break;

            case LT_ARRAY_FORT:
                readArrayLines(line, collected, fin);
                parsePosArray(collected, arrayFortification);
                break;

            case LT_ARRAY_URBAN:
                readArrayLines(line, collected, fin);
                parsePosArray(collected, arrayUrban);
                break;

            case LT_ARRAY_SPECIAL:
                readArrayLines(line, collected, fin);
                parsePosArray(collected, arraySpecialZone);
                break;

            case LT_EVENT_CODE: {
                // Extract integer after '=' and normalize to [0,99]
                int raw = std::stoi(line.substr(11));
                eventCode = (raw < 0 ? 0 : raw % 100);
                break;
            }

            case LT_UNIT_LIST: {
                // Read entire bracketed UNIT_LIST and split into tokens
                readArrayLines(line, collected, fin);
                size_t bOpen  = collected.find('[');
                size_t bClose = collected.rfind(']');
                if (bOpen == std::string::npos || bClose == std::string::npos)
                    break;

                std::string body = collected.substr(bOpen+1, bClose-bOpen-1);
                std::string token;
                int depth = 0;

                // Iterate characters to split on commas at depth 0
                for (size_t idx = 0; idx < body.size(); ++idx) {
                    char ch = body[idx];
                    if (ch == '(') ++depth;
                    else if (ch == ')') --depth;

                    bool splitHere = (ch == ',' && depth == 0) ||
                                     (depth == 0 && !token.empty() && token.back() == ')' &&
                                      std::isupper(static_cast<unsigned char>(ch)));

                    if (splitHere) {
                        finishToken(token);  // process completed token
                        token.clear();
                        if (std::isupper(static_cast<unsigned char>(ch))) {
                            token.push_back(ch);
                        }
                    } else {
                        token.push_back(ch);
                    }
                }
                finishToken(token); // final token
                break;
            }

            case LT_UNKNOWN:
            default:
                // Unrecognized line → skip
                break;
        }
    }
}

// Helper: Purge units with attackScore <= threshold from an army
static void purgeArmy(Army* army, int threshold) {
    // Collect pointers to remove
    std::vector<Unit*> toRemove;
    // Use forEach to iterate units; lambda remains since no for-loop here
    army->units()->forEach([&](Unit* u) {
        if (u->getAttackScore() <= threshold) {
            toRemove.push_back(u);
        }
    });
    // Remove collected units and update indices
    army->units()->remove(toRemove);
    army->update();
}

// Constructor: load config, build battlefield, armies
HCMCampaign::HCMCampaign(const std::string& path) {
    // 1) Load configuration from file
    cfg = new Configuration(path);

    // 2) Build battlefield from config data
    bf = new BattleField(
        cfg->getNumRows(), cfg->getNumCols(),
        cfg->getForestPositions(), cfg->getRiverPositions(),
        cfg->getFortificationPositions(),
        cfg->getUrbanPositions(), cfg->getSpecialZonePositions()
    );

    // 3) Extract raw unit lists for each side
    std::vector<Unit*> libVec = cfg->stealLiberation();
    std::vector<Unit*> arvnVec = cfg->stealARVN();

    // 4) Convert std::vector to C-style arrays
    std::size_t libSize = libVec.size();
    Unit** libArr = new Unit*[ std::max<std::size_t>(1, libSize) ];
    if (libSize > 0) {
        // “Weird” loop: increment inside the expression
        for (std::size_t idx = 0; idx < libSize; libArr[idx] = libVec[idx], ++idx) { }
    }
    // if libSize == 0, we leave libArr[0] uninitialized (size=1)

    std::size_t arvnSize = arvnVec.size();
    Unit** arvArr = new Unit*[ std::max<std::size_t>(1, arvnSize) ];
    if (arvnSize > 0) {
        // Traditional for-loop, but decrementing from the end
        for (std::size_t idx = 0; idx < arvnSize; ++idx) {
            arvArr[idx] = arvnVec.at(idx);
        }
    }
    // if arvnSize == 0, arvArr[0] remains undefined

    // 5) Instantiate armies
    lib  = new LiberationArmy(libArr, static_cast<int>(libSize), bf);
    arvn = new ARVN(arvArr, static_cast<int>(arvnSize), bf);

    // 6) Clean up temporary C-arrays
    delete[] libArr;
    delete[] arvArr;
}


// Destructor: release all allocated resources
HCMCampaign::~HCMCampaign() {
    delete cfg;
    delete bf;
    delete lib;
    delete arvn;
}

// Run the full simulation: terrain then battle, followed by purging
void HCMCampaign::run() {
    // 1) Apply terrain effects
    bf->apply(lib);
    bf->apply(arvn);

    // 2) Determine attacker by event code
    int ev = cfg->getEventCode();
    bool liberationAttacks = (ev < 75);

    // 3) Define possible actions using a simple enum and array of vectors
    enum Action { ATTACK_LIB, ATTACK_ARV };
    static const std::vector<Action> sequences[2] = {
        // [0]: ARVN attacks, then Liberation counterattacks
        { ATTACK_ARV, ATTACK_LIB },
        // [1]: Liberation attacks only
        { ATTACK_LIB }
    };

    // Select and execute the appropriate sequence
    int key = liberationAttacks ? 1 : 0;
    for (std::size_t i = 0; i < sequences[key].size(); ++i) {
        Action act = sequences[key][i];
        if (act == ATTACK_LIB) {
            lib->fight(arvn, false);
        } else {
            arvn->fight(lib, false);
        }
    }

    // 4) Purge any unit with attackScore <= 5 from both armies
    ::purgeArmy(lib, 5);
    ::purgeArmy(arvn, 5);

}

// Print the final LF and EXP of both armies
std::string HCMCampaign::printResult() const {
    std::ostringstream oss;
    oss << "LIBERATIONARMY[LF=" << lib->getLF()
        << ",EXP=" << lib->getEXP() << "]-"
        << "ARVN[LF=" << arvn->getLF()
        << ",EXP=" << arvn->getEXP() << "]";
    return oss.str();
}
