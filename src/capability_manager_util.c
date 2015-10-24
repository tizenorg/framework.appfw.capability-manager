
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <package-manager.h>

#include "capability_manager_util.h"
#include "capability_manager_defs.h"
#include "capability_manager_xml.h"

void safe_free(void* ptr)
{
	if (ptr) {
		free(ptr);
		ptr = NULL;
	}
}

void safe_xml_free(void* ptr)
{
	if (ptr) {
		xmlFree(ptr);
		ptr = NULL;
	}
}

gboolean is_file_exist(const char* srcPath)
{
	FILE* _file = NULL;

	if ((_file = fopen(srcPath, "r")) == NULL) {
		LOGD("File %s NOT Exists", srcPath);
		return FALSE;
	}

	fclose(_file);
	LOGD("File %s Exists", srcPath);
	return TRUE;
}

