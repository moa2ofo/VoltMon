/*==================[VoltMon.h]==============================================*/
/**
 * @file    VoltMon.h
 * @brief   Public interface for the VoltMon (Voltage Monitoring) module.
 * @author  -
 * @date    2026-01-30
 *
 * @defgroup VoltMon Voltage Monitoring Module
 * @{
 *
 * @details
 * The VoltMon module monitors an input voltage value provided by the platform
 * (e.g., ADC raw counts) and evaluates it against configurable thresholds.
 * The module is split into:
 * - Platform/logic: VoltMon.c/.h (stand-alone)
 * - Configuration: VoltMon_cfg.c/.h (project-dependent)
 */

#ifndef VOLTMON_H
#define VOLTMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*==================[macros]=================================================*/

/** @brief Module status flag: initialized. */
#define VOLTMON_STATUS_INIT_U32      (0x00000001u)
/** @brief Module status flag: any error detected (generic). */
#define VOLTMON_STATUS_ERR_U32       (0x00000002u)
/** @brief Module status flag: undervoltage active. */
#define VOLTMON_STATUS_UV_U32        (0x00000004u)
/** @brief Module status flag: overvoltage active. */
#define VOLTMON_STATUS_OV_U32        (0x00000008u)
/** @brief Module status flag: last input invalid. */
#define VOLTMON_STATUS_INVAL_U32     (0x00000010u)

/*==================[types]==================================================*/

/**
 * @brief VoltMon operating mode.
 */
typedef enum
{
  VoltMon_modeIdle_e = 0,
  VoltMon_modeRun_e  = 1,
  VoltMon_modeDiag_e = 2
} VoltMon_mode_e;

/**
 * @brief Result codes for VoltMon services.
 */
typedef enum
{
  VoltMon_retOk_e             = 0,
  VoltMon_retNotInit_e        = 1,
  VoltMon_retInvalidParam_e   = 2,
  VoltMon_retBusyOrRejected_e = 3
} VoltMon_ret_e;

/*==================[public function prototypes]=============================*/

/**
 * @brief Initialize the VoltMon module.
 *
 * @details
 * **Goal of the function**
 *
 * Bring the module into a known and safe state:
 * - Load basic defaults.
 * - Clear flags.
 * - Set initial mode to IDLE.
 * - Validate configuration availability.
 *
 * The function is intended to be called once at startup.
 *
 * @par Interface summary
 *
 * | Interface         | In | Out | Type / Signature           | Param | Factor | Offset | Size | Range | Unit |
 * |------------------|----|-----|----------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_Init_u8  |    |  X  | uint8_t (void)             |  -    |   1    |   0    |  1   | 0..3  | [-]  |
 * | StatusFlg_u32    |    |  X  | uint32_t (static)          |  -    |   1    |   0    |  1   | -     | [-]  |
 * | Mode_e           |    |  X  | VoltMon_mode_e (static)    |  -    |   1    |   0    |  1   | -     | [-]  |
 *
 * @return uint8_t
 * @retval 0 Module initialized successfully.
 * @retval 1 Configuration missing/invalid.
 */
uint8_t VoltMon_Init_u8(void);

/**
 * @brief De-initialize the VoltMon module.
 *
 * @details
 * **Goal of the function**
 *
 * Put the module into an uninitialized state:
 * - Clear INIT flag.
 * - Force IDLE mode.
 * - Clear voltage value and error state.
 *
 * This is useful for tests and controlled shutdown.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Type / Signature          | Param | Factor | Offset | Size | Range | Unit |
 * |---------------------|----|-----|---------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_DeInit_u8   |    |  X  | uint8_t (void)            |  -    |   1    |   0    |  1   | 0..3  | [-]  |
 * | StatusFlg_u32       |    |  X  | uint32_t (static)         |  -    |   1    |   0    |  1   | -     | [-]  |
 * | Mode_e              |    |  X  | VoltMon_mode_e (static)   |  -    |   1    |   0    |  1   | -     | [-]  |
 *
 * @return uint8_t
 * @retval 0 De-initialized.
 * @retval 1 Module was not initialized.
 */
uint8_t VoltMon_DeInit_u8(void);

/**
 * @brief Set the VoltMon operating mode.
 *
 * @details
 * **Goal of the function**
 *
 * Switch the module behavior according to the requested mode:
 * - IDLE: no threshold evaluation (still stores last voltage).
 * - RUN: evaluate thresholds and set UV/OV flags deterministically.
 * - DIAG: behaves like RUN but keeps ERR flag sticky for diagnostics.
 *
 * Invalid mode requests are rejected and the ERR flag is set.
 *
 * @par Interface summary
 *
 * | Interface              | In | Out | Type / Signature                          | Param | Factor | Offset | Size | Range | Unit |
 * |-----------------------|----|-----|-------------------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_SetMode_u8    | X  |  X  | uint8_t (VoltMon_mode_e mode)             | mode  |   1    |   0    |  1   | -     | [-]  |
 * | Mode_e                |    |  X  | VoltMon_mode_e (static)                   |  -    |   1    |   0    |  1   | -     | [-]  |
 * | StatusFlg_u32         |    |  X  | uint32_t (static)                         |  -    |   1    |   0    |  1   | -     | [-]  |
 *
 * @param mode Requested mode.
 * @return uint8_t
 * @retval 0 Mode accepted.
 * @retval 1 Not initialized.
 * @retval 2 Invalid mode.
 */
