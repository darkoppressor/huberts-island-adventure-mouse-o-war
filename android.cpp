/* Copyright (c) 2012-2013 Cheese and Bacon Games, LLC */
/* See the file docs/COPYING.txt for copying permission. */

#include "android.h"
#include "symbols.h"

using namespace std;

#ifdef GAME_OS_ANDROID
    extern "C"{
        static bool sensor_available[SENSOR_TYPE_COUNT];
        static bool sensor_enabled[SENSOR_TYPE_COUNT];
        static int sensor_value_count_actual[SENSOR_TYPE_COUNT];
        static float sensor_values[SENSOR_TYPE_COUNT][SENSOR_VALUES_MAX];

        static bool gps_available;
        static bool gps_accessible;
        static bool gps_enabled;
        static double gps_values[GPS_VALUES_MAX];

        void Java_org_libsdl_app_SDLActivity_nativeUpdateSensorAvailable(JNIEnv* env,jclass jcls,jint sensortype,jboolean available){
            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                sensor_available[sensortype-1]=available;
            }
        }

        void Java_org_libsdl_app_SDLActivity_nativeUpdateSensorEnabled(JNIEnv* env,jclass jcls,jint sensortype,jboolean enabled){
            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                sensor_enabled[sensortype-1]=enabled;
            }
        }

        void Java_org_libsdl_app_SDLActivity_nativeSensorUpdate(JNIEnv* env,jclass jcls,jint sensortype,jfloatArray values){
            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                jsize length=env->GetArrayLength(values);

                sensor_value_count_actual[sensortype-1]=length;

                jfloat* values_array=env->GetFloatArrayElements(values,0);

                for(int i=0;i<length && i<SENSOR_VALUES_MAX;i++){
                    sensor_values[sensortype-1][i]=values_array[i];
                }

                env->ReleaseFloatArrayElements(values,values_array,0);
            }
        }

        void Java_org_libsdl_app_SDLActivity_nativeUpdateGPSAvailable(JNIEnv* env,jclass jcls,jboolean available){
            gps_available=available;
        }

        void Java_org_libsdl_app_SDLActivity_nativeUpdateGPSAccessible(JNIEnv* env,jclass jcls,jboolean accessible){
            gps_accessible=accessible;
        }

        void Java_org_libsdl_app_SDLActivity_nativeUpdateGPSEnabled(JNIEnv* env,jclass jcls,jboolean enabled){
            gps_enabled=enabled;
        }

        void Java_org_libsdl_app_SDLActivity_nativeGPSUpdate(JNIEnv* env,jclass jcls,jfloat accuracy,jdouble altitude,jfloat bearing,jdouble latitude,jdouble longitude,jfloat speed){
            gps_values[0]=accuracy;
            gps_values[1]=altitude;
            gps_values[2]=bearing;
            gps_values[3]=latitude;
            gps_values[4]=longitude;
            gps_values[5]=speed;
        }

        void jni_initialize(){
            for(int i=0;i<SENSOR_TYPE_COUNT;i++){
                sensor_available[i]=false;
                sensor_enabled[i]=false;
                sensor_value_count_actual[i]=0;

                for(int n=0;n<SENSOR_VALUES_MAX;n++){
                    sensor_values[i][n]=0.0f;
                }
            }

            gps_available=false;
            gps_accessible=false;
            gps_enabled=false;

            for(int i=0;i<GPS_VALUES_MAX;i++){
                gps_values[i]=0.0;
            }
        }

        int jni_get_sensor_number(const char* sensor_name){
            if(strcmp(sensor_name,"accelerometer")==0){
                return Android::SENSOR_TYPE_ACCELEROMETER;
            }
            else if(strcmp(sensor_name,"ambient_temperature")==0){
                return Android::SENSOR_TYPE_AMBIENT_TEMPERATURE;
            }
            else if(strcmp(sensor_name,"game_rotation_vector")==0){
                return Android::SENSOR_TYPE_GAME_ROTATION_VECTOR;
            }
            else if(strcmp(sensor_name,"geomagnetic_rotation_vector")==0){
                return Android::SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR;
            }
            else if(strcmp(sensor_name,"gravity")==0){
                return Android::SENSOR_TYPE_GRAVITY;
            }
            else if(strcmp(sensor_name,"gyroscope")==0){
                return Android::SENSOR_TYPE_GYROSCOPE;
            }
            else if(strcmp(sensor_name,"gyroscope_uncalibrated")==0){
                return Android::SENSOR_TYPE_GYROSCOPE_UNCALIBRATED;
            }
            else if(strcmp(sensor_name,"light")==0){
                return Android::SENSOR_TYPE_LIGHT;
            }
            else if(strcmp(sensor_name,"linear_acceleration")==0){
                return Android::SENSOR_TYPE_LINEAR_ACCELERATION;
            }
            else if(strcmp(sensor_name,"magnetic_field")==0){
                return Android::SENSOR_TYPE_MAGNETIC_FIELD;
            }
            else if(strcmp(sensor_name,"magnetic_field_uncalibrated")==0){
                return Android::SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED;
            }
            else if(strcmp(sensor_name,"pressure")==0){
                return Android::SENSOR_TYPE_PRESSURE;
            }
            else if(strcmp(sensor_name,"proximity")==0){
                return Android::SENSOR_TYPE_PROXIMITY;
            }
            else if(strcmp(sensor_name,"relative_humidity")==0){
                return Android::SENSOR_TYPE_RELATIVE_HUMIDITY;
            }
            else if(strcmp(sensor_name,"rotation_vector")==0){
                return Android::SENSOR_TYPE_ROTATION_VECTOR;
            }
            else if(strcmp(sensor_name,"step_counter")==0){
                return Android::SENSOR_TYPE_STEP_COUNTER;
            }
            else if(strcmp(sensor_name,"step_detector")==0){
                return Android::SENSOR_TYPE_STEP_DETECTOR;
            }
            else{
                return 0;
            }
        }

        bool jni_get_sensor_available(const char* sensor_name){
            int sensortype=jni_get_sensor_number(sensor_name);

            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                return sensor_available[sensortype-1];
            }
            else{
                return false;
            }
        }

        bool jni_get_sensor_enabled(const char* sensor_name){
            int sensortype=jni_get_sensor_number(sensor_name);

            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                return sensor_enabled[sensortype-1];
            }
            else{
                return false;
            }
        }

        int jni_get_sensor_value_count_actual(const char* sensor_name){
            int sensortype=jni_get_sensor_number(sensor_name);

            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                return sensor_value_count_actual[sensortype-1];
            }
            else{
                return 0;
            }
        }

        void jni_get_sensor_values(const char* sensor_name,float values[SENSOR_VALUES_MAX]){
            int sensortype=jni_get_sensor_number(sensor_name);

            if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
                for(int i=0;i<SENSOR_VALUES_MAX;i++){
                    values[i]=sensor_values[sensortype-1][i];
                }
            }
        }

        bool jni_get_gps_available(){
            return gps_available;
        }

        bool jni_get_gps_accessible(){
            return gps_accessible;
        }

        bool jni_get_gps_enabled(){
            return gps_enabled;
        }

        void jni_get_gps_values(double values[GPS_VALUES_MAX]){
            for(int i=0;i<GPS_VALUES_MAX;i++){
                values[i]=gps_values[i];
            }
        }
    }
