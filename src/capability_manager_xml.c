
#include <string.h>
#include <dlog.h>

#include "capability_manager.h"
#include "capability_manager_defs.h"
#include "capability_manager_util.h"
#include "capability_manager_xml.h"

xmlDocPtr capability_manager_xml_util_get_doc(const char* docname)
{
	xmlDocPtr doc;

	doc = xmlParseFile(docname);

	if (doc == NULL) {
		_LOG("Document not parsed successfully.");
		return NULL;
	}

	return doc;
}

xmlXPathObjectPtr capability_manager_xml_util_get_nodeset(xmlDocPtr doc, xmlChar* xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);

	if (context == NULL) {
		_LOG("Error in xmlXPathNewContext");
		return NULL;
	}

	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);

	if (result == NULL) {
		_LOG("Error in xmlXPathEvalExpression");
		return NULL;
	}

	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		_LOG("No result");
		return NULL;
	}

	return result;
}

char* capability_manager_xml_util_get_single_item(xmlDocPtr doc, char* tag)
{
	char* ret = NULL;
	xmlNodeSetPtr nodeset;
	xmlXPathObjectPtr result;

	//char xpath[(int)strlen(tag)+3];
	char xpath[PATH_MAX] = {0,};
	strcpy(xpath, "//");
	strncat(xpath, tag, PATH_MAX - 3);

	result = capability_manager_xml_util_get_nodeset(doc, (xmlChar*) xpath);

	if (result) {
		nodeset = result ->nodesetval;
		ret = (char*) xmlNodeGetContent(nodeset->nodeTab[0]);
		xmlXPathFreeObject(result);
	}

	return ret;
}

int capability_manager_xml_util_get_hash_items(xmlDocPtr doc, char* tag, GHashTable* hash)
{
	xmlXPathObjectPtr result;
	xmlNodePtr cur;

	char xpath[PATH_MAX] = {0,};
	//char xpath[(int)strlen(tag)+3];
	strcpy(xpath, "//");
	strncat(xpath, tag, PATH_MAX - 3);

	result = capability_manager_xml_util_get_nodeset(doc, (xmlChar*) xpath);

	if (result) {
		cur = result->nodesetval->nodeTab[0]->xmlChildrenNode;

		while (cur) {

			if (xmlStrcasecmp(cur->name, (const xmlChar*) "text") != 0) {
				char* content = (char*) xmlNodeGetContent(cur);
				char* key = (char*) strdup((char*) cur->name);
				LOGV("HASH: Key[%s] Value[%s]", cur->name, content);
				g_hash_table_insert(hash, key, content);
			}

			cur = cur->next;
		}

		xmlXPathFreeObject(result);
	} else {
		_LOG("Fail to get tag: %s", tag);
		return 0;
	}

	return 1;
}

