#ifndef CAPABILITY_MANAGER_H
#define CAPABILITY_MANAGER_H

#include <stdbool.h>
#include <tizen.h>

#include "bundle.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief device info handle.
 */
typedef void* device_info_h;

/**
 * @brief app info handle.
 */
typedef void* app_info_h;

/**
 * @brief Enumerations of error code
 */
typedef enum {
	CAPABILITY_MANAGER_ERROR_NONE = TIZEN_ERROR_NONE, /**< Successful */
	CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER, /**< Invalid parameter */
	CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY = TIZEN_ERROR_OUT_OF_MEMORY, /**< Out of memory */
	CAPABILITY_MANAGER_ERROR_IO_ERROR = TIZEN_ERROR_IO_ERROR, /**< Internal I/O error */
	CAPABILITY_MANAGER_ERROR_NO_SUCH_APP = TIZEN_ERROR_APPLICATION_CLASS | 0x01, /**< No such app */
	CAPABILITY_MANAGER_ERROR_DEVICE_NOT_CONNECTED, /**< Device is not connected */

} capability_manager_error_e;

/**
 * @brief Creates the device info handle.
 * @remarks device_info is set to NULL if an error occurs.
 * @param [out] device_info The device info handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CAPABILITY_MANAGER_ERROR_IO_ERROR IO error
 * @retval #CAPABILITY_MANAGER_ERROR_DEVICE_NOT_CONNECTED Device is not connected.
 * @see capability_manager_destroy_device_info()
 */
int capability_manager_create_device_info(device_info_h* device_info);

/**
 * @brief Destroys the device info handle and releases all its resources.
 * @param [in] device_info The device info handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see capability_manager_create_device_info()
 */
int capability_manager_destroy_device_info(device_info_h device_info);

/**
 * @brief Gets the id of device
 * @remarks device_id must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_id The id of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_id(device_info_h device_info, char** device_id);

/**
 * @brief Gets the name of device
 * @remarks device_name must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_name The name of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_name(device_info_h device_info, char** device_name);

/**
 * @brief Gets the platform of device
 * @remarks device_platform must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_platform The platform of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_platform(device_info_h device_info, char** device_platform);

/**
 * @brief Gets the platform version of device
 * @remarks device_platform_version must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_platform_version The platform version of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_platform_version(device_info_h device_info, char** device_platform_version);

/**
 * @brief Gets the type of device
 * @remarks device_type must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_type The type of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_type(device_info_h device_info, char** device_type);

/**
 * @brief Gets the model number of device
 * @remarks device_model_number must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_model_number The model number of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_model_number(device_info_h device_info, char** device_model_number);

/**
 * @brief Gets the software version of device
 * @remarks device_sw_version must be released with free().
 * @param [in] device_info The device info handle
 * @param [out] device_sw_version The software version of device
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_sw_version(device_info_h device_info, char** device_sw_version);

/**
 * @brief Gets the features of device
 * @remarks features must be released with bundle_free().
 * @param [in] device_info The device info handle
 * @param [out] features The feature list
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_device_features(device_info_h device_info, bundle** features);

/**
 * @brief Creates the app info handle.
 * @remarks app_info is set to NULL if an error occurs.
 * @param [in] package_name The package name
 * @param [out] app_info The app info handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #CAPABILITY_MANAGER_ERROR_IO_ERROR IO error
 * @retval #CAPABILITY_MANAGER_ERROR_NO_SUCH_APP The app is not installed.
 * @retval #CAPABILITY_MANAGER_ERROR_DEVICE_NOT_CONNECTED Device is not connected.
 * @see capability_manager_destroy_app_info()
 */
int capability_manager_create_app_info(const char* package_name, app_info_h* app_info);

/**
 * @brief Destroys the app info handle and releases all its resources.
 * @param [in] app_info The app info handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see capability_manager_create_app_info()
 */
int capability_manager_destroy_app_info(app_info_h app_info);

/**
 * @brief Gets the version of app
 * @remarks version must be released with free().
 * @param [in] app_info_h The app info handle
 * @param [out] version The version of app
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_app_version(app_info_h app_info, char** version);

/**
 * @brief Gets the features of app
 * @remarks features must be released with bundle_free().
 * @param [in] app_info_h The app info handle
 * @param [out] features The feature list
 * @return 0 on success, otherwise a negative error value.
 * @retval #CAPABILITY_MANAGER_ERROR_NONE Successful
 * @retval #CAPABILITY_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #CAPABILITY_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int capability_manager_get_app_features(app_info_h app_info, bundle** features);

/**
 * @brief Gets whether the app is installed or not
 * @param [in] package_name The package name
 * @param [out] installed Whether the app is installed
 * @return 0 on success, otherwise a negative error value.
 * @retval #PACKAGE_MANAGER_ERROR_NONE Successful
 * @retval #PACKAGE_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #PACKAGE_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 * @retval #PACKAGE_MANAGER_ERROR_IO_ERROR IO error
 */
int capability_manager_is_app_installed(const char* package_name, bool* installed);

#ifdef __cplusplus
}
#endif

#endif /* CAPABILITY_MANAGER_H */
