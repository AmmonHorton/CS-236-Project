//
//  Tokenizer.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 1/26/21.
//

#ifndef TokenChecks_h
#define TokenChecks_h


#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
using namespace std;

class TokenCheck {
private:
    string currentInput;
    size_t characterRead;
    bool endOfFile;
public:
    TokenCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : currentInput("defaultInput"), characterRead(defaultCharacterPos), endOfFile(false) {}
    virtual ~TokenCheck(void) {}
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
    char CheckChar(void) {
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
    virtual string str() {
        return currentInput.substr(0, characterRead);
    }
};



class UndefinedCheck : public TokenCheck {
public:
    UndefinedCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        DeclineState();
        return;
    }
    string str(){
        return ("UNDEFINED,\"" + GetInput().substr(0, 1) + "\"");
    }
};


class StringCheck : public TokenCheck {
public:
    StringCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar()== '\'') {
            IncCharacter();
            S1();
        }
        else DeclineState();
        return;
    }
    void S1() {
        if(GetRead() == InputSize() && !getFileStatus()) return;
        else if (GetRead() == InputSize() && getFileStatus()) {
            DeclineState();
            return;
        }
        if(CheckChar() != '\'') {
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
        if(CheckChar() == '\'') {
            IncCharacter();
            S1();
        }
        else {
            return;
        }
    }
    string str(){
        return ("STRING,\"" + TokenCheck::str() + "\"");
    }
};

class CommentCheck : public TokenCheck {
public:
    CommentCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar()== '#') {
            IncCharacter();
            S1();
        }
        else DeclineState();
        return;
    }
    void S1() {
        if(CheckChar() == '|') {
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
        if(CheckChar() == '|') {
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
        if(CheckChar() != '\n') {
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
        if(CheckChar() == '#') {
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
        return ("COMMENT,\"" + TokenCheck::str() + "\"");
    }
};

class IDCheck : public TokenCheck {
public:
    IDCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(isalpha(CheckChar())) {
            IncCharacter();
            S1();
        }
        return;
    }
    void S1() {
        if(GetRead() == InputSize()) return;
        
        
        if(isalpha(CheckChar())|| isdigit(CheckChar())) {
            IncCharacter();
            S1();
        }
        else if(CheckChar() == ' ' || CheckChar() == '\n' || CheckChar() == ',' || CheckChar() == '(' || CheckChar() == ')')
            return;
        else {
            DeclineState();
        }
    }
    string str(){
        return ("ID,\"" + TokenCheck::str() + "\"");
    }
};

class CommaCheck : public TokenCheck {
public:
    CommaCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == ',') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("COMMA,\"" + TokenCheck::str() + "\"");
    }
};

class PeriodCheck : public TokenCheck {
public:
    PeriodCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == '.') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("PERIOD,\"" + TokenCheck::str() + "\"");
    }
};

class Q_MarkCheck : public TokenCheck {
public:
    Q_MarkCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == '?') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("Q_MARK,\"" + TokenCheck::str() + "\"");
    }
};


class Left_ParenCheck : public TokenCheck {
public:
    Left_ParenCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == '(') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("LEFT_PAREN,\"" + TokenCheck::str() + "\"");
    }
};

class Right_ParenCheck : public TokenCheck {
public:
    Right_ParenCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == ')') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("RIGHT_PAREN,\"" + TokenCheck::str() + "\"");
    }
};

class ColonCheck : public TokenCheck {
public:
    ColonCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == ':') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("COLON,\"" + TokenCheck::str() + "\"");
    }
};

class Colon_DashCheck : public TokenCheck {
public:
    Colon_DashCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == ':') {
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
        
        if(CheckChar() == '-') {
            IncCharacter();
            return;
        }
    }
    string str(){
        return ("COLON_DASH,\"" + TokenCheck::str() + "\"");
    }
};

class MultiplyCheck : public TokenCheck {
public:
    MultiplyCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == '*') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("MULTIPLY,\"" + TokenCheck::str() + "\"");
    }
};

class AddCheck : public TokenCheck {
public:
    AddCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == '+') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("ADD,\"" + TokenCheck::str() + "\"");
    }
};

class SchemesCheck : public TokenCheck {
public:
    SchemesCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == 'S') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(CheckChar() == 'c') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
    }
    void S2() {
        if(CheckChar() == 'h') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CheckChar() == 'e') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CheckChar() == 'm') {
            IncCharacter();
            S5();
        }
        else {
            DeclineState();
        }
    }
    void S5() {
        if(CheckChar() == 'e') {
            IncCharacter();
            S6();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S6() {
        if(CheckChar() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
    }
    string str(){
        return ("SCHEMES,\"" + TokenCheck::str() + "\"");
    }
};

class FactsCheck : public TokenCheck {
public:
    FactsCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == 'F') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(CheckChar() == 'a') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
    }
    void S2() {
        if(CheckChar() == 'c') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CheckChar() == 't') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CheckChar() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("FACTS,\"" + TokenCheck::str() + "\"");
    }
};

class RulesCheck : public TokenCheck {
public:
    RulesCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == 'R') {
            IncCharacter();
            S1();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S1() {
        if(CheckChar() == 'u') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S2() {
        if(CheckChar() == 'l') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CheckChar() == 'e') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CheckChar() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("RULES,\"" + TokenCheck::str() + "\"");
    }
};

class QueriesCheck : public TokenCheck {
public:
    QueriesCheck(string defaultInput = "", size_t defaultCharacterPos = 0) : TokenCheck(defaultInput, defaultCharacterPos) {}
    void S0() {
        if(CheckChar() == 'Q') {
            IncCharacter();
            S1();
        }
        return;
    }
    void S1() {
        if(CheckChar() == 'u') {
            IncCharacter();
            S2();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S2() {
        if(CheckChar() == 'e') {
            IncCharacter();
            S3();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S3() {
        if(CheckChar() == 'r') {
            IncCharacter();
            S4();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S4() {
        if(CheckChar() == 'i') {
            IncCharacter();
            S5();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S5() {
        if(CheckChar() == 'e') {
            IncCharacter();
            S6();
        }
        else {
            DeclineState();
        }
        return;
    }
    void S6() {
        if(CheckChar() == 's') {
            IncCharacter();
            return;
        }
        else {
            DeclineState();
        }
        return;
    }
    string str(){
        return ("QUERIES,\"" + TokenCheck::str() + "\"");
    }
};
#endif /* TokenChecks_h */
