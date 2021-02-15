//
//  NonTerminals.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 2/12/21.
//

#ifndef NonTerminals_h
#define NonTerminals_h
#include "TokenFSA.h"
#include "Terminals.h"
#include <set>





class IDList : public TokenItemLists {
public:
    IDList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new TokenItemInterface("ID");
        followSetItemPtr = new TokenItemInterface("RIGHT_PAREN");
    }
    ~IDList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("COMMA");
        if(*tokenDeque.front() == *tItemPtr) {
            if(!(CheckForToken(tokenDeque))) return false;
            
            //Check for all ID's
            
            tItemPtr = new TokenItemInterface("ID");
            if(!(CheckForToken(tokenDeque))) return false;
            
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() {
        ostringstream os;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            os << "," << **it++;
        }
        return os.str();
    }
};


class StringList : public TokenItemLists {
public:
    StringList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new TokenItemInterface("STRING");
        followSetItemPtr = new TokenItemInterface("RIGHT_PAREN");
    }
    ~StringList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("COMMA");
        if(*tokenDeque.front() == *tItemPtr) {
            if(!(CheckForToken(tokenDeque))) return false;
            
            //Check for all Strings
            
            tItemPtr = new TokenItemInterface("STRING");
            if(!(CheckForToken(tokenDeque))) return false;
            
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() {
        ostringstream os;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            os << "," << **it++;
        }
        return os.str();
    }
    deque<string*> GetDomain() {
        deque<string*> domainDeque;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            ostringstream os;
            os << **it++;
            string* strPtr = new string(os.str());
            domainDeque.push_front(strPtr);
        }
        return domainDeque;
    }
    
};



class Operator : public TokenItem{
public:
    TokenItemInterface* addToken;
    TokenItemInterface* multiplyToken;
    IDList idList;
    Operator(string st = "OPERATOR") : TokenItem(st){
        tItemPtr = NULL;
        addToken = new TokenItemInterface("ADD");
        multiplyToken = new TokenItemInterface("MULTIPLY");
    }
    ~Operator() {
        if(addToken) delete addToken;
        if(multiplyToken) delete multiplyToken;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        if(*tokenDeque.front() == *addToken) {
            delete multiplyToken;
            multiplyToken = NULL;
            
            tItemPtr = new TokenItemInterface("ADD");
            if(CheckForToken(tokenDeque, addToken)) return true;
        }
        else if(*tokenDeque.front() == *multiplyToken) {
            delete addToken;
            addToken = NULL;
            
            tItemPtr = new TokenItemInterface("MULTIPLY");
            if(CheckForToken(tokenDeque, multiplyToken)) return true;
        }
        return false;
    }
    string str() {
        if(multiplyToken) return multiplyToken->str();
        else return addToken->str();
    }
};



class Parameter : public TokenItem {
public:
    TokenItemInterface* expressionToken;
    TokenItemInterface* strTokenPtr;
    TokenItemInterface* idTokenPtr;
    Parameter(string st = "PARAMETER") : TokenItem(st){
        tItemPtr = NULL;
        expressionToken = NULL;
        strTokenPtr = new TokenItemInterface("STRING");
        idTokenPtr = new TokenItemInterface("ID");
    }
    ~Parameter() {
        if(expressionToken) delete expressionToken;
        if(strTokenPtr) delete strTokenPtr;
        if(idTokenPtr) delete idTokenPtr;
    }
    