int
capability_manager_xml_util_get_device_info(xmlDocPtr doc, device_info_s* deviceInfo)
{
	_LOG("capability_manager_xml_util_get_device_info() called.");

	xmlXPathObjectPtr result;
	xmlNodePtr cur, child, grand_child;

	result = capability_manager_xml_util_get_nodeset(doc, (xmlChar*) "//device");
	TryReturn(result, CAPABILITY_MANAGER_ERROR_IO_ERROR, "capability_manager_xml_util_get_nodeset(//device) failed.");

	deviceInfo->features = bundle_create();

	cur = result->nodesetval->nodeTab[0];
	if (cur)
	{
		child = cur->xmlChildrenNode;
		while (child)
		{
			if (!xmlStrcasecmp(child->name, (const xmlChar*)"deviceID"))
			{
				char* deviceID = (char*) xmlNodeGetContent(child);
				deviceInfo->device_id = deviceID;
				_LOG(" - deviceID = [%s]", deviceID);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"deviceName"))
			{
				char* deviceName = (char*) xmlNodeGetContent(child);
				deviceInfo->device_name = deviceName;
				_LOG(" - deviceName = [%s]", deviceName);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"devicePlatform"))
			{
				char* devicePlatform = (char*) xmlNodeGetContent(child);
				deviceInfo->device_platform = devicePlatform;
				_LOG(" - devicePlatform = [%s]", devicePlatform);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"devicePlatformVersion"))
			{
				char* devicePlatformVersion = (char*) xmlNodeGetContent(child);
				deviceInfo->device_platform_version = devicePlatformVersion;
				_LOG(" - devicePlatformVersion = [%s]", devicePlatformVersion);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"deviceType"))
			{
				char* deviceType = (char*) xmlNodeGetContent(child);
				deviceInfo->device_type = deviceType;
				_LOG(" - deviceType = [%s]", deviceType);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"modelNumber"))
			{
				char* modelNumber = (char*) xmlNodeGetContent(child);
				deviceInfo->device_model_number = modelNumber;
				_LOG(" - modelNumber = [%s]", modelNumber);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"swVersion"))
			{
				char* swVersion = (char*) xmlNodeGetContent(child);
				deviceInfo->device_sw_version = swVersion;
				_LOG(" - swVersion = [%s]", swVersion);
			}
			else if (!xmlStrcasecmp(child->name, (const xmlChar*)"deviceFeature"))
			{
				grand_child = child->xmlChildrenNode;
				while (grand_child)
				{
					if (grand_child->name != NULL && xmlStrcasecmp(grand_child->name, (const xmlChar*) "text") != 0)
					{
						char* device_feature_key = strdup((char*)grand_child->name);
						char* device_feature_value = NULL;
						char default_value[2] = { 0, };

						char* value = (char*) xmlNodeGetContent(grand_child);
						if (value != NULL)
						{
							device_feature_value = value;
						}
						else
						{
							device_feature_value = default_value;
						}
						_LOG(" - device_feature_key = [%s], device_feature_value = [%s]", device_feature_key, device_feature_value);

						bundle_add_str(deviceInfo->features, device_feature_key, device_feature_value);
						safe_free(device_feature_key);
						safe_xml_free(value);
					}

					grand_child = grand_child->next;
				}
			}

			child = child->next;
		}
	}

	xmlXPathFreeObject(result);

	return CAPABILITY_MANAGER_ERROR_NONE;
}

int
capability_manager_xml_util_get_app_info(xmlDocPtr doc, const char* package_name, app_info_s* appInfo)
{
	if (package_name == NULL || appInfo == NULL)
	{
		return CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER;
	}
	_LOG("capability_manager_xml_util_get_app_info(%s)", package_name);

	int res = CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER;
	xmlXPathObjectPtr result;
	xmlNodePtr cur, child, grand_child;

	result = capability_manager_xml_util_get_nodeset(doc, (xmlChar*) "//app");
	TryReturn(result, CAPABILITY_MANAGER_ERROR_IO_ERROR, "capability_manager_xml_util_get_nodeset(//app) failed. [%s]", package_name);

	appInfo->features = bundle_create();

	cur = result->nodesetval->nodeTab[0];
	while (cur)
	{
		if (!xmlStrcasecmp(cur->name, (const xmlChar*)"app"))
		{
			child = cur->xmlChildrenNode;
			while (child)
			{
				if (child->name != NULL && !xmlStrcasecmp(child->name, (const xmlChar*)"packagename"))
				{
					char* value = (char*) xmlNodeGetContent(child);
					if (value != NULL && strcmp(package_name, value) == 0)
					{
						res = CAPABILITY_MANAGER_ERROR_NONE;

						while (child)
						{
							if (!xmlStrcasecmp(child->name, (const xmlChar*)"features"))
							{
								grand_child = child->xmlChildrenNode;
								while (grand_child)
								{
									if (xmlStrncasecmp(grand_child->name, (const xmlChar*) "feature", strlen("feature")) == 0)
									{
										char* feature_key = NULL;
										char* feature_value = NULL;
										char default_value[2] = { 0, };

										if (grand_child->properties)
										{
											if (xmlStrcasecmp(grand_child->properties->name, (const xmlChar*) "id") == 0)
											{
												feature_key = strdup((char*)grand_child->properties->children->content);
											}
											else
											{
												grand_child = grand_child->next;
												continue;
											}
										}
										else
										{
											feature_key = strdup((char*)grand_child->name);
										}

										char* value = (char*) xmlNodeGetContent(grand_child);
										if (value != NULL)
										{
											feature_value = value;
										}
										else
										{
											feature_value = default_value;
										}

										_LOG(" - feature_key = [%s], feature_value = [%s]", feature_key, feature_value);
										bundle_add_str(appInfo->features, feature_key, feature_value);
										safe_free(feature_key);
										safe_xml_free(value);
									}

									grand_child = grand_child->next;
								}
							}
							else if (!xmlStrcasecmp(child->name, (const xmlChar*)"version"))
							{
								char* version = (char*) xmlNodeGetContent(child);
								appInfo->version = version;
								_LOG(" - version = [%s]", version);
							}

							child = child->next;
						}
					}
					safe_xml_free(value);
				}

				if (child)
				{
					child = child->next;
				}
			}
		}

		cur = cur->next;
	}

	xmlXPathFreeObject(result);

	return res;
}

