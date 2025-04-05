#include "tay_nguyen_campaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const int MAX_LINES = 5;         
const int MAX_LINE_LENGTH = 100;
const int MAX_LF_SIZE = 17;

int clamp(int val, int min, int max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

// Helper function to parse LF1, LF2 values
bool parseLF(const string &line, int LF[], int size, int minVal, int maxVal) {
  string processedLine = line;

  // Remove '[' and ']' characters
  for (int i = 0; i < (int)processedLine.size(); i++) {
    if (processedLine[i] == '[' || processedLine[i] == ']') {
      processedLine.erase(i, 1);
      i--;
    }
  }

  stringstream ss(processedLine);
  string val;
  int idx = 0;

  while (getline(ss, val, ',') && idx < size) {
    try {
      int parsedValue = stoi(val);
      LF[idx++] = clamp(parsedValue, minVal, maxVal);
    } catch (...) {
      return false;
    }
  }

  if (idx < size) {
    for (int i = idx; i < size; i++) {
      LF[i] = 0;
    }
  }

  return true;
}

// Helper function to parse EXP1, EXP2, T1, T2, E values
bool parseEXP_T(const string &line, int &val1, int &val2, int minVal, int maxVal) {
  stringstream ss(line);
  if (!(ss >> val1 >> val2)) {
    return false;
  }

  val1 = clamp(val1, minVal, maxVal);
  val2 = clamp(val2, minVal, maxVal);

  return true;
}

bool parseE(const string &line, int &valE, int minVal, int maxVal) {
  stringstream ss(line);
  if (!(ss >> valE)) {
    return false;
  }

  valE = clamp(valE, minVal, maxVal);

  return true;
}

// Task 0: Read input file
bool readFile(
    const string &filename,
    int LF1[], int LF2[],
    int &EXP1, int &EXP2,
    int &T1, int &T2, int &E)
{
  char data[MAX_LINES][MAX_LINE_LENGTH];
  int numLines = 0;

  ifstream ifs(filename);
  if (!ifs.is_open())
  {
    return false;
  }

  while (numLines < MAX_LINES && ifs.getline(data[numLines], MAX_LINE_LENGTH))
  {
    numLines++;
  }
  ifs.close();

  if (numLines < MAX_LINES)
  {
    return false;
  }

  // TODO: Extract values from the `data` array and store them in respective variables

  // Parse LF1, LF2
  if (!parseLF(data[0], LF1, MAX_LF_SIZE, 0, 1000) || !parseLF(data[1], LF2, MAX_LF_SIZE, 0, 1000)) {
    return false;
  }

  // Parse EXP1, EXP2
  stringstream ssEXP(data[2]);
  if (!parseEXP_T(data[2], EXP1, EXP2, 0, 600)) {
    return false;
  }
  
  // Parse T1, T2
  if (!parseEXP_T(data[3], T1, T2, 0, 3000)) {
    return false;
  }

  // Parse E
  if (!parseE(data[4], E, 0, 99)) {
    return false;
  }

  return true;
}

// Task 1
int gatherForces(int LF1[], int LF2[])
{
  // TODO: Implement this function

  int LF = 0;
  int weight[17] = {1, 2, 3, 4, 5, 7, 8, 9, 10,
                    12, 15, 18, 20, 30, 40, 50, 70};
  for (int i = 0; i < 17; i++)
  {
    LF1[i] = clamp(LF1[i], 0, 1000);
    LF2[i] = clamp(LF2[i], 0, 1000);
    LF += (LF1[i] + LF2[i]) * weight[i];
  }
  // cout << LF << " " << LF11 << " " << LF22 << endl;
  return LF;
}

// Task 2
string determineRightTarget(const string &target)
{
  int number[4], count = 0;
  stringstream ss(target);
  string code;
  
  while (ss >> code) {
    string extractedNum = "";
    for (char c : code) {
      if (isdigit(c)) {
        extractedNum += c;
      }
      else if (!extractedNum.empty()) {
        int num = stoi(extractedNum);
        if (num >= 0 && num <= 100) {
          if (count < 4) {
            number[count++] = num;
          }
          else {
            return "INVALID";
          }
        }
        extractedNum = "";
      }
    }
      
    if (!extractedNum.empty()) {
      int num = stoi(extractedNum);
      if (num >= 0 && num <= 100) {
        if (count < 4) {
          number[count++] = num;
        }
        else {
          return "INVALID";
        }
      }
    }
  }

  int id = 0;
  if (count == 1) {
      id = number[0];
      if (id == 3) return "Buon Ma Thuot";
      if (id == 4) return "Duc Lap";
      if (id == 5) return "Dak Lak";
      if (id == 6) return "National Route 21";
      if (id == 7) return "National Route 14";
      if (id >= 0 && id <= 2) return "DECOY";
      return "INVALID"; // Must return something for all cases
  }
  else if (count == 2) {
      id = (number[0] + number[1]) % 5 + 3;
  }
  else if (count == 3) {
      id = max(number[0], max(number[1], number[2])) % 5 + 3;
  }
  else {
    return "INVALID"; // Case: no valid numbers found
  }

  if (id == 3) return "Buon Ma Thuot";
  if (id == 4) return "Duc Lap";
  if (id == 5) return "Dak Lak";
  if (id == 6) return "National Route 21";
  if (id == 7) return "National Route 14";
  return "INVALID";
}

// Helper function for decodeTarget
string validTargets[] = {
  "Buon Ma Thuot", "Duc Lap", "Dak Lak", "National Route 21", "National Route 14"
};

string tolowerCase(const string &s) {
  string result = s;
  for (char &c : result) {
      if (c >= 'A' && c <= 'Z') {
          c = tolower(c);
      }
  }
  return result;
}

// Hàm giải mã Caesar Cipher
string caesarCipherDecode(const string &message, int shift) {
  string decoded;
  for (char c : message) {
      if (!isalnum(c) && c != ' ') return "INVALID"; // Kiểm tra ký tự hợp lệ
      if (isalpha(c)) {
          char base = islower(c) ? 'a' : 'A';
          decoded += (c - base + shift) % 26 + base;
      } else {
          decoded += c;
      }
  }
  return decoded;
}

// Hàm đảo ngược chuỗi
string reverseString(string message) {
  for (char c : message) {
    if (!isalnum(c) && c != ' ') {
      return "INVALID";
    }
  }

  int left = 0, right = message.length() - 1;
  while (left < right) {
      char temp = message[left];
      message[left] = message[right];
      message[right] = temp;
      left++;
      right--;
  }
  return message;
}

// Task 2.2
string decodeTarget(const string &message, int EXP1, int EXP2)
{
  // TODO: Implement this function
  EXP1 = clamp(EXP1, 0, 600); EXP2 = clamp(EXP2, 0, 600);
  string decodedMessage;
    
  if (EXP1 >= 300 && EXP2 >= 300) {
      int shift = (EXP1 + EXP2) % 26;
      decodedMessage = caesarCipherDecode(message, shift);
      // cout << decodedMessage << endl;
      // if (decodedMessage == "INVALID") return "INVALID";
  } else {
      decodedMessage = reverseString(message);
      // cout << decodedMessage << endl;
  }
   
  for (int i = 0; i < 5; i++) {
      if (tolowerCase(decodedMessage) == tolowerCase(validTargets[i])) return validTargets[i];
  }

  return "INVALID";
}

// Task 3
void manageLogistics(int LF1, int LF2, int EXP1, int EXP2, int &T1, int &T2, int E)
{
  // TODO: Implement this function
  LF1 = max(0, LF1); LF2 = max(0, LF2);
  EXP1 = clamp(EXP1, 0, 600); EXP2 = clamp(EXP2, 0, 600);
  T1 = clamp(T1, 0, 3000); T2 = clamp(T2, 0, 3000);
  double deltaT1 = 0, deltaT2 = 0;
  if (E == 0) {
    deltaT1 = (LF1 / (double)(LF1 + LF2) * (T1 + T2)) * (1 + (EXP1 - EXP2) / 100.0);
    deltaT2 = T1 + T2 - deltaT1;
    deltaT1 += T1;
    deltaT2 += T2;
  }
  else if (E >= 1 && E <= 9) {
    deltaT1 = T1 - E*0.01*T1;
    deltaT2 = T2 - E*0.005*T2;
  }
  else if (E >= 10 && E <= 29) {
    deltaT1 = T1 + E*50;
    deltaT2 = T2 + E*50;
  }
  else if (E >= 30 && E <= 59) {
    deltaT1 = T1 + E*0.005*T1;
    deltaT2 = T2 + E*0.002*T2;
  }
  else if (E >= 60 && E <= 99) {
    T1 = clamp(T1, 0, 3000);
    T2 = clamp(T2, 0, 3000);
    return;
  }

  T1 = ceil(deltaT1); T2 = ceil(deltaT2);
  T1 = clamp(T1, 0, 3000);
  T2 = clamp(T2, 0, 3000);
}

// Task 4
int planAttack(int LF1, int LF2, int EXP1, int EXP2, int T1, int T2, int battleField[10][10])
{
  // TODO: Implement this function
  LF1 = max(0, LF1); LF2 = max(0, LF2);
  EXP1 = clamp(EXP1, 0, 600); EXP2 = clamp(EXP2, 0, 600);
  T1 = clamp(T1, 0, 3000); T2 = clamp(T2, 0, 3000);
  double S = (LF1 + LF2) + (EXP1 + EXP2)*5 + (T1 + T2)*2;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (i % 2 == 0) {
        S -= battleField[i][j]*2.0/3.0;
      }
      else {
        S -= battleField[i][j]*3.0/2.0;
      }
    }
  }
  S = ceil(S);
  return S;
}

