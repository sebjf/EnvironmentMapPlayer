/**\file */
#ifndef SLIC_DECLARATIONS_EnvironmentMapPlayer1_H
#define SLIC_DECLARATIONS_EnvironmentMapPlayer1_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EnvironmentMapPlayer1_DisplayTotalWidth (1138)
#define EnvironmentMapPlayer1_DisplayActiveHeight (1920)
#define EnvironmentMapPlayer1_DisplayActiveWidth (1080)
#define EnvironmentMapPlayer1_DisplayTotalHeight (1933)


/*----------------------------------------------------------------------------*/
/*---------------------- Interface memoryInitialisation ----------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'memoryInitialisation'.
 * 
 * \param [in] param_address Interface Parameter "address".
 * \param [in] param_size Interface Parameter "size".
 * \param [in] instream_environment_map_in The stream should be of size param_size bytes.
 */
void EnvironmentMapPlayer1_memoryInitialisation(
	int64_t param_address,
	int64_t param_size,
	const uint8_t *instream_environment_map_in);

/**
 * \brief Basic static non-blocking function for the interface 'memoryInitialisation'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] param_address Interface Parameter "address".
 * \param [in] param_size Interface Parameter "size".
 * \param [in] instream_environment_map_in The stream should be of size param_size bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_memoryInitialisation_nonblock(
	int64_t param_address,
	int64_t param_size,
	const uint8_t *instream_environment_map_in);

/**
 * \brief Advanced static interface, structure for the engine interface 'memoryInitialisation'
 * 
 */
typedef struct { 
	int64_t param_address; /**<  [in] Interface Parameter "address". */
	int64_t param_size; /**<  [in] Interface Parameter "size". */
	const uint8_t *instream_environment_map_in; /**<  [in] The stream should be of size param_size bytes. */
} EnvironmentMapPlayer1_memoryInitialisation_actions_t;

/**
 * \brief Advanced static function for the interface 'memoryInitialisation'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void EnvironmentMapPlayer1_memoryInitialisation_run(
	max_engine_t *engine,
	EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'memoryInitialisation'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_memoryInitialisation_run_nonblock(
	max_engine_t *engine,
	EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'memoryInitialisation'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void EnvironmentMapPlayer1_memoryInitialisation_run_group(max_group_t *group, EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'memoryInitialisation'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_memoryInitialisation_run_group_nonblock(max_group_t *group, EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'memoryInitialisation'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void EnvironmentMapPlayer1_memoryInitialisation_run_array(max_engarray_t *engarray, EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'memoryInitialisation'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_memoryInitialisation_run_array_nonblock(max_engarray_t *engarray, EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* EnvironmentMapPlayer1_memoryInitialisation_convert(max_file_t *maxfile, EnvironmentMapPlayer1_memoryInitialisation_actions_t *interface_actions);



/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_num_banks_used Input scalar parameter "MapSampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_start_bank_num Input scalar parameter "MapSampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_MapSampleReaderKernel_backgroundColour Input scalar parameter "MapSampleReaderKernel.backgroundColour".
 * \param [in] inscalar_MaxVideoSignalKernel_HSyncPolarity Input scalar parameter "MaxVideoSignalKernel.HSyncPolarity".
 * \param [in] inscalar_MaxVideoSignalKernel_VSyncPolarity Input scalar parameter "MaxVideoSignalKernel.VSyncPolarity".
 * \param [in] inscalar_RayCasterKernel_ipd Input scalar parameter "RayCasterKernel.ipd".
 * \param [in] inscalar_RayCasterKernel_viewplane_hres Input scalar parameter "RayCasterKernel.viewplane_hres".
 * \param [in] inscalar_RayCasterKernel_viewplane_pixelsize Input scalar parameter "RayCasterKernel.viewplane_pixelsize".
 * \param [in] inscalar_RayCasterKernel_viewplane_viewdistance Input scalar parameter "RayCasterKernel.viewplane_viewdistance".
 * \param [in] inscalar_RayCasterKernel_viewplane_vres Input scalar parameter "RayCasterKernel.viewplane_vres".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_num_banks_used Input scalar parameter "RaySampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress Input scalar parameter "RaySampleCommandGeneratorKernel.sampleParameterMapAddress".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_start_bank_num Input scalar parameter "RaySampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_RaySampleReaderKernel_sampleParameterMapAddress Input scalar parameter "RaySampleReaderKernel.sampleParameterMapAddress".
 * \param [in] instream_cameraUpdates Stream "cameraUpdates".
 * \param [in] instream_size_cameraUpdates The size of the stream instream_cameraUpdates in bytes.
 * \param [in] instream_environment_map_in Stream "environment_map_in".
 * \param [in] instream_size_environment_map_in The size of the stream instream_environment_map_in in bytes.
 * \param [in] lmem_address_environment_map Linear LMem control for "environment_map" stream: base address, in bytes.
 * \param [in] lmem_arr_size_environment_map Linear LMem control for "environment_map" stream: array size, in bytes.
 */
