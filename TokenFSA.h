//
//  Tokenizer.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 1/26/21.
//

#ifndef TokenFSA_h
#define TokenFSA_h


#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sstream>
using namespace std;

class TokenFSA {
public:
    string selfType;
    bool accepting;
private:
    string currentInput;
    size_t characterRead;
    size_t linePos;
    bool endOfFile;
public:
    TokenFSA(string st = "TokenFSA", string defaultInput = "", size_t defaultCharacterPos = 0, size_t defaultLine = 0) : selfType(st), currentInput(defaultInput), characterRead(defaultCharacterPos), linePos(defaultLine), endOfFile(false) {}
    virtual ~TokenFSA(void) {}
    
    TokenFSA(const TokenFSA &tc) {
        selfType = tc.selfType;
        currentInput = tc.currentInput;
        characterRead = tc.characterRead;
        linePos = tc.linePos;
        endOfFile = tc.endOfFile;
    }
    size_t GetRead(string input, bool eof = false){
        endOfFile = eof;
        if(input == currentInput) return characterRead;
        else{
            currentInput = input;
            DeclineState();
            S0();
            return characterRead;
        }
    }
    bool getFileStatus() {return endOfFile;}
    size_t GetRead(){return characterRead;}
    size_t InputSize(void) {return currentInput.size();}
    char CharacterAtRead(void) {
        if(GetRead() == InputSize()) {
            DeclineState();
            return '\n';
        }
        return currentInput.at(characterRead);
        
    }
    string GetInput() {return currentInput;}
    void IncCharacter(void) {characterRead++;}
    void DeclineState(void) {characterRead = 0;}
    virtual void S0() = 0;
    string str() const {
        ostringstream os;
        os << "(" << selfType << ",\"" << currentInput.substr(0, characterRead) << "\"," << linePos << ")";
        return os.str();
    }
    string GetToken() const {
        return currentInput.substr(0, characterRead);
    }
    bool operator==(const TokenFSA& tc) {return this->selfType == tc.selfType;}
    void SetVar(string input, size_t characterPos, size_t line) {
        characterRead = characterPos;
        currentInput = input;
        linePos = line;
        return;
    }
    void SetVar(size_t line) {linePos = line; return;}
};



class UndefinedFSA : public TokenFSA {
public:
    UndefinedFSA(string st = "UNDEFINED", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st, defaultInput, defaultCharacterPos) {}
    UndefinedFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        DeclineState();
        return;
    }
};

class EOFFSA : public TokenFSA {
public:
    EOFFSA(string st = "EOF", size_t line = 0, string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st, defaultInput, defaultCharacterPos, line) {}
    void S0() {
        DeclineState();
        return;
    }
    string str(){
        return ("");
    }
};


class StringFSA : public TokenFSA {
public:
    StringFSA(string st = "STRING", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st, defaultInput, defaultCharacterPos) {}
    StringFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        accepting = false;
        if(CharacterAtRead()== '\'') {
            IncCharacter();
            S1();
        }
        else DeclineState();
        return;
    }
    void S1() {
        if(GetRead() == InputSize() && !getFileStatus()) {accepting = true; return;}
        else if (GetRead() == InputSize() && getFileStatus()) {
            DeclineState();
            return;
        }
        if(CharacterAtRead() != '\'') {
            IncCharacter();
            S1();
        }
        else{
            IncCharacter();
            S2();
        }
        return;
    }
    void S2() {
        if(GetRead() == InputSize()) return;
        if(CharacterAtRead() == '\'') {
            IncCharacter();
            S1();
        }
        else {
            accepting = true;
            return;
        }
    }
};

class CommentFSA : public TokenFSA {
public:
    CommentFSA(string st = "COMMENT", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    CommentFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        accepting = false;
        if(CharacterAtRead()== '#') {
            IncCharacter();
            S1();
        }
        else DeclineState();
        return;
    }
    void S1() {
        if(CharacterAtRead() == '|') {
            IncCharacter();
            S2Block();
        }
        else{
            IncCharacter();
            S2Line();
        }
        return;
    }
    void S2Block() {
        if(GetRead() == InputSize() && !getFileStatus()) return;
        else if (GetRead() == InputSize() && getFileStatus()) {
            DeclineState();
            return;
        }
        if(CharacterAtRead() == '|') {
            IncCharacter();
            S3Block();
        }
        else {
            IncCharacter();
            S2Block();
        }
        return;
    }
    void S2Line() {
        if(GetRead() == InputSize()) return;
        if(CharacterAtRead() != '\n') {
            IncCharacter();
            S2Line();
        }
        else{
            return;
        }
        return;
    }
    void S3Block() {
        if(GetRead() == InputSize() && !getFileStatus()) return;
        else if (GetRead() == InputSize() && getFileStatus()) {
            DeclineState();
            return;
        }
        if(CharacterAtRead() == '#') {
            accepting = true;
            IncCharacter();
            return;
        }
        else {
            IncCharacter();
            S2Block();
        }
        return;
    }
    string str(){
        return ("COMMENT,\"" + TokenFSA::str() + "\"");
    }
};

