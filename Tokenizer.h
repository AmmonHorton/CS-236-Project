//
//  Tokenizer.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 1/26/21.
//

#ifndef Tokenizer_h
#define Tokenizer_h

#define NUMBER_OF_TOKENS 17

#include "TokenChecks.h"
#include <sstream>

class Tokenizer {
private:
    TokenCheck* fsaArry[NUMBER_OF_TOKENS];
    bool inputNxtLine;
    size_t lengthOfToken;
    struct WinningToken {
        size_t arryPos = 0;
        size_t correctCharacters = 0;
    };
public:
    Tokenizer(){
        fsaArry[0] = new UndefinedCheck();
        fsaArry[1] = new StringCheck();
        fsaArry[2] = new CommentCheck();
        fsaArry[3] = new CommaCheck();
        fsaArry[4] = new PeriodCheck();
        fsaArry[5] = new Q_MarkCheck();
        fsaArry[6] = new Left_ParenCheck();
        fsaArry[7] = new Right_ParenCheck();
        fsaArry[8] = new ColonCheck();
        fsaArry[9] = new Colon_DashCheck();
        fsaArry[10] = new MultiplyCheck();
        fsaArry[11] = new AddCheck();
        fsaArry[12] = new SchemesCheck();
        fsaArry[13] = new FactsCheck();
        fsaArry[14] = new RulesCheck();
        fsaArry[15] = new QueriesCheck();
        fsaArry[16] = new IDCheck();
        inputNxtLine = false;
        lengthOfToken = 0;
    }
    ~Tokenizer(void) {
        for(int i = NUMBER_OF_TOKENS; i > 0; i--) {
            delete fsaArry[i];
        }
    }
    string IdentifyToken(string input, size_t line, bool eof = false) {
        if(input.at(0) == ' ' || input.at(0) == '\t') {
            lengthOfToken = 1;
            return "";
        }
        WinningToken wt;
        wt.correctCharacters = fsaArry[0]->GetRead(input, eof);
        inputNxtLine = false;
        
        for(int i = 0; i < NUMBER_OF_TOKENS; i++) {
            if (fsaArry[i]->GetRead(input, eof) > fsaArry[wt.arryPos]->GetRead(input, eof)){
                wt.arryPos = i;
                wt.correctCharacters = fsaArry[i]->GetRead(input, eof);
            }
        }
        lengthOfToken = wt.correctCharacters;
        if(wt.arryPos == 0) lengthOfToken = 1;
        //The following Lines of code are messy, but they are used to diffreentiate
        //between certain aspects of undefined/Multiline-stirngs/Multiline-comments
        
        if((input.size() == 1 && input.at(0) == '\'') || (wt.arryPos == 1 && wt.correctCharacters == input.size() && ((input.at(input.size() - 1)) != '\'' || (input.substr((input.size() - 2), 2) == "''")))) inputNxtLine = true;
        else if(wt.arryPos == 2 && wt.correctCharacters == input.size() && input.at(1) == '|' && !(input.substr((input.size() - 2), 2) == "|#")) inputNxtLine = true;
        if(eof && wt.arryPos == 0 && (input.substr(0, 2) == "#|" || input.at(0) == '\'')) {
            lengthOfToken = input.size();
            ostringstream os;
            os << "(UNDEFINED,\"" << input << "\"" << "," << line << ")" << endl;
            return os.str();
        }
         
        ostringstream os;
        os << "(" << fsaArry[wt.arryPos]->str() << "," << line << ")" << endl;
        return os.str();
    }
    bool GetNextLine() {return inputNxtLine;}
    size_t GetLengthOfToken() {return lengthOfToken;}
};

#endif /* Tokenizer_h */
