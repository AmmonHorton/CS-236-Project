//
//  Lexer.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 1/26/21.
//

#ifndef Lexer_h
#define Lexer_h

#define NUMBER_OF_TOKENS 17

#include "TokenFSA.h"
#include <sstream>
#include <fstream>
#include <deque>

class Lexer {
private:
    TokenFSA* fsaArry[NUMBER_OF_TOKENS];
    bool inputNxtLine;
    size_t lengthOfToken;
    size_t correctArryPos;
    size_t correctCharacters;
public:
    vector<TokenFSA*> tokenVector;
    Lexer(){
        correctArryPos = 0;
        correctCharacters = 0;
        fsaArry[0] = new UndefinedFSA();
        fsaArry[1] = new StringFSA();
        fsaArry[2] = new CommentFSA();
        fsaArry[3] = new CommaFSA();
        fsaArry[4] = new PeriodFSA();
        fsaArry[5] = new Q_MarkFSA();
        fsaArry[6] = new Left_ParenFSA();
        fsaArry[7] = new Right_ParenFSA();
        fsaArry[8] = new ColonFSA();
        fsaArry[9] = new Colon_DashFSA();
        fsaArry[10] = new MultiplyFSA();
        fsaArry[11] = new AddFSA();
        fsaArry[12] = new SchemesFSA();
        fsaArry[13] = new FactsFSA();
        fsaArry[14] = new RulesFSA();
        fsaArry[15] = new QueriesFSA();
        fsaArry[16] = new IDFSA();
        inputNxtLine = false;
        lengthOfToken = 0;
    }
    ~Lexer(void) {
        for(int i = NUMBER_OF_TOKENS; i > 0; i--) {
            delete fsaArry[i - 1];
        }
        for(size_t i = tokenVector.size(); i > 0; i--) {
            delete tokenVector.back();
            tokenVector.pop_back();
        }
    }
    TokenFSA* IdentifyToken(string input, size_t line, bool eof = false) {
        correctCharacters = fsaArry[0]->GetRead(input, eof);
        correctArryPos = 0;
        inputNxtLine = false;
        
        
        
        if(input.at(0) == ' ' || input.at(0) == '\t' || input.at(0) == '\n') {
            lengthOfToken = 1;
            return NULL;
        }
        
        for(int i = 0; i < NUMBER_OF_TOKENS; i++) {
            if (fsaArry[i]->GetRead(input, eof) > correctCharacters){
                correctArryPos = i;
                correctCharacters = fsaArry[i]->GetRead(input, eof);
                
            }
        }
        lengthOfToken = correctCharacters;
        if(correctArryPos == 0 && (input.at(0) != '\'' && input.substr(0,2) != "#|")) {
            lengthOfToken = 1;
            fsaArry[correctArryPos]->SetVar(input.substr(0,1), 1, 0);
        }
        else if(correctArryPos == 0) {
            lengthOfToken = input.size();
            fsaArry[correctArryPos]->SetVar(input, input.size(), 0);
        }
        
        
        
        
        
        //The following Lines of code are messy, but they are used to diffreentiate
        //between certain aspects of undefined/Multiline-stirngs/Multiline-comments
        
        if((input.size() == 1 && input.at(0) == '\'') || (correctArryPos == 1 && correctCharacters == input.size() && ((input.at(input.size() - 1)) != '\'' || (input.substr((input.size() - 2), 2) == "''")))) inputNxtLine = true;
        else if(correctArryPos == 2 && correctCharacters == input.size() && input.at(1) == '|' && !(input.substr((input.size() - 2), 2) == "|#")) inputNxtLine = true;
        if(eof && ((correctArryPos == 1 && !fsaArry[1]->accepting) || (correctArryPos == 2 && !fsaArry[2]->accepting)) && (input.substr(0, 2) == "#|" || input.at(0) == '\'')) {
            lengthOfToken = input.size();
            fsaArry[0]->SetVar(input, lengthOfToken, line);
            return fsaArry[0];
        }
         
        return fsaArry[correctArryPos];
    }
    void readInput(ifstream &inFile) {
        TokenFSA* tcPtr;
        string tempLineStr = ""; //For temporary line assignments
        string tempStr = "";//For temporary word assignments
        ostringstream outStr;
        size_t lineInFile = 1;
        size_t lineIncrease = 0;
        size_t totalTokens = 0;
        
        while(inFile)
        {
            if(!getline(inFile, tempLineStr)) break;
            while(tempLineStr.size() > 0)
            {
                tcPtr = IdentifyToken(tempLineStr, lineInFile, inFile.eof());
                lineIncrease = 0;
                while(inputNxtLine) {
                    tempStr = tempLineStr;
                    getline(inFile, tempLineStr);
                    lineIncrease++;
                    tempLineStr = tempStr + '\n' + tempLineStr;
                    tcPtr = IdentifyToken((tempLineStr), lineInFile, inFile.eof());
                }
                
                if(tcPtr != NULL) {
                    tcPtr->SetVar(lineInFile);
                    AddToVector(tcPtr);
                }
                if(tcPtr != NULL) totalTokens++;
                if(inFile.eof() && tempLineStr.at(tempLineStr.size() - 1) == '\n') lineInFile--;
                tempLineStr.erase(0, lengthOfToken);
                lineInFile += lineIncrease;
            }
            lineInFile++;
            tempStr = "";
        }
        tokenVector.push_back(new EOFFSA("EOF", lineInFile));
        totalTokens++;
        //For EOF Token
    }
    void AddToVector(TokenFSA* tcPtr) {
        if(correctArryPos == 0) tokenVector.push_back(new UndefinedFSA(*tcPtr));
        else if(correctArryPos == 1) tokenVector.push_back(new StringFSA(*tcPtr));
        else if(correctArryPos == 2) tokenVector.push_back(new CommentFSA(*tcPtr));
        else if(correctArryPos == 3) tokenVector.push_back(new CommaFSA(*tcPtr));
        else if(correctArryPos == 4) tokenVector.push_back(new PeriodFSA(*tcPtr));
        else if(correctArryPos == 5) tokenVector.push_back(new Q_MarkFSA(*tcPtr));
        else if(correctArryPos == 6) tokenVector.push_back(new Left_ParenFSA(*tcPtr));
        else if(correctArryPos == 7) tokenVector.push_back(new Right_ParenFSA(*tcPtr));
        else if(correctArryPos == 8) tokenVector.push_back(new ColonFSA(*tcPtr));
        else if(correctArryPos == 9) tokenVector.push_back(new Colon_DashFSA(*tcPtr));
        else if(correctArryPos == 10) tokenVector.push_back(new MultiplyFSA(*tcPtr));
        else if(correctArryPos == 11) tokenVector.push_back(new AddFSA(*tcPtr));
        else if(correctArryPos == 12) tokenVector.push_back(new SchemesFSA(*tcPtr));
        else if(correctArryPos == 13) tokenVector.push_back(new FactsFSA(*tcPtr));
        else if(correctArryPos == 14) tokenVector.push_back(new RulesFSA(*tcPtr));
        else if(correctArryPos == 15) tokenVector.push_back(new QueriesFSA(*tcPtr));
        else if(correctArryPos == 16) tokenVector.push_back(new IDFSA(*tcPtr));
        return;
    }
    
    
    size_t GetLengthOfToken() {return lengthOfToken;}
};

#endif /* Lexer_h */