void EnvironmentMapPlayer1(
	uint64_t inscalar_MapSampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_MapSampleReaderKernel_backgroundColour,
	uint64_t inscalar_MaxVideoSignalKernel_HSyncPolarity,
	uint64_t inscalar_MaxVideoSignalKernel_VSyncPolarity,
	double inscalar_RayCasterKernel_ipd,
	double inscalar_RayCasterKernel_viewplane_hres,
	double inscalar_RayCasterKernel_viewplane_pixelsize,
	double inscalar_RayCasterKernel_viewplane_viewdistance,
	double inscalar_RayCasterKernel_viewplane_vres,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_RaySampleReaderKernel_sampleParameterMapAddress,
	const void *instream_cameraUpdates,
	size_t instream_size_cameraUpdates,
	const void *instream_environment_map_in,
	size_t instream_size_environment_map_in,
	size_t lmem_address_environment_map,
	size_t lmem_arr_size_environment_map);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_num_banks_used Input scalar parameter "MapSampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_start_bank_num Input scalar parameter "MapSampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_MapSampleReaderKernel_backgroundColour Input scalar parameter "MapSampleReaderKernel.backgroundColour".
 * \param [in] inscalar_MaxVideoSignalKernel_HSyncPolarity Input scalar parameter "MaxVideoSignalKernel.HSyncPolarity".
 * \param [in] inscalar_MaxVideoSignalKernel_VSyncPolarity Input scalar parameter "MaxVideoSignalKernel.VSyncPolarity".
 * \param [in] inscalar_RayCasterKernel_ipd Input scalar parameter "RayCasterKernel.ipd".
 * \param [in] inscalar_RayCasterKernel_viewplane_hres Input scalar parameter "RayCasterKernel.viewplane_hres".
 * \param [in] inscalar_RayCasterKernel_viewplane_pixelsize Input scalar parameter "RayCasterKernel.viewplane_pixelsize".
 * \param [in] inscalar_RayCasterKernel_viewplane_viewdistance Input scalar parameter "RayCasterKernel.viewplane_viewdistance".
 * \param [in] inscalar_RayCasterKernel_viewplane_vres Input scalar parameter "RayCasterKernel.viewplane_vres".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_num_banks_used Input scalar parameter "RaySampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress Input scalar parameter "RaySampleCommandGeneratorKernel.sampleParameterMapAddress".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_start_bank_num Input scalar parameter "RaySampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_RaySampleReaderKernel_sampleParameterMapAddress Input scalar parameter "RaySampleReaderKernel.sampleParameterMapAddress".
 * \param [in] instream_cameraUpdates Stream "cameraUpdates".
 * \param [in] instream_size_cameraUpdates The size of the stream instream_cameraUpdates in bytes.
 * \param [in] instream_environment_map_in Stream "environment_map_in".
 * \param [in] instream_size_environment_map_in The size of the stream instream_environment_map_in in bytes.
 * \param [in] lmem_address_environment_map Linear LMem control for "environment_map" stream: base address, in bytes.
 * \param [in] lmem_arr_size_environment_map Linear LMem control for "environment_map" stream: array size, in bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_nonblock(
	uint64_t inscalar_MapSampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_MapSampleReaderKernel_backgroundColour,
	uint64_t inscalar_MaxVideoSignalKernel_HSyncPolarity,
	uint64_t inscalar_MaxVideoSignalKernel_VSyncPolarity,
	double inscalar_RayCasterKernel_ipd,
	double inscalar_RayCasterKernel_viewplane_hres,
	double inscalar_RayCasterKernel_viewplane_pixelsize,
	double inscalar_RayCasterKernel_viewplane_viewdistance,
	double inscalar_RayCasterKernel_viewplane_vres,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_RaySampleReaderKernel_sampleParameterMapAddress,
	const void *instream_cameraUpdates,
	size_t instream_size_cameraUpdates,
	const void *instream_environment_map_in,
	size_t instream_size_environment_map_in,
	size_t lmem_address_environment_map,
	size_t lmem_arr_size_environment_map);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	uint64_t inscalar_MapSampleCommandGeneratorKernel_num_banks_used; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.num_banks_used". */
	uint64_t inscalar_MapSampleCommandGeneratorKernel_start_bank_num; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.start_bank_num". */
	uint64_t inscalar_MapSampleReaderKernel_backgroundColour; /**<  [in] Input scalar parameter "MapSampleReaderKernel.backgroundColour". */
	uint64_t inscalar_MaxVideoSignalKernel_HSyncPolarity; /**<  [in] Input scalar parameter "MaxVideoSignalKernel.HSyncPolarity". */
	uint64_t inscalar_MaxVideoSignalKernel_VSyncPolarity; /**<  [in] Input scalar parameter "MaxVideoSignalKernel.VSyncPolarity". */
	double inscalar_RayCasterKernel_ipd; /**<  [in] Input scalar parameter "RayCasterKernel.ipd". */
	double inscalar_RayCasterKernel_viewplane_hres; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_hres". */
	double inscalar_RayCasterKernel_viewplane_pixelsize; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_pixelsize". */
	double inscalar_RayCasterKernel_viewplane_viewdistance; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_viewdistance". */
	double inscalar_RayCasterKernel_viewplane_vres; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_vres". */
	uint64_t inscalar_RaySampleCommandGeneratorKernel_num_banks_used; /**<  [in] Input scalar parameter "RaySampleCommandGeneratorKernel.num_banks_used". */
	uint64_t inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress; /**<  [in] Input scalar parameter "RaySampleCommandGeneratorKernel.sampleParameterMapAddress". */
	uint64_t inscalar_RaySampleCommandGeneratorKernel_start_bank_num; /**<  [in] Input scalar parameter "RaySampleCommandGeneratorKernel.start_bank_num". */
	uint64_t inscalar_RaySampleReaderKernel_sampleParameterMapAddress; /**<  [in] Input scalar parameter "RaySampleReaderKernel.sampleParameterMapAddress". */
	const void *instream_cameraUpdates; /**<  [in] Stream "cameraUpdates". */
	size_t instream_size_cameraUpdates; /**<  [in] The size of the stream instream_cameraUpdates in bytes. */
	const void *instream_environment_map_in; /**<  [in] Stream "environment_map_in". */
	size_t instream_size_environment_map_in; /**<  [in] The size of the stream instream_environment_map_in in bytes. */
	size_t lmem_address_environment_map; /**<  [in] Linear LMem control for "environment_map" stream: base address, in bytes. */
	size_t lmem_arr_size_environment_map; /**<  [in] Linear LMem control for "environment_map" stream: array size, in bytes. */
} EnvironmentMapPlayer1_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void EnvironmentMapPlayer1_run(
	max_engine_t *engine,
	EnvironmentMapPlayer1_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'default'.
 *
 * Schedule the actions to run on the engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in] interface_actions Actions to be executed.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_run_nonblock(
	max_engine_t *engine,
	EnvironmentMapPlayer1_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void EnvironmentMapPlayer1_run_group(max_group_t *group, EnvironmentMapPlayer1_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule the actions to run on the first device available in the group and return immediately.
 * The status of the run must be checked with ::max_wait. 
 * Note that use of ::max_nowait is prohibited with non-blocking running on groups:
 * see the ::max_run_group_nonblock documentation for more explanation.
 *
 * \param [in] group Group to use.
 * \param [in] interface_actions Actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_run_group_nonblock(max_group_t *group, EnvironmentMapPlayer1_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void EnvironmentMapPlayer1_run_array(max_engarray_t *engarray, EnvironmentMapPlayer1_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'default'.
 * 
 *
 * Schedule to run the array of actions on the array of engines, and return immediately.
 * The length of interface_actions must match the size of engarray.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 *
 * \param [in] engarray The array of devices to use.
 * \param [in] interface_actions The array of actions to run.
 * \return A handle on the execution status of the actions, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer1_run_array_nonblock(max_engarray_t *engarray, EnvironmentMapPlayer1_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* EnvironmentMapPlayer1_convert(max_file_t *maxfile, EnvironmentMapPlayer1_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* EnvironmentMapPlayer1_init(void);

/* Error handling functions */
int EnvironmentMapPlayer1_has_errors(void);
const char* EnvironmentMapPlayer1_get_errors(void);
void EnvironmentMapPlayer1_clear_errors(void);
/* Free statically allocated maxfile data */
void EnvironmentMapPlayer1_free(void);
/* These are dummy functions for hardware builds. */
int EnvironmentMapPlayer1_simulator_start(void);
int EnvironmentMapPlayer1_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_EnvironmentMapPlayer1_H */

