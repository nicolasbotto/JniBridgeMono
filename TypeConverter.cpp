#include "TypeConverter.h"

void TypeConverter::init(JNIEnv* env)
{
    assert(env);

    jclass intClazzTemp = env->FindClass("java/lang/Integer");
    intValue = env->GetMethodID(intClazzTemp, "intValue", "()I");
    intCtor = env->GetMethodID(intClazzTemp, "<init>", "(I)V");

    intClazz = (jclass)env->NewGlobalRef(intClazzTemp);
    env->DeleteLocalRef(intClazzTemp);

    jclass doubleClazzTemp = env->FindClass("java/lang/Double");
    doubleValue = env->GetMethodID(doubleClazzTemp, "doubleValue", "()D");
    doubleCtor = env->GetMethodID(doubleClazzTemp, "<init>", "(D)V");

    doubleClazz = (jclass)env->NewGlobalRef(doubleClazzTemp);
    env->DeleteLocalRef(doubleClazzTemp);

    jclass charClazzTemp = env->FindClass("java/lang/Character");
    charValue = env->GetMethodID(charClazzTemp, "charValue", "()C");
    charCtor = env->GetMethodID(charClazzTemp, "<init>", "(C)V");

    characterClazz = (jclass)env->NewGlobalRef(charClazzTemp);
    env->DeleteLocalRef(charClazzTemp);

    jclass longClazzTemp = env->FindClass("java/lang/Long");
    longValue = env->GetMethodID(longClazzTemp, "longValue", "()J");
    longCtor = env->GetMethodID(longClazzTemp, "<init>", "(J)V");

    longClazz = (jclass)env->NewGlobalRef(longClazzTemp);
    env->DeleteLocalRef(longClazzTemp);

    jclass booleanClazzTemp = env->FindClass("java/lang/Boolean");
    booleanValue = env->GetMethodID(booleanClazzTemp, "booleanValue", "()Z");
    booleanCtor = env->GetMethodID(booleanClazzTemp, "<init>", "(Z)V");

    booleanClazz = (jclass)env->NewGlobalRef(booleanClazzTemp);
    env->DeleteLocalRef(booleanClazzTemp);

    jclass byteClazzTemp = env->FindClass("java/lang/Byte");
    byteValue = env->GetMethodID(byteClazzTemp, "byteValue", "()B");
    byteCtor = env->GetMethodID(byteClazzTemp, "<init>", "(B)V");

    byteClazz = (jclass)env->NewGlobalRef(byteClazzTemp);
    env->DeleteLocalRef(byteClazzTemp);

    jclass shortClazzTemp = env->FindClass("java/lang/Short");
    shortValue = env->GetMethodID(shortClazzTemp, "shortValue", "()S");
    shortCtor = env->GetMethodID(shortClazzTemp, "<init>", "(S)V");

    shortClazz = (jclass)env->NewGlobalRef(shortClazzTemp);
    env->DeleteLocalRef(shortClazzTemp);

    jclass floatClazzTemp = env->FindClass("java/lang/Float");
    floatValue = env->GetMethodID(floatClazzTemp, "floatValue", "()F");
    floatCtor = env->GetMethodID(floatClazzTemp, "<init>", "(F)V");

    floatClazz = (jclass)env->NewGlobalRef(floatClazzTemp);
    env->DeleteLocalRef(floatClazzTemp);

    jclass clazz = env->FindClass("java/lang/Class");
    getClassName = env->GetMethodID(clazz, "getName", "()Ljava/lang/String;");
    env->DeleteLocalRef(clazz);

    // java/util/Map
    jclass mapClazzTmp = env->FindClass("java/util/Map");

    if (mapClazzTmp == NULL)
    {
        printf("Cannot find class java/util/Map");
        return;
    }

    mapClazz = (jclass)env->NewGlobalRef(mapClazzTmp);
    env->DeleteLocalRef(mapClazzTmp);

    size = env->GetMethodID(mapClazz, "size", "()I");

    if (size == NULL)
    {
        printf("Cannot find class java/util/Map size method");
        return;
    }

    getMapValue = env->GetMethodID(mapClazz, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");

    if (getMapValue == NULL)
    {
        printf("Cannot find class java/util/Map get method");
    }

    keySet = env->GetMethodID(mapClazz, "keySet", "()Ljava/util/Set;");

    if (keySet == NULL)
    {
        printf("Cannot find class java/util/Map keySet method");
    }

    // java/util/LinkedHashMap
    jclass setLinkedHashMapTmp = env->FindClass("java/util/LinkedHashMap");

    if (setLinkedHashMapTmp == NULL)
    {
        printf("Cannot find class java/util/LinkedHashMap");
    }

    hashMapClazz = (jclass)env->NewGlobalRef(setLinkedHashMapTmp);
    env->DeleteLocalRef(setLinkedHashMapTmp);

    mapCtor = env->GetMethodID(hashMapClazz, "<init>", "()V");

    if (mapCtor == NULL)
    {
        printf("Cannot find class java/util/LinkedHashMap ctor");
    }

    putMapValue = env->GetMethodID(hashMapClazz, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    if (putMapValue == NULL)
    {
        printf("Cannot find class java/util/LinkedHashMap put method");
    }

    // java/util/Set
    jclass setClazzTmp = env->FindClass("java/util/Set");

    if (setClazzTmp == NULL)
    {
        printf("Cannot find class java/util/Set");
    }

    setClazz = (jclass)env->NewGlobalRef(setClazzTmp);
    env->DeleteLocalRef(setClazzTmp);

    toArray = env->GetMethodID(setClazz, "toArray", "()[Ljava/lang/Object;");
}

template<>
string TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jstring input = (jstring)obj;
    if (input == NULL)
    {
        return NULL;
    }

    const char * tmp = env->GetStringUTFChars(input, JNI_FALSE);
    //copy the string
    string result(tmp);
    env->ReleaseStringUTFChars(input, tmp);

    return result;
}

template<>
const char* TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jstring input = (jstring)obj;
    if (input == NULL)
    {
        return NULL;
    }

    const char * tmp = env->GetStringUTFChars(input, JNI_FALSE);
    //copy the string
    string result(tmp);
    env->ReleaseStringUTFChars(input, tmp);

    return result.c_str();
}

