#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string>
#include <string.h>
#include <vector>

using namespace std;
 
struct hashTable{
	int hashValue = -1 ; //hash value 
    string token = "" ;
};

struct outTable{
	int tableNum = -1 ;
	int tableNo = -1 ;	
};

class Token{
public:
	string inputFileName = "";  	
	string outFileName = "";
	ifstream inFile; // 讀檔 
    ofstream outFile; // 寫檔
    
    vector<string> table1 ;
  	vector<string> table2 ;
  	vector<string> table3 ;
  	vector<string> table4 ;
  	vector<outTable> outList ;
  	
  	hashTable table5[100] ;
  	hashTable table6[100] ;
  	hashTable table7[100] ;
  	
  	void readTable(vector<string> &table, string tableNum){
  		string fileName = "" ;
  		string str;
  		char ch ='\0' ;
  		fileName = "Table"+ tableNum + ".table" ;
  		
  		inFile.open(fileName.c_str()) ;
  		inFile.get(ch) ; //讀下一個字元	
  		
  		while (!inFile.eof()) {
  			while ( ch != '\n') { //讀到\n代表完整的字串已全部放入str 
  			    if(ch != ' ')
					str = str + ch ;
				inFile.get(ch) ;
				if(inFile.eof()) break ;
			}//while
			
			table.push_back(str) ; 	
			str = "" ;	//str重置
			inFile.get(ch) ;					
		}//while
		
       inFile.close() ;  		
	}//readTable()
	
	void readAllTable(){
		readTable(table1, "1") ;
		 
		/*cout << "table1: " << endl ;
		for ( int i = 0 ; i < table1.size() ; i++)
		    cout << table1[i] << endl ; 
		cout << table1.size() << endl ;*/
		     
		readTable(table2, "2") ;
		/*cout << "table2: " << endl ;
		for ( int i = 0 ; i < table2.size() ; i++)
		    cout << table2[i] << endl ;
		cout << table2.size() << endl ;*/
			 
		readTable(table3, "3") ;
		/*cout << "table3: " << endl ;
		for ( int i = 0 ; i < table3.size() ; i++)
		    cout << table3[i] << endl ;
		cout << table3.size() << endl ;*/
			 
		readTable(table4, "4") ;
		/*cout << "table4: " << endl ;
		for ( int i = 0 ; i < table4.size() ; i++)
		      cout << table4[i] << endl ;
	    cout << table4.size() << endl ;*/
		      
	}//readAllTable()
	
	void readFile(){
			
		do {
			cout << "請輸入檔名：";
			cin >> inputFileName ;
			inFile.open( inputFileName.c_str()); // 開檔 
			if ( inFile.fail() ) { 
      			cout << "NO FILE " << inputFileName << " PLEASE ENTER AGAIN!!" << endl; // 不能開 
    		} // if ( 找不到檔案 )
		}while(inFile.fail()) ;
		
		cout << inputFileName << " open success!!!" << endl;
		inFile.close() ;
	}//readFile()
	
	long long countAscii(string token){
		long long num = 0 ;
	
		for ( int i = 0 ; i < token.size() ; i++){
			num = num + int(token[i]) ;			  
		}//for
		
		return num ;		
	}//countAscii()
	
	int hashValue(long long num){
		int hashValue = 0 ;			
		hashValue = num % 100 ;	
		return hashValue ;		
	}//hashValue()
	
	void createLinearHash(int hashNum, string token, hashTable table[100], int &tableNo){
		int curIndex = 0 ;//目前hashTable的index位置 
		bool isDone = false ; //判斷是否找到空格 
		
		if(table[hashNum].hashValue == -1){
			table[hashNum].hashValue = hashNum ;
			table[hashNum].token = copyString(token) ;
			tableNo = hashNum ;
		}//if
		
		else{
			curIndex = hashNum ;
			while(!isDone){ //當還沒有找到空格，執行以下動作 
				curIndex++ ;				
				if(curIndex > 99){
					curIndex = 0 ;
				}//if
										
				if(table[curIndex].hashValue == -1){
				    isDone = true ;						
				}//if					
			}//while
			
			table[curIndex].hashValue = hashNum ;
			table[curIndex].token = copyString(token) ;
			tableNo = curIndex ;
			
		}//else
	}//createLinearHash()
	