    class Expression : public TokenItem {
    public:
        TokenItemInterface* parameterToken;
        TokenItemInterface* operatorToken;
        TokenItemInterface* scndParameterToken;
        Expression(string st = "EXPRESSION") : TokenItem(st){
            tItemPtr = NULL;
            parameterToken = new Parameter();
            operatorToken = new Operator();
            scndParameterToken = new Parameter();
        }
        ~Expression() {
            delete parameterToken;
            delete operatorToken;
            delete scndParameterToken;
        }
        bool isValid(deque<TokenItemInterface*> &tokenDeque) {
            TokenItemInterface* nullPtr = NULL;
            
            tItemPtr = new TokenItemInterface("LEFT_PAREN");
            if(!(CheckForToken(tokenDeque, nullPtr))) return false;
            
            if(!(parameterToken->isValid(tokenDeque))) return false;
            if(!(operatorToken->isValid(tokenDeque))) return false;
            if(!(scndParameterToken->isValid(tokenDeque))) return false;
            
            tItemPtr = new TokenItemInterface("RIGHT_PAREN");
            if(!(CheckForToken(tokenDeque, nullPtr))) return false;
            
            return true;
        }
        string str() {
            return "(" + parameterToken->str() + operatorToken->str() + scndParameterToken->str() + ")";
        }
    };
    
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* leftParenTokenPtr = new TokenItemInterface("LEFT_PAREN");
        
        if(*tokenDeque.front() == *strTokenPtr) {
            delete idTokenPtr;
            if(expressionToken) delete expressionToken;
            
            idTokenPtr = NULL;
            expressionToken = NULL;
            
            tItemPtr = new TokenItemInterface("STRING");
            if(CheckForToken(tokenDeque, strTokenPtr)) return true;
        }
        else if(*tokenDeque.front() == *idTokenPtr) {
            delete strTokenPtr;
            if(expressionToken) delete expressionToken;
            
            strTokenPtr = NULL;
            expressionToken = NULL;
            
            tItemPtr = new TokenItemInterface("ID");
            if(CheckForToken(tokenDeque, idTokenPtr)) return true;
        }
        else if(*tokenDeque.front() == *leftParenTokenPtr) {
            delete idTokenPtr;
            delete strTokenPtr;
            
            idTokenPtr = NULL;
            strTokenPtr = NULL;
            
            expressionToken = new Expression();
            if(expressionToken->isValid(tokenDeque)) return true;
        }
        return false;
    }
    string str() {
        if(idTokenPtr) return idTokenPtr->str();
        else if(strTokenPtr) return strTokenPtr->str();
        else return expressionToken->str();
    }
};


class ParameterList : public TokenItemLists{
public:
    ParameterList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new Parameter();
        followSetItemPtr = new TokenItemInterface("RIGHT_PAREN");
    }
    ~ParameterList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        tItemPtr = new TokenItemInterface("COMMA");
        if(*tokenDeque.front() == *tItemPtr) {
            if(!(CheckForToken(tokenDeque))) return false;
            
            
            //Check for all Schemes
            if(listTokenfsaPtr->isValid(tokenDeque)) {
                listTokenfsaDeque.push_back(listTokenfsaPtr);
                listTokenfsaPtr = new Parameter();
            }
            else return false;
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() {
        ostringstream os;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            os << "," << **it++;
        }
        return os.str();
    }
};

