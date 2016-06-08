/**\file */
#ifndef SLIC_DECLARATIONS_EnvironmentMapPlayer_H
#define SLIC_DECLARATIONS_EnvironmentMapPlayer_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EnvironmentMapPlayer_DisplayActiveWidth (1080)
#define EnvironmentMapPlayer_DisplayActiveHeight (1920)
#define EnvironmentMapPlayer_DisplayTotalHeight (1933)
#define EnvironmentMapPlayer_DisplayTotalWidth (1138)
#define EnvironmentMapPlayer_PCIE_ALIGNMENT (16)


/*----------------------------------------------------------------------------*/
/*----------------- Interface rayParameterMap_initialisation -----------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'rayParameterMap_initialisation'.
 * 
 * \param [in] param_address Interface Parameter "address".
 * \param [in] param_size Interface Parameter "size".
 * \param [in] instream_rayParameterMap_fromCPU The stream should be of size param_size bytes.
 */
void EnvironmentMapPlayer_rayParameterMap_initialisation(
	int64_t param_address,
	int64_t param_size,
	const uint8_t *instream_rayParameterMap_fromCPU);

/**
 * \brief Basic static non-blocking function for the interface 'rayParameterMap_initialisation'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] param_address Interface Parameter "address".
 * \param [in] param_size Interface Parameter "size".
 * \param [in] instream_rayParameterMap_fromCPU The stream should be of size param_size bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer_rayParameterMap_initialisation_nonblock(
	int64_t param_address,
	int64_t param_size,
	const uint8_t *instream_rayParameterMap_fromCPU);

/**
 * \brief Advanced static interface, structure for the engine interface 'rayParameterMap_initialisation'
 * 
 */
typedef struct { 
	int64_t param_address; /**<  [in] Interface Parameter "address". */
	int64_t param_size; /**<  [in] Interface Parameter "size". */
	const uint8_t *instream_rayParameterMap_fromCPU; /**<  [in] The stream should be of size param_size bytes. */
} EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t;

/**
 * \brief Advanced static function for the interface 'rayParameterMap_initialisation'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void EnvironmentMapPlayer_rayParameterMap_initialisation_run(
	max_engine_t *engine,
	EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'rayParameterMap_initialisation'.
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
max_run_t *EnvironmentMapPlayer_rayParameterMap_initialisation_run_nonblock(
	max_engine_t *engine,
	EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'rayParameterMap_initialisation'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void EnvironmentMapPlayer_rayParameterMap_initialisation_run_group(max_group_t *group, EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'rayParameterMap_initialisation'.
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
max_run_t *EnvironmentMapPlayer_rayParameterMap_initialisation_run_group_nonblock(max_group_t *group, EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'rayParameterMap_initialisation'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void EnvironmentMapPlayer_rayParameterMap_initialisation_run_array(max_engarray_t *engarray, EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'rayParameterMap_initialisation'.
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
max_run_t *EnvironmentMapPlayer_rayParameterMap_initialisation_run_array_nonblock(max_engarray_t *engarray, EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* EnvironmentMapPlayer_rayParameterMap_initialisation_convert(max_file_t *maxfile, EnvironmentMapPlayer_rayParameterMap_initialisation_actions_t *interface_actions);



/*----------------------------------------------------------------------------*/
/*-------------------- Interface sampleMap_initialisation --------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'sampleMap_initialisation'.
 * 
 * \param [in] param_address Interface Parameter "address".
 * \param [in] param_size Interface Parameter "size".
 * \param [in] instream_sampleMap_fromCPU The stream should be of size param_size bytes.
 */
void EnvironmentMapPlayer_sampleMap_initialisation(
	int64_t param_address,
	int64_t param_size,
	const uint8_t *instream_sampleMap_fromCPU);

