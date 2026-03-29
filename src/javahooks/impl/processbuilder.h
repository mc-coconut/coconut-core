#pragma once
#include <cstdio>
#ifndef PROCESS_BUILDER_HOOK
#define PROCESS_BUILDER_HOOK
#include "windows.h"

namespace Hooks {
    jobject JNICALL processBuilderHook(JNIEnv *env, jclass clazz, jobject builder) {
        std::vector<std::string> command;

        /* Fetch the command list from the processbuilder object */

        jclass pbClass = env->GetObjectClass(builder);
        jfieldID commandField = env->GetFieldID(pbClass, "command", "Ljava/util/List;");
        if (commandField == nullptr) {
            env->ExceptionClear();
            printf("[hook] Failed to get command field\n");
            return nullptr;
        }

        jobject commandList = env->GetObjectField(builder, commandField);
        if (commandList == nullptr) {
            printf("[hook] command list is null\n");
            return nullptr;
        }

        jclass listClass = env->FindClass("java/util/List");
        jmethodID sizeMethod = env->GetMethodID(listClass, "size", "()I");
        jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

        jint size = env->CallIntMethod(commandList, sizeMethod);

        for (jint i = 0; i < size; i++) {
            jstring arg = (jstring) env->CallObjectMethod(commandList, getMethod, i);
            if (arg == nullptr) continue;

            const char *argStr = env->GetStringUTFChars(arg, nullptr);
            command.push_back(argStr);
            env->ReleaseStringUTFChars(arg, argStr);
            env->DeleteLocalRef(arg);
        }

        env->DeleteLocalRef(commandList);
        env->DeleteLocalRef(listClass);
        env->DeleteLocalRef(pbClass);

        printf("[java/lang/ProcessBuilder] start() called\n");
        printf("arguments:\n");
        std::string cmdline = "";
        for (auto c: command) {
            printf(" - %s\n", c.c_str());
            cmdline += c + " ";
        }

        MessageBoxA(NULL, cmdline.c_str(), "ProcessBuilder.start()", MB_OK);

        jclass hookStatus = env->FindClass("dev/coconut/javahooks/HookStatus");
        jfieldID cancelField = env->GetStaticFieldID(
            hookStatus,
            "CANCEL",
            "Ldev/coconut/javahooks/HookStatus;" // descriptor for the enum type itself
        );

        jobject cancelValue = env->GetStaticObjectField(hookStatus, cancelField);
        return cancelValue;
    }
}

#endif
