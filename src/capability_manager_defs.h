#ifndef CAPABILITY_MANAGER_DEFS_H
#define CAPABILITY_MANAGER_DEFS_H

#include "bundle.h"

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#define LOG_TAG "capability-manager"

#define _LOG(fmt, arg...) SECURE_SLOGE(fmt, ##arg)

#define CAPABILITY_MANAGER_VERSION "version=[20140127.1]"

typedef struct device_info_s {
	char* device_id;
	char* device_name;
	char* device_platform;
	char* device_platform_version;
	char* device_type;
	char* device_model_number;
	char* device_sw_version;
	bundle* features;

} device_info_s;

typedef struct app_info_s {
	char* version;
	bundle* features;

} app_info_s;

#define TryReturn(condition, returnValue, ...)	\
	if (!(condition)) { \
		_LOG(__VA_ARGS__); \
		return returnValue;	\
	} \
	else {;}

#define TryCatch(condition, expr, ...) \
	if (!(condition)) { \
		_LOG(__VA_ARGS__); \
		expr; \
		goto CATCH;	\
	} \
	else {;}

#if 0 // format
#define DIR_BIN				L"/bin"
static const int INSTALLER_BLOCK_SIZE = 4096;
#define OSP_INSTALLER		"osp-installer"

enum InstallerRdsState
{
	INSTALLER_RDS_STATE_NONE,
	INSTALLER_RDS_STATE_DELETE,
	INSTALLER_RDS_STATE_ADD,
	INSTALLER_RDS_STATE_MODIFY,

};

typedef struct
{
	const char* pDir;
	int dirType;
	int ownerIndex;
	int modeIndex;
	int smackIndex;

} RdsPermssions;
#endif

#ifdef AppLog
#undef AppLog
#define AppLog(...)		AppLogTagInternal(OSP_INSTALLER, __PRETTY_FUNCTION__, __LINE__, __VA_ARGS__); \
						InstallerUtil::AppendLog(__PRETTY_FUNCTION__, __LINE__, false, __VA_ARGS__);
#endif

#endif // CAPABILITY_MANAGER_DEFS_H
