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
#include <deque>
#include <set>

#include "TokenFSA.h"
#include "Tokenizer.h"
#include "Parser.h"

#include "RelationalDatabase.h"


using namespace std;

int main(int argc, const char * argv[]) {
    
    Lexer lexer;
    Parser parser;
    
    ifstream inFile(argv[1]);
    lexer.readInput(inFile);
    inFile.close();
    
    deque<ProductionElementBase*> productionElementsDeque;
    
    for(size_t i = lexer.tokenVector.size(); i > 0; i--) {
        string st = lexer.tokenVector.at(i - 1)->selfType;
        string tokenStr = lexer.tokenVector.at(i - 1)->GetToken();
        if(st == "COMMENT") {
            delete lexer.tokenVector.at(i - 1);
            lexer.tokenVector.erase(lexer.tokenVector.begin()+i - 1);
            continue;
        }
        else {
            ProductionElementBase* tIIPtr = new ProductionElementBase(st, tokenStr);
            productionElementsDeque.push_front(tIIPtr);
        }
    }
    
    ostringstream outStr;
    ostringstream os;
    
    try {
        parser.CheckTokens(productionElementsDeque);
        outStr << parser.str();
        //os << outStr.str();
        Database database(parser.dp);
        os << database.str();
    }
    catch (ProductionElementBase* tcPtr) {
        os << "Failure!" << endl << "  ";
        os << lexer.tokenVector.at(lexer.tokenVector.size() - productionElementsDeque.size())->str() << endl;
    }
    
    
    
    
    //ofstream outFile(argv[2]);
    //Open the output file
    //outFile << os.str();
    cout << os.str();
    //Check status of file
    //outFile.close();//*/
    
    return 0;
}
