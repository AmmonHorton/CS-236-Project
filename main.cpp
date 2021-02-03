//
//  main.cpp
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 1/26/21.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Tokenizer.h"
#include "TokenChecks.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    Tokenizer tker;
    string tempLineStr = ""; //For temporary line assignments
    string tempStr = "";//For temporary word assignments
    ostringstream outStr;
    size_t lineInFile = 1;
    size_t lineIncrease = 0;
    size_t totalTokens = 0;
    
    ifstream inFile(argv[1]);
    //cout << endl << "Input file is open" << endl << endl;
    while(inFile)
    {
        if(!getline(inFile, tempLineStr)) break;
        //istringstream is(tempLineStr);
        while(tempLineStr.size() > 0)
        {
            tempStr = tker.IdentifyToken(tempLineStr, lineInFile, inFile.eof());
            lineIncrease = 0;
            while(tker.GetNextLine()) {
                tempStr = tempLineStr;
                if(!getline(inFile, tempLineStr)) break;
                lineIncrease++;
                tempLineStr = tempStr + '\n' + tempLineStr;
                tempStr = tker.IdentifyToken((tempLineStr), lineInFile, inFile.eof());
            }
            outStr << tempStr;
            totalTokens++;
            if(tempLineStr.at(0) == ' ' || tempLineStr.at(0) == '\t') {
                totalTokens--;
            }
            tempLineStr.erase(0, tker.GetLengthOfToken());
            lineInFile += lineIncrease;
        }
        lineInFile++;
        tempStr = "";
    }
    outStr << "(EOF,\"\"," << lineInFile << ")" << endl;
    
    totalTokens++;
    //For EOF Token
    outStr << "Total Tokens = " << totalTokens;
    
    inFile.close();
    
    
    ofstream outFile(argv[2]);
    //Open the output file
    outFile << outStr.str();
    cout << outStr.str();
    //Check status of file
    outFile.close();//*/
    
    return 0;
}