template<>
int TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallIntMethod(obj, intValue);
}

template<>
double TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallDoubleMethod(obj, doubleValue);
}

template<>
char TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallCharMethod(obj, charValue);
}

template<>
byte TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallByteMethod(obj, byteValue);
}

template<>
short TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallShortMethod(obj, shortValue);
}

template<>
long TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallLongMethod(obj, longValue);
}

template<>
float TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallFloatMethod(obj, floatValue);
}

template<>
bool TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    return env->CallBooleanMethod(obj, booleanValue);
}

template<>
vector<int> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jintArray input = (jintArray)obj;

    const int intsSize = env->GetArrayLength(input);
    vector<int> intArray(intsSize);

    jint *body = env->GetIntArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        intArray[i] = body[i];
    }

    env->ReleaseIntArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return intArray;
}

template<>
vector<double> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jdoubleArray input = (jdoubleArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<double> doubleArray(intsSize);

    jdouble *body = env->GetDoubleArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        doubleArray[i] = body[i];
    }

    env->ReleaseDoubleArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return doubleArray;
}

template<>
vector<char> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jcharArray input = (jcharArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<char> charArray(intsSize);

    jchar *body = env->GetCharArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        charArray[i] = body[i];
    }

    env->ReleaseCharArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return charArray;
}

template<>
vector<long> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jlongArray input = (jlongArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<long> longArray(intsSize);

    jlong *body = env->GetLongArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        longArray[i] = body[i];
    }

    env->ReleaseLongArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return longArray;
}

template<>
vector<bool> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jbooleanArray input = (jbooleanArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<bool> booleanArray(intsSize);

    jboolean *body = env->GetBooleanArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        booleanArray[i] = body[i];
    }

    env->ReleaseBooleanArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return booleanArray;
}

template<>
vector<float> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jfloatArray input = (jfloatArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<float> floatArray(intsSize);

    jfloat *body = env->GetFloatArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        floatArray[i] = body[i];
    }

    env->ReleaseFloatArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return floatArray;
}

template<>
vector<short> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jshortArray input = (jshortArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<short> shortArray(intsSize);

    jshort *body = env->GetShortArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        shortArray[i] = body[i];
    }

    env->ReleaseShortArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return shortArray;
}

template<>
vector<byte> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jbyteArray input = (jbyteArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<byte> byteArray(intsSize);

    jbyte *body = env->GetByteArrayElements(input, JNI_FALSE);

    for (int i = 0; i < intsSize; i++)
    {
        byteArray[i] = body[i];
    }

    env->ReleaseByteArrayElements(input, body, 0);
    env->DeleteLocalRef(input);

    return byteArray;
}

template<>
vector<string> TypeConverter::convertToC(JNIEnv* env, jobject obj)
{
    assert(env);

    jobjectArray input = (jobjectArray)obj;

    const int intsSize = env->GetArrayLength(input);

    vector<string> stringArray(intsSize);

    for (int i = 0; i < intsSize; i++)
    {
        jobject item = env->GetObjectArrayElement(input, i);

        stringArray[i] = convertToC<string>(env, item);

        env->DeleteLocalRef(item);
    }

    env->DeleteLocalRef(input);

    return stringArray;
}

jobject TypeConverter::convertToJavaMap(JNIEnv* env, const GenericValue<UTF8<> >& jsonValue)
{
    jobject map = env->NewObject(hashMapClazz, mapCtor);

    if(jsonValue.HasMember("Java_Type"))
    {
        if (strcmp(jsonValue["Java_Type"].GetString(), "Map") == 0)
        {
            if(jsonValue.HasMember("Java_Value"))
            {
                for (Value::ConstMemberIterator itr = jsonValue["Java_Value"].MemberBegin(); itr != jsonValue["Java_Value"].MemberEnd(); ++itr)
                {
                    string keyName = string(itr->name.GetString());
                    const GenericValue<UTF8<> >& mapValue = itr->value;

                    jobject key = env->NewStringUTF(keyName.c_str());
                    jobject value = toJavaObject(env, mapValue);

                    env->CallObjectMethod(map, putMapValue, key, value);

                    env->DeleteLocalRef(key);
                    env->DeleteLocalRef(value);
                }
            }
        }
    }
    
    return map;
}