uint8_t VoltMon_SetMode_u8(VoltMon_mode_e mode);

/**
 * @brief Provide a new ADC raw sample to VoltMon.
 *
 * @details
 * **Goal of the function**
 *
 * Update the internal raw sample and compute the corresponding voltage in mV
 * using the configuration scaling:
 * - Compute mV = raw * factor + offset
 * - Store last computed voltage
 * - Validate input range (raw <= rawMax)
 *
 * The function does not directly set UV/OV flags; evaluation is done in
 * VoltMon_Process() to keep timing deterministic.
 *
 * @par Interface summary
 *
 * | Interface                 | In | Out | Type / Signature                         | Param  | Factor | Offset | Size | Range | Unit |
 * |--------------------------|----|-----|------------------------------------------|--------|--------|--------|------|-------|------|
 * | VoltMon_UpdateAdc_u8     | X  |  X  | uint8_t (uint16_t rawAdc_u16)            | rawAdc |   1    |   0    |  2   | -     | [-]  |
 * | LastRawAdc_u16           |    |  X  | uint16_t (static)                         |   -    |   1    |   0    |  2   | -     | [-]  |
 * | LastVoltage_mV_u16       |    |  X  | uint16_t (static)                         |   -    |   1    |   0    |  2   | -     | [mV] |
 *
 * @param rawAdc_u16 ADC raw counts.
 * @return uint8_t
 * @retval 0 Updated.
 * @retval 1 Not initialized.
 * @retval 2 Input invalid/out of configured range.
 */
uint8_t VoltMon_UpdateAdc_u8(uint16_t rawAdc_u16);

/**
 * @brief Run one deterministic processing step of the VoltMon module.
 *
 * @details
 * **Goal of the function**
 *
 * Execute a bounded amount of work per call:
 * - Maintain a function-static cycle counter (for deterministic internal pacing).
 * - When in RUN/DIAG mode, evaluate the last computed voltage against thresholds:
 *   - Set/clear UV/OV flags.
 *   - In DIAG mode keep ERR flag sticky once asserted.
 * - Always execute a bounded loop to keep deterministic timing.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Type / Signature              | Param | Factor | Offset | Size | Range | Unit |
 * |---------------------|----|-----|-------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_Process     |    |  X  | void (void)                   |  -    |   1    |   0    |  -   | -     | [-]  |
 * | Mode_e              | X  |     | VoltMon_mode_e (static)       |  -    |   1    |   0    |  1   | -     | [-]  |
 * | LastVoltage_mV_u16  | X  |     | uint16_t (static)             |  -    |   1    |   0    |  2   | -     | [mV] |
 * | StatusFlg_u32       | X  |  X  | uint32_t (static)             |  -    |   1    |   0    |  4   | -     | [-]  |
 * | l_CycleCnt_u32      | X  |  X  | uint32_t (static local)       |  -    |   1    |   0    |  4   | -     | [-]  |
 *
 * @return void
 */
void VoltMon_Process(void);

/**
 * @brief Read the last computed voltage in millivolts.
 *
 * @details
 * **Goal of the function**
 *
 * Provide the last stored voltage value (mV) computed from the last ADC input.
 * If the module is not initialized, 0 mV is returned and ERR flag is set.
 *
 * @par Interface summary
 *
 * | Interface                   | In | Out | Type / Signature              | Param | Factor | Offset | Size | Range | Unit |
 * |----------------------------|----|-----|-------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_GetVoltage_mV_u16  |    |  X  | uint16_t (void)               |  -    |   1    |   0    |  2   | -     | [mV] |
 * | LastVoltage_mV_u16         | X  |     | uint16_t (static)             |  -    |   1    |   0    |  2   | -     | [mV] |
 *
 * @return uint16_t Last computed voltage [mV], or 0 if not initialized.
 */
uint16_t VoltMon_GetVoltage_mV_u16(void);

/**
 * @brief Get the current module status flags.
 *
 * @details
 * **Goal of the function**
 *
 * Return the current status bitmask (INIT/ERR/UV/OV/INVAL).
 * No side effects are performed.
 *
 * @par Interface summary
 *
 * | Interface                | In | Out | Type / Signature        | Param | Factor | Offset | Size | Range | Unit |
 * |-------------------------|----|-----|-------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_GetStatus_u32   |    |  X  | uint32_t (void)         |  -    |   1    |   0    |  4   | -     | [-]  |
 * | StatusFlg_u32           | X  |     | uint32_t (static)       |  -    |   1    |   0    |  4   | -     | [-]  |
 *
 * @return uint32_t Status flags bitmask.
 */
uint32_t VoltMon_GetStatus_u32(void);

#ifdef __cplusplus
}
#endif

/** @} */ /* end of defgroup VoltMon */

#endif /* VOLTMON_H */
