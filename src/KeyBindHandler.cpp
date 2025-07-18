
#include "KeyBindHandler.hpp"
#include <iostream>

// #include <useful.hpp>

void DIY_KBH::KeyBindHandler::update(std::vector<int> _pressed_keys) {
    static auto _prevCalled = std::chrono::steady_clock::now();
    auto timeNow = std::chrono::steady_clock::now();
    if(timeNow == _prevCalled) return;

    for(size_t i=0; i<_keyBinds.size(); i++) {
        if(_keyBinds[i]._keys[0] == -69) continue;

        bool _match = false;
        bool _call  = false;
        int newDecayVal = _callDecay[i];
        
        if(
            ( // valid instances
                !_keyBinds[i]._isolImportant ||
                (_keyBinds[i]._isolImportant && _keyBinds[i]._keys.size()==_pressed_keys.size())
            ) && ( // general valid check
                _keyBinds[i]._keys.size()<=_pressed_keys.size()
            )
        ) {
            /// isolImportance and general size checks passed
            try {
                if(find_vector<int>(_keyBinds[i]._keys, _pressed_keys, _keyBinds[i]._orderImportant)) {
                    /// Keybind is pressed
                    _match = true;
                    newDecayVal = _callDecay[i]+1;
                }
                else {
                    newDecayVal = _callDecay[i]-1;
                }
            }
            catch(const std::exception& e) {std::cout << e.what() << std::endl;}
            
        }
        else {
            /// Keybind is not pressed

            newDecayVal = _callDecay[i]-1;
        }

        if(_released[i]) {
            if(newDecayVal==0 && _callDecay[i]>0 && !_keyBinds[i]._callEdge && _released[i]) {
                /// Falling edge signal func call
                _call = true;
            }
            else if(newDecayVal==setting_callDecay && _callDecay[i]<setting_callDecay && _keyBinds[i]._callEdge && _released[i]) {
                // Rising edge signal func call
                _call = true;
            }
        }

        if(_call) {
            if(_keyBinds[i]._callFunc) _keyBinds[i]._callFunc();
            _released[i] = false;
        }
        else {

        }

        _called[i] = _call;
        
        /// Update _released
        if((_keyBinds[i]._callEdge && newDecayVal==0) || !_keyBinds[i]._callEdge && newDecayVal==setting_callDecay) {
            _released[i] = true;
            _just_released[i] = true;
        }
        else {
            _just_released[i] = false;
        }


        if(newDecayVal<0) {newDecayVal = 0;}
        if(newDecayVal>setting_callDecay) {newDecayVal = setting_callDecay;}
        
        _callDecay[i] = newDecayVal;
    }

    _prevCalled = std::chrono::steady_clock::now();
}

DIY_KBH::KeyBindHandler::KeyBindHandler() {}
DIY_KBH::KeyBindHandler::KeyBindHandler(KeyBindHandler_unit _kbh_inst) {
    _keyBinds.push_back(_kbh_inst);
    _callDecay.push_back(0);
    _released.push_back(_kbh_inst._callEdge);
    _called.push_back(false);
    _just_released.push_back(false);
}
DIY_KBH::KeyBindHandler::KeyBindHandler(std::string _label, std::vector<int> _keys, void (*_func)(void), bool _ordImportant, bool _isolImportant, bool _callSignalEdge) {
    _keyBinds.push_back(KeyBindHandler_unit{_label, _keys, _func, _ordImportant, _isolImportant, _callSignalEdge});
    _callDecay.push_back(0);
    _released.push_back(_callSignalEdge);
    _called.push_back(false);
    _just_released.push_back(false);
}
DIY_KBH::KeyBindHandler::KeyBindHandler(std::initializer_list<KeyBindHandler_unit> _kbhi_cont) {
    _keyBinds = std::vector<KeyBindHandler_unit>(_kbhi_cont);
    _callDecay = std::vector<int>(_kbhi_cont.size(), 0);
    for(size_t i=0; i<_keyBinds.size(); i++) {
        _released.push_back(_keyBinds[i]._callEdge);
    }
    _called = std::vector<bool>(_kbhi_cont.size(), false);
    _just_released = std::vector<bool>(_kbhi_cont.size(), false);
}
DIY_KBH::KeyBindHandler::KeyBindHandler(std::vector<KeyBindHandler_unit> _kbhi_cont) {
    _keyBinds = std::vector<KeyBindHandler_unit>(_kbhi_cont);
    _callDecay = std::vector<int>(_kbhi_cont.size(), 0);
    for(size_t i=0; i<_keyBinds.size(); i++) {
        _released.push_back(_keyBinds[i]._callEdge);
    }
    _called = std::vector<bool>(_kbhi_cont.size(), false);
    _just_released = std::vector<bool>(_kbhi_cont.size(), false);
}

