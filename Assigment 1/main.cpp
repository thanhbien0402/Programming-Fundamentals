/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2025
* Date: 07.02.2025
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "tay_nguyen_campaign.h"

using namespace std;

void tnc_tc_01() {
    cout << "----- Sample Testcase 01 -----" << endl;
    string input_file("tnc_tc_01_input");
    int LF1[17], LF2[17], EXP1, EXP2, T1, T2, E;
    if (!readFile(input_file, LF1, LF2, EXP1, EXP2, T1, T2, E)) {
        return;
    }

    cout << "LF1[0]: " << LF1[0] << ", LF2[0]: " << LF2[0] << endl;
    int result = gatherForces(LF1, LF2);
    cout << "Result: " << result << endl;
}

void testcase01 () {
    // Testcase 1
    cout << "----- Sample Testcase 01 -----" << endl;
    {
        string input_file("tnc_tc_01_input");
        int LF1[17], LF2[17], EXP1, EXP2, T1, T2, E;

        if (!readFile(input_file, LF1, LF2, EXP1, EXP2, T1, T2, E))
        {
            cout << "Error: Could not read file." << endl;
            return ;
        }

        // Print all elements of LF1
        cout << "LF1: ";
        for (int i = 0; i < 17; i++)
            cout << LF1[i] << " ";
        cout << endl;

        // Print all elements of LF2
        cout << "LF2: ";
        for (int i = 0; i < 17; i++)
            cout << LF2[i] << " ";
        cout << endl;

        // Print other values
        cout << "EXP1: " << EXP1 << ", EXP2: " << EXP2 << endl;
        cout << "T1: " << T1 << ", T2: " << T2 << endl;
        cout << "E: " << E << endl;
    }
}

void testcase02 () {
    // Testcase 2
    cout << "----- Sample Testcase 02 -----" << endl;
    {
        string input_file("tnc_tc_01_input");
        int LF1[17], LF2[17], EXP1, EXP2, T1, T2, E;

        if (!readFile(input_file, LF1, LF2, EXP1, EXP2, T1, T2, E))
        {
            cout << "Error: Could not read file." << endl;
            return ;
        }

        // Print all elements of LF1
        cout << "LF1: ";
        for (int i = 0; i < 17; i++)
            cout << LF1[i] << " ";
        cout << endl;

        // Print all elements of LF2
        cout << "LF2: ";
        for (int i = 0; i < 17; i++)
            cout << LF2[i] << " ";
        cout << endl;

        // Print other values
        cout << "EXP1: " << EXP1 << ", EXP2: " << EXP2 << endl;
        cout << "T1: " << T1 << ", T2: " << T2 << endl;
        cout << "E: " << E << endl;

        // Test gatherForces
        int result = gatherForces(LF1, LF2);
        cout << "Result: " << result << endl;
    }
}

void testcase03 () {
    // Testcase 3
    cout << "----- Sample Testcase 03 -----" << endl;
    {
    string encodedTarget1 = "Ple9i ku5";
    string encodedTarget2 = "Kon1 Tu4m8";
    string encodedTarget3 = "Ko1n Tum"; //Self-Test

    cout << "Decoded Target 1: " << determineRightTarget(encodedTarget1) << endl;
    cout << "Decoded Target 2: " << determineRightTarget(encodedTarget2) << endl;
    cout << "Decoded Target 3: " << determineRightTarget(encodedTarget3) << endl;
    }
}

void testcase04 () {
    //Testcase 4
    cout << "----- Sample Testcase 04 -----" << endl;
    {
        int EXP1 = 350;
        int EXP2 = 400;
        string encodedMessage = "Heo Peo";
        string decodedTarget = decodeTarget(encodedMessage, EXP1, EXP2);
        cout << "Decoded Right Target: " << decodedTarget << endl;

        EXP1 = 150;
        EXP2 = 320;

        encodedMessage = "Pal cUd";
        decodedTarget = decodeTarget(encodedMessage, EXP1, EXP2);
        cout << "Decoded Target: " << decodedTarget << endl;

        EXP1 = 598;
        EXP2 = 1000;
        encodedMessage = "BYi jyI";
        decodedTarget = decodeTarget(encodedMessage, EXP1, EXP2);
        cout << "Decoded Target: " << decodedTarget << endl;

        EXP1 = 596;
        EXP2 = 1000;
        encodedMessage = "National RoutE 21";
        decodedTarget = decodeTarget(encodedMessage, EXP1, EXP2);
        cout << "Decoded Target: " << decodedTarget << endl;
    }
}

