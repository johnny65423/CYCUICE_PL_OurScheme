
#include <cstdlib>
#include <stdlib.h>
#include <new>
#include <cstring>
#include <iostream>	                                            // cout, endl
#include <fstream>                                              // open, is_open
#include <string.h>                                               // string
#include <stdio.h>
#include <vector>                                               // vector
#include <cstdlib>                                              // atoi, system
#include <iomanip>                                              // setw
#include <ctime>                                                // clock, CLOCKS_PER_SEC
#include <algorithm>
#include <string>
#define PAGE_SIZE   30                                          // number of output per page

using namespace std;

bool readProblem();
bool readTable();
void work();
void writeAnswer();

string   fileName ;
char problem[100] [100];
char answer[100] [100];
char table1[101] [101];
char table2[101] [101];
char table3[101] [101];
char table4[101] [101];
char table5[100] [101];
char table6[100] [101];
char table7[100] [101];
int aP, aT1, aT2, aT3, aT4 = 0;
int xA = 0;
int yA = 0;
int yT4 = 0;
int reg = 0;
bool sign = false;

int main(void){
    int i = 0;

    if ( readProblem() == true ){
        readTable();

        while( i < 100 ){
            table5[i][0] = '-';
            table6[i][0] = '-';
            table7[i][0] = '-';
            i = i + 1;
        }

        work();

        writeAnswer();

    }  // if problem true

} // main

void writeAnswer(){
    int i = 0;
    fstream outFile;
    string fname = fileName + "-output.txt";
    outFile.open( fname.c_str(),fstream::out );

    if( !outFile.is_open() ){
        cout << endl << "## Can't create " << fname << " ! ##"<< endl;
        return;
    }

    while( i <= aP ){
        outFile << problem[i] << "\n";
        outFile << answer[i] << "\n";
        i = i + 1;
    }

    outFile.close();
}

bool check( char a, char b ){
    if( a <= 'z' && a >= 'a' ){
        if( b <= 'Z' && b >= 'A' )
            b = b + 32;
    }

    if( a <= 'Z' && a >= 'A' ){
        if( b <= 'z' && b >= 'a' )
            a = a + 32;
    }

    if( a == b )
        return true;
    else
        return false;
}