DIY_KBH::KeyBindHandler::~KeyBindHandler() {

}

size_t DIY_KBH::KeyBindHandler::size() const {
    return this->_keyBinds.size();
}

// int DIY_KBH::KeyBindHandler::add(KeyBindHandler_unit _newKBH) {
//     return 0;
// }
// int DIY_KBH::KeyBindHandler::add(std::string _label, std::vector<int> _keys, _funcDef _callFunc, bool _ordImportant, bool _isolImportant, bool _callEdge) {
//     return 0;
// }

int DIY_KBH::KeyBindHandler::edit(std::string _label, std::vector<int> _newKeys, bool _call_except) {

    for(size_t i=0; i<this->_keyBinds.size(); i++) {
        if(_keyBinds[i]._label==_label) {
            if(_newKeys.size()==0) _keyBinds[i]._keys = std::vector<int>{-69}; /// keybind disabled
            else _keyBinds[i]._keys = _newKeys;
            this->_callDecay[i] = 0;
            this->_released[i]  = _keyBinds[i]._callEdge;
            return 0;
        }
    }
    if(_call_except) throw std::invalid_argument("edit(std::string, std::vector<int>, bool) the label \""+_label+"\" doesn't exist in storage as a keybind.");

    return 1;
}

DIY_KBH::KeyBindHandler_unit DIY_KBH::KeyBindHandler::get(std::string _label) {
    for(size_t i=0; i<this->_keyBinds.size(); i++) {
        if(_keyBinds[i]._label==_label) {
            return _keyBinds[i];
        }
    }
    throw std::invalid_argument("get(std::string) the label \""+_label+"\" doesn't exist in storage as a keybind.");
}
DIY_KBH::KeyBindHandler_unit DIY_KBH::KeyBindHandler::get(std::string _label) const {
    for(size_t i=0; i<this->_keyBinds.size(); i++) {
        if(_keyBinds[i]._label==_label) {
            return _keyBinds[i];
        }
    }
    throw std::invalid_argument("get(std::string) const   the label \""+_label+"\" doesn't exist in storage as a keybind.");
}

bool DIY_KBH::KeyBindHandler::clicked(std::string _label) {
    for(size_t i=0; i<this->_keyBinds.size(); i++) {
        if(_keyBinds[i]._label==_label) {
            return this->_called[i];
        }
    }
    throw std::invalid_argument("clicked(std::string)   the label \""+_label+"\" doesn't exist in storage as a keybind.");
    return false;
}
bool DIY_KBH::KeyBindHandler::pressing(std::string _label) {
    for(size_t i=0; i<this->_keyBinds.size(); i++) {
        if(_keyBinds[i]._label==_label) {
            return !this->_released[i];
        }
    }
    throw std::invalid_argument("pressing(std::string)   the label \""+_label+"\" doesn't exist in storage as a keybind.");
    return false;
}
bool DIY_KBH::KeyBindHandler::released(std::string _label) {
    for(size_t i=0; i<this->_just_released.size(); i++) {
        if(_keyBinds[i]._label==_label) {
            return this->_just_released[i];
        }
    }
    throw std::invalid_argument("released(std::string)  the label \""+_label+"\" doesn't exist in storage as a keybind.");
    return false;
}