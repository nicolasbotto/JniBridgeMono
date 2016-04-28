#include <jni.h>
#include <cassert>
#include <vector>
#include <string>
#include <exception>
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

typedef unsigned char byte;

using namespace std;
using namespace rapidjson;

class TypeConverter
{
private:
    jmethodID doubleValue;
    jmethodID doubleCtor;
    jmethodID intValue;
    jmethodID intCtor;
    jmethodID charValue;
    jmethodID charCtor;
    jmethodID floatValue;
    jmethodID floatCtor;
    jmethodID byteValue;
    jmethodID byteCtor;
    jmethodID shortValue;
    jmethodID shortCtor;
    jmethodID longValue;
    jmethodID longCtor;
    jmethodID booleanValue;
    jmethodID booleanCtor;
    jmethodID mapCtor;
    jclass hashMapClazz;
    jclass setClazz;
    jclass intClazz;
    jclass doubleClazz;
    jclass booleanClazz;
    jclass shortClazz;
    jclass longClazz;
    jclass byteClazz;
    jclass characterClazz;
    jclass floatClazz;

public:
    jmethodID getClassName;
    jmethodID getMapValue;
    jclass mapClazz;
    jmethodID keySet;
    jmethodID putMapValue;
    jmethodID toArray;

    jmethodID size;
    void init(JNIEnv*);
    // conversion functions
    template<typename TOut>
    TOut convertToC(JNIEnv*, jobject);
    void cleanup(JNIEnv*);

    jobject convertToJavaMap(JNIEnv*, const GenericValue<UTF8<> >&);
    jobject toJavaObject(JNIEnv*, const GenericValue<UTF8<> >&);
    
    template <typename Writer>
    void toObjectJson(JNIEnv* env, jobject obj, Writer& writer) 
    {
        assert(env);

        jclass clazz = env->GetObjectClass(obj);
        jstring clazzName = (jstring)env->CallObjectMethod(clazz, getClassName);

        string className = string(convertToC<string>(env, clazzName));

        writer.StartObject();

        if (className == "java.lang.Integer")
        {
            int value = convertToC<int>(env, obj);

            writer.String("Mono_Type");
            writer.String("Int32");
            writer.String("Mono_Value");
            writer.Int(value);
        }

        if (className == "java.lang.String")
        {
            string value = convertToC<string>(env, obj);

            writer.String("Mono_Type");
            writer.String("String");
            writer.String("Mono_Value");
            writer.String(value.c_str());
        }

        if (className == "java.lang.Boolean")
        {
            bool value = convertToC<bool>(env, obj);

            writer.String("Mono_Type");
            writer.String("Boolean");
            writer.String("Mono_Value");
            writer.Bool(value);
        }

        if (className == "java.lang.Character")
        {
            char value = convertToC<char>(env, obj);
            writer.String("Mono_Type");
            writer.String("Char");
            writer.String("Mono_Value");
            writer.Int(value);
        }

        if (className == "java.lang.Long")
        {
            long value = convertToC<long>(env, obj);
            writer.String("Mono_Type");
            writer.String("Int64");
            writer.String("Mono_Value");
            writer.Int64(value);
        }

        if (className == "java.lang.Short")
        {
            short value = convertToC<short>(env, obj);

            writer.String("Mono_Type");
            writer.String("Int16");
            writer.String("Mono_Value");
            writer.Int(value);
        }

        if (className =="java.lang.Byte")
        {
            byte value = convertToC<byte>(env, obj);

            writer.String("Mono_Type");
            writer.String("Byte");
            writer.String("Mono_Value");
            writer.Uint(value);
        }

        if (className == "java.lang.Double" )
        {
            double value = convertToC<double>(env, obj);
            writer.String("Mono_Type");
            writer.String("Double");
            writer.String("Mono_Value");
            writer.Double(value);
        }

        if (className == "java.lang.Float" )
        {
            float value = convertToC<float>(env, obj);
            writer.String("Mono_Type");
            writer.String("Single");
            writer.String("Mono_Value");
            writer.Double(value);
        }
        
        if (className == "[I" )
        {
            writer.String("Mono_Type");
            writer.String("Int32[]");
            writer.String("Mono_Value");
            vector<int> value = convertToC< vector<int> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Int(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[B" )
        {
            writer.String("Mono_Type");
            writer.String("Byte[]");
            writer.String("Mono_Value");
            vector<byte> value = convertToC< vector<byte> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Uint(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[C" )
        {
            writer.String("Mono_Type");
            writer.String("Char[]");
            writer.String("Mono_Value");
            vector<char> value = convertToC< vector<char> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Int(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[D" )
        {
            writer.String("Mono_Type");
            writer.String("Double[]");
            writer.String("Mono_Value");
            vector<double> value = convertToC< vector<double> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Double(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[Z" )
        {
            writer.String("Mono_Type");
            writer.String("Bool[]");
            writer.String("Mono_Value");
            vector<bool> value = convertToC< vector<bool> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Bool(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[S" )
        {
            writer.String("Mono_Type");
            writer.String("Short[]");
            writer.String("Mono_Value");
            vector<short> value = convertToC< vector<short> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Int(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[J" )
        {
            writer.String("Mono_Type");
            writer.String("Long[]");
            writer.String("Mono_Value");
            vector<long> value = convertToC< vector<long> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Int64(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[F" )
        {
            writer.String("Mono_Type");
            writer.String("Float[]");
            writer.String("Mono_Value");
            vector<float> value = convertToC< vector<float> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.Double(value[i]);
            }
            
            writer.EndArray();
        }
        
        if (className == "[Ljava.lang.String;" )
        {
            writer.String("Mono_Type");
            writer.String("String[]");
            writer.String("Mono_Value");
            vector<string> value = convertToC< vector<string> >(env, obj);
            
            int vectorSize = value.size();
            
            writer.StartArray();
            
            for(int i=0; i<vectorSize; i++)
            {
                writer.String(value[i].c_str());
            }
            
            writer.EndArray();
        }

        if (env->IsInstanceOf(obj, mapClazz))
        {
            int mapSize = env->CallIntMethod(obj, size);

            jobject keys = env->CallObjectMethod(obj, keySet);

            jobjectArray arrayOfKeys = (jobjectArray)env->CallObjectMethod(keys, toArray);

            for (int i = 0; i < mapSize; i++)
            {
                jstring keyName = (jstring)env->GetObjectArrayElement(arrayOfKeys, i);
                jobject mapValue = env->CallObjectMethod(obj, getMapValue, keyName);

                string key = convertToC<string>(env, keyName);

                writer.String(key.c_str());

                bool isMap = env->IsInstanceOf(mapValue, mapClazz);

                /* check if it's a map to construct json object*/
                if (isMap)
                {
                    writer.StartObject();
                    writer.String("Mono_Type");
                    writer.String("Map");
                    writer.String("Mono_Value");
                }

                toObjectJson(env, mapValue, writer);

                env->DeleteLocalRef(keyName);
                env->DeleteLocalRef(mapValue);

                if(isMap)
                {
                    writer.EndObject();
                }
            }

            env->DeleteLocalRef(keys);
            env->DeleteLocalRef(arrayOfKeys);
            env->DeleteLocalRef(obj);
        }

        writer.EndObject();
    }
};