bool
capability_manager_xml_util_is_app_installed(xmlDocPtr doc, const char* package_name)
{
	if (package_name == NULL)
	{
		return false;
	}
	_LOG("capability_manager_xml_util_is_app_installed(%s)", package_name);

	bool res = false;
	xmlXPathObjectPtr result;
	xmlNodePtr cur, child, grand_child;

	result = capability_manager_xml_util_get_nodeset(doc, (xmlChar*) "//app");
	TryReturn(result, false, "capability_manager_xml_util_get_nodeset(//app) failed. [%s]", package_name);

	cur = result->nodesetval->nodeTab[0];
	while (cur)
	{
		if (!xmlStrcasecmp(cur->name, (const xmlChar*)"app"))
		{
			child = cur->xmlChildrenNode;
			while (child)
			{
				if (child->name != NULL && !xmlStrcasecmp(child->name, (const xmlChar*)"packagename"))
				{
					char* value = (char*) xmlNodeGetContent(child);
					if (value != NULL && strcmp(package_name, value) == 0)
					{
						while (child)
						{
							if (child->name != NULL && !xmlStrcasecmp(child->name, (const xmlChar*)"features"))
							{
								grand_child = child->xmlChildrenNode;
								while (grand_child)
								{
									if (grand_child->name != NULL && xmlStrcasecmp(grand_child->name, (const xmlChar*) "Installed") == 0)
									{
										char* installed = (char*) xmlNodeGetContent(grand_child);
										_LOG(" - <%s>%s</%s>", grand_child->name, installed, grand_child->name);

										if (installed != NULL && strcmp(installed, "true") == 0)
										{
											res = true;
										}

										safe_xml_free(installed);
										break;
									}

									grand_child = grand_child->next;
								}
							}

							child = child->next;
						}
					}
					safe_xml_free(value);
				}

				if (child)
				{
					child = child->next;
				}
			}
		}

		cur = cur->next;
	}

	xmlXPathFreeObject(result);

	return res;
}

bool
capability_manager_xml_util_has_app_required_feature(xmlDocPtr doc)
{
	bool res = false;
	xmlXPathObjectPtr result;
	xmlNodePtr cur, child;

	result = capability_manager_xml_util_get_nodeset(doc, (xmlChar*) "//features");
	TryReturn(result, false, "capability_manager_xml_util_get_nodeset(//features) failed.");

	cur = result->nodesetval->nodeTab[0];
	if (cur)
	{
		if (!xmlStrcasecmp(cur->name, (const xmlChar*)"features"))
		{
			child = cur->xmlChildrenNode;
			while (child)
			{
				if ((!xmlStrcasecmp(child->name, (const xmlChar*)"requiredPackage"))
						|| (!xmlStrcasecmp(child->name, (const xmlChar*)"requiredDeviceFeature")))
				{
					char* value = (char*) xmlNodeGetContent(child);
					if (value != NULL)
					{
						_LOG(" - <%s>%s</%s>", child->name, value, child->name);
						res = true;
						safe_xml_free(value);
						break;
					}
				}

				child = child->next;
			}
		}
	}

	xmlXPathFreeObject(result);

	return res;
}