void work(){
    int yP = 0;
    int xP = 0;
    int xT = 0;
    int yT = 1;
    bool right = false;

    while( yP <= aP ){

        while( xP < strlen( problem[yP] ) ){             //切第一條字串

            if( problem[yP][xP] == ' ' || problem[yP][xP] == '\t' || problem[yP][xP] == '.' ){
                if( problem[yP][xP] == '.' ){
                    xP = 1000000;   //  problem換行
                    answer[yA][xA] = '(';
                    answer[yA][xA+1] = '4';
                    answer[yA][xA+2] = ',';
                    answer[yA][xA+3] = '1';
                    answer[yA][xA+4] = '0';
                    answer[yA][xA+5] = ')';
                    xA = xA + 6;
                }
                else
                    xP = xP + 1;

            }
            else{    // 如果不是空白，要開始找table位置
                reg = xP;
                yT = 1;

                // table3 有 X 所以要判斷是不是X' '的形式(這樣是數字，要去table6)

                if( problem[yP][xP] == 'x' || problem[yP][xP] == 'X' ){
                    if( xP < strlen( problem[yP] ) && problem[yP][xP+1] == '\''  )
                        yT = 100000;   // 直接不判斷是否在table3
                }

                while( right == false && yT <= aT3 ){    // 找是不是在table3
                    if( check( problem[yP][xP], table3[yT][xT] ) ){ //如果第一個字母一樣，往後比看是不是同一個字串
                        right = true;
                        xP = xP + 1;
                        xT = xT + 1;
                        while( sign == false && right == true && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){

                            if( xT == strlen( table3[yT] ) ){   // 題目資料已超出table該框內容，看是不是標點符號
                                yT4 = 1;
                                right = false;
                                while( right == false && yT4 <= aT4 ){    // 找是不是在table4

                                    if( problem[yP][xP] == table4[yT4][0] ){ //如果第一個字母一樣，就正確
                                        right = true;
                                        sign = true;
                                    }
                                    else                                 //第一個字母不一樣，table要往下找
                                        yT4 = yT4 + 1;
                                }
                            }
                            else {
                                if( !check( problem[yP][xP], table3[yT][xT] ) ) // 若非table1內容
                                    right = false;

                            }

                            xP = xP + 1;
                            xT = xT + 1;
                        }

                        if( right == false ){
                            yT = yT + 1;
                            xT = 0;
                        }
                        else{

                            answer[yA][xA] = '(';
                            answer[yA][xA+1] = '3';
                            answer[yA][xA+2] = ',';
                            if( yT > 99 ){
                                answer[yA][xA+3] = yT/100+48;
                                answer[yA][xA+4] = (yT-100)/10+48;
                                answer[yA][xA+5] = (yT-100)%10+48;
                                answer[yA][xA+6] = ')';
                                xA = xA + 7;
                            }
                            else{
                                if( yT > 9 ){
                                    answer[yA][xA+3] = yT/10+48;
                                    answer[yA][xA+4] = yT%10+48;
                                    answer[yA][xA+5] = ')';
                                    xA = xA + 6;
                                }
                                else{
                                    answer[yA][xA+3] = yT+48;
                                    answer[yA][xA+4] = ')';
                                    xA = xA + 5;
                                }
                            }

                            if( sign == true ){
                                answer[yA][xA] = '(';
                                answer[yA][xA+1] = '4';
                                answer[yA][xA+2] = ',';

                                if( yT4 > 99 ){
                                    answer[yA][xA+3] = yT4/100+48;
                                    answer[yA][xA+4] = (yT4-100)/10+48;
                                    answer[yA][xA+5] = (yT4-100)%10+48;
                                    answer[yA][xA+6] = ')';
                                    xA = xA + 7;
                                }
                                else{
                                    if( yT4 > 9 ){
                                        answer[yA][xA+3] = yT4/10+48;
                                        answer[yA][xA+4] = yT4%10+48;
                                        answer[yA][xA+5] = ')';
                                        xA = xA + 6;
                                    }
                                    else{
                                        answer[yA][xA+3] = yT4+48;
                                        answer[yA][xA+4] = ')';
                                        xA = xA + 5;
                                    }

                                }

                            }
                        }
                    }
                    else {                                //第一個字母不一樣，table要往下找
                            yT = yT + 1;
                            xT = 0;
                            xP = reg;
                    }
                }

                if( right == true ){                       // 成功找到table位置了，往右找其他字串位置
                    if( sign == true ){                             //此位置是table4標點符號
                        if( table4[yT4][0] == '.' )
                             xP = 1000000;   //  problem換行
                        else
                            xP = xP ;
                        sign = false;

                    }
                    else
                        xP = xP + 1;
                    yT = 1;
                    xT = 0;
                    right = false;
                }
                else{
                    xP = reg;
                    yT = 1;                                 //table3沒有，要找找看其他table
                    while( right == false && yT <= aT1 ){    // 找是不是在table1

                        if( check( problem[yP][xP], table1[yT][xT] ) ){ //如果第一個字母一樣，往後比看是不是同一個字串
                            right = true;

                            while(  sign == false && right == true && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){

                                if( xT == strlen( table1[yT] ) ) {
                                    yT4 = 1;
                                    right = false;
                                    while( right == false && yT4 <= aT4 ){

                                        if( problem[yP][xP] == table4[yT4][0] ){ //如果第一個字母一樣，就正確
                                            right = true;
                                            sign = true;
                                        }
                                        else                                 //第一個字母不一樣，table要往下找
                                            yT4 = yT4 + 1;
                                    }
                                }
                                else {
                                    if( !check( problem[yP][xP], table1[yT][xT] ) )
                                        right = false;

                                }

                                xP = xP + 1;
                                xT = xT + 1;
                            }
                            if( right == false ){
                                yT = yT + 1;
                                xT = 0;
                                xP = reg;
                            }
                            else{

                                answer[yA][xA] = '(';
                                answer[yA][xA+1] = '1';
                                answer[yA][xA+2] = ',';
                                if( yT > 99 ){
                                        answer[yA][xA+3] = yT/100+48;
                                        answer[yA][xA+4] = (yT-100)/10+48;
                                        answer[yA][xA+5] = (yT-100)%10+48;
                                        answer[yA][xA+6] = ')';
                                        xA = xA + 7;
                                }
                                else{
                                    if( yT > 9 ){
                                        answer[yA][xA+3] = yT/10+48;
                                        answer[yA][xA+4] = yT%10+48;
                                        answer[yA][xA+5] = ')';
                                        xA = xA + 6;
                                    }
                                    else{
                                        answer[yA][xA+3] = yT+48;
                                        answer[yA][xA+4] = ')';
                                        xA = xA + 5;
                                    }
                                }

                                if( sign == true ){
                                    xP = xP - 1;
                                    answer[yA][xA] = '(';
                                    answer[yA][xA+1] = '4';
                                    answer[yA][xA+2] = ',';
                                    if( yT4 > 99 ){
                                        answer[yA][xA+3] = yT4/100+48;
                                        answer[yA][xA+4] = (yT4-100)/10+48;
                                        answer[yA][xA+5] = (yT4-100)%10+48;
                                        answer[yA][xA+6] = ')';
                                        xA = xA + 7;
                                    }
                                    else{
                                        if( yT4 > 9 ){
                                            answer[yA][xA+3] = yT4/10+48;
                                            answer[yA][xA+4] = yT4%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = yT4+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }
                                    }
                                }
                            }
                        }
                        else {                                //第一個字母不一樣，table要往下找
                            yT = yT + 1;
                            xT = 0;
                            xP = reg;
                        }
                    }

                    if( right == true ){                       // 成功找到table位置了，往右找其他字串位置
                        if( sign == true ){                             //此位置是table4標點符號
                            if( table4[yT4][0] == '.' )
                                 xP = 1000000;   //  problem換行
                            else
                                xP = xP ;
                            sign = false;

                        }
                        else
                            xP = xP + 1;

                        yT = 1;
                        xT = 0;
                        right = false;
                    }
                    else{                                    //table1沒有，要找找看其他table

                        xP = reg;
                        yT = 1;
                        while( right == false && yT <= aT2 ){    // 找是不是在table2

                            if( check( problem[yP][xP], table2[yT][xT] ) ){ //如果第一個字母一樣，往後比看是不是同一個字串
                                right = true;

                                while( sign == false && right == true && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){

                                    if( xT == strlen( table2[yT] ) ){   // 題目資料已超出table該框內容，看是不是標點符號
                                        yT4 = 1;
                                        right = false;
                                        while( right == false && yT4 <= aT4 ){    // 找是不是在table4

                                            if( problem[yP][xP] == table4[yT4][0] ){ //如果第一個字母一樣，就正確
                                                right = true;
                                                sign = true;
                                            }
                                            else                                 //第一個字母不一樣，table要往下找
                                                yT4 = yT4 + 1;
                                        }
                                    }
                                    else {
                                        if( check( problem[yP][xP], table2[yT][xT] ) == false )
                                            right = false;
                                            //cout << problem[yP][xP]<< "   "<< table2[yT][xT]<< "\n";
                                    }

                                    xP = xP + 1;
                                    xT = xT + 1;
                                }

                                if( right == false ){
                                    yT = yT + 1;
                                    xT = 0;
                                    xP = reg;
                                }
                                else{

                                    answer[yA][xA] = '(';
                                    answer[yA][xA+1] = '2';
                                    answer[yA][xA+2] = ',';
                                    if( yT > 99 ){
                                        answer[yA][xA+3] = yT/100+48;
                                        answer[yA][xA+4] = (yT-100)/10+48;
                                        answer[yA][xA+5] = (yT-100)%10+48;
                                        answer[yA][xA+6] = ')';
                                        xA = xA + 7;
                                    }
                                    else{
                                        if( yT > 9 ){
                                            answer[yA][xA+3] = yT/10+48;
                                            answer[yA][xA+4] = yT%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = yT+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }
                                    }

                                    if( sign == true ){
                                        xP = xP - 1;
                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '4';
                                        answer[yA][xA+2] = ',';
                                        if( yT4 > 99 ){
                                            answer[yA][xA+3] = yT4/100+48;
                                            answer[yA][xA+4] = (yT4-100)/10+48;
                                            answer[yA][xA+5] = (yT4-100)%10+48;
                                            answer[yA][xA+6] = ')';
                                            xA = xA + 7;
                                        }
                                        else{
                                            if( yT4 > 9 ){
                                                answer[yA][xA+3] = yT4/10+48;
                                                answer[yA][xA+4] = yT4%10+48;
                                                answer[yA][xA+5] = ')';
                                                xA = xA + 6;
                                            }
                                            else{
                                                answer[yA][xA+3] = yT4+48;
                                                answer[yA][xA+4] = ')';
                                                xA = xA + 5;
                                            }
                                        }
                                    }
                                }
                            }
                            else {                                //第一個字母不一樣，table要往下找
                                yT = yT + 1;
                                xT = 0;
                                xP = reg;
                            }
                        }

                        if( right == true ){                       // 成功找到table位置了，往右找其他字串位置
                            xP = xP + 1;
                            yT = 1;
                            xT = 0;
                            right = false;
                        }
                        else{                                    //table2沒有，要找找看其他table
                            xP = reg;
                            yT = 1;

                            while( right == false && yT <= aT4 ){    // 找是不是在table4

                                if( problem[yP][xP] == table4[yT][xT] ){ //如果第一個字母一樣，就正確
                                    right = true;

                                    answer[yA][xA] = '(';
                                    answer[yA][xA+1] = '4';
                                    answer[yA][xA+2] = ',';
                                    if( yT > 99 ){
                                        answer[yA][xA+3] = yT/100+48;
                                        answer[yA][xA+4] = (yT-100)/10+48;
                                        answer[yA][xA+5] = (yT-100)%10+48;
                                        answer[yA][xA+6] = ')';
                                        xA = xA + 7;
                                    }
                                    else{
                                        if( yT > 9 ){
                                            answer[yA][xA+3] = yT/10+48;
                                            answer[yA][xA+4] = yT%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = yT+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }
                                    }
                                }
                                else                                 //第一個字母不一樣，table要往下找
                                        yT = yT + 1;
                            }

                            if( right == true ){                       // 成功找到table位置了，往右找其他字串位置
                                xP = xP + 1;
                                yT = 1;
                                xT = 0;
                                right = false;
                            }
                            else{                                    //table4沒有，要找找看其他table
                                // 先判斷是不是數字 table6
                                xP = reg;
                                yT = 1;
                                int ascii = 0;
                                char aReg[300];
                                int i = 0 ;

                                // 有兩種可能 數字 or X' ' 16進位
                                if( problem[yP][xP] == 'x' || problem[yP][xP] == 'X' ){
                                    if( xP < strlen( problem[yP] ) && problem[yP][xP+1] == '\'' ){     // 是X' '形式的數字

                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '4';
                                        answer[yA][xA+2] = ',';
                                        answer[yA][xA+3] = '9';
                                        answer[yA][xA+4] = ')';
                                        xA = xA + 5;

                                        xP = xP + 2;
                                        while( xP < strlen( problem[yP] ) && problem[yP][xP] != '\'' ){   // 開始要處理ASCII碼
                                            // 要區分0~9 a~z A~Z

                                            if( problem[yP][xP] >= '0' && problem[yP][xP] <= '9' )
                                                ascii = ascii + problem[yP][xP] ;

                                            if( problem[yP][xP] >= 'A' && problem[yP][xP] <= 'Z' )
                                                ascii = ascii + problem[yP][xP] ;
                                            if( problem[yP][xP] >= 'a' && problem[yP][xP] <= 'z' )
                                                ascii = ascii + problem[yP][xP] ;

                                            aReg[i] = problem[yP][xP];

                                            i = i + 1;
                                            xP = xP + 1;
                                        }

                                        ascii = ascii % 100;

                                        while( table6[ascii][0] != '-' && strcmp( table6[ascii], aReg ) != 0 ){
                                            ascii = ascii + 1;
                                        }

                                        strcpy( table6[ascii], aReg );

                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '6';
                                        answer[yA][xA+2] = ',';
                                        if( ascii > 9 ){
                                            answer[yA][xA+3] = ascii/10+48;
                                            answer[yA][xA+4] = ascii%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = ascii+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }

                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '4';
                                        answer[yA][xA+2] = ',';
                                        answer[yA][xA+3] = '9';
                                        answer[yA][xA+4] = ')';

                                        xA = xA + 5;
                                        xP = xP + 1;
                                        right = true;
                                    } // 是X' '的形式
                                } // 可能只有一個X
                                else{ // 要判斷有沒有可能是純數字

                                    ascii = 0;
                                    i = 0;
                                    memset(aReg,'\0',sizeof(aReg));
                                    if( problem[yP][xP] >= '0' && problem[yP][xP] <= '9' ) {  // 第一個是數字
                                        right = true;

                                        while(  sign == false && right == true && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){

                                            if( problem[yP][xP] >= '0' && problem[yP][xP] <= '9' ){
                                                ascii = ascii + problem[yP][xP] ;
                                                aReg[i] = problem[yP][xP];

                                                i = i + 1;
                                            }
                                            else{  // 若不是數字，看有沒有可能是標點符號

                                                yT4 = 1;
                                                right = false;
                                                while( right == false && yT4 <= aT4 ){

                                                    if( problem[yP][xP] == table4[yT4][0] ){ //如果第一個字母一樣，就正確
                                                        right = true;
                                                        sign = true;
                                                    }
                                                    else                                 //第一個字母不一樣，table要往下找
                                                        yT4 = yT4 + 1;
                                                }
                                            }

                                            xP = xP + 1;
                                        }


                                        if( right == true ){
                                            ascii = ascii % 100;

                                            while( table6[ascii][0] != '-' && strcmp( table6[ascii], aReg ) != 0 ){
                                                ascii = ascii + 1;

                                            }

                                            strcpy( table6[ascii], aReg );

                                            answer[yA][xA] = '(';
                                            answer[yA][xA+1] = '6';
                                            answer[yA][xA+2] = ',';
                                            if( ascii > 9 ){
                                                answer[yA][xA+3] = ascii/10+48;
                                                answer[yA][xA+4] = ascii%10+48;
                                                answer[yA][xA+5] = ')';
                                                xA = xA + 6;
                                            }
                                            else{
                                                answer[yA][xA+3] = ascii+48;
                                                answer[yA][xA+4] = ')';
                                                xA = xA + 5;
                                            }
                                        }

                                        if( sign == true ){
                                            answer[yA][xA] = '(';
                                            answer[yA][xA+1] = '4';
                                            answer[yA][xA+2] = ',';
                                            if( yT4 > 99 ){
                                                answer[yA][xA+3] = yT4/100+48;
                                                answer[yA][xA+4] = (yT4-100)/10+48;
                                                answer[yA][xA+5] = (yT4-100)%10+48;
                                                answer[yA][xA+6] = ')';
                                                xA = xA + 7;
                                            }
                                            else{
                                                if( yT4 > 9 ){
                                                    answer[yA][xA+3] = yT4/10+48;
                                                    answer[yA][xA+4] = yT4%10+48;
                                                    answer[yA][xA+5] = ')';
                                                    xA = xA + 6;
                                                }
                                                else{
                                                    answer[yA][xA+3] = yT4+48;
                                                    answer[yA][xA+4] = ')';
                                                    xA = xA + 5;
                                                }
                                            }
                                        }
                                    }  // 若第一個為數字
                                }

                                if( right == true ){
                                    if( sign == true ){                             //此位置是table4標點符號
                                        if( table4[yT4][0] == '.' )
                                             xP = 1000000;   //  problem換行
                                        else
                                            xP = xP ;
                                        sign = false;

                                    }
                                    else
                                        xP = xP + 1;

                                    yT = 1;
                                    xT = 0;
                                    right = false;
                                }
                                else{    // 不是table6(數字) ， 要看是不是string or 其他
                                    // 先判斷是不是table7 (字串)
                                    xP = reg;
                                    yT = 1;
                                    int ascii = 0;
                                    char aReg[300];
                                    int i = 0 ;

                                    // 有兩種可能  C'' or ' '

                                    if( problem[yP][xP] == 'C' || problem[yP][xP] == 'c' ){
                                        if( xP < strlen( problem[yP] ) && problem[yP][xP+1] == '\'' ){     // 是X' '形式的字串

                                            answer[yA][xA] = '(';
                                            answer[yA][xA+1] = '4';
                                            answer[yA][xA+2] = ',';
                                            answer[yA][xA+3] = '9';
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;

                                            xP = xP + 2;
                                            while( xP < strlen( problem[yP] ) && problem[yP][xP] != '\'' ){   // 開始要處理ASCII碼
                                            // 要區分0~9 a~z A~Z

                                                if( problem[yP][xP] >= '0' && problem[yP][xP] <= '9' )
                                                    ascii = ascii + problem[yP][xP] ;

                                                if( problem[yP][xP] >= 'A' && problem[yP][xP] <= 'Z' )
                                                    ascii = ascii + problem[yP][xP] ;
                                                if( problem[yP][xP] >= 'a' && problem[yP][xP] <= 'z' )
                                                    ascii = ascii + problem[yP][xP] ;

                                                aReg[i] = problem[yP][xP];

                                                i = i + 1;
                                                xP = xP + 1;
                                            }

                                            right = true;
                                        }  // 確定是字串
                                    }  // 有可能是單純C，要放到table5

                                    if( right == true ){
                                        ascii = ascii % 100;
                                        while( table6[ascii][0] != '-' && strcmp( table5[ascii], aReg ) != 0 ){
                                            ascii = ascii + 1;
                                        }

                                        strcpy( table5[ascii], aReg );

                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '7';
                                        answer[yA][xA+2] = ',';
                                        if( ascii > 9 ){
                                            answer[yA][xA+3] = ascii/10+48;
                                            answer[yA][xA+4] = ascii%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = ascii+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }

                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '4';
                                        answer[yA][xA+2] = ',';
                                        answer[yA][xA+3] = '9';
                                        answer[yA][xA+4] = ')';

                                        xA = xA + 5;
                                        xP = xP + 1;
                                        yT = 1;
                                        xT = 0;
                                        right = false;
                                    }
                                    else{    // 不是table7(字串) ， 直接放在table5
                                        memset(aReg,'\0',sizeof(aReg));
                                        i = 0;
                                        ascii = 0;
                                        while(  sign == false && xP < strlen( problem[yP] ) && problem[yP][xP] != ' ' && problem[yP][xP] != '\t' ){
                                            yT4 = 1;
                                            right = false;

                                            while( yT4 <= aT4 && sign == false ){

                                                if( problem[yP][xP] == table4[yT4][0] ) //如果第一個字母一樣，就正確
                                                    sign = true;
                                                else                                 //第一個字母不一樣，table要往下找
                                                    yT4 = yT4 + 1;

                                            }

                                            if( sign == false ){
                                                ascii = ascii + problem[yP][xP] ;
                                                aReg[i] = problem[yP][xP];
                                                i = i + 1;
                                            }

                                            xP = xP + 1;
                                        }


                                        ascii = ascii % 100;
                                        while( table5[ascii][0] != '-' && strcmp( table5[ascii], aReg ) != 0  ){
                                            ascii = ascii + 1;
                                        }

                                        strcpy( table5[ascii], aReg );

                                        answer[yA][xA] = '(';
                                        answer[yA][xA+1] = '5';
                                        answer[yA][xA+2] = ',';
                                        if( ascii > 9 ){
                                            answer[yA][xA+3] = ascii/10+48;
                                            answer[yA][xA+4] = ascii%10+48;
                                            answer[yA][xA+5] = ')';
                                            xA = xA + 6;
                                        }
                                        else{
                                            answer[yA][xA+3] = ascii+48;
                                            answer[yA][xA+4] = ')';
                                            xA = xA + 5;
                                        }

                                        if( sign == true ){

                                            answer[yA][xA] = '(';
                                            answer[yA][xA+1] = '4';
                                            answer[yA][xA+2] = ',';
                                            if( yT4 > 99 ){
                                                answer[yA][xA+3] = yT4/100+48;
                                                answer[yA][xA+4] = (yT4-100)/10+48;
                                                answer[yA][xA+5] = (yT4-100)%10+48;
                                                answer[yA][xA+6] = ')';
                                                xA = xA + 7;
                                            }
                                            else{
                                                if( yT4 > 9 ){
                                                    answer[yA][xA+3] = yT4/10+48;
                                                    answer[yA][xA+4] = yT4%10+48;
                                                    answer[yA][xA+5] = ')';
                                                    xA = xA + 6;
                                                }
                                                else{
                                                    answer[yA][xA+3] = yT4+48;
                                                    answer[yA][xA+4] = ')';
                                                    xA = xA + 5;
                                                }
                                            }
                                        }
                                    }
                                }
                            }  // 是數字
                        }
                    }
                }
            }//if 非空白
        }

        yP = yP + 1;
        yA = yA + 1;
        xP = 0;
        xA = 0;
        right = false;
        sign = false;

    }




}// work

