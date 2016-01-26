#define LOG_TAG "dynamicdex"
#include "com_jltxgcy_dynamicdex_DexLoader.h"
#include <android/log.h>
#include <cstddef>


#define ALOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define ALOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

static void loadApk(JNIEnv * env, jclass clazz, jstring package) {
	jclass activityThreadClazz;
	jmethodID currentActivityThreadMethodID;
	jobject activityThreadObject;
	const char *packageName;
	const char *className;
	const char *methodName;
	int codeoff;

	jfieldID mPackagesFieldID;
	jobject mPackagesJObject;
	jclass mPackagesClazz;
	jmethodID getMethodID;

	jobject weakReferenceJObject;
	jclass weakReferenceJClazz;
	jmethodID getweakMethodID;

	jobject loadedApkJObject;
	jclass loadedApkJClazz;
	jfieldID mClassLoaderFieldID;
	jobject mClassLoaderJObject;
	jstring dexPath;
	jstring dexOptPath;

	jclass dexClassLoaderClazz;
	jmethodID initDexLoaderMethod;
	jobject dexClassLoaderJObject;

	activityThreadClazz = env->FindClass("android/app/ActivityThread");
	currentActivityThreadMethodID = env->GetStaticMethodID(activityThreadClazz, "currentActivityThread",
            "()Landroid/app/ActivityThread;");
	activityThreadObject = env->CallStaticObjectMethod(activityThreadClazz, currentActivityThreadMethodID);
	packageName = env->GetStringUTFChars(package, JNI_FALSE);
	mPackagesFieldID = env->GetFieldID(activityThreadClazz, "mPackages", "Ljava/util/HashMap;");
	mPackagesJObject = env->GetObjectField(activityThreadObject, mPackagesFieldID);
	mPackagesClazz = env->GetObjectClass(mPackagesJObject);
	getMethodID = env->GetMethodID(mPackagesClazz, "get",
            "(Ljava/lang/Object;)Ljava/lang/Object;");
	weakReferenceJObject = env->CallObjectMethod(mPackagesJObject, getMethodID, package);
	weakReferenceJClazz = env->GetObjectClass(weakReferenceJObject);
	getweakMethodID = env->GetMethodID(weakReferenceJClazz, "get",
            "()Ljava/lang/Object;");
	loadedApkJObject = env->CallObjectMethod(weakReferenceJObject, getweakMethodID);
	loadedApkJClazz = env->GetObjectClass(loadedApkJObject);
	mClassLoaderFieldID = env->GetFieldID(loadedApkJClazz, "mClassLoader", "Ljava/lang/ClassLoader;");
	mClassLoaderJObject = env->GetObjectField(loadedApkJObject, mClassLoaderFieldID);
	dexPath = env->NewStringUTF("/sdcard/payload_odex/ForceApkObj.apk");
	dexOptPath = env->NewStringUTF("/sdcard/payload_odex/");
	dexClassLoaderClazz = env->FindClass("dalvik/system/DexClassLoader");
	initDexLoaderMethod = env->GetMethodID(dexClassLoaderClazz, "<init>","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");
	dexClassLoaderJObject = env->NewObject(dexClassLoaderClazz,initDexLoaderMethod, dexPath, dexOptPath, NULL, mClassLoaderJObject);
	env->SetObjectField(loadedApkJObject, mClassLoaderFieldID, dexClassLoaderJObject);
	ALOGD("packageName:%s", packageName);
}

