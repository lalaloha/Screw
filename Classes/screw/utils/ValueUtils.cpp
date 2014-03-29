/*
 * ObjectUtils.cpp
 *
 *  Created on: Mar 16, 2014
 *      Author: hiepnd
 */

#include "ValueUtils.h"
#include "StringUtils.h"

NS_SCREW_UTILS_BEGIN

Value ValueGetter::NonConstNull;

#pragma mark Setter
Value &ValueGetter::get(const Value &data, const string &path) {
    vector<string> coms = StringUtils::split(path, "/");
    
    Value* d = const_cast<Value*>(&data);
    for (vector<string>::iterator it = coms.begin(); it != coms.end(); it++) {
        CCASSERT(d->getType() == Value::Type::MAP, "Fix me");
        CCASSERT(it->length(), "Fix me - empty key is not allowed");
        
        ValueMap& v = d->asValueMap();
        ValueMap::iterator i = v.find(*it);
        
        if (i == v.end()) {
            return ValueGetter::NonConstNull;
        } else {
            d = &i->second;
        }
    }
    
    return *d;
}

int ValueGetter::getInt(const Value &data, const string &path){
    return ValueGetter::get(data, path).asInt();
}

long ValueGetter::getLong(const Value &data, const string &path){
    //Long not yet supported be Value
    string str = ValueGetter::get(data, path).asString();
    return std::atol(str.c_str());
}

float ValueGetter::getFloat(const Value &data, const string &path){
    return ValueGetter::get(data, path).asFloat();
}

double ValueGetter::getDouble(const Value &data, const string &path){
    return ValueGetter::get(data, path).asDouble();
}

string ValueGetter::getString(const Value &data, const string &path){
    return ValueGetter::get(data, path).asString();
}

#pragma mark Setter
void ValueSetter::set(Value& data, const string &path, const Value &value) {
    vector<string> coms = StringUtils::split(string(path), "/");
    
    CCASSERT(coms.size() > 0, "Fix me - path must be something");
    CCASSERT(data.getType() == Value::Type::MAP, "Fix me - data must be a MAP");
    
    ValueMap* d = &data.asValueMap();
    for (int i = 0; i < coms.size() - 1; i++) {
        ValueMap::iterator it = d->find(coms[i]);
        if (it == d->end()) {
            ValueMap m;
            pair<ValueMap::iterator, bool> newI = d->insert(pair<string, Value>(coms[i], Value(m)));
            d = &(newI.first->second.asValueMap());
        } else {
            CCASSERT(it->second.getType() == Value::Type::MAP, "Fix me");
            d = &it->second.asValueMap();
        }
    }
    
    (*d)[coms[coms.size() - 1]] = value;
}

void ValueSetter::set(Value& data, const string &path, int value){
    set(data, path, Value(value));
}

void ValueSetter::set(Value& data, const string &path, long value){
    set(data, path, Value(StringUtils::toString(value)));
}

void ValueSetter::set(Value& data, const string &path, float value){
    set(data, path, Value(value));
}

void ValueSetter::set(Value& data, const string &path, double value){
    set(data, path, Value(value));
}

void ValueSetter::set(Value& data, const string &path, bool value){
    set(data, path, Value(value));
}

void ValueSetter::set(Value& data, const string &path, const string& value){
    set(data, path, Value(value));
}

void ValueSetter::clear(Value& data, const string &path){
    vector<string> coms = StringUtils::split(path, "/");
    if (coms.size() >= 1) {
        vector<string> parent;
        for (int i = 0; i < coms.size() - 1; i++) {
            parent.push_back(coms[i]);
        }
        
        string parentPath = StringUtils::join(parent, "/");
        Value& obj = ValueGetter::get(data, parentPath.c_str());
        if (obj.getType() == Value::Type::MAP) {
            ValueMap& m = obj.asValueMap();
            ValueMap::iterator it = m.find(coms[coms.size() - 1]);
            if (it != m.end()) {
                m.erase(it);
            }
        }
    }
}

NS_SCREW_UTILS_END