#endif

Android_Sensor::Android_Sensor(){
    value_count=0;
    units="";
}

void Android_Sensor::setup(int get_value_count,string get_units,const vector<string>& get_value_labels){
    value_count=get_value_count;
    units=get_units;

    value_labels=get_value_labels;
    while(value_labels.size()<SENSOR_VALUES_MAX){
        value_labels.push_back("");
    }
}

const string Android_GPS::UNITS_ACCURACY="m";
const string Android_GPS::UNITS_ALTITUDE="m";
const string Android_GPS::UNITS_BEARING=Symbols::degrees();
const string Android_GPS::UNITS_LATITUDE=Symbols::degrees();
const string Android_GPS::UNITS_LONGITUDE=Symbols::degrees();
const string Android_GPS::UNITS_SPEED="m/s";

Android_GPS::Android_GPS(){
    accuracy=0.0;
    altitude=0.0;
    bearing=0.0;
    latitude=0.0;
    longitude=0.0;
    speed=0.0;
}

const int Android::SENSOR_TYPE_ACCELEROMETER=1;
const int Android::SENSOR_TYPE_AMBIENT_TEMPERATURE=13;
const int Android::SENSOR_TYPE_GAME_ROTATION_VECTOR=15;
const int Android::SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR=20;
const int Android::SENSOR_TYPE_GRAVITY=9;
const int Android::SENSOR_TYPE_GYROSCOPE=4;
const int Android::SENSOR_TYPE_GYROSCOPE_UNCALIBRATED=16;
const int Android::SENSOR_TYPE_LIGHT=5;
const int Android::SENSOR_TYPE_LINEAR_ACCELERATION=10;
const int Android::SENSOR_TYPE_MAGNETIC_FIELD=2;
const int Android::SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED=14;
const int Android::SENSOR_TYPE_PRESSURE=6;
const int Android::SENSOR_TYPE_PROXIMITY=8;
const int Android::SENSOR_TYPE_RELATIVE_HUMIDITY=12;
const int Android::SENSOR_TYPE_ROTATION_VECTOR=11;
const int Android::SENSOR_TYPE_STEP_COUNTER=19;
const int Android::SENSOR_TYPE_STEP_DETECTOR=18;

