//
//  Tuple.h
//  CS_236_Project_1
//
//  Created by AMMON HORTON on 3/8/21.
//

#ifndef Tuple_h
#define Tuple_h

template<typename T>
class Tuple {
public:
    vector<T> tupleObjects;
    Tuple() {}
    Tuple(const Tuple& tp) {
        for(size_t i = 0; i < tp.tupleObjects.size(); i++) {
            tupleObjects.push_back(tp.tupleObjects[i]);
        }
    }
    T& push_back(T& object) {
        tupleObjects.push_back(object);
        return tupleObjects.back();
    }
    size_t size() {
        return tupleObjects.size();
    }
    string operator[] (size_t index) {
        if (index >= size()) throw string("Array index out of bound, exiting");
        return tupleObjects[index];
    }
    bool operator<(const Tuple& rhs) {
        if(tupleObjects.size() < rhs.tupleObjects.size()) return true;
        else if(tupleObjects.size() > rhs.tupleObjects.size()) return false;
        else {
            for(size_t i = 0; i < tupleObjects.size(); i++) {
                if(tupleObjects[i] < rhs.tupleObjects[i]) return true;
                if(tupleObjects[i] > rhs.tupleObjects[i]) return false;
            }
            return true;
        }
    }
    bool operator==(const Tuple& rhs) {
        if(tupleObjects.size() != rhs.tupleObjects.size()) return false;
        else {
            for(size_t i = 0; i < tupleObjects.size(); i++) {
                if(tupleObjects[i] != rhs.tupleObjects[i]) return false;
            }
        }
        return true;
    }
    void switch_columns(size_t index1, size_t index2) {
        T tempObject;
        tempObject = tupleObjects[index1];
        tupleObjects[index1] = tupleObjects[index2];
        tupleObjects[index2] = tempObject;
    }
    
    bool select_row(size_t index, T& value) const {
        if(tupleObjects[index] == value) return true;
        return false;
    }
    
    T select_column(size_t index) const {
        return tupleObjects[index];
    }
};

#endif /* Tuple_h */
