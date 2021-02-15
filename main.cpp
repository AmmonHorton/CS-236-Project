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
#include <vector>
#include "Tokenizer.h"
#include "TokenFSA.h"
#include "Parser.h"
#include "Terminals.h"

using namespace std;

int main(int argc, const char * argv[]) {
    
    Lexer lexer;
    ostringstream outStr;
    deque<TokenItemInterface*> tokenItems;
    vector <TokenFSA*> tokenVector;
    
    ifstream inFile(argv[1]);
    lexer.readInput(inFile);
    inFile.close();
    
    
    for(size_t i = lexer.tokenDeque.size(); i > 0; i--) {
        tokenVector.push_back(lexer.tokenDeque.front());
        lexer.tokenDeque.pop_front();
    }
    
    for(size_t i = tokenVector.size(); i > 0; i--) {
        string st = tokenVector.at(i - 1)->selfType;
        string tokenStr = tokenVector.at(i - 1)->GetToken();
        if(st == "COMMENT") {
            delete tokenVector.at(i - 1);
            tokenVector.erase(tokenVector.begin()+i - 1);
            //i++;
            continue;
        }
        else {
            TokenItemInterface* tIIPtr = new TokenItemInterface(st, tokenStr);
            tokenItems.push_front(tIIPtr);
        }
    }
    
    
    
    /*deque<string> domainDeque;
    
    deque<TokenItemInterface*>::iterator it = tokenItems.begin();
    while(it != tokenItems.end()) {
        ostringstream os;
        os << **it;
        string str = os.str();
        if(str.substr(0,1) == "'") {
            //str.erase(0,1);
            //str.erase(str.end() - 1, str.end());
            
            deque<string>::iterator iterator = domainDeque.begin();
            
            if(domainDeque.size() != 0){
                while(iterator != domainDeque.end()) {
                    string st = *iterator;
                    if(st == str) break;
                    iterator++;
                }
                if(iterator == domainDeque.end()) domainDeque.push_front(str);
            }
            else domainDeque.push_front(str);
        }
        it++;
    }
    sort(domainDeque.begin(), domainDeque.end());
    */
    
    
    
    
    
    ostringstream os;
    try {
        Parser parser(tokenItems);
        //parser.SetDomain(domainDeque);
        outStr << parser.str();
        os << outStr.str();
    }
    catch (TokenItemInterface* tcPtr) {
        os << "Failure!" << endl << "  ";
        os << tokenVector.at(tokenVector.size() - tokenItems.size())->str() << endl;
    }
    
    
    ofstream outFile(argv[2]);
    //Open the output file
    outFile << os.str();
    cout << os.str();
    //Check status of file
    outFile.close();//*/
    
    return 0;
}