	string copyString(string token){
		string str = "" ;
		for ( int i = 0 ; i < token.size() ; i++){
		   	str = str + token[i] ;
		}//for
		return str ;
	}//copyString()
	
	
	string LowToUpCase(string token){
		string upperCase = "" ;
		for ( int i = 0 ; i < token.size() ; i++){
			if(token[i] == 'a') upperCase = upperCase + 'A' ;
			else if(token[i] == 'b') upperCase = upperCase + 'B' ;
			else if(token[i] == 'c') upperCase = upperCase + 'C' ;
			else if(token[i] == 'd') upperCase = upperCase + 'D' ;
			else if(token[i] == 'e') upperCase = upperCase + 'E' ;
			else if(token[i] == 'f') upperCase = upperCase + 'F' ;
			else if(token[i] == 'g') upperCase = upperCase + 'G' ;
			else if(token[i] == 'h') upperCase = upperCase + 'H' ;
			else if(token[i] == 'i') upperCase = upperCase + 'I' ;
			else if(token[i] == 'j') upperCase = upperCase + 'J' ;
			else if(token[i] == 'k') upperCase = upperCase + 'K' ;
			else if(token[i] == 'l') upperCase = upperCase + 'L' ;
			else if(token[i] == 'm') upperCase = upperCase + 'M' ;
			else if(token[i] == 'n') upperCase = upperCase + 'N' ;
			else if(token[i] == 'o') upperCase = upperCase + 'O' ;
			else if(token[i] == 'p') upperCase = upperCase + 'P' ;
			else if(token[i] == 'q') upperCase = upperCase + 'Q' ;
			else if(token[i] == 'r') upperCase = upperCase + 'R' ;
			else if(token[i] == 's') upperCase = upperCase + 'S' ;
			else if(token[i] == 't') upperCase = upperCase + 'T' ;
			else if(token[i] == 'u') upperCase = upperCase + 'U' ;
			else if(token[i] == 'v') upperCase = upperCase + 'V' ;
			else if(token[i] == 'w') upperCase = upperCase + 'W' ;
			else if(token[i] == 'x') upperCase = upperCase + 'X' ;
			else if(token[i] == 'y') upperCase = upperCase + 'Y' ;
			else if(token[i] == 'z') upperCase = upperCase + 'Z' ;					
			else  upperCase = upperCase + token[i] ;			 
		}//for
		
		return upperCase ;
	}//upToLowCase()
	