static void run(JNIEnv * env, jclass clazz) {
	jclass activityThreadClazz;
	jmethodID currentActivityThreadMethodID;
	jobject activityThreadObject;

	jfieldID mBoundApplicationFieldID;
	jobject mBoundApplicationJObject;
	jclass mBoundApplicationClazz;
	jfieldID mInfoFieldID;
	jobject mInfoJObject;
	jclass mInfoClazz;

	jfieldID mApplicationFieldID;
	jobject mApplicationJObject;

	jfieldID mInitialApplicationFieldID;
	jobject mInitialApplicationJObject;

	jfieldID mAllApplicationsFieldID;
	jobject mAllApplicationsJObject;
	jclass mAllApplicationsClazz;
	jmethodID removeMethodID;

	jfieldID mApplicationInfoFieldID;
	jobject mApplicationInfoJObject;
	jclass mApplicationInfoClazz;

	jfieldID mBindApplicationInfoFieldID;
	jobject mBindApplicationInfoJObject;
	jclass mBindApplicationInfoClazz;

	jfieldID classNameFieldID;
	jfieldID mBindClassNameFieldID;
	jstring applicationName;

	jmethodID makeApplicationMethodID;
	jobject ApplicationJObject;
	jclass ApplicationClazz;
	jmethodID onCreateMethodID;

	activityThreadClazz = env->FindClass("android/app/ActivityThread");
	currentActivityThreadMethodID = env->GetStaticMethodID(activityThreadClazz, "currentActivityThread",
	            "()Landroid/app/ActivityThread;");
	activityThreadObject = env->CallStaticObjectMethod(activityThreadClazz, currentActivityThreadMethodID);
	mBoundApplicationFieldID = env->GetFieldID(activityThreadClazz, "mBoundApplication", "Landroid/app/ActivityThread$AppBindData;");
	mBoundApplicationJObject = env->GetObjectField(activityThreadObject, mBoundApplicationFieldID);
	mBoundApplicationClazz = env->GetObjectClass(mBoundApplicationJObject);
	mInfoFieldID = env->GetFieldID(mBoundApplicationClazz, "info", "Landroid/app/LoadedApk;");
	mInfoJObject = env->GetObjectField(mBoundApplicationJObject, mInfoFieldID);
	mInfoClazz = env->GetObjectClass(mInfoJObject);
	mApplicationFieldID = env->GetFieldID(mInfoClazz, "mApplication", "Landroid/app/Application;");
	mApplicationJObject = env->GetObjectField(mInfoJObject, mApplicationFieldID);
	env->SetObjectField(mInfoJObject, mApplicationFieldID, NULL);
	mInitialApplicationFieldID = env->GetFieldID(activityThreadClazz, "mInitialApplication", "Landroid/app/Application;");
	mInitialApplicationJObject = env->GetObjectField(activityThreadObject, mInitialApplicationFieldID);
	mAllApplicationsFieldID = env->GetFieldID(activityThreadClazz, "mAllApplications", "Ljava/util/ArrayList;");
	mAllApplicationsJObject = env->GetObjectField(activityThreadObject, mAllApplicationsFieldID);
	mAllApplicationsClazz = env->GetObjectClass(mAllApplicationsJObject);
	removeMethodID = env->GetMethodID(mAllApplicationsClazz, "remove",
            "(Ljava/lang/Object;)Z");
	jboolean isTrue = env->CallBooleanMethod(mAllApplicationsJObject, removeMethodID, mInitialApplicationJObject);
	mApplicationInfoFieldID = env->GetFieldID(mInfoClazz, "mApplicationInfo", "Landroid/content/pm/ApplicationInfo;");
	mApplicationInfoJObject = env->GetObjectField(mInfoJObject, mApplicationInfoFieldID);
	mApplicationInfoClazz = env->GetObjectClass(mApplicationInfoJObject);
	mBindApplicationInfoFieldID = env->GetFieldID(mBoundApplicationClazz, "appInfo", "Landroid/content/pm/ApplicationInfo;");
	mBindApplicationInfoJObject = env->GetObjectField(mBoundApplicationJObject, mBindApplicationInfoFieldID);
	mBindApplicationInfoClazz = env->GetObjectClass(mBindApplicationInfoJObject);
	classNameFieldID = env->GetFieldID(mApplicationInfoClazz, "className", "Ljava/lang/String;");
	mBindClassNameFieldID = env->GetFieldID(mBindApplicationInfoClazz, "className", "Ljava/lang/String;");
	applicationName = env->NewStringUTF("com.example.forceapkobj.MyApplication");
	env->SetObjectField(mApplicationInfoJObject, classNameFieldID, applicationName);
	env->SetObjectField(mBindApplicationInfoJObject, mBindClassNameFieldID, applicationName);
	makeApplicationMethodID = env->GetMethodID(mInfoClazz, "makeApplication","(ZLandroid/app/Instrumentation;)Landroid/app/Application;");
	ApplicationJObject = env->CallObjectMethod(mInfoJObject, makeApplicationMethodID, JNI_FALSE, NULL);
	env->SetObjectField(activityThreadObject, mInitialApplicationFieldID, ApplicationJObject);
	ApplicationClazz = env->GetObjectClass(ApplicationJObject);
	onCreateMethodID = env->GetMethodID(ApplicationClazz, "onCreate","()V");
	env->CallVoidMethod(ApplicationJObject, onCreateMethodID);
}

JNIEXPORT void JNICALL Java_com_jltxgcy_dynamicdex_DexLoader_load
  (JNIEnv * env, jclass clazz, jstring packageName) {
	loadApk(env, clazz, packageName);
	ALOGD("Java_com_jltxgcy_dynamicdex_DexLoader_load");
}

JNIEXPORT void JNICALL Java_com_jltxgcy_dynamicdex_DexLoader_run
  (JNIEnv * env, jclass clazz) {
	run(env, clazz);
	ALOGD("Java_com_jltxgcy_dynamicdex_DexLoader_run");
}