void testcase05 () {
    //Testcase 5
    cout << "----- Sample Testcase 05 -----" << endl;
    {
        int LF1[17] = {250, 200, 150, 100, 80, 50, 30, 20, 10, 5, 2, 1, 1, 1, 1, 0, 0};
        int LF2[17] = {300, 250, 200, 150, 100, 80, 50, 30, 20, 10, 5, 2, 1, 1, 1, 0, 0};
        int EXP1 = 400, EXP2 = 450;
        int T1 = 2800, T2 = 3000;
        int E = 5;

        // Call function
        manageLogistics(LF1[0], LF2[0], EXP1, EXP2, T1, T2, E);

        // Print the modified supply values
        cout << "T1: " << T1 << ", T2: " << T2 << endl;
    }
}

void testcase06 () {
    //Testcase 6
    cout << "----- Sample Testcase 06 -----" << endl;
    {
        int LF1 = 300, LF2 = 280;
        int EXP1 = 450, EXP2 = 470;
        int T1 = 2500, T2 = 2600;

        int battleField[10][10] = {
            {106, 15, 20, 25, 305, 635, 540, 145, 50, 55},
            {25, 18, 24, 330, 36, 442, 48, 54, 660, 665},
            {14, 21, 28, 35, 452, 49, 56, 63, 70, 77},
            {162, 24, 323, 404, 484, 60, 40, 72, 80, 88},
            {181, 27, 36, 52, 543, 63, 72, 81, 90, 99},
            {5, 30, 40, 501, 602, 70, 80, 90, 100, 110},
            {22, 33, 442, 55, 66, 77, 58, 99, 10, 121},
            {24, 36, 48, 60, 72, 84, 96, 108, 20, 132},
            {264, 39, 525, 65, 78, 91, 104, 70, 130, 143},
            {28, 42, 56, 50, 84, 98, 125, 126, 140, 154}};

        int result = planAttack(LF1, LF2, EXP1, EXP2, T1, T2, battleField);

        cout << "Result: " << result << endl;
    }
}
void testcase06_b () {
    //Testcase 6 - B - Expample 4.11
    cout << "----- Sample Testcase 06 - B -----" << endl;
    {
        int LF1 = 300, LF2 = 280;
        int EXP1 = 450, EXP2 = 470;
        int T1 = 2500, T2 = 2600;

        int battleField[10][10] = {
            {506,15,20,25,305,635,540,145,50,55},
            {325,18,24,330,36,442,48,54,660,665},
            {14,21,28,35,452,49,56,63,70,77},
            {562,24,323,404,484,60,40,72,80,88},
            {181,27,36,52,543,63,72,81,90,99},
            {505,30,40,501,602,70,80,90,100,110},
            {922,33,442,55,66,77,58,99,110,121},
            {124,36,48,60,72,84,96,108,120,132},
            {264,39,525,65,78,91,104,170,130,143},
            {28,42,56,50,84,98,125,126,140,154}
        };

        int result = planAttack(LF1, LF2, EXP1, EXP2, T1, T2, battleField);

        cout << "Result: " << result << endl;
    }
}
void testcase07 () {
    //Testcase 7
    cout << "----- Sample Testcase 07 -----" << endl;
    {
        int shortfall = 1050;
        int supply[5][5] = {
            {150, 200, 180,  90, 110},
            { 70,  80, 120, 140, 160},
            {220, 240, 200, 190, 130},
            {100, 110, 300, 280, 320},
            {170, 210, 260, 230, 290}
        };

        int result = resupply(shortfall, supply);

        cout << "Result: " << result << endl;
    }
}

void testcase08 () {
    //Testcase 8
    cout << "----- Sample Testcase 09 -----" << endl;
    {
        int shortfall = 30;
        int supply[5][5] ={
          {5,5,5,5,5},       // tổng 25
          {10,10,10,10,10},   // tổng 50
          {20,20,20,20,20},   // tổng 100
          {1,1,1,1,1},        // tổng 5
          {2,2,2,2,2}         // tổng 10
        };

        int result = resupply(shortfall, supply);

        cout << "Result: " << result << endl;
    }
}