	bool isTable1(string token, int &tableNo){
		string upCase = "" ;
		for ( int i = 0 ; i < table1.size() ; i++){
			upCase = LowToUpCase(table1[i]) ;
			if(token.compare(upCase) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if			
		}//for		
		return false ;
	}//isTable1()
	
	bool isTable2(string token, int &tableNo ){
		string upCase = "" ;
		for ( int i = 0 ; i < table2.size() ; i++){
			upCase = LowToUpCase(table2[i]) ;
			if(token.compare(upCase) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if			
		}//for		
		return false ;
	}//isTable2()
	
	bool isTable3(string token, int &tableNo ){
		string upCase = "" ;
		for ( int i = 0 ; i < table3.size() ; i++){
			upCase = LowToUpCase(table3[i]) ;
			if(token.compare(upCase) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if			
		}//for		
		return false ;
	}//isTable3()
	
	
	bool isTable1To3(string token, int &tableNum, int &tableNo){
		string upCase = LowToUpCase(token) ;
		if(isTable1(upCase, tableNo)) {
			tableNum = 1 ;
			return true ;
		}//if
		
		else if(isTable2(upCase, tableNo)){
			tableNum = 2 ;
			return true ;
		}//if
		
		else if(isTable3(upCase, tableNo)){
			tableNum = 3 ;
			return true ;
		}//if
		
		return false ;				
	}//isTable1To3()
	
	bool isDelimiter(char ch, int&tableNo){
		 string str = "" ;
		 str = str + ch ;
		 for ( int i = 0 ; i < table4.size() ; i++){		
			if(str.compare(table4[i]) == 0) {
				tableNo = i + 1 ;
				return true ;
			}//if				
		 }//for		 
		 return false ;
	}//isDelimiter()
	
	bool isInteger(string token){
		for ( int i = 0 ; i < token.size() ; i++){
			if( token[i] < '0' || token[i] >'9')
			   return false ;
		}//for		
		return true ;
	}//isInteger()
	
	bool isInTable5(string token, int &tableNo){
        for ( int i = 0 ; i < 100 ; i++){
            if (table5[i].hashValue != -1){
            	if(table5[i].token.compare(token) == 0){
            		tableNo = i ;
            		return true ;
				}//if
			}//if
		}//for
		
		return false ;
	}//isTable5()
	
	bool isInTable6(string token, int &tableNo){
		string upCase = "" ;
		upCase = LowToUpCase(token) ;
        for ( int i = 0 ; i < 100 ; i++){
            if (table6[i].hashValue != -1){
            	if(LowToUpCase(table6[i].token).compare(upCase) == 0){
            		tableNo = i ;
            		return true ;
				}//if
			}//if
		}//for
		
		return false ;
	}//isTable6()
	
	bool isInTable7(string token, int &tableNo){
        for ( int i = 0 ; i < 100 ; i++){
            if (table7[i].hashValue != -1){
            	if(table7[i].token.compare(token) == 0){
            		tableNo = i ;
            		return true ;
				}//if
			}//if
		}//for
		
		return false ;
	}//isTable7()
	
	
	
	void cutCmdToken(string cmdLine){
		string token = "" ;
		int tableNum = -1 ;
		int tableNo = -1 ;
		int delNo = -1 ;
		outTable tmp ;
		
		for ( int i = 0 ; i < cmdLine.size() ; i++){
		    if (cmdLine[i] == ' ' || cmdLine[i] == '\t' ){		    	
				if(!token.empty()){
				  	//cout << "token: " << token << endl ;
					if(isTable1To3(token, tableNum, tableNo)){
						tmp.tableNum = tableNum ;
						tmp.tableNo = tableNo ;
						outList.push_back(tmp) ;
					}//if
					
					else if(isInteger(token)) {
						if(!isInTable6(token, tableNo)){
							createLinearHash(hashValue(countAscii(token)), token, table6, tableNo) ;
						}//if
						
						tmp.tableNum = 6 ;
						tmp.tableNo = tableNo ;
						outList.push_back(tmp) ;
					}//if	
				
					else {
						if(!isInTable5(token, tableNo)){
							createLinearHash(hashValue(countAscii(token)), token, table5, tableNo) ;
						}//if
						
						tmp.tableNum = 5 ;
						tmp.tableNo = tableNo ;
						outList.push_back(tmp) ;
					}//else  
					token = "" ;			   
			    }//if					
				
			}//if
			
			else if(isDelimiter(cmdLine[i], tableNo)){ //table4
			    //cout << "token: " << cmdLine[i] << endl ;
			    delNo = tableNo ;
			    if(token.compare("C") == 0 && cmdLine[i] =='\''){
			    	token = "" ;
			    	
			    	tmp.tableNum = 4 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					//cout << "token: " << cmdLine[i] << endl ;
					
					i++ ;
					while(cmdLine[i]!= '\''){
						token = token + cmdLine[i] ;
						i++ ;
					}//while
					
					if(!isInTable7(token, tableNo)){
						createLinearHash(hashValue(countAscii(token)), token, table7, tableNo) ;
					}//if
					
					tmp.tableNum = 7 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					//cout << "token: " << token << endl ;
					
					tmp.tableNum = 4 ;
					tmp.tableNo = delNo ;
					outList.push_back(tmp) ;
					
					
					
				}//if
				
				else if(token.compare("X") == 0 && cmdLine[i] =='\''){
			    	token = "" ;
			    	tmp.tableNum = 4 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					
					i++ ;
					while(cmdLine[i]!= '\''){
						token = token + cmdLine[i] ;
						i++ ;
					}//while
					
					if(!isInTable6(LowToUpCase(token), tableNo)){
						createLinearHash(hashValue(countAscii(LowToUpCase(token))), token, table6, tableNo) ;
					}//if
					//cout << "token: " << token << endl ;
					
					tmp.tableNum = 6 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
					
					tmp.tableNum = 4 ;
					tmp.tableNo = delNo ;
					outList.push_back(tmp) ;
				}//if
				
			    else {
			    	delNo = tableNo ;
			    	if(!token.empty()){
					  	//cout << "token: " << token << endl ;
						if(isTable1To3(token, tableNum, tableNo)){
							tmp.tableNum = tableNum ;
							tmp.tableNo = tableNo ;
							outList.push_back(tmp) ;
						}//if
						
						else if(isInteger(token)) {
							if(!isInTable6(token, tableNo)){
								createLinearHash(hashValue(countAscii(token)), token, table6, tableNo) ;
							}//if
							
							tmp.tableNum = 6 ;
							tmp.tableNo = tableNo ;
							outList.push_back(tmp) ;
						}//if	
					
						else {
							if(!isInTable5(token, tableNo)){
								createLinearHash(hashValue(countAscii(token)), token, table5, tableNo) ;
							}//if
							
							tmp.tableNum = 5 ;
							tmp.tableNo = tableNo ;
							outList.push_back(tmp) ;
						}//else  			   
			       }//if
				   
				   if(cmdLine[i]== '.'){				   	    
					    i = cmdLine.size() ;
				   }//if	
				   
				    tmp.tableNum = 4 ;
					tmp.tableNo = delNo ;
					outList.push_back(tmp) ;		
				}//else		
				
				token = "" ;	    				
			}//if
			
			else{
				token = token + cmdLine[i] ;
				cout << token << "<<" << endl ;
			}//else
						
		}//for
		
			if(!token.empty()){
			  	//cout << "token: " << token << endl ;
				if(isTable1To3(token, tableNum, tableNo)){
					tmp.tableNum = tableNum ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
				}//if
				
				else if(isInteger(token)) {
					if(!isInTable6(token, tableNo)){
						createLinearHash(hashValue(countAscii(token)), token, table6, tableNo) ;
					}//if
					
					tmp.tableNum = 6 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
				}//if	
			
				else {
					if(!isInTable5(token, tableNo)){
						createLinearHash(hashValue(countAscii(token)), token, table5, tableNo) ;
					}//if
					
					tmp.tableNum = 5 ;
					tmp.tableNo = tableNo ;
					outList.push_back(tmp) ;
				}//else  			   
			}//if					
		
		token = "" ;
	}//cutCmdToken()
	
	void cutFileToken(){
		string cmdLine = "" ;
		char ch = '\0' ;
		
		inFile.open(inputFileName.c_str()) ; //開檔
		outFile.open( "ResultOutput.txt", ios::trunc );
		
		inFile.get(ch) ;
		while(!inFile.eof()){
			while (ch != '\n'){
				cmdLine = cmdLine + ch ;
				inFile.get(ch) ;
				if(inFile.eof()) break ;
			}//while
						
			cout << cmdLine << endl ;
			outFile << cmdLine << "\n" ;
			cutCmdToken(cmdLine) ;
			
			for ( int i = 0 ; i < outList.size() ; i++){
				cout << "(" << outList[i].tableNum << "," << outList[i].tableNo << ")" ;
				outFile << "(" << outList[i].tableNum << "," << outList[i].tableNo << ")" ;
			}//for
			
			cout << "\n" ;
			outFile << "\n" ;
			outList.clear() ;
			cmdLine = "" ;
			inFile.get(ch) ;
		}//while
		
		inFile.close() ;
		outFile.close() ;
	}//cutFileToken()
	
};

int main(int argc, char** argv) {
	Token token ;
	int tableNum = -1 ;
	int tableNo = -1 ;
	
	token.readAllTable() ;
	token.readFile() ;
	token.cutFileToken() ;
	 	
	return 0;
}
