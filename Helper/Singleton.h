//
// Created by tm on 06/01/16.
//

#ifndef POCOSERVER_SINGELETON_H
#define POCOSERVER_SINGELETON_H
#include <memory>
#include "../Requests/RequestState/State.h"
#include <mutex>

using namespace std;
template <typename T>
class Singleton {

public:
    static T& getInstance();

protected:
    Singleton() { }
    //Derived objects should never be deleted by pointers of this type
    ~Singleton() {cout<<"Singleton Destructor"<<endl; }

private:

    static std::once_flag flag1;
    Singleton(const Singleton & src)= delete;
    Singleton & operator=(const Singleton & lhs)= delete;
    static unique_ptr<T> _instance;
    static void createInstance();

};
template <typename T>
unique_ptr<T> Singleton<T>::_instance;
/*we have to use Singleton<T>:: definition instead of Singleton:: when defining outside of the header
    even for ctor dtor
 */

template <typename T>
std::once_flag Singleton<T>::flag1;

template <typename T>
T& Singleton<T>::getInstance(){
    if(_instance.get()== nullptr){
        //std::lock_guard<std::mutex> lock(g_i_mutex);
        createInstance(); //thread safe call once
    }
    return *_instance;

}
template <typename T>

void Singleton<T>::createInstance() {
    std::call_once(flag1, [](){ _instance.reset(new T()); });
}
#endif //POCOSERVER_SINGELETON_H