/**
 * \brief Basic static non-blocking function for the interface 'sampleMap_initialisation'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] param_address Interface Parameter "address".
 * \param [in] param_size Interface Parameter "size".
 * \param [in] instream_sampleMap_fromCPU The stream should be of size param_size bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer_sampleMap_initialisation_nonblock(
	int64_t param_address,
	int64_t param_size,
	const uint8_t *instream_sampleMap_fromCPU);

/**
 * \brief Advanced static interface, structure for the engine interface 'sampleMap_initialisation'
 * 
 */
typedef struct { 
	int64_t param_address; /**<  [in] Interface Parameter "address". */
	int64_t param_size; /**<  [in] Interface Parameter "size". */
	const uint8_t *instream_sampleMap_fromCPU; /**<  [in] The stream should be of size param_size bytes. */
} EnvironmentMapPlayer_sampleMap_initialisation_actions_t;

/**
 * \brief Advanced static function for the interface 'sampleMap_initialisation'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void EnvironmentMapPlayer_sampleMap_initialisation_run(
	max_engine_t *engine,
	EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'sampleMap_initialisation'.
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
max_run_t *EnvironmentMapPlayer_sampleMap_initialisation_run_nonblock(
	max_engine_t *engine,
	EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'sampleMap_initialisation'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void EnvironmentMapPlayer_sampleMap_initialisation_run_group(max_group_t *group, EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'sampleMap_initialisation'.
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
max_run_t *EnvironmentMapPlayer_sampleMap_initialisation_run_group_nonblock(max_group_t *group, EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'sampleMap_initialisation'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void EnvironmentMapPlayer_sampleMap_initialisation_run_array(max_engarray_t *engarray, EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'sampleMap_initialisation'.
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
max_run_t *EnvironmentMapPlayer_sampleMap_initialisation_run_array_nonblock(max_engarray_t *engarray, EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* EnvironmentMapPlayer_sampleMap_initialisation_convert(max_file_t *maxfile, EnvironmentMapPlayer_sampleMap_initialisation_actions_t *interface_actions);



/*----------------------------------------------------------------------------*/
/*------------------- Interface primitives_initialisation --------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'primitives_initialisation'.
 * 
 * \param [in] instream_primitivesStream The stream should be of size 1152 bytes.
 */
void EnvironmentMapPlayer_primitives_initialisation(
	const uint8_t *instream_primitivesStream);

/**
 * \brief Basic static non-blocking function for the interface 'primitives_initialisation'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] instream_primitivesStream The stream should be of size 1152 bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer_primitives_initialisation_nonblock(
	const uint8_t *instream_primitivesStream);

/**
 * \brief Advanced static interface, structure for the engine interface 'primitives_initialisation'
 * 
 */
typedef struct { 
	const uint8_t *instream_primitivesStream; /**<  [in] The stream should be of size 1152 bytes. */
} EnvironmentMapPlayer_primitives_initialisation_actions_t;

/**
 * \brief Advanced static function for the interface 'primitives_initialisation'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void EnvironmentMapPlayer_primitives_initialisation_run(
	max_engine_t *engine,
	EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions);

/**
 * \brief Advanced static non-blocking function for the interface 'primitives_initialisation'.
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
max_run_t *EnvironmentMapPlayer_primitives_initialisation_run_nonblock(
	max_engine_t *engine,
	EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'primitives_initialisation'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void EnvironmentMapPlayer_primitives_initialisation_run_group(max_group_t *group, EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions);

/**
 * \brief Group run advanced static non-blocking function for the interface 'primitives_initialisation'.
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
max_run_t *EnvironmentMapPlayer_primitives_initialisation_run_group_nonblock(max_group_t *group, EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'primitives_initialisation'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void EnvironmentMapPlayer_primitives_initialisation_run_array(max_engarray_t *engarray, EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions[]);

/**
 * \brief Array run advanced static non-blocking function for the interface 'primitives_initialisation'.
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
max_run_t *EnvironmentMapPlayer_primitives_initialisation_run_array_nonblock(max_engarray_t *engarray, EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* EnvironmentMapPlayer_primitives_initialisation_convert(max_file_t *maxfile, EnvironmentMapPlayer_primitives_initialisation_actions_t *interface_actions);



/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_bank_address_bits_offset Input scalar parameter "MapSampleCommandGeneratorKernel.bank_address_bits_offset".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_max_mip_level Input scalar parameter "MapSampleCommandGeneratorKernel.max_mip_level".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_min_mip_level Input scalar parameter "MapSampleCommandGeneratorKernel.min_mip_level".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_num_banks_used Input scalar parameter "MapSampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_start_bank_num Input scalar parameter "MapSampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_MapSampleReaderKernel_backgroundColour Input scalar parameter "MapSampleReaderKernel.backgroundColour".
 * \param [in] inscalar_MaxVideoSignalKernel_HSyncPolarity Input scalar parameter "MaxVideoSignalKernel.HSyncPolarity".
 * \param [in] inscalar_MaxVideoSignalKernel_VSyncPolarity Input scalar parameter "MaxVideoSignalKernel.VSyncPolarity".
 * \param [in] inscalar_RayCasterKernel_ipd Input scalar parameter "RayCasterKernel.ipd".
 * \param [in] inscalar_RayCasterKernel_viewplane_hres Input scalar parameter "RayCasterKernel.viewplane_hres".
 * \param [in] inscalar_RayCasterKernel_viewplane_pixelsize_h Input scalar parameter "RayCasterKernel.viewplane_pixelsize_h".
 * \param [in] inscalar_RayCasterKernel_viewplane_pixelsize_v Input scalar parameter "RayCasterKernel.viewplane_pixelsize_v".
 * \param [in] inscalar_RayCasterKernel_viewplane_viewdistance Input scalar parameter "RayCasterKernel.viewplane_viewdistance".
 * \param [in] inscalar_RayCasterKernel_viewplane_vres Input scalar parameter "RayCasterKernel.viewplane_vres".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_num_banks_used Input scalar parameter "RaySampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress Input scalar parameter "RaySampleCommandGeneratorKernel.sampleParameterMapAddress".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_start_bank_num Input scalar parameter "RaySampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_RaySampleReaderKernel_sampleParameterMapAddress Input scalar parameter "RaySampleReaderKernel.sampleParameterMapAddress".
 * \param [in] inscalar_rayParameterMap_toMem_addrGen_numBursts Input scalar parameter "rayParameterMap_toMem_addrGen.numBursts".
 * \param [in] inscalar_rayParameterMap_toMem_addrGen_numRepeats Input scalar parameter "rayParameterMap_toMem_addrGen.numRepeats".
 * \param [in] inscalar_rayParameterMap_toMem_addrGen_startAddress Input scalar parameter "rayParameterMap_toMem_addrGen.startAddress".
 * \param [in] inscalar_sampleMapDimm1_toMem_addrGen_numBursts Input scalar parameter "sampleMapDimm1_toMem_addrGen.numBursts".
 * \param [in] inscalar_sampleMapDimm1_toMem_addrGen_numRepeats Input scalar parameter "sampleMapDimm1_toMem_addrGen.numRepeats".
 * \param [in] inscalar_sampleMapDimm1_toMem_addrGen_startAddress Input scalar parameter "sampleMapDimm1_toMem_addrGen.startAddress".
 * \param [in] inscalar_sampleMapDimm2_toMem_addrGen_numBursts Input scalar parameter "sampleMapDimm2_toMem_addrGen.numBursts".
 * \param [in] inscalar_sampleMapDimm2_toMem_addrGen_numRepeats Input scalar parameter "sampleMapDimm2_toMem_addrGen.numRepeats".
 * \param [in] inscalar_sampleMapDimm2_toMem_addrGen_startAddress Input scalar parameter "sampleMapDimm2_toMem_addrGen.startAddress".
 * \param [in] instream_cameraUpdates Stream "cameraUpdates".
 * \param [in] instream_size_cameraUpdates The size of the stream instream_cameraUpdates in bytes.
 * \param [in] instream_coefficients Stream "coefficients".
 * \param [in] instream_size_coefficients The size of the stream instream_coefficients in bytes.
 * \param [in] instream_primitivesStream Stream "primitivesStream".
 * \param [in] instream_size_primitivesStream The size of the stream instream_primitivesStream in bytes.
 * \param [in] instream_rayParameterMap_fromCPU Stream "rayParameterMap_fromCPU".
 * \param [in] instream_size_rayParameterMap_fromCPU The size of the stream instream_rayParameterMap_fromCPU in bytes.
 * \param [in] instream_sampleMap_fromCPU Stream "sampleMap_fromCPU".
 * \param [in] instream_size_sampleMap_fromCPU The size of the stream instream_sampleMap_fromCPU in bytes.
 * \param [out] outstream_displayDataOut Stream "displayDataOut".
 * \param [in] outstream_size_displayDataOut The size of the stream outstream_displayDataOut in bytes.
 * \param [in] inmem_RayCasterKernel_alphaMapSlot0 Mapped ROM inmem_RayCasterKernel_alphaMapSlot0, should be of size (16384 * sizeof(uint64_t)).
 * \param [in] inmem_RayCasterKernel_alphaMapSlot1 Mapped ROM inmem_RayCasterKernel_alphaMapSlot1, should be of size (16384 * sizeof(uint64_t)).
 * \param [in] routing_string A string containing comma-separated "from_name -> to_name" routing commands.
 */
void EnvironmentMapPlayer(
	uint64_t inscalar_MapSampleCommandGeneratorKernel_bank_address_bits_offset,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_max_mip_level,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_min_mip_level,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_MapSampleReaderKernel_backgroundColour,
	uint64_t inscalar_MaxVideoSignalKernel_HSyncPolarity,
	uint64_t inscalar_MaxVideoSignalKernel_VSyncPolarity,
	double inscalar_RayCasterKernel_ipd,
	double inscalar_RayCasterKernel_viewplane_hres,
	double inscalar_RayCasterKernel_viewplane_pixelsize_h,
	double inscalar_RayCasterKernel_viewplane_pixelsize_v,
	double inscalar_RayCasterKernel_viewplane_viewdistance,
	double inscalar_RayCasterKernel_viewplane_vres,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_RaySampleReaderKernel_sampleParameterMapAddress,
	uint64_t inscalar_rayParameterMap_toMem_addrGen_numBursts,
	uint64_t inscalar_rayParameterMap_toMem_addrGen_numRepeats,
	uint64_t inscalar_rayParameterMap_toMem_addrGen_startAddress,
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_numBursts,
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_numRepeats,
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_startAddress,
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_numBursts,
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_numRepeats,
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_startAddress,
	const void *instream_cameraUpdates,
	size_t instream_size_cameraUpdates,
	const void *instream_coefficients,
	size_t instream_size_coefficients,
	const void *instream_primitivesStream,
	size_t instream_size_primitivesStream,
	const void *instream_rayParameterMap_fromCPU,
	size_t instream_size_rayParameterMap_fromCPU,
	const void *instream_sampleMap_fromCPU,
	size_t instream_size_sampleMap_fromCPU,
	void *outstream_displayDataOut,
	size_t outstream_size_displayDataOut,
	const uint64_t *inmem_RayCasterKernel_alphaMapSlot0,
	const uint64_t *inmem_RayCasterKernel_alphaMapSlot1,
	const char * routing_string);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_bank_address_bits_offset Input scalar parameter "MapSampleCommandGeneratorKernel.bank_address_bits_offset".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_max_mip_level Input scalar parameter "MapSampleCommandGeneratorKernel.max_mip_level".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_min_mip_level Input scalar parameter "MapSampleCommandGeneratorKernel.min_mip_level".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_num_banks_used Input scalar parameter "MapSampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_MapSampleCommandGeneratorKernel_start_bank_num Input scalar parameter "MapSampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_MapSampleReaderKernel_backgroundColour Input scalar parameter "MapSampleReaderKernel.backgroundColour".
 * \param [in] inscalar_MaxVideoSignalKernel_HSyncPolarity Input scalar parameter "MaxVideoSignalKernel.HSyncPolarity".
 * \param [in] inscalar_MaxVideoSignalKernel_VSyncPolarity Input scalar parameter "MaxVideoSignalKernel.VSyncPolarity".
 * \param [in] inscalar_RayCasterKernel_ipd Input scalar parameter "RayCasterKernel.ipd".
 * \param [in] inscalar_RayCasterKernel_viewplane_hres Input scalar parameter "RayCasterKernel.viewplane_hres".
 * \param [in] inscalar_RayCasterKernel_viewplane_pixelsize_h Input scalar parameter "RayCasterKernel.viewplane_pixelsize_h".
 * \param [in] inscalar_RayCasterKernel_viewplane_pixelsize_v Input scalar parameter "RayCasterKernel.viewplane_pixelsize_v".
 * \param [in] inscalar_RayCasterKernel_viewplane_viewdistance Input scalar parameter "RayCasterKernel.viewplane_viewdistance".
 * \param [in] inscalar_RayCasterKernel_viewplane_vres Input scalar parameter "RayCasterKernel.viewplane_vres".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_num_banks_used Input scalar parameter "RaySampleCommandGeneratorKernel.num_banks_used".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress Input scalar parameter "RaySampleCommandGeneratorKernel.sampleParameterMapAddress".
 * \param [in] inscalar_RaySampleCommandGeneratorKernel_start_bank_num Input scalar parameter "RaySampleCommandGeneratorKernel.start_bank_num".
 * \param [in] inscalar_RaySampleReaderKernel_sampleParameterMapAddress Input scalar parameter "RaySampleReaderKernel.sampleParameterMapAddress".
 * \param [in] inscalar_rayParameterMap_toMem_addrGen_numBursts Input scalar parameter "rayParameterMap_toMem_addrGen.numBursts".
 * \param [in] inscalar_rayParameterMap_toMem_addrGen_numRepeats Input scalar parameter "rayParameterMap_toMem_addrGen.numRepeats".
 * \param [in] inscalar_rayParameterMap_toMem_addrGen_startAddress Input scalar parameter "rayParameterMap_toMem_addrGen.startAddress".
 * \param [in] inscalar_sampleMapDimm1_toMem_addrGen_numBursts Input scalar parameter "sampleMapDimm1_toMem_addrGen.numBursts".
 * \param [in] inscalar_sampleMapDimm1_toMem_addrGen_numRepeats Input scalar parameter "sampleMapDimm1_toMem_addrGen.numRepeats".
 * \param [in] inscalar_sampleMapDimm1_toMem_addrGen_startAddress Input scalar parameter "sampleMapDimm1_toMem_addrGen.startAddress".
 * \param [in] inscalar_sampleMapDimm2_toMem_addrGen_numBursts Input scalar parameter "sampleMapDimm2_toMem_addrGen.numBursts".
 * \param [in] inscalar_sampleMapDimm2_toMem_addrGen_numRepeats Input scalar parameter "sampleMapDimm2_toMem_addrGen.numRepeats".
 * \param [in] inscalar_sampleMapDimm2_toMem_addrGen_startAddress Input scalar parameter "sampleMapDimm2_toMem_addrGen.startAddress".
 * \param [in] instream_cameraUpdates Stream "cameraUpdates".
 * \param [in] instream_size_cameraUpdates The size of the stream instream_cameraUpdates in bytes.
 * \param [in] instream_coefficients Stream "coefficients".
 * \param [in] instream_size_coefficients The size of the stream instream_coefficients in bytes.
 * \param [in] instream_primitivesStream Stream "primitivesStream".
 * \param [in] instream_size_primitivesStream The size of the stream instream_primitivesStream in bytes.
 * \param [in] instream_rayParameterMap_fromCPU Stream "rayParameterMap_fromCPU".
 * \param [in] instream_size_rayParameterMap_fromCPU The size of the stream instream_rayParameterMap_fromCPU in bytes.
 * \param [in] instream_sampleMap_fromCPU Stream "sampleMap_fromCPU".
 * \param [in] instream_size_sampleMap_fromCPU The size of the stream instream_sampleMap_fromCPU in bytes.
 * \param [out] outstream_displayDataOut Stream "displayDataOut".
 * \param [in] outstream_size_displayDataOut The size of the stream outstream_displayDataOut in bytes.
 * \param [in] inmem_RayCasterKernel_alphaMapSlot0 Mapped ROM inmem_RayCasterKernel_alphaMapSlot0, should be of size (16384 * sizeof(uint64_t)).
 * \param [in] inmem_RayCasterKernel_alphaMapSlot1 Mapped ROM inmem_RayCasterKernel_alphaMapSlot1, should be of size (16384 * sizeof(uint64_t)).
 * \param [in] routing_string A string containing comma-separated "from_name -> to_name" routing commands.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer_nonblock(
	uint64_t inscalar_MapSampleCommandGeneratorKernel_bank_address_bits_offset,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_max_mip_level,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_min_mip_level,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_MapSampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_MapSampleReaderKernel_backgroundColour,
	uint64_t inscalar_MaxVideoSignalKernel_HSyncPolarity,
	uint64_t inscalar_MaxVideoSignalKernel_VSyncPolarity,
	double inscalar_RayCasterKernel_ipd,
	double inscalar_RayCasterKernel_viewplane_hres,
	double inscalar_RayCasterKernel_viewplane_pixelsize_h,
	double inscalar_RayCasterKernel_viewplane_pixelsize_v,
	double inscalar_RayCasterKernel_viewplane_viewdistance,
	double inscalar_RayCasterKernel_viewplane_vres,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_num_banks_used,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress,
	uint64_t inscalar_RaySampleCommandGeneratorKernel_start_bank_num,
	uint64_t inscalar_RaySampleReaderKernel_sampleParameterMapAddress,
	uint64_t inscalar_rayParameterMap_toMem_addrGen_numBursts,
	uint64_t inscalar_rayParameterMap_toMem_addrGen_numRepeats,
	uint64_t inscalar_rayParameterMap_toMem_addrGen_startAddress,
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_numBursts,
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_numRepeats,
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_startAddress,
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_numBursts,
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_numRepeats,
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_startAddress,
	const void *instream_cameraUpdates,
	size_t instream_size_cameraUpdates,
	const void *instream_coefficients,
	size_t instream_size_coefficients,
	const void *instream_primitivesStream,
	size_t instream_size_primitivesStream,
	const void *instream_rayParameterMap_fromCPU,
	size_t instream_size_rayParameterMap_fromCPU,
	const void *instream_sampleMap_fromCPU,
	size_t instream_size_sampleMap_fromCPU,
	void *outstream_displayDataOut,
	size_t outstream_size_displayDataOut,
	const uint64_t *inmem_RayCasterKernel_alphaMapSlot0,
	const uint64_t *inmem_RayCasterKernel_alphaMapSlot1,
	const char * routing_string);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	uint64_t inscalar_MapSampleCommandGeneratorKernel_bank_address_bits_offset; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.bank_address_bits_offset". */
	uint64_t inscalar_MapSampleCommandGeneratorKernel_max_mip_level; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.max_mip_level". */
	uint64_t inscalar_MapSampleCommandGeneratorKernel_min_mip_level; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.min_mip_level". */
	uint64_t inscalar_MapSampleCommandGeneratorKernel_num_banks_used; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.num_banks_used". */
	uint64_t inscalar_MapSampleCommandGeneratorKernel_start_bank_num; /**<  [in] Input scalar parameter "MapSampleCommandGeneratorKernel.start_bank_num". */
	uint64_t inscalar_MapSampleReaderKernel_backgroundColour; /**<  [in] Input scalar parameter "MapSampleReaderKernel.backgroundColour". */
	uint64_t inscalar_MaxVideoSignalKernel_HSyncPolarity; /**<  [in] Input scalar parameter "MaxVideoSignalKernel.HSyncPolarity". */
	uint64_t inscalar_MaxVideoSignalKernel_VSyncPolarity; /**<  [in] Input scalar parameter "MaxVideoSignalKernel.VSyncPolarity". */
	double inscalar_RayCasterKernel_ipd; /**<  [in] Input scalar parameter "RayCasterKernel.ipd". */
	double inscalar_RayCasterKernel_viewplane_hres; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_hres". */
	double inscalar_RayCasterKernel_viewplane_pixelsize_h; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_pixelsize_h". */
	double inscalar_RayCasterKernel_viewplane_pixelsize_v; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_pixelsize_v". */
	double inscalar_RayCasterKernel_viewplane_viewdistance; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_viewdistance". */
	double inscalar_RayCasterKernel_viewplane_vres; /**<  [in] Input scalar parameter "RayCasterKernel.viewplane_vres". */
	uint64_t inscalar_RaySampleCommandGeneratorKernel_num_banks_used; /**<  [in] Input scalar parameter "RaySampleCommandGeneratorKernel.num_banks_used". */
	uint64_t inscalar_RaySampleCommandGeneratorKernel_sampleParameterMapAddress; /**<  [in] Input scalar parameter "RaySampleCommandGeneratorKernel.sampleParameterMapAddress". */
	uint64_t inscalar_RaySampleCommandGeneratorKernel_start_bank_num; /**<  [in] Input scalar parameter "RaySampleCommandGeneratorKernel.start_bank_num". */
	uint64_t inscalar_RaySampleReaderKernel_sampleParameterMapAddress; /**<  [in] Input scalar parameter "RaySampleReaderKernel.sampleParameterMapAddress". */
	uint64_t inscalar_rayParameterMap_toMem_addrGen_numBursts; /**<  [in] Input scalar parameter "rayParameterMap_toMem_addrGen.numBursts". */
	uint64_t inscalar_rayParameterMap_toMem_addrGen_numRepeats; /**<  [in] Input scalar parameter "rayParameterMap_toMem_addrGen.numRepeats". */
	uint64_t inscalar_rayParameterMap_toMem_addrGen_startAddress; /**<  [in] Input scalar parameter "rayParameterMap_toMem_addrGen.startAddress". */
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_numBursts; /**<  [in] Input scalar parameter "sampleMapDimm1_toMem_addrGen.numBursts". */
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_numRepeats; /**<  [in] Input scalar parameter "sampleMapDimm1_toMem_addrGen.numRepeats". */
	uint64_t inscalar_sampleMapDimm1_toMem_addrGen_startAddress; /**<  [in] Input scalar parameter "sampleMapDimm1_toMem_addrGen.startAddress". */
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_numBursts; /**<  [in] Input scalar parameter "sampleMapDimm2_toMem_addrGen.numBursts". */
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_numRepeats; /**<  [in] Input scalar parameter "sampleMapDimm2_toMem_addrGen.numRepeats". */
	uint64_t inscalar_sampleMapDimm2_toMem_addrGen_startAddress; /**<  [in] Input scalar parameter "sampleMapDimm2_toMem_addrGen.startAddress". */
	const void *instream_cameraUpdates; /**<  [in] Stream "cameraUpdates". */
	size_t instream_size_cameraUpdates; /**<  [in] The size of the stream instream_cameraUpdates in bytes. */
	const void *instream_coefficients; /**<  [in] Stream "coefficients". */
	size_t instream_size_coefficients; /**<  [in] The size of the stream instream_coefficients in bytes. */
	const void *instream_primitivesStream; /**<  [in] Stream "primitivesStream". */
	size_t instream_size_primitivesStream; /**<  [in] The size of the stream instream_primitivesStream in bytes. */
	const void *instream_rayParameterMap_fromCPU; /**<  [in] Stream "rayParameterMap_fromCPU". */
	size_t instream_size_rayParameterMap_fromCPU; /**<  [in] The size of the stream instream_rayParameterMap_fromCPU in bytes. */
	const void *instream_sampleMap_fromCPU; /**<  [in] Stream "sampleMap_fromCPU". */
	size_t instream_size_sampleMap_fromCPU; /**<  [in] The size of the stream instream_sampleMap_fromCPU in bytes. */
	void *outstream_displayDataOut; /**<  [out] Stream "displayDataOut". */
	size_t outstream_size_displayDataOut; /**<  [in] The size of the stream outstream_displayDataOut in bytes. */
	const uint64_t *inmem_RayCasterKernel_alphaMapSlot0; /**<  [in] Mapped ROM inmem_RayCasterKernel_alphaMapSlot0, should be of size (16384 * sizeof(uint64_t)). */
	const uint64_t *inmem_RayCasterKernel_alphaMapSlot1; /**<  [in] Mapped ROM inmem_RayCasterKernel_alphaMapSlot1, should be of size (16384 * sizeof(uint64_t)). */
	const char * routing_string; /**<  [in] A string containing comma-separated "from_name -> to_name" routing commands. */
} EnvironmentMapPlayer_actions_t;

/**
 * \brief Advanced static function for the interface 'default'.
 * 
 * \param [in] engine The engine on which the actions will be executed.
 * \param [in,out] interface_actions Actions to be executed.
 */
void EnvironmentMapPlayer_run(
	max_engine_t *engine,
	EnvironmentMapPlayer_actions_t *interface_actions);

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
max_run_t *EnvironmentMapPlayer_run_nonblock(
	max_engine_t *engine,
	EnvironmentMapPlayer_actions_t *interface_actions);

/**
 * \brief Group run advanced static function for the interface 'default'.
 * 
 * \param [in] group Group to use.
 * \param [in,out] interface_actions Actions to run.
 *
 * Run the actions on the first device available in the group.
 */
void EnvironmentMapPlayer_run_group(max_group_t *group, EnvironmentMapPlayer_actions_t *interface_actions);

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
max_run_t *EnvironmentMapPlayer_run_group_nonblock(max_group_t *group, EnvironmentMapPlayer_actions_t *interface_actions);

/**
 * \brief Array run advanced static function for the interface 'default'.
 * 
 * \param [in] engarray The array of devices to use.
 * \param [in,out] interface_actions The array of actions to run.
 *
 * Run the array of actions on the array of engines.  The length of interface_actions
 * must match the size of engarray.
 */
