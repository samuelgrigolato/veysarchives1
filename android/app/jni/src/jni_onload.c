#include <jni.h>


JavaVM* gJavaVM;

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  gJavaVM = vm;
  return JNI_VERSION_1_6;
}
