#ifndef _DEV_SIGN_H_
#define _DEV_SIGN_H_
#include "pub/pub.h"
#include "dev_sign_internal.h"

LIB_EXPORT int32_t IOT_Sign_MQTT(iotx_mqtt_region_types_t region, iotx_dev_meta_info_t *meta, iotx_sign_mqtt_t *signout);

#endif


