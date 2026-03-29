#include <stdexcept>
#include "core/coconut.h"


std::string getBootstrapJarPath() {
    std::string path;

#if defined(_WIN32) || defined(_WIN64)
    const char* appdata = std::getenv("APPDATA");  // still works cross-platform
    if (!appdata) throw std::runtime_error("APPDATA not found");
    path = std::string(appdata) + "\\.coconut\\bridge.jar";
#else
    const char* home = std::getenv("HOME");
    if (!home) throw std::runtime_error("HOME not found");
    path = std::string(home) + "/.coconut/bridge.jar";
#endif

    return path;
}

void JNICALL onVMInit(jvmtiEnv* jvmti, JNIEnv* env, jthread thread) {
    Coconut::instance().init(jvmti, env);
}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM* vm, char* options, void* reserved) {
    jvmtiEnv* jvmti = nullptr;
    if (vm->GetEnv((void**)&jvmti, JVMTI_VERSION_1_2) != JNI_OK) {
        printf("[agent-load] Failed to get JVMTI environment\n");
        return JNI_ERR;
    }

    if (jvmti->AddToBootstrapClassLoaderSearch(getBootstrapJarPath().c_str()) != JVMTI_ERROR_NONE) {
        printf("[agent-load] Failed to add bootstrap JAR\n");
        return JNI_ERR;
    }


    jvmtiEventCallbacks callbacks = {};
    callbacks.VMInit = &onVMInit;
    jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_INIT, nullptr);
    return JNI_OK;

}
