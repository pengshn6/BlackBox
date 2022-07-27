//
// Created by Milk on 4/25/21.
//

#include "SystemPropertiesHook.h"
#include <IO.h>
#include <BoxCore.h>
//#include "UnixFileSystemHook.h"
#import "JniHook/JniHook.h"
#include "shadowhook.h"
#include "Log.h"
static std::map<std::string , std::string > prop_map;


HOOK_JNI(jint, __system_property_get, const char* name, char* value) {
    SHADOWHOOK_STACK_SCOPE();
    log_print_debug("calling __system_property_get");
    if(NULL == name || NULL == value){
        return orig___system_property_get(name,value);
    }
//    log_print_debug("calling __system_property_get");
    ALOGD(name, value);
    auto ret = prop_map.find(name);
    if(ret != prop_map.end()){
        const char* ret_value = ret->second.c_str();
        strcpy(value,ret_value);
        return strlen(ret_value);
    }
    return orig___system_property_get(name,value);
}


void SystemPropertiesHook::init(JNIEnv *env) {
    prop_map.insert(map<std::string , std::string >::value_type ("ro.product.board", "umi"));
    prop_map.insert(map<std::string , std::string >::value_type ("ro.product.brand", "Xiaomi"));
    prop_map.insert(map<std::string , std::string >::value_type ("ro.product.device", "umi"));
    prop_map.insert(map<std::string , std::string >::value_type ("ro.build.display.id", "QKQ1.191117.002 test-keys"));
    prop_map.insert(map<std::string , std::string >::value_type ("ro.build.host", "c5-miui-ota-bd074.bj"));
    prop_map.insert(map<std::string , std::string >::value_type ("ro.build.id", "QKQ1.191117.002"));
    prop_map.insert(map<std::string , std::string >::value_type ("ro.product.manufacturer", "Xiaomi"));
    shadowhook_hook_sym_name("libc.so", "__system_property_get", (void *) new___system_property_get, (void **)(&orig___system_property_get));
}