class HeadPredicate : public TokenItem{
public:
    TokenItemInterface* idToken;
    TokenItemInterface* scndIdToken;
    IDList idList;
    HeadPredicate(string st = "HEADPREDICATE") : TokenItem(st){
        tItemPtr = NULL;
        idToken = new TokenItemInterface("ID");
        scndIdToken = new TokenItemInterface("ID");
    }
    ~HeadPredicate() {
        delete idToken;
        delete scndIdToken;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* nullPtr = NULL;
        
        tItemPtr = new TokenItemInterface("ID");
        if(!(CheckForToken(tokenDeque, idToken))) return false;
        
        tItemPtr = new TokenItemInterface("LEFT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        //Check for all ID's
        
        tItemPtr = new TokenItemInterface("ID");
        if(!(CheckForToken(tokenDeque, scndIdToken))) return false;
        
        if(!(idList.isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("RIGHT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        return true;
    }
    string str() {
        return idToken->str() + "(" + scndIdToken->str() + idList.str() + ")" ;
    }
};


class Predicate : public TokenItem {
public:
    TokenItemInterface* idToken;
    TokenItemInterface* parameterToken;
    ParameterList parameterList;
    Predicate(string st = "PREDICATE") : TokenItem(st){
        tItemPtr = NULL;
        idToken = new TokenItemInterface("ID");
        parameterToken = new Parameter;
    }
    ~Predicate() {
        delete idToken;
        delete parameterToken;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* nullPtr = NULL;
        
        tItemPtr = new TokenItemInterface("ID");
        if(!(CheckForToken(tokenDeque, idToken))) return false;
        
        tItemPtr = new TokenItemInterface("LEFT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        //Check for all ID's
        
        if(!(parameterToken->isValid(tokenDeque))) return false;
        if(!(parameterList.isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("RIGHT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        return true;
    }
    string str() {
        return idToken->str() + "(" + parameterToken->str() + parameterList.str() + ")" ;
    }
};

class PredicateList : public TokenItemLists{
public:
    PredicateList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new Predicate();
        followSetItemPtr = new TokenItemInterface("PERIOD");
    }
    ~PredicateList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("COMMA");
        if(*tokenDeque.front() == *tItemPtr) {
            if(!(CheckForToken(tokenDeque))) return false;
            
            
            //Check for all Schemes
            if(listTokenfsaPtr->isValid(tokenDeque)) {
                listTokenfsaDeque.push_back(listTokenfsaPtr);
                listTokenfsaPtr = new Predicate();
            }
            else return false;
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    string str() {
        ostringstream os;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            os << "," << **it++;
        }
        return os.str();
    }
};



class Scheme : public TokenItem {
public:
    TokenItemInterface* idToken;
    TokenItemInterface* scndIdToken;
    IDList idList;
    Scheme(string st = "SCHEME") : TokenItem(st){
        tItemPtr = NULL;
        idToken = new TokenItemInterface("ID");
        scndIdToken = new TokenItemInterface("ID");
    }
    ~Scheme() {
        delete idToken;
        delete scndIdToken;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* nullPtr = NULL;
        
        tItemPtr = new TokenItemInterface("ID");
        if(!(CheckForToken(tokenDeque, idToken))) return false;
        
        tItemPtr = new TokenItemInterface("LEFT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        //Check for all ID's
        
        tItemPtr = new TokenItemInterface("ID");
        if(!(CheckForToken(tokenDeque, scndIdToken))) return false;
        
        if(!(idList.isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("RIGHT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        return true;
    }
    string str() {
        return "  " + idToken->str() + "(" + scndIdToken->str() + idList.str() + ")";
    }
};

class Fact : public TokenItem {
public:
    TokenItemInterface* idToken;
    TokenItemInterface* stringToken;
    StringList stringList;
    Fact(string st = "FACT") : TokenItem(st){
        tItemPtr = NULL;
        idToken = new TokenItemInterface("ID");
        stringToken = new TokenItemInterface("STRING");
    }
    ~Fact() {
        delete idToken;
        delete stringToken;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* nullPtr = NULL;
        
        tItemPtr = new TokenItemInterface("ID");
        if(!(CheckForToken(tokenDeque, idToken))) return false;
        
        tItemPtr = new TokenItemInterface("LEFT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        tItemPtr = new TokenItemInterface("STRING");
        if(!(CheckForToken(tokenDeque, stringToken))) return false;
        
        if(!(stringList.isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("RIGHT_PAREN");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        tItemPtr = new TokenItemInterface("PERIOD");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        return true;
    }
    string str() {
        return "  " + idToken->str() + "(" + stringToken->str() + stringList.str() + ").";
    }
    deque<string*> GetDomain() {
        deque<string*> domainDeque = stringList.GetDomain();
        string* str = new string(stringToken->str());
        domainDeque.push_front(str);
        return domainDeque;
    }
};

class Rule : public TokenItem{
public:
    TokenItemInterface* headPredicate;
    TokenItemInterface* predicate;
    PredicateList predicateList;
    Rule(string st = "RULE") : TokenItem(st){
        tItemPtr = NULL;
        headPredicate = new HeadPredicate;
        predicate = new Predicate;
    }
    ~Rule() {
        delete headPredicate;
        delete predicate;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* nullPtr = NULL;
        
        
        if(!(headPredicate->isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("COLON_DASH");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        if(!(predicate->isValid(tokenDeque))) return false;
        if(!(predicateList.isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("PERIOD");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        return true;
    }
    string str() {
        return "  " + headPredicate->str() + " :- " + predicate->str() + predicateList.str() + ".";
    }
};

class Query : public TokenItem{
public:
    TokenItemInterface* predicate;
    Query(string st = "QUERY") : TokenItem(st){
        tItemPtr = NULL;
        predicate = new Predicate;
    }
    ~Query() {
        delete predicate;
    }
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        TokenItemInterface* nullPtr = NULL;
        
        if(!(predicate->isValid(tokenDeque))) return false;
        
        tItemPtr = new TokenItemInterface("Q_MARK");
        if(!(CheckForToken(tokenDeque, nullPtr))) return false;
        
        return true;
    }
    string str() {
        return "  " + predicate->str() + "?";
    }
};


class SchemeList : public TokenItemLists{
public:
    SchemeList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new Scheme();
        followSetItemPtr = new TokenItemInterface("FACTS");
    }
    ~SchemeList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("ID");
        if(*tokenDeque.front() == *tItemPtr) {
            //Check for all Schemes
            if(listTokenfsaPtr->isValid(tokenDeque)) {
                listTokenfsaDeque.push_back(listTokenfsaPtr);
                listTokenfsaPtr = new Scheme();
            }
            else return false;
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
};

class FactList : public TokenItemLists{
public:
    FactList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new Fact();
        followSetItemPtr = new TokenItemInterface("RULES");
    }
    ~FactList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("ID");
        if(*tokenDeque.front() == *tItemPtr) {
            //Check for all Schemes
            if(listTokenfsaPtr->isValid(tokenDeque)) {
                listTokenfsaDeque.push_back(listTokenfsaPtr);
                listTokenfsaPtr = new Fact();
            }
            else return false;
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
    set<string> GetDomain() {
        ostringstream os;
        set<string> domainSet;
        deque<TokenItemInterface*>::iterator it = listTokenfsaDeque.begin();
        while(it != listTokenfsaDeque.end()) {
            TokenItemInterface* ptr = *it;
            deque<string*> domainDeque = ptr->GetDomain();
            deque<string*>::iterator iter = domainDeque.begin();
            while(iter != domainDeque.end()) {
                domainSet.insert(**iter);
                iter++;
            }
            it++;
            for(size_t i = domainDeque.size(); i > 0; i--) {
                delete domainDeque.front();
                domainDeque.pop_front();
            }
        }
        
        return domainSet;
    }
};

class RuleList : public TokenItemLists {
public:
    RuleList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new Rule();
        followSetItemPtr = new TokenItemInterface("QUERIES");
    }
    ~RuleList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("ID");
        if(*tokenDeque.front() == *tItemPtr) {
            //Check for all Schemes
            if(listTokenfsaPtr->isValid(tokenDeque)) {
                listTokenfsaDeque.push_back(listTokenfsaPtr);
                listTokenfsaPtr = new Rule();
            }
            else return false;
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
};

class QueryList : public TokenItemLists {
public:
    QueryList() {
        tItemPtr = NULL;
        listTokenfsaPtr = new Query();
        followSetItemPtr = new TokenItemInterface("EOF");
    }
    ~QueryList() {}
    bool isValid(deque<TokenItemInterface*> &tokenDeque) {
        
        tItemPtr = new TokenItemInterface("ID");
        if(*tokenDeque.front() == *tItemPtr) {
            //Check for all Schemes
            if(listTokenfsaPtr->isValid(tokenDeque)) {
                listTokenfsaDeque.push_back(listTokenfsaPtr);
                listTokenfsaPtr = new Query();
            }
            else return false;
            if(!(isValid(tokenDeque))) return false;
            else return true;
        }
        else if(!(*tokenDeque.front() == *followSetItemPtr)) return false;
        return true;
    }
};


#endif /* NonTerminals_h */