void EnvironmentMapPlayer_run_array(max_engarray_t *engarray, EnvironmentMapPlayer_actions_t *interface_actions[]);

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
max_run_t *EnvironmentMapPlayer_run_array_nonblock(max_engarray_t *engarray, EnvironmentMapPlayer_actions_t *interface_actions[]);

/**
 * \brief Converts a static-interface action struct into a dynamic-interface max_actions_t struct.
 *
 * Note that this is an internal utility function used by other functions in the static interface.
 *
 * \param [in] maxfile The maxfile to use.
 * \param [in] interface_actions The interface-specific actions to run.
 * \return The dynamic-interface actions to run, or NULL in case of error.
 */
max_actions_t* EnvironmentMapPlayer_convert(max_file_t *maxfile, EnvironmentMapPlayer_actions_t *interface_actions);

/**
 * \brief Initialise a maxfile.
 */
max_file_t* EnvironmentMapPlayer_init(void);

/* Error handling functions */
int EnvironmentMapPlayer_has_errors(void);
const char* EnvironmentMapPlayer_get_errors(void);
void EnvironmentMapPlayer_clear_errors(void);
/* Free statically allocated maxfile data */
void EnvironmentMapPlayer_free(void);
/* returns: -1 = error running command; 0 = no error reported */
int EnvironmentMapPlayer_simulator_start(void);
/* returns: -1 = error running command; 0 = no error reported */
int EnvironmentMapPlayer_simulator_stop(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_EnvironmentMapPlayer_H */