void testcase09 () {
    //Testcase 9
    cout << "----- Sample Testcase 09 -----" << endl;
    {
        int shortfall = 50;
        int supply[5][5] ={
          {0,0,0,0,0},
          {10,20,30,40,50},
          {5,15,25,35,45},
          {7,14,21,28,35},
          {2,4,6,8,10}
        };

        int result = resupply(shortfall, supply);

        cout << "Result: " << result << endl;
    }
}

void testcase10 () {
    //Testcase 10
    cout << "----- Sample Testcase 10 -----" << endl;
    {
        int shortfall = 26;
        int supply[5][5] ={
          {1,1,1,1,1},
          {1,1,1,1,1},
          {1,1,1,1,1},
          {1,1,1,1,1},
          {1,1,1,1,1}
        };

        int result = resupply(shortfall, supply);

        cout << "Result: " << result << endl;
    }
}

void testcase11 () {
    //Testcase 11
    cout << "----- Sample Testcase 11 -----" << endl;
    {
        int shortfall = 2400;
        int supply[5][5] = {{100, 100, 100, 100, 100},
                            {100, 100, 100, 100, 100},
                            {100, 100, 100, 100, 100},
                            {100, 100, 100, 100, 100},
                            {100, 100, 100, 100, 100}};

        int result = resupply(shortfall, supply);
        
        cout << "Result: " << result << endl;
    }
}

int main(int argc, const char * argv[]) {
    tnc_tc_01();
    // cout << determineRightTarget("Kon3 Tum") << endl;
    // cout << determineRightTarget("P3leiku18") << endl;
    // cout << determineRightTarget("NaTiOnAl RoUtE 14") << endl;
    // cout << determineRightTarget("Ple1Ku4Nat9") << endl;
    // cout << determineRightTarget("KonTum") << endl;
    // cout << determineRightTarget("Buon1Ma3Thuot5-8") << endl;
    // cout << decodeTarget("Pal cUd", 150, 320) << endl;
    // int LF1 = 300, LF2 = 280;
    // int EXP1 = 450, EXP2 = 470;
    // int T1 = 2500, T2 = 2600;
    // int E = 20; // Mã sự kiện
    // int battleField[10][10] = {
    //     {506, 15, 20, 25, 305, 635, 540, 145, 50, 55},
    //     {325, 18, 24, 330, 36, 442, 48, 54, 660, 665},
    //     {14, 21, 28, 35, 452, 49, 56, 63, 70, 77},
    //     {562, 24, 323, 404, 484, 60, 40, 72, 80, 88},
    //     {181, 27, 36, 52, 543, 63, 72, 81, 90, 99},
    //     {505, 30, 40, 501, 602, 70, 80, 90, 100, 110},
    //     {922, 33, 442, 55, 66, 77, 58, 99, 110, 121},
    //     {124, 36, 48, 60, 72, 84, 96, 108, 120, 132},
    //     {264, 39, 525, 65, 78, 91, 104, 170, 130, 143},
    //     {28, 42, 56, 50, 84, 98, 125, 126, 140, 154}
    // };
    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         cin >> battleField[i][j];
    //     }
    // }
    // for (int i = 0; i < 10; i++) {
    //     for (int j = 0; j < 10; j++) {
    //         cout << battleField[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << planAttack(LF1, LF2, EXP1, EXP2, T1, T2, battleField) << endl;
    // manageLogistics(LF1, LF2, EXP1, EXP2, T1, T2, E);
    // cout << "T1: " << T1 << ", T2: " << T2 << endl;
    // int supply[5][5] = {
    //     {150, 200, 180, 90, 110},
    //     {70, 80, 120, 140, 160},
    //     {220, 240, 200, 190, 130},
    //     {100, 110, 300, 280, 320},
    //     {170, 210, 260, 230, 290}
    // };

    // int shortfall = 1278;

    // int result = resupply(shortfall, supply);

    // cout << "Total supply: " << result << endl; // Output should be 1050

    testcase01();
    testcase02();
    testcase03();
    testcase04();
    testcase05();
    testcase06();
    testcase06_b();
    testcase07();
    // testcase08();
    // testcase09();
    // testcase10();
    // testcase11();
    return 0;
}
