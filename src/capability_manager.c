#include <dlog.h>
#include <stdbool.h>

#include "capability_manager.h"
#include "capability_manager_defs.h"
#include "capability_manager_xml.h"
#include "capability_manager_util.h"

API int
capability_manager_create_device_info(device_info_h* device_info)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_create_device_info() failed. (invalid-device_info)");

	int res = CAPABILITY_MANAGER_ERROR_NONE;
	device_info_s* deviceInfo = NULL;
	const char* file_path = "/opt/usr/share/wms/HostStatus.xml";

	TryReturn(is_file_exist(file_path) == TRUE, CAPABILITY_MANAGER_ERROR_DEVICE_NOT_CONNECTED, "is_file_exist(%s) failed.", file_path);

	xmlDocPtr doc = capability_manager_xml_util_get_doc(file_path);
	TryCatch(doc, res = CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_xml_util_get_doc(%s) failed.", file_path);

	deviceInfo = (device_info_s*)calloc(1, sizeof(device_info_s));
	TryCatch(deviceInfo, res = CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY, "calloc() failed.");

	res = capability_manager_xml_util_get_device_info(doc, deviceInfo);
	TryCatch(res == CAPABILITY_MANAGER_ERROR_NONE, , "capability_manager_xml_util_get_device_info() failed.");

	xmlFreeDoc(doc);

	*device_info = (void*) deviceInfo;
	return res;

CATCH:
	if (doc)
	{
		xmlFreeDoc(doc);
	}
	if (deviceInfo)
	{
		capability_manager_destroy_device_info((device_info_h)deviceInfo);
	}

	*device_info = NULL;
	return res;
}

