#pragma once
#ifndef URL_HOOK
#define URL_HOOK

#include <cctype>
#include <cstdio>
#include <string>
#include "windows.h"

namespace Hooks {
    jobject JNICALL urlHook(JNIEnv *env, jclass clazz, jobject urlObj) {
        jclass urlClass = env->FindClass("java/net/URL");
        jmethodID getProtocol = env->GetMethodID(urlClass, "getProtocol", "()Ljava/lang/String;");
        jmethodID toExternalForm = env->GetMethodID(urlClass, "toExternalForm", "()Ljava/lang/String;");

        jstring protocolJava = static_cast<jstring>(env->CallObjectMethod(urlObj, getProtocol));
        jstring externalJava = static_cast<jstring>(env->CallObjectMethod(urlObj, toExternalForm));

        const char *protocolChars = env->GetStringUTFChars(protocolJava, nullptr);
        const char *externalChars = env->GetStringUTFChars(externalJava, nullptr);

        std::string protocol = protocolChars;
        std::string external = externalChars;
        for (char &ch: protocol) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        printf("[java/net/URL] open called: %s\n", external.c_str());

        env->ReleaseStringUTFChars(protocolJava, protocolChars);
        env->ReleaseStringUTFChars(externalJava, externalChars);
        env->DeleteLocalRef(protocolJava);
        env->DeleteLocalRef(externalJava);
        env->DeleteLocalRef(urlClass);

        jclass hookStatus = env->FindClass("dev/coconut/javahooks/HookStatus");

        if (protocol == "http" || protocol == "https" || protocol == "ftp" ||
            (protocol == "jar" && (
                 external.rfind("jar:http://", 0) == 0 ||
                 external.rfind("jar:https://", 0) == 0 ||
                 external.rfind("jar:ftp://", 0) == 0
             ))) {
            printf("[hook] blocked URL.openConnection/openStream\n");
            MessageBoxA(NULL, external.c_str(), "URL.openConnection/openStream blocked", MB_OK);
            jfieldID cancelField = env->GetStaticFieldID(
                hookStatus,
                "CANCEL",
                "Ldev/coconut/javahooks/HookStatus;"
            );
            return env->GetStaticObjectField(hookStatus, cancelField);
        }

        jfieldID passField = env->GetStaticFieldID(
            hookStatus,
            "PASS",
            "Ldev/coconut/javahooks/HookStatus;"
        );
        return env->GetStaticObjectField(hookStatus, passField);
    }
}

#endif