class IDFSA : public TokenFSA {
public:
    IDFSA(string st = "ID", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    IDFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(isalpha(CharacterAtRead())) {
            IncCharacter();
            S1();
        }
        return;
    }
    void S1() {
        if(GetRead() == InputSize()) return;
        
        
        if(isalpha(CharacterAtRead())|| isdigit(CharacterAtRead())) {
            IncCharacter();
            S1();
        }
        else if(!(isalpha(CharacterAtRead())|| isdigit(CharacterAtRead())))
            return;
        else {
            DeclineState();
        }
    }
    string str(){
        return ("ID,\"" + TokenFSA::str() + "\"");
    }
};

class CommaFSA : public TokenFSA {
public:
    CommaFSA(string st = "COMMA", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    CommaFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == ',') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("COMMA,\"" + TokenFSA::str() + "\"");
    }
};

class PeriodFSA : public TokenFSA {
public:
    PeriodFSA(string st = "PERIOD", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    PeriodFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == '.') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("PERIOD,\"" + TokenFSA::str() + "\"");
    }
};

class Q_MarkFSA : public TokenFSA {
public:
    Q_MarkFSA(string st = "Q_MARK", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    Q_MarkFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == '?') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("Q_MARK,\"" + TokenFSA::str() + "\"");
    }
};


class Left_ParenFSA : public TokenFSA {
public:
    Left_ParenFSA(string st = "LEFT_PAREN", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    Left_ParenFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == '(') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("LEFT_PAREN,\"" + TokenFSA::str() + "\"");
    }
};

class Right_ParenFSA : public TokenFSA {
public:
    Right_ParenFSA(string st = "RIGHT_PAREN", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    Right_ParenFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == ')') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("RIGHT_PAREN,\"" + TokenFSA::str() + "\"");
    }
};

class ColonFSA : public TokenFSA {
public:
    ColonFSA(string st = "COLON", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    ColonFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == ':') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("COLON,\"" + TokenFSA::str() + "\"");
    }
};

class Colon_DashFSA : public TokenFSA {
public:
    Colon_DashFSA(string st = "COLON_DASH", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    Colon_DashFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == ':') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(GetRead() == InputSize()) {
            DeclineState();
            return;
        }
        
        if(CharacterAtRead() == '-') {
            IncCharacter();
            return;
        }
    }
    string str(){
        return ("COLON_DASH,\"" + TokenFSA::str() + "\"");
    }
};

class MultiplyFSA : public TokenFSA {
public:
    MultiplyFSA(string st = "MULTIPLY", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    MultiplyFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == '*') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("MULTIPLY,\"" + TokenFSA::str() + "\"");
    }
};

class AddFSA : public TokenFSA {
public:
    AddFSA(string st = "ADD", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    AddFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == '+') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("ADD,\"" + TokenFSA::str() + "\"");
    }
};

class SchemesFSA : public TokenFSA {
public:
    SchemesFSA(string st = "SCHEMES", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    SchemesFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == 'S') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(CharacterAtRead() == 'c') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
    }
    void S2() {
        if(CharacterAtRead() == 'h') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CharacterAtRead() == 'e') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CharacterAtRead() == 'm') {
            IncCharacter();
            S5();
        }
        else {
            DeclineState();
        }
    }
    void S5() {
        if(CharacterAtRead() == 'e') {
            IncCharacter();
            S6();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S6() {
        if(CharacterAtRead() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
    }
    string str(){
        return ("SCHEMES,\"" + TokenFSA::str() + "\"");
    }
};

class FactsFSA : public TokenFSA {
public:
    FactsFSA(string st = "FACTS", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    FactsFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == 'F') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(CharacterAtRead() == 'a') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
    }
    void S2() {
        if(CharacterAtRead() == 'c') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CharacterAtRead() == 't') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CharacterAtRead() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("FACTS,\"" + TokenFSA::str() + "\"");
    }
};

class RulesFSA : public TokenFSA {
public:
    RulesFSA(string st = "RULES", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    RulesFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == 'R') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(CharacterAtRead() == 'u') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S2() {
        if(CharacterAtRead() == 'l') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CharacterAtRead() == 'e') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CharacterAtRead() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("RULES,\"" + TokenFSA::str() + "\"");
    }
};

class QueriesFSA : public TokenFSA {
public:
    QueriesFSA(string st = "QUERIES", string defaultInput = "", size_t defaultCharacterPos = 0) : TokenFSA(st,defaultInput, defaultCharacterPos) {}
    QueriesFSA(const TokenFSA &tc) : TokenFSA(tc) {}
    void S0() {
        if(CharacterAtRead() == 'Q') {
            IncCharacter();
            S1();
        }
        return;
    }
    void S1() {
        if(CharacterAtRead() == 'u') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S2() {
        if(CharacterAtRead() == 'e') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CharacterAtRead() == 'r') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CharacterAtRead() == 'i') {
            IncCharacter();
            S5();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S5() {
        if(CharacterAtRead() == 'e') {
            IncCharacter();
            S6();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S6() {
        if(CharacterAtRead() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("QUERIES,\"" + TokenFSA::str() + "\"");
    }
};
#endif /* TokenFSA_h */