// Task 5
int resupply(int shortfall, int supply[5][5])
{
  // TODO: Implement this function
  int supplies[25]; // Mảng 1D lưu trữ các giá trị của ma trận supply
  int index = 0;

  // Flatten ma trận supply thành mảng supplies
  for (int i = 0; i < 5; ++i) {
      for (int j = 0; j < 5; ++j) {
          supplies[index++] = supply[i][j];
      }
  }

  // Sort mảng supplies theo thứ tự tăng dần
  for (int i = 0; i < 25; ++i) {
      for (int j = i + 1; j < 25; ++j) {
          if (supplies[i] > supplies[j]) {
              int temp = supplies[i];
              supplies[i] = supplies[j];
              supplies[j] = temp;
          }
      }
  }

  // Tìm tổ hợp 5 ô có tổng giá trị nhỏ nhất và thỏa mãn điều kiện
  int min_total_supply = INT_MAX;

  // Duyệt qua tất cả các tổ hợp 5 ô
  for (int i = 0; i < 21; ++i) {
      for (int j = i + 1; j < 22; ++j) {
          for (int k = j + 1; k < 23; ++k) {
              for (int l = k + 1; l < 24; ++l) {
                  for (int m = l + 1; m < 25; ++m) {
                      // Tính tổng của 5 giá trị
                      int totalSupply = supplies[i] + supplies[j] + supplies[k] + supplies[l] + supplies[m];

                      // Kiểm tra xem tổng có thỏa mãn yêu cầu không
                      if (totalSupply >= shortfall && totalSupply < min_total_supply) {
                          min_total_supply = totalSupply;
                          // cout << supplies[i] << " " << supplies[j] << " " << supplies[k] << " " << supplies[l] << " " << supplies[m] << endl;
                          break;
                      }
                  }
              }
          }
      }
  }

  return (min_total_supply == INT_MAX) ? -1 : min_total_supply;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////