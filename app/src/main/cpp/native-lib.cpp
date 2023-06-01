#include <jni.h>
#include <string>
#include <android/log.h>
#define TAG "lee"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);

extern "C" JNIEXPORT jstring JNICALL
Java_com_sawyer_studyjniexception_MainActivity_stringFromJNI(JNIEnv* env,jobject ) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

int compare(const jint * num1, const jint * num2){
    return *num1 - *num2;
}

// todo 数组排序
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_MainActivity_sort(JNIEnv *env, jobject thiz, jintArray arr) {
    int * array = env->GetIntArrayElements(arr, nullptr);
    int length = env->GetArrayLength(arr);

    /**
     * NDK工具集自带的排序函数
     * void qsort(
     *      void* __base,       //数组首地址
     *      size_t __nmemb,     //数组长度
     *      size_t __size,      //数组元素数据类型大小
     *      int (*__comparator)(const void* __lhs, const void* __rhs)); //数组比较方法指针
     */
    qsort(array, length, sizeof(int),reinterpret_cast<int (*)(const void *, const void *)>(compare));

    env->ReleaseIntArrayElements(arr, array, JNI_OK);

}

// todo 普通局部缓存,小弊端：多次被调用，由于没有缓存，造成内存的浪费
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_MainActivity_localCache(JNIEnv *env, jclass clazz, jstring name) {
    static jfieldID f_id = env->GetStaticFieldID(clazz, "name1", "Ljava/lang/String;");
    env->SetStaticObjectField(clazz, f_id, name);
}

//todo ======================全局静态缓存=============================

// 全局静态缓存(一般在构造函数中初始化，即只调用一次，初始化一次)
static jfieldID f_name1_id = nullptr;
static jfieldID f_name2_id = nullptr;
static jfieldID f_name3_id = nullptr;

//初始化静态缓存
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_MainActivity_initStaticCache(JNIEnv *env, jclass clazz) {
    f_name1_id = env->GetStaticFieldID(clazz, "name1", "Ljava/lang/String;");
    f_name2_id = env->GetStaticFieldID(clazz, "name2", "Ljava/lang/String;");
    f_name3_id = env->GetStaticFieldID(clazz, "name3", "Ljava/lang/String;");
}

//使用静态缓存
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_MainActivity_staticCache(JNIEnv *env, jclass clazz,
                                                           jstring name) {
    // 如果这个方法被调用多次，也不会反复的去获取jfieldID。因为是先初始化静态缓存，然后再执行此函数的
    env->SetStaticObjectField(clazz, f_name1_id, name);
    env->SetStaticObjectField(clazz, f_name2_id, name);
    env->SetStaticObjectField(clazz, f_name3_id, name);
}

//释放静态缓存
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_MainActivity_clearStaticCache(JNIEnv *env, jclass clazz) {
    f_name1_id = nullptr;
    f_name2_id = nullptr;
    f_name3_id = nullptr;
    LOGD("静态缓存清除完毕！");
}

//todo ======================C++层的异常情况============================

//处理异常，方式一：C++层代码编写有错误，自己处理
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_SecondActivity_exception1(JNIEnv *env, jclass clazz) {
    //SecondActivity中只有name111这个静态成员，没有name666,故代码运行到此处会崩溃
    jfieldID f_id = env->GetStaticFieldID(clazz,"name666", "Ljava/lang/String;");

    //todo =====防止程序崩溃，处理异常方式一：

    //检测函数执行过程中有没有异常出现
    jthrowable throwable = env->ExceptionOccurred();
    if (throwable){ //即出现异常
        LOGD("==============Native层检测到异常=============")
        //1.先清除异常
        env->ExceptionClear();
        //2.修改出现异常的代码
        f_id = env->GetStaticFieldID(clazz,"name111", "Ljava/lang/String;");
    }
}

//处理异常，方式二：C++层检测到异常，将异常抛给Java层
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_SecondActivity_exception2(JNIEnv *env, jclass clazz) {
    //SecondActivity中只有name111这个静态成员，没有name666,故代码运行到此处会崩溃
    jfieldID f_id = env->GetStaticFieldID(clazz,"name666", "Ljava/lang/String;");

    //todo =====防止程序崩溃，处理异常方式二：

    //检测函数执行过程中有没有异常出现
    jthrowable throwable = env->ExceptionOccurred();
    if (throwable){ //即出现异常
        LOGD("==============Native层检测到异常=============")
        //1.先清除异常
        env->ExceptionClear();
        //2.将异常抛给Java层
        jclass no_such_field_clazz = env->FindClass("java/lang/NoSuchFieldException");
        env->ThrowNew(no_such_field_clazz,"NoSuchFieldException: 实在是找不到name666，没办法，奔溃了!");
    }
}

//处理异常，方式三：Java层抛出了异常，然后C++层去清除
//注意：Java层的异常，Native层无法捕获
extern "C"
JNIEXPORT void JNICALL
Java_com_sawyer_studyjniexception_SecondActivity_exception3(JNIEnv *env, jclass clazz) {
    jmethodID showMid = env->GetStaticMethodID(clazz, "show", "()V");
    env->CallStaticVoidMethod(clazz, showMid);

    // 按道理来说，上面的这行代码：env->CallStaticVoidMethod(clazz, showMid)。就已经奔溃了，但是事实是否如此呢？
    LOGI("C++层：>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>1")
    LOGI("C++层：>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>2")
    LOGI("C++层：>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>3")
    LOGI("C++层：>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>4")
    LOGI("C++层：>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>5")

    /**
     *Java层出错了，C++层可以监测到，并且可以清除Java的异常
     *上边的日志在即使有错误的情况下，也打印了出来，说明：程序不会马上崩溃，而是预留了时间，给我们做处理
     *注意：处理异常的代码要紧跟着发生异常的代码。如果有多次的异常代码，那么就需要写多个处理异常的代码
     */
    if (env->ExceptionCheck()){
        env->ExceptionDescribe(); //输出异常描述
        env->ExceptionClear(); //清理异常
    }
}