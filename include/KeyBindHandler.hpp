#pragma once
#ifndef HPP__KeyBindHandler
#define HPP__KeyBindHandler


#include <iostream>

#include <string>
#include <vector>
#include <initializer_list>

#include <chrono>


/// @brief DIY keyBind handler namespace
namespace DIY_KBH {
    using _funcDef = void(*)(void);
    
    /// @brief Number of instances that a key can not be called for it to still hold the "called" state.
    inline size_t setting_callDecay = 2;


    /// @brief Instance struct used for holding a single keybind configuration
    struct KeyBindHandler_unit {
        std::string         _label;
        std::vector<int>    _keys;
        _funcDef            _callFunc       = nullptr;
        bool                _orderImportant = false;
        bool                _isolImportant  = true;
        bool                _callEdge       = true;
    };


    /**
     * Handler for detecting key binds/combinations and calling assosciated functions:
     *  - key   type: `int`
     *  - func  type: `void (*)(void)`
     */
    class KeyBindHandler {
        private:
        // std::vector<std::string>        _label;         // label for the keybind.
        // std::vector<std::vector<int>>   _keybinds;      // Main container to hold the keybind combinations.
        // std::vector<_funcDef>           _callFuncs;     // Function to call when keybind is called.
        // std::vector<bool>               _orderImportant;// Boolean for whether order of the keybind combination is important. Default is false
        // std::vector<bool>               _callEdge;      // What edge of the signal to call the assosciated function in: `true`-rising edge; `false`-falling edge. Default to `true`

        /**
         * Main container that holds the configuration and values of every keybind
         */
        std::vector<KeyBindHandler_unit>   _keyBinds;
        // Positive integer value to assosciate with a keybind call to hold the decay value. 
        std::vector<int>                    _callDecay;
        // Boolean for whether the keyBind decay value has reached the "opposite decay value" for it's callEdge after latest funcCall
        std::vector<bool>                   _released;
        // Boolean for whether the keybind 
        std::vector<bool>                   _just_released;
        // Boolean for whether the keyBind has been called in this frame.
        std::vector<bool>                   _called;

        public:
        void update(std::vector<int> _pressed_keys);
 
        
        KeyBindHandler();
        KeyBindHandler(KeyBindHandler_unit _kbh_inst);
        KeyBindHandler(std::string _label, std::vector<int> _keys, void (*_func)(void), bool _ordImportant=false, bool _isolImportant=false, bool _callSignalEdge=true);
        KeyBindHandler(std::initializer_list<KeyBindHandler_unit> _kbhi_cont);
        KeyBindHandler(std::vector<KeyBindHandler_unit> _kbhi_cont);

        ~KeyBindHandler();

        size_t size() const;

        // int add(KeyBindHandler_unit _newKBH);
        // int add(std::string _label, std::vector<int> _keys, _funcDef _callFunc, bool _ordImportant=false, bool _isolImportant=false, bool _callEdge=true);

        int edit(std::string _label, std::vector<int> _newKeys, bool _call_except=true);

        KeyBindHandler_unit get(std::string _label);
        KeyBindHandler_unit get(std::string _label) const;

        /**
         * @brief Bool for whether the _label assosciated keybind is clicked in current program loop-iteration/update according to `callEdge` parameter (`callEdge==true`->initial press signal; `callEdge==false`->press release signal)
         * @note if the keyBind is held, this function will only return true during the initial signal (according to callEdge as previously mentioned)
         * @param _label `std::string` of keybind label
         * @return true if keybind is pressed(callEdge*) in current update call.
         * @return false every other scenario.
         */
        bool clicked(std::string _label);
        /**
         * @brief Bool for whether the _label assosciated keybind is currently held
         * 
         * @param _label `std::string` of keybind label
         * @return true if keybind is pressed(callEdge*)
         * @return false every other scenario
         */
        bool pressing(std::string _label);
        /**
         * @brief Bool for whether the _label assosciated keybind was released (reached opposite callEdge signal) in current program loop-iteration/update.
         * 
         * @param _label `std::string` of keybind label
         * @return true if keybind was released
         * @return false 
         */
        bool released(std::string _label);
    };


    /// ========== Helper Functions ==========

    template<typename checkType>
    inline int findVectorIndex(std::vector<checkType> vec, checkType toFind) {
        typename std::vector<checkType>::iterator idx = find(vec.begin(), vec.end(), toFind);
        if(idx!=vec.end()) return idx-vec.begin();
        else return -1;
    }

    /**
     * @brief search and find index in std::string vector to given std::string to find
     * @param vec `std::std::vector<std::string>` vector search through
     * @param toFind `std::string` var to find in vector
     * @return index in vector, or `-1` if not found
    */
    inline int findVectorIndex(std::vector<std::string> vec, std::string toFind) {
        std::vector<std::string>::iterator idx = find(vec.begin(), vec.end(), toFind);

        if(idx != vec.end()) {
            return idx-vec.begin();
        }
        else {
            return -1;
        }
    }
    /**
     * @brief search and find index in std::string vector to given std::string to find
     * @param vecvec `std::std::vector<std::std::vector<std::string>>` vector with vector search through
     * @param toFind `std::string` var to find in vector
     * @return 2d `std::std::vector<int>` of position/coordinates
    */
    inline std::vector<int> findVectorIndex(std::vector<std::vector<std::string>> vecvec, std::string toFind, bool printChecks=false) {
        if(printChecks) {
            std::cout <<"\"" <<toFind << "\": "<< vecvec.size()<< ": {\n";
            for(std::vector<std::string> vecStr: vecvec) {
                std::cout << "\t{ ";
                for(std::string strIIng: vecStr) std::cout << strIIng << " ";
                std::cout<<"}" << std::endl;
            }
            std::cout << "}"<<std::endl;
        }
    
        if(vecvec.size()==0 || vecvec[0].size()==0) return std::vector<int>(2, -1);

        std::vector<int> indices(2, -1);
        for(size_t i=0; i<vecvec.size(); i++) {
            for(size_t n=0; n<vecvec[i].size(); n++) {
                if(vecvec[i][n]==toFind) {
                    indices[0] = i;
                    indices[1] = n;
                    return indices;
                }
            }
        }
        return indices;
    }
    inline std::vector<int> findVectorIndex(std::vector<std::vector<std::string>> vecvec, std::vector<std::string> toFind, bool printChecks=false) {
        std::vector<int> indices(2, -1);
        for(std::string subFind: toFind) {
            indices = findVectorIndex(vecvec, subFind, printChecks);
            if(indices[0] != -1) return indices;
        }
        return indices;
    }

    /**
     * @brief Match vectors for whether they contain the same elements
     * 
     * @tparam typeVar type of the elements in the vectors
     * @param vec0 first vector
     * @param vec1 second vector
     * @param orderImportant whether the order element values have to match
     * @return true if the vectors matched
     * @return false if the vectors didn't match
     */
    template<typename typeVar>
    inline bool match_vectors(std::vector<typeVar> vec0, std::vector<typeVar> vec1, bool orderImportant=false) {
        if(vec0.size()!=vec1.size()) throw std::invalid_argument("the vector's aren't the same size.");

        std::vector<size_t> avoidIdx;
        for(size_t i=0; i<vec0.size(); i++) {
            for(size_t ii=0; ii<vec1.size(); ii++) {
                if(vec0[i]==vec1[ii] && findVectorIndex<size_t>(avoidIdx, ii)!=ii) {
                    avoidIdx.push_back(ii);
                    continue;
                }
                else if(orderImportant) return false;
            }
        }
        if(avoidIdx.size()!=vec0.size()) return false;

        return true;
    }

    /**
     * @brief Find a std::vector inside another std::vector
     * 
     * @tparam typeVar type of the elements in the vectors
     * @param toFind the vector to find
     * @param toSearch the vector to find in / search through.
     * @param orderImportant whether the order of element values have to match the one's found-
     * @return bool for whether all the elements of `toFind` was found in `toSearch` according to previous arguments
     */
    template<typename typeVar>
    inline bool find_vector(std::vector<typeVar> toFind, std::vector<typeVar> toSearch, bool orderImportant=false) {
        if(toFind.size() > toSearch.size()) throw std::invalid_argument("the toFind vector is bigger than toSearch.");


        if(orderImportant) {
            int foundIdx = -1;
            for(size_t i=0; i<=toSearch.size()-toFind.size(); i++) {
                std::vector<typeVar> subVec;
                for(size_t ii=i; ii<toFind.size()+i; ii++) subVec.push_back(toSearch[ii]);

                if(match_vectors<typeVar>(toFind, subVec, orderImportant)) {
                    foundIdx = static_cast<int>(i);
                    break;
                }
            }
            return (foundIdx!=-1? true : false);
        }
        else {
            size_t ignBits = 0; // index to toFind elements to ignore cause they've been found.

            for(size_t i=0; i<toSearch.size(); i++) {
                
                for(size_t ii=0; ii<toFind.size(); ii++) {
                    if(!((ignBits & (1<<ii))>0) && toSearch[i]==toFind[ii]) {
                        /// ii not found in ignBits byte string and it's a found element from toFind
                        
                        ignBits += 1<<ii;
                        break;
                    }
                }
            }
            // std::cout << std::bitset<5>(ignBits) << " ";
            return (ignBits==pow(2, toFind.size())-1? true : false);
        }
    }

}


#endif