//
//  Header.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 3/8/21.
//

#ifndef Header_h
#define Header_h

class Header {
private:
    vector<string> attributeNames;
public:
    Header() {}
    size_t size() const {
        return attributeNames.size();
    }
    string& operator[] (size_t index) {
        if (index >= size()) throw string("Array index out of bound, exiting");
        return attributeNames[index];
    }
    string push_back(string str) {
        attributeNames.push_back(str);
        return str;
    }
};

#endif /* Header_h */
