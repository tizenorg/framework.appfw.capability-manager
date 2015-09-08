#ifndef CAPABILITY_MANAGER_UTIL_H
#define CAPABILITY_MANAGER_UTIL_H

#include <dlog.h>
#include <glib.h>
#include <stdbool.h>

void safe_free(void* ptr);
void safe_xml_free(void* ptr);
gboolean is_file_exist(const char* srcPath);

void _disable_packages_for_required_feature(const char* root_path);
bool _disable_package(const char* package_id);

#endif /* CAPABILITY_MANAGER_UTIL_H */


