#pragma once
#include <jni.h>
#include "../javahooks/javahooks.h"

class Coconut {
public:
    static Coconut &instance();

    void init(jvmtiEnv *jvmti, JNIEnv *env);

    void hook(JNIEnv *env);

    jclass getCoconutClass() {
        return this->coconutClass;
    }

private:
    Coconut();

    ~Coconut();

    jclass coconutClass;
    JavaHooks *hooks;

    Coconut(const Coconut &) = delete;

    Coconut &operator=(const Coconut &) = delete;
};
