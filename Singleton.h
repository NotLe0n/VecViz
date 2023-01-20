#ifndef VECVIZ_SINGLETON_H
#define VECVIZ_SINGLETON_H

template<typename T>
class Singleton {
protected:
    Singleton() = default;
    virtual ~Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& GetInstance() {
        static T inst;
        return inst;
    }
};

#endif //VECVIZ_SINGLETON_H