API int
capability_manager_destroy_device_info(device_info_h device_info)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_destroy_device_info() failed. (invalid-device_info)");

	device_info_s* info = (device_info_s*) device_info;

	safe_xml_free(info->device_id);
	safe_xml_free(info->device_name);
	safe_xml_free(info->device_platform);
	safe_xml_free(info->device_platform_version);
	safe_xml_free(info->device_type);
	safe_xml_free(info->device_model_number);
	safe_xml_free(info->device_sw_version);

	if (info->features)
	{
		bundle_free(info->features);
	}

	safe_free(info);

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_id(device_info_h device_info, char** device_id)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_id() failed. (invalid-device_info)");
	TryReturn(device_id, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_id() failed. (invalid-device_id)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_id)
	{
		*device_id = strdup((char*) info->device_id);
	}
	else
	{
		*device_id = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_name(device_info_h device_info, char** device_name)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_name() failed. (invalid-device_info)");
	TryReturn(device_name, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_name() failed. (invalid-device_name)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_name)
	{
		*device_name = strdup((char*) info->device_name);
	}
	else
	{
		*device_name = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_platform(device_info_h device_info, char** device_platform)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_platform() failed. (invalid-device_info)");
	TryReturn(device_platform, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_platform() failed. (invalid-device_platform)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_platform)
	{
		*device_platform = strdup((char*) info->device_platform);
	}
	else
	{
		*device_platform = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_platform_version(device_info_h device_info, char** device_platform_version)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_platform_version() failed. (invalid-device_info)");
	TryReturn(device_platform_version, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_platform_version() failed. (invalid-device_platform_version)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_platform_version)
	{
		*device_platform_version = strdup((char*) info->device_platform_version);
	}
	else
	{
		*device_platform_version = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_type(device_info_h device_info, char** device_type)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_type() failed. (invalid-device_info)");
	TryReturn(device_type, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_type() failed. (invalid-device_type)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_type)
	{
		*device_type = strdup((char*) info->device_type);
	}
	else
	{
		*device_type = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_model_number(device_info_h device_info, char** device_model_number)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_model_number() failed. (invalid-device_info)");
	TryReturn(device_model_number, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_model_number() failed. (invalid-device_model_number)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_model_number)
	{
		*device_model_number = strdup((char*) info->device_model_number);
	}
	else
	{
		*device_model_number = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_sw_version(device_info_h device_info, char** device_sw_version)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_sw_version() failed. (invalid-device_info)");
	TryReturn(device_sw_version, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_sw_version() failed. (invalid-device_sw_version)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->device_sw_version)
	{
		*device_sw_version = strdup((char*) info->device_sw_version);
	}
	else
	{
		*device_sw_version = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_device_features(device_info_h device_info, bundle** features)
{
	TryReturn(device_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_features() failed. (invalid-device_info)");
	TryReturn(features, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_device_features() failed. (invalid-features)");

	device_info_s* info = (device_info_s*) device_info;
	if (info->features)
	{
		*features = bundle_dup(info->features);
	}
	else
	{
		*features = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_create_app_info(const char* package_name, app_info_h* app_info)
{
	TryReturn(package_name, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_create_app_info() failed. (invalid-package_name)");
	TryReturn(app_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_create_app_info() failed. (invalid-app_info)");

	int res = CAPABILITY_MANAGER_ERROR_NONE;
	app_info_s* appInfo = NULL;
	const char* file_path = "/opt/usr/share/wms/HostStatus.xml";

	TryReturn(is_file_exist(file_path) == TRUE, CAPABILITY_MANAGER_ERROR_DEVICE_NOT_CONNECTED, "is_file_exist(%s) failed.", file_path);

	xmlDocPtr doc = capability_manager_xml_util_get_doc(file_path);
	TryCatch(doc, res = CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_xml_util_get_doc(%s) failed.", file_path);

	appInfo = (app_info_s*)calloc(1, sizeof(app_info_s));
	TryCatch(appInfo, res = CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY, "calloc() failed.");

	res = capability_manager_xml_util_get_app_info(doc, package_name, appInfo);
	TryCatch(res == CAPABILITY_MANAGER_ERROR_NONE, , "capability_manager_xml_util_get_app_info(%s) failed.", package_name);

	xmlFreeDoc(doc);

	*app_info = (void*) appInfo;
	return res;

CATCH:
	if (doc)
	{
		xmlFreeDoc(doc);
	}
	if (appInfo)
	{
		capability_manager_destroy_app_info((app_info_h)appInfo);
	}

	*app_info = NULL;
	return res;
}

API int
capability_manager_destroy_app_info(app_info_h app_info)
{
	TryReturn(app_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_destroy_app_info() failed. (invalid-app_info)");

	app_info_s* info = (app_info_s*) app_info;

	safe_free(info->version);
	if (info->features)
	{
		bundle_free(info->features);
	}

	safe_free(info);

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_app_version(app_info_h app_info, char** version)
{
	TryReturn(app_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_app_version() failed. (invalid-app_info)");
	TryReturn(version, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_app_version() failed. (invalid-version)");

	app_info_s* info = (app_info_s*) app_info;
	if (info->version)
	{
		*version = strdup((char*) info->version);
	}
	else
	{
		*version = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_get_app_features(app_info_h app_info, bundle** features)
{
	TryReturn(app_info, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_app_version() failed. (invalid-app_info)");
	TryReturn(features, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_get_app_version() failed. (invalid-features)");

	app_info_s* info = (app_info_s*) app_info;
	if (info->features)
	{
		*features = bundle_dup(info->features);
	}
	else
	{
		*features = NULL;
	}

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_is_app_installed(const char* package_name, bool* installed)
{
	TryReturn(package_name, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_is_app_installed() failed. (invalid-package_name)");
	TryReturn(installed, CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER, "capability_manager_is_app_installed() failed. (invalid-installed)");

	const char* file_path = "/opt/usr/share/wms/HostStatus.xml";

	xmlDocPtr doc = capability_manager_xml_util_get_doc(file_path);
	TryReturn(doc, CAPABILITY_MANAGER_ERROR_IO_ERROR, "capability_manager_xml_util_get_doc(%s) failed.", file_path);

	*installed = capability_manager_xml_util_is_app_installed(doc, package_name);

	xmlFreeDoc(doc);

	return CAPABILITY_MANAGER_ERROR_NONE;
}

API int
capability_manager_disable_packages(void)
{
	const char* preload_path = "/usr/apps/";
	const char* download_path = "/opt/usr/apps/";

	_disable_packages_for_required_feature(download_path);
	_disable_packages_for_required_feature(preload_path);

	return CAPABILITY_MANAGER_ERROR_NONE;
}
