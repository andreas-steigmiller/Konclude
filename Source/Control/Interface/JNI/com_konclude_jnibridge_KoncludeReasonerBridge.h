/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_konclude_jnibridge_KoncludeReasonerBridge */

#ifndef _Included_com_konclude_jnibridge_KoncludeReasonerBridge
#define _Included_com_konclude_jnibridge_KoncludeReasonerBridge
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_konclude_jnibridge_KoncludeReasonerBridge
 * Method:    initKoncludeLibraryInstance
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_initKoncludeLibraryInstance
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_konclude_jnibridge_KoncludeReasonerBridge
 * Method:    closeKoncludeLibraryInstance
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_closeKoncludeLibraryInstance
  (JNIEnv *, jobject);

/*
 * Class:     com_konclude_jnibridge_KoncludeReasonerBridge
 * Method:    initAxiomExpressionVisitingBridge
 * Signature: (Lcom/konclude/jnibridge/AxiomExpressionBuildingBridge;)V
 */
JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_initAxiomExpressionVisitingBridge
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_konclude_jnibridge_KoncludeReasonerBridge
 * Method:    finalizeAxiomExpressionVisitingBridge
 * Signature: (Lcom/konclude/jnibridge/AxiomExpressionBuildingBridge;)V
 */
JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_finalizeAxiomExpressionVisitingBridge
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_konclude_jnibridge_KoncludeReasonerBridge
 * Method:    initQueryingBridge
 * Signature: (Lcom/konclude/jnibridge/QueryingBridge;)V
 */
JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_initQueryingBridge
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_konclude_jnibridge_KoncludeReasonerBridge
 * Method:    finalizeQueryingBridge
 * Signature: (Lcom/konclude/jnibridge/QueryingBridge;)V
 */
JNIEXPORT void JNICALL Java_com_konclude_jnibridge_KoncludeReasonerBridge_finalizeQueryingBridge
  (JNIEnv *, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif