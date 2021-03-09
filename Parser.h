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
#include "Elements.h"
#include "ElementParentClasses.h"

class DatalogProgram {
public:
//private:
    ProductionElementBase* tItemPtr;
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
    
    bool isValid(deque<ProductionElementBase*> &productionElementDeque) {
        
        //Checking against all Schemes
        
        tItemPtr = new ProductionElementBase("SCHEMES");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        tItemPtr = new ProductionElementBase("COLON");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        if(!(scheme.isValid(productionElementDeque))) return false;
        if(!(schemeList.isValid(productionElementDeque))) return false;
        
        //Checking against all facts
        
        tItemPtr = new ProductionElementBase("FACTS");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        tItemPtr = new ProductionElementBase("COLON");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        if(!(factList.isValid(productionElementDeque))) return false;
        
        
        //Checking against all rules
        
        tItemPtr = new ProductionElementBase("RULES");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        tItemPtr = new ProductionElementBase("COLON");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        if(!(ruleList.isValid(productionElementDeque))) return false;
        
        //Checking against all queries
        
        tItemPtr = new ProductionElementBase("QUERIES");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        tItemPtr = new ProductionElementBase("COLON");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        if(!(query.isValid(productionElementDeque))) return false;
        if(!(queryList.isValid(productionElementDeque))) return false;
        
        //Checking for EOF
        
        tItemPtr = new ProductionElementBase("EOF");
        if(!(CheckForToken(productionElementDeque))) return false;
        
        domainSet = factList.GetDomain();
        
        return true;
    }
    
    bool CheckForToken(deque<ProductionElementBase*> &productionElementDeque) {
        if(*productionElementDeque.front() == *tItemPtr) {
            delete productionElementDeque.front();
            productionElementDeque.pop_front();
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
    
    string const str() {
        ostringstream os;
        
        os << "Success!" << endl;
        os << "Schemes(" << (1 + schemeList.size()) << "):" << endl;
        os << scheme.str() << endl;
        os << schemeList.str();
        os << "Facts(" << factList.size() << "):" << endl;
        os << factList.str();
        os << "Rules(" << ruleList.size() << "):" << endl;
        os << ruleList.str();
        os << "Queries(" << (1 + queryList.size()) << "):" << endl;
        os << query.str() << endl;
        os << queryList.str();
        
        os << "Domain(" << (domainSet.size()) << "):" << endl;
        for (auto it = domainSet.begin(); it != domainSet.end(); ++it) {
            os << "  " << *it << endl;
        }
        return os.str();
    }
};



class Parser {
public:
//private:
    DatalogProgram dp;
public:
    Parser() {}
    void CheckTokens(deque<ProductionElementBase*> &productionElementDeque) {
        if(!(dp.isValid(productionElementDeque))) {
            throw productionElementDeque.front();
        }
        return;
    }
    string str() {
        ostringstream os;
        os << dp.str();
        return os.str();
    }
    
    
};

#endif /* Parser_h */