jobject TypeConverter::toJavaObject(JNIEnv* env, const GenericValue<UTF8<> >& jsonValue)
{
    string className = jsonValue["Java_Type"].GetString();
    
    if (className == "NULL")
    {
        return NULL;
    }
    
    if (className == "java.lang.Integer")
    {
        int value = jsonValue["Java_Value"].GetInt();
        return env->NewObject(intClazz, intCtor, value);
    }

    if (className == "java.lang.String")
    {
        string value = jsonValue["Java_Value"].GetString();
        return env->NewStringUTF(value.c_str());
    }

    if (className == "java.lang.Boolean")
    {
        bool value = jsonValue["Java_Value"].GetBool();
        return env->NewObject(booleanClazz, booleanCtor, value);
    }

    if (className == "java.lang.Character")
    {
        char value = (char)jsonValue["Java_Value"].GetInt();
        return env->NewObject(characterClazz, charCtor, value);
    }

    if (className == "java.lang.Long")
    {
        long value = jsonValue["Java_Value"].GetInt64();
        return env->NewObject(longClazz, longCtor, value);
    }

    if (className == "java.lang.Short")
    {
        short value = (short)jsonValue["Java_Value"].GetInt();
        return env->NewObject(shortClazz, shortCtor, value);
    }

    if (className =="java.lang.Byte")
    {
        byte value = (byte)jsonValue["Java_Value"].GetUint();
        return env->NewObject(byteClazz, byteCtor, value);
    }

    if (className == "java.lang.Double")
    {
        double value = jsonValue["Java_Value"].GetDouble();
        return env->NewObject(doubleClazz, doubleCtor, value);
    }

    if (className == "java.lang.Float")
    {
        float value = (float)jsonValue["Java_Value"].GetDouble();
        return env->NewObject(floatClazz, floatCtor, value);
    }
    
    if (className == "[B")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jbyteArray byteArray = env->NewByteArray(arr.Size());

            jbyte* buffer = env->GetByteArrayElements(byteArray, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = (byte)arr[i].GetUint();
            }

            env->ReleaseByteArrayElements(byteArray, buffer, 0);

            return byteArray;
        }
    }
    
    if (className == "[I")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jintArray intArray = env->NewIntArray(arr.Size());

            jint* buffer = env->GetIntArrayElements(intArray, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = arr[i].GetInt();
            }

            env->ReleaseIntArrayElements(intArray, buffer, 0);

            return intArray;
        }
    }
    
    if (className == "[C")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jcharArray array = env->NewCharArray(arr.Size());

            jchar* buffer = env->GetCharArrayElements(array, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = (char)arr[i].GetInt();
            }

            env->ReleaseCharArrayElements(array, buffer, 0);

            return array;
        }
    }
    
    if (className == "[D")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jdoubleArray array = env->NewDoubleArray(arr.Size());

            jdouble* buffer = env->GetDoubleArrayElements(array, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = (double)arr[i].GetDouble();
            }

            env->ReleaseDoubleArrayElements(array, buffer, 0);

            return array;
        }
    }

    if (className == "[F")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jfloatArray array = env->NewFloatArray(arr.Size());

            jfloat* buffer = env->GetFloatArrayElements(array, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = (float)arr[i].GetDouble();
            }

            env->ReleaseFloatArrayElements(array, buffer, 0);

            return array;
        }
    }
    
    if (className == "[Z")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jbooleanArray array = env->NewBooleanArray(arr.Size());

            jboolean* buffer = env->GetBooleanArrayElements(array, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = arr[i].GetBool();
            }

            env->ReleaseBooleanArrayElements(array, buffer, 0);

            return array;
        }
    }
    
    if (className == "[J")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jlongArray array = env->NewLongArray(arr.Size());

            jlong* buffer = env->GetLongArrayElements(array, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = arr[i].GetInt64();
            }

            env->ReleaseLongArrayElements(array, buffer, 0);

            return array;
        }
    }
    
    if (className == "[S")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jshortArray array = env->NewShortArray(arr.Size());

            jshort* buffer = env->GetShortArrayElements(array, JNI_FALSE);

            for (SizeType i = 0; i < arr.Size(); i++)
            {
                buffer[i] = (short)arr[i].GetInt();
            }

            env->ReleaseShortArrayElements(array, buffer, 0);

            return array;
        }
    }
    
    if (className == "[Ljava.lang.String;")
    {
        const Value& arr = jsonValue["Java_Value"];

        if(arr.IsArray())
        {
            jobjectArray array = env->NewObjectArray(arr.Size(), env->FindClass("java/lang/String"), 0);
    
            for (SizeType i = 0; i < arr.Size(); i++)
            {
                jobject javaString = env->NewStringUTF(arr[i].GetString());
                env->SetObjectArrayElement(array, i, javaString);
            }

            return array;
        }
    }
}