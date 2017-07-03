#ifndef __ENV_H__
#define __ENV_H__

#include "obj.h"
#include <unordered_map>
#include <utility>
using namespace std;

class Env { 
    public:
        void set(const string& name, const shared_ptr<Object>& val) {
            env_.insert(make_pair(name, val));
        }
    
        pair<bool, shared_ptr<Object>> get(const string& name) {
            auto it = env_.find(name);

            if(it != env_.end()) {
                return make_pair(true, it->second);
            } else {
                return make_pair(false, nullptr);
            }
        } 
    private:
        unordered_map<string, shared_ptr<Object>> env_;

};

#endif
