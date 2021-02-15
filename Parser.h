//
//  Parser.h
//
//
//  Created by AMMON HORTON on 2/4/21.
//

#ifndef Parser_h
#define Parser_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "TokenFSA.h"
#include "NonTerminals.h"

class DatalogProgram {
private:
    TokenItemInterface* tItemPtr;
    Scheme scheme;
    SchemeList schemeList;
    FactList factList;
    RuleList ruleList;
    Query query;
    QueryList queryList;
    set<string> domainSet;
public:
    DatalogProgram() {
        tItemPtr = NULL;
    }
    ~DatalogProgram() {
        if(tItemPtr) {
            delete tItemPtr;
            tItemPtr = NULL;
        }
    }
    
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        //Checking against all Schemes
        
        tItemPtr = new TokenItemInterface("SCHEMES");
        if(!(CheckForToken(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("COLON");
        if(!(CheckForToken(tokenDeque))) return false;
        
        if(!(scheme.isValid(tokenDeque))) return false;
        if(!(schemeList.isValid(tokenDeque))) return false;
        
        //Checking against all facts
        
        tItemPtr = new TokenItemInterface("FACTS");
        if(!(CheckForToken(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("COLON");
        if(!(CheckForToken(tokenDeque))) return false;
        
        if(!(factList.isValid(tokenDeque))) {
            return false;
        }
        
        
        //Checking against all rules
        
        tItemPtr = new TokenItemInterface("RULES");
        if(!(CheckForToken(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("COLON");
        if(!(CheckForToken(tokenDeque))) return false;
        
        if(!(ruleList.isValid(tokenDeque))) return false;
        
        //Checking against all queries
        
        tItemPtr = new TokenItemInterface("QUERIES");
        if(!(CheckForToken(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("COLON");
        if(!(CheckForToken(tokenDeque))) return false;
        
        if(!(query.isValid(tokenDeque))) return false;
        if(!(queryList.isValid(tokenDeque))) return false;
        
        //Checking for EOF
        
        tItemPtr = new TokenItemInterface("EOF");
        if(!(CheckForToken(tokenDeque))) return false;
        
        domainSet = factList.GetDomain();
        
        return true;
    }
    
    bool CheckForToken(deque<TokenItemInterface*> &tokenDeque) {
        if(*tokenDeque.front() == *tItemPtr) {
            delete tokenDeque.front();
            tokenDeque.pop_front();
            delete tItemPtr;
            tItemPtr = NULL;
            return true;
        }
        else {
            delete tItemPtr;
            tItemPtr = NULL;
            return false;
        }
    }
    
    string str() {
        ostringstream os;
        
        os << "Success!" << endl;
        os << "Schemes(" << (1 + schemeList.GetSizeOfList()) << "):" << endl;
        os << scheme.str() << endl;
        os << schemeList.str();
        os << "Facts(" << factList.GetSizeOfList() << "):" << endl;
        os << factList.str();
        os << "Rules(" << ruleList.GetSizeOfList() << "):" << endl;
        os << ruleList.str();
        os << "Queries(" << (1 + queryList.GetSizeOfList()) << "):" << endl;
        os << query.str() << endl;
        os << queryList.str();
        
        os << "Domain(" << (domainSet.size()) << "):" << endl;
        for (auto it = domainSet.begin(); it != domainSet.end(); ++it) {
            os << *it << endl;
        }
        return os.str();
    }
};



class Parser {
private:
    DatalogProgram dp;
public:
    Parser(deque<TokenItemInterface*> &tokenDeque) {
        if(!(dp.isValid(tokenDeque))) {
            throw tokenDeque.front();
        }
        return;
    }
    /*void SetDomain(deque<string> &tokenDeque) {
        domainDeque = tokenDeque;
        return;
    }*/
    string str() {
        ostringstream os;
        os << dp.str();
        /*if(domainDeque.size() > 0) {
            os << "Domain(" << domainDeque.size() << "):" << endl;
            deque<string>::iterator it = domainDeque.begin();
            while(it != domainDeque.end()) {
                os << "  " << *it << endl;
                it++;
            }
        }*/
        return os.str();
    }
    
    
};

#endif /* Parser_h */
