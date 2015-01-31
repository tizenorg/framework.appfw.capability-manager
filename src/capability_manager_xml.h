#ifndef CAPABILITY_MANAGER_XML_H
#define CAPABILITY_MANAGER_XML_H

#include <glib.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "bundle.h"
#include "capability_manager_defs.h"

xmlDocPtr capability_manager_xml_util_get_doc(const char* docname);

xmlXPathObjectPtr capability_manager_xml_util_get_nodeset(xmlDocPtr doc, xmlChar* xpath);

char* capability_manager_xml_util_get_single_item(xmlDocPtr doc, char* tag);

int capability_manager_xml_util_get_hash_items(xmlDocPtr doc, char* tag, GHashTable* hash);

int capability_manager_xml_util_get_device_info(xmlDocPtr doc, device_info_s* deviceInfo);

int capability_manager_xml_util_get_app_info(xmlDocPtr doc, const char* package_name, app_info_s* appInfo);

bool capability_manager_xml_util_is_app_installed(xmlDocPtr doc, const char* package_name);

bool capability_manager_xml_util_has_app_required_feature(xmlDocPtr doc);

#endif /* CAPABILITY_MANAGER_XML_H */
