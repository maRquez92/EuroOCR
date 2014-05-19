#include <string.h>
#include <jni.h>

#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

jstring Java_com_example_ndktest_MainActivity_invokeNativeFunction(JNIEnv* env, jobject javaThis) {
	 // return (*env)->NewStringUTF(env, "Hello from native code!");
	  return env->NewStringUTF("Hello from native code!");
}

#ifdef __cplusplus
}
#endif
