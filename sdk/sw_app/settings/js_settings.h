#ifndef __JS_SETTINGS_H__
#define __JS_SETTINGS_H__
////////////////////////////////////////////////////////////////////////////////

#include "app_set.h"

#define ENABLE_JSON_CONFIG_FILE (0)

int js_read_settings(app_set_t* const set, const char* fname);
int js_write_settings(const app_set_t* const set, const char* fname);

////////////////////////////////////////////////////////////////////////////////
#endif//__JS_SETTINGS_H__
