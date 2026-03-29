#pragma once
#include <jni.h>
#include <jvmti.h>
#include <vector>
#include <string>
#include <algorithm>

class JavaHooks {

public:
    bool init(JNIEnv* env, jvmtiEnv* jvmti);
    bool retransform(const char* name);
    void register_hook_callback(const char* clazz, const char* method, const char* sig, void* callback);

private:

    static void JNICALL ClassFileLoadHook(
       jvmtiEnv* jvmti,
       JNIEnv* env,
       jclass class_being_redefined,
       jobject loader,
       const char* name,
       jobject protection_domain,
       jint class_data_len,
       const unsigned char* class_data,
       jint* new_class_data_len,
       unsigned char** new_class_data
   );
    JNIEnv* env;
    jvmtiEnv* jvmti;

    static jclass javaHook_class;
    static std::vector<std::string> classes;

};
