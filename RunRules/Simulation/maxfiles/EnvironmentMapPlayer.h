/**\file */
#ifndef SLIC_DECLARATIONS_EnvironmentMapPlayer_H
#define SLIC_DECLARATIONS_EnvironmentMapPlayer_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define EnvironmentMapPlayer_PCIE_ALIGNMENT (16)


/*----------------------------------------------------------------------------*/
/*---------------------------- Interface default -----------------------------*/
/*----------------------------------------------------------------------------*/




/**
 * \brief Basic static function for the interface 'default'.
 * 
 * \param [in] ticks_EnvironmentMapPlayerKernel The number of ticks for which kernel "EnvironmentMapPlayerKernel" will run.
 * \param [in] inscalar_EnvironmentMapPlayerKernel_camera_u Input scalar parameter "EnvironmentMapPlayerKernel.camera_u".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_camera_v Input scalar parameter "EnvironmentMapPlayerKernel.camera_v".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_camera_w Input scalar parameter "EnvironmentMapPlayerKernel.camera_w".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_hres Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_hres".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_pixelsize Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_pixelsize".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_viewdistance Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_viewdistance".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_vres Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_vres".
 * \param [in] instream_col Stream "col".
 * \param [in] instream_size_col The size of the stream instream_col in bytes.
 * \param [in] instream_row Stream "row".
 * \param [in] instream_size_row The size of the stream instream_row in bytes.
 * \param [out] outstream_d Stream "d".
 * \param [in] outstream_size_d The size of the stream outstream_d in bytes.
 */
void EnvironmentMapPlayer(
	uint64_t ticks_EnvironmentMapPlayerKernel,
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_u,
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_v,
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_w,
	uint64_t inscalar_EnvironmentMapPlayerKernel_viewplane_hres,
	double inscalar_EnvironmentMapPlayerKernel_viewplane_pixelsize,
	double inscalar_EnvironmentMapPlayerKernel_viewplane_viewdistance,
	uint64_t inscalar_EnvironmentMapPlayerKernel_viewplane_vres,
	const void *instream_col,
	size_t instream_size_col,
	const void *instream_row,
	size_t instream_size_row,
	void *outstream_d,
	size_t outstream_size_d);

/**
 * \brief Basic static non-blocking function for the interface 'default'.
 * 
 * Schedule to run on an engine and return immediately.
 * The status of the run can be checked either by ::max_wait or ::max_nowait;
 * note that one of these *must* be called, so that associated memory can be released.
 * 
 * 
 * \param [in] ticks_EnvironmentMapPlayerKernel The number of ticks for which kernel "EnvironmentMapPlayerKernel" will run.
 * \param [in] inscalar_EnvironmentMapPlayerKernel_camera_u Input scalar parameter "EnvironmentMapPlayerKernel.camera_u".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_camera_v Input scalar parameter "EnvironmentMapPlayerKernel.camera_v".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_camera_w Input scalar parameter "EnvironmentMapPlayerKernel.camera_w".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_hres Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_hres".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_pixelsize Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_pixelsize".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_viewdistance Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_viewdistance".
 * \param [in] inscalar_EnvironmentMapPlayerKernel_viewplane_vres Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_vres".
 * \param [in] instream_col Stream "col".
 * \param [in] instream_size_col The size of the stream instream_col in bytes.
 * \param [in] instream_row Stream "row".
 * \param [in] instream_size_row The size of the stream instream_row in bytes.
 * \param [out] outstream_d Stream "d".
 * \param [in] outstream_size_d The size of the stream outstream_d in bytes.
 * \return A handle on the execution status, or NULL in case of error.
 */
max_run_t *EnvironmentMapPlayer_nonblock(
	uint64_t ticks_EnvironmentMapPlayerKernel,
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_u,
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_v,
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_w,
	uint64_t inscalar_EnvironmentMapPlayerKernel_viewplane_hres,
	double inscalar_EnvironmentMapPlayerKernel_viewplane_pixelsize,
	double inscalar_EnvironmentMapPlayerKernel_viewplane_viewdistance,
	uint64_t inscalar_EnvironmentMapPlayerKernel_viewplane_vres,
	const void *instream_col,
	size_t instream_size_col,
	const void *instream_row,
	size_t instream_size_row,
	void *outstream_d,
	size_t outstream_size_d);

/**
 * \brief Advanced static interface, structure for the engine interface 'default'
 * 
 */
typedef struct { 
	uint64_t ticks_EnvironmentMapPlayerKernel; /**<  [in] The number of ticks for which kernel "EnvironmentMapPlayerKernel" will run. */
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_u; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.camera_u". */
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_v; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.camera_v". */
	uint64_t inscalar_EnvironmentMapPlayerKernel_camera_w; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.camera_w". */
	uint64_t inscalar_EnvironmentMapPlayerKernel_viewplane_hres; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_hres". */
	double inscalar_EnvironmentMapPlayerKernel_viewplane_pixelsize; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_pixelsize". */
	double inscalar_EnvironmentMapPlayerKernel_viewplane_viewdistance; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_viewdistance". */
	uint64_t inscalar_EnvironmentMapPlayerKernel_viewplane_vres; /**<  [in] Input scalar parameter "EnvironmentMapPlayerKernel.viewplane_vres". */
	const void *instream_col; /**<  [in] Stream "col". */
	size_t instream_size_col; /**<  [in] The size of the stream instream_col in bytes. */
	const void *instream_row; /**<  [in] Stream "row". */
	size_t instream_size_row; /**<  [in] The size of the stream instream_row in bytes. */
	void *outstream_d; /**<  [out] Stream "d". */
	size_t outstream_size_d; /**<  [in] The size of the stream outstream_d in bytes. */
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