bool readProblem() {                         // definition: read records from a file
    fstream     inFile;                                         // input file handle
    char        rBuf[255];
    int i = 0 ;

    cout << "請輸入檔名：" ;
    cin >> fileName ;                                           // input file name is fixed
    fileName = fileName + ".txt";

    inFile.open(fileName.c_str(), fstream::in);                 // open file to read

    if (!inFile.is_open()){                                  // unable to open file
        cout << endl << "### " << fileName << " does not exist! ###" << endl;
        return false;
    } // if
    else
        while (inFile.getline(rBuf, 255, '\n')){                // get each input record line by line
            strcpy( problem[i], ( rBuf ) );
            i = i + 1;
        }

    inFile.close();                                             // close file
    if (i == 0) {
        cout << endl << "### Get nothing from the file " << fileName << " ! ### " << endl;
        return false;
    } // if
    else
        aP = i - 1;

    return true;
}   // end readProblem


bool readTable() {                         // definition: read records from a file
    fstream     inFile;                                         // input file handle
    char        rBuf0[255];
    int i = 1 ;
    string tableName;
    tableName = "Table1.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf0, 255, '\n')){                // get each input record line by line
        strcpy( table1[i], ( rBuf0 ) );
        i = i + 1;
    }
    aT1 = i - 1;
    inFile.close();                                             // close file

    char        rBuf2[255];
    i = 1 ;
    tableName = "Table2.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf2, 255, '\n')){                // get each input record line by line
        strcpy( table2[i], ( rBuf2 ) );
        i = i + 1;
    }

    aT2 = i - 1;
    inFile.close();

    char        rBuf3[255];
    i = 1 ;
    tableName = "Table3.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf3, 255, '\n')){                // get each input record line by line
        strcpy( table3[i], ( rBuf3 ) );
        i = i + 1;
    }

    aT3 = i - 1;
    inFile.close();

    char        rBuf4[255];
    i = 1 ;
    tableName = "Table4.table";

    inFile.open(tableName.c_str(), fstream::in);                 // open file to read

    while (inFile.getline(rBuf4, 255, '\n')){                // get each input record line by line
        strcpy( table4[i], ( rBuf4 ) );
        i = i + 1;
    }

    aT4 = i - 1;
    inFile.close();

}   // end readTable
