#include "coconut.h"

#include "../javahooks/impl/processbuilder.h"

Coconut &Coconut::instance() {
    static Coconut coconut;
    return coconut;
}

Coconut::Coconut()
{
}

Coconut::~Coconut() {

}


void Coconut::hook(JNIEnv* env) {

    hooks->register_hook_callback(
        "dev/coconut/javahooks/impl/java/lang/ProcessBuilderTransformer",
        "nativeHook",
        "(Ljava/lang/ProcessBuilder;)Ldev/coconut/javahooks/HookStatus;",
        (void*)Hooks::processBuilderHook
        );
    hooks->retransform("java/lang/ProcessBuilder");
}

void Coconut::init(jvmtiEnv* jvmti, JNIEnv* env) {
    hooks = new JavaHooks();
    if (!hooks->init(env, jvmti)) {
        printf("[severe] could no initialize java hooks\n");
        return;
    }

    printf("[coconut] initialized java hooks\n");

    this->hook(env);

}