bool Android::initialized=false;

Android_Sensor Android::sensors[SENSOR_TYPE_COUNT];

void Android::initialize(){
    if(!initialized){
        vector<string> value_labels;

        value_labels.clear();
        value_labels.push_back("Acceleration on the x-axis");
        value_labels.push_back("Acceleration on the y-axis");
        value_labels.push_back("Acceleration on the z-axis");
        sensors[SENSOR_TYPE_ACCELEROMETER-1].setup(3,"m/s"+Symbols::squared(),value_labels);

        value_labels.clear();
        value_labels.push_back("Ambient temperature");
        sensors[SENSOR_TYPE_AMBIENT_TEMPERATURE-1].setup(1,Symbols::degrees()+"C",value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(theta/2)");
        value_labels.push_back("y*sin(theta/2)");
        value_labels.push_back("z*sin(theta/2)");
        value_labels.push_back("cos(theta/2)");
        sensors[SENSOR_TYPE_GAME_ROTATION_VECTOR-1].setup(4,"",value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(theta/2)");
        value_labels.push_back("y*sin(theta/2)");
        value_labels.push_back("z*sin(theta/2)");
        value_labels.push_back("cos(theta/2)");
        value_labels.push_back("Estimated heading accuracy");
        sensors[SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR-1].setup(5,"",value_labels);

        value_labels.clear();
        value_labels.push_back("Acceleration due to gravity on the x-axis");
        value_labels.push_back("Acceleration due to gravity on the y-axis");
        value_labels.push_back("Acceleration due to gravity on the z-axis");
        sensors[SENSOR_TYPE_GRAVITY-1].setup(3,"m/s"+Symbols::squared(),value_labels);

        value_labels.clear();
        value_labels.push_back("Angular speed around the x-axis");
        value_labels.push_back("Angular speed around the y-axis");
        value_labels.push_back("Angular speed around the z-axis");
        sensors[SENSOR_TYPE_GYROSCOPE-1].setup(3,"rad/s",value_labels);

        value_labels.clear();
        value_labels.push_back("Uncalibrated angular speed around the x-axis");
        value_labels.push_back("Uncalibrated angular speed around the y-axis");
        value_labels.push_back("Uncalibrated angular speed around the z-axis");
        value_labels.push_back("Estimated drift around the x-axis");
        value_labels.push_back("Estimated drift around the y-axis");
        value_labels.push_back("Estimated drift around the z-axis");
        sensors[SENSOR_TYPE_GYROSCOPE_UNCALIBRATED-1].setup(6,"rad/s",value_labels);

        value_labels.clear();
        value_labels.push_back("Ambient light level");
        sensors[SENSOR_TYPE_LIGHT-1].setup(1,"lx",value_labels);

        value_labels.clear();
        value_labels.push_back("Acceleration without gravity on the x-axis");
        value_labels.push_back("Acceleration without gravity on the y-axis");
        value_labels.push_back("Acceleration without gravity on the z-axis");
        sensors[SENSOR_TYPE_LINEAR_ACCELERATION-1].setup(3,"m/s"+Symbols::squared(),value_labels);

        value_labels.clear();
        value_labels.push_back("Ambient magnetic field in the x-axis");
        value_labels.push_back("Ambient magnetic field in the y-axis");
        value_labels.push_back("Ambient magnetic field in the z-axis");
        sensors[SENSOR_TYPE_MAGNETIC_FIELD-1].setup(3,"uT",value_labels);

        value_labels.clear();
        value_labels.push_back("Uncalibrated ambient magnetic field in the x-axis");
        value_labels.push_back("Uncalibrated ambient magnetic field in the y-axis");
        value_labels.push_back("Uncalibrated ambient magnetic field in the z-axis");
        value_labels.push_back("Estimated iron bias in the x-axis");
        value_labels.push_back("Estimated iron bias in the y-axis");
        value_labels.push_back("Estimated iron bias in the z-axis");
        sensors[SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED-1].setup(6,"uT",value_labels);

        value_labels.clear();
        value_labels.push_back("Atmospheric pressure");
        sensors[SENSOR_TYPE_PRESSURE-1].setup(1,"hPa",value_labels);

        value_labels.clear();
        value_labels.push_back("Proximity distance");
        sensors[SENSOR_TYPE_PROXIMITY-1].setup(1,"cm",value_labels);

        value_labels.clear();
        value_labels.push_back("Relative ambient humidity");
        sensors[SENSOR_TYPE_RELATIVE_HUMIDITY-1].setup(1,"%",value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(theta/2)");
        value_labels.push_back("y*sin(theta/2)");
        value_labels.push_back("z*sin(theta/2)");
        value_labels.push_back("cos(theta/2)");
        value_labels.push_back("Estimated heading accuracy");
        sensors[SENSOR_TYPE_ROTATION_VECTOR-1].setup(5,"",value_labels);

        value_labels.clear();
        value_labels.push_back("Steps");
        sensors[SENSOR_TYPE_STEP_COUNTER-1].setup(1,"",value_labels);

        value_labels.clear();
        value_labels.push_back("");
        sensors[SENSOR_TYPE_STEP_DETECTOR-1].setup(1,"",value_labels);

        #ifdef GAME_OS_ANDROID
            jni_initialize();
        #endif

        initialized=true;
    }
}

bool Android::get_sensor_availability(string sensor_type){
    #ifdef GAME_OS_ANDROID
        return jni_get_sensor_available(sensor_type.c_str());
    #endif

    return false;
}

bool Android::get_sensor_state(string sensor_type){
    #ifdef GAME_OS_ANDROID
        return jni_get_sensor_enabled(sensor_type.c_str());
    #endif

    return false;
}

int Android::get_sensor_value_count_actual(string sensor_type){
    #ifdef GAME_OS_ANDROID
        return jni_get_sensor_value_count_actual(sensor_type.c_str());
    #endif

    return 0;
}

int Android::get_sensor_value_count(string sensor_type){
    #ifdef GAME_OS_ANDROID
        int sensortype=jni_get_sensor_number(sensor_type.c_str());

        if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
            return sensors[sensortype-1].value_count;
        }
    #endif

    return 0;
}

string Android::get_sensor_units(string sensor_type){
    #ifdef GAME_OS_ANDROID
        int sensortype=jni_get_sensor_number(sensor_type.c_str());

        if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
            return sensors[sensortype-1].units;
        }
    #endif

    return "";
}

void Android::get_sensor_value_labels(string sensor_type,string value_labels[SENSOR_VALUES_MAX]){
    #ifdef GAME_OS_ANDROID
        int sensortype=jni_get_sensor_number(sensor_type.c_str());

        if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
            for(int i=0;i<SENSOR_VALUES_MAX;i++){
                value_labels[i]=sensors[sensortype-1].value_labels[i];
            }
        }
    #endif
}

void Android::get_sensor_values(string sensor_type,float values[SENSOR_VALUES_MAX]){
    #ifdef GAME_OS_ANDROID
        jni_get_sensor_values(sensor_type.c_str(),values);
    #endif
}

void Android::set_sensor_enabled(string sensor_type,bool enabled){
    #ifdef GAME_OS_ANDROID
        int sensortype=jni_get_sensor_number(sensor_type.c_str());

        if(sensortype>0 && sensortype<=SENSOR_TYPE_COUNT){
            call_android_method("enableSensor","(IZ)V",sensortype,enabled);
        }
    #endif
}

void Android::set_gps_enabled(bool enabled,uint32_t minimum_update_time,float minimum_update_distance){
    #ifdef GAME_OS_ANDROID
        call_android_method("enableGPS","(ZIF)V",enabled,minimum_update_time,minimum_update_distance);
    #endif
}

void Android::enable_sensor(string sensor_type){
    set_sensor_enabled(sensor_type,true);
}

void Android::disable_sensor(string sensor_type){
    set_sensor_enabled(sensor_type,false);
}

void Android::vibrate(uint32_t length){
    call_android_method("vibrate","(I)V",(int)length);
}

void Android::vibrate_stop(){
    call_android_method("vibrateStop","()V");
}

bool Android::get_gps_availability(){
    #ifdef GAME_OS_ANDROID
        return jni_get_gps_available();
    #endif

    return false;
}

bool Android::get_gps_accessibility(){
    #ifdef GAME_OS_ANDROID
        return jni_get_gps_accessible();
    #endif

    return false;
}

bool Android::get_gps_state(){
    #ifdef GAME_OS_ANDROID
        return jni_get_gps_enabled();
    #endif

    return false;
}

Android_GPS Android::get_gps_readout(){
    Android_GPS gps;

    #ifdef GAME_OS_ANDROID
        double values[GPS_VALUES_MAX];

        jni_get_gps_values(values);

        gps.accuracy=values[0];
        gps.altitude=values[1];
        gps.bearing=values[2];
        gps.latitude=values[3];
        gps.longitude=values[4];
        gps.speed=values[5];
    #endif

    return gps;
}

void Android::enable_gps(uint32_t minimum_update_time,float minimum_update_distance){
    set_gps_enabled(true,minimum_update_time,minimum_update_distance);
}

void Android::disable_gps(){
    set_gps_enabled(false,0,0.0f);
}
