
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

void
_disable_packages_for_required_feature(const char* root_path)
{
	DIR* dir = NULL;
	struct dirent* entry = NULL;

	dir = opendir(root_path);
	if (dir != NULL)
	{
		char disabled_pkg_list[PATH_MAX] = {0,};
		int disabled_pkg_count = 0;

		while ((entry = readdir(dir)) != NULL)
		{
			char* package_id = entry->d_name;
			if (package_id != NULL)
			{
				if (!strcmp(package_id, ".") || !strcmp(package_id, ".."))
				{
					continue;
				}

				_LOG("dir = [%s]", package_id);

				bool rpm_disable = false;
				bool wgt_disable = false;
				char feature_xml_path[PATH_MAX + 1] = {0,};
				strncpy(feature_xml_path, root_path, PATH_MAX);
				strncat(feature_xml_path, package_id, PATH_MAX - strlen(root_path));
				strncat(feature_xml_path, "/res/features.xml",
						PATH_MAX - strlen(root_path) - strlen(package_id));

				xmlDocPtr doc = capability_manager_xml_util_get_doc(feature_xml_path);
				if (doc)
				{
					rpm_disable = capability_manager_xml_util_has_app_required_feature(doc);

					xmlFreeDoc(doc);
					doc = NULL;
				}

				char wgt_feature_xml_path[PATH_MAX + 1] = {0,};
				strncpy(wgt_feature_xml_path, root_path, PATH_MAX);
				strncat(wgt_feature_xml_path, package_id, PATH_MAX - strlen(root_path));
				strncat(wgt_feature_xml_path, "/res/wgt/res/features.xml",
						PATH_MAX - strlen(root_path) - strlen(package_id));

				doc = capability_manager_xml_util_get_doc(feature_xml_path);
				if (doc)
				{
					wgt_disable = capability_manager_xml_util_has_app_required_feature(doc);

					xmlFreeDoc(doc);
					doc = NULL;
				}

				if ((rpm_disable == true) || (wgt_disable == true))
				{
					bool update = false;
					if (strcmp(root_path, "/usr/apps/") == 0)
					{
						char download_feature_xml_path[1024] = {0,};
						strcpy(download_feature_xml_path, "/opt/usr/apps/");
						strcat(download_feature_xml_path, package_id);
						strcat(download_feature_xml_path, "/res/features.xml");

						char download_wgt_feature_xml_path[1024] = {0,};
						strcpy(download_wgt_feature_xml_path, "/opt/usr/apps/");
						strcat(download_wgt_feature_xml_path, package_id);
						strcat(download_wgt_feature_xml_path, "/res/wgt/res/features.xml");

						if ((is_file_exist(download_feature_xml_path) == TRUE)
								|| (is_file_exist(download_wgt_feature_xml_path) == TRUE))
						{
							_LOG("package[%s] is updated.", package_id);
							update = true;
						}
					}

					if (update == false)
					{
						disabled_pkg_count++;

						if (strlen(disabled_pkg_list) != 0)
						{
							strncat(disabled_pkg_list, ":", 1);
						}
						strncat(disabled_pkg_list, package_id, strlen(package_id));
						_LOG("disabled pkg list : %s", disabled_pkg_list);
					}
				}
			}
			else
			{
				_LOG("package_id is NULL.");
			}
		}
		closedir(dir);
		dir = NULL;

		int ret = pkgmgr_client_disable_pkg(disabled_pkg_list);
		if (ret != 0)
		{
			_LOG("pkgmgr_client_deactivate failed.");
		}
		else
		{
			_LOG("pkgmgr_client_deactivate success. count[%d]", disabled_pkg_count);
		}
	}
	else
	{
		_LOG("opendir(%s) failed.", root_path);
	}
}

bool
_disable_package(const char* package_id)
{
	bool res = true;

	pkgmgr_client* pc = pkgmgr_client_new(PC_REQUEST);
	TryReturn(pc, false, "pkgmgr_client_new(PC_REQUEST) failed.");

	int ret = pkgmgr_client_deactivate(pc, NULL, package_id);
	if (ret != 0)
	{
		_LOG("pkgmgr_client_deactivate(%s) failed.", package_id);
		res = false;
	}
	else
	{
		_LOG("pkgmgr_client_deactivate(%s) success", package_id);
	}

	pkgmgr_client_free(pc);

	return res;
}
