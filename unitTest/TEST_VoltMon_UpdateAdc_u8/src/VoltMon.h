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
 * @par Activity diagram
 * @code
 * start
 * :l_cfg_pcs = VoltMon_CfgGet_pcfg();
 * if ((l_cfg_pcs == 0) || (l_cfg_pcs->rawMax_u16 == 0)) then (invalid cfg)
 *   :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *   :return 1;
 * else (ok)
 *   :StatusFlg_u32 = VOLTMON_STATUS_INIT_U32;
 *   :Mode_e = VoltMon_modeIdle_e;
 *   :LastRawAdc_u16 = 0; LastVoltage_mV_u16 = 0;
 *   :UvActive_b = false; OvActive_b = false;
 *   :StatusFlg_u32 &= ~(VOLTMON_STATUS_UV_U32 | VOLTMON_STATUS_OV_U32 | VOLTMON_STATUS_INVAL_U32);
 *   :return 0;
 * endif
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface         | In | Out | Type / Signature           | Param | Factor | Offset | Size | Range | Unit |
 * |------------------|----|-----|----------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_Init_u8  |    |  X  | uint8_t (void)             |  -    |   1    |   0    |  1   | 0..3  | [-]  |
 * | StatusFlg_u32    |    |  X  | uint32_t (static)          |  -    |   1    |   0    |  1   | [0,UINT32_MAX] | [-]  |
 * | Mode_e           |    |  X  | VoltMon_mode_e (static)    |  -    |   1    |   0    |  1   | -     | [0,2]  |
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
 * @par Activity diagram
 * @code
 * start
 * if ((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *   :return 1;
 * else (init)
 *   :Mode_e = VoltMon_modeIdle_e;
 *   :LastRawAdc_u16 = 0; LastVoltage_mV_u16 = 0;
 *   :UvActive_b = false; OvActive_b = false;
 *   :StatusFlg_u32 = 0;
 *   :return 0;
 * endif
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Type / Signature          | Param | Factor | Offset | Size | Range | Unit |
 * |---------------------|----|-----|---------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_DeInit_u8   |    |  X  | uint8_t (void)            |  -    |   1    |   0    |  1   | 0..3  | [-]  |
 * | StatusFlg_u32       |    |  X  | uint32_t (static)         |  -    |   1    |   0    |  1   | [0,UINT32_MAX] | [-]  |
 * | Mode_e              |    |  X  | VoltMon_mode_e (static)   |  -    |   1    |   0    |  1   | -     | [0,2]  |
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
 * @par Activity diagram
 * @code
 * start
 * if ((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *   :return 1;
 * elseif ((mode != VoltMon_modeIdle_e) && (mode != VoltMon_modeRun_e) && (mode != VoltMon_modeDiag_e)) then (invalid mode)
 *   :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *   :return 2;
 * else (accepted)
 *   :Mode_e = mode;
 *   :StatusFlg_u32 &= ~VOLTMON_STATUS_INVAL_U32;
 *   :return 0;
 * endif
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface              | In | Out | Type / Signature                          | Param | Factor | Offset | Size | Range | Unit |
 * |-----------------------|----|-----|-------------------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_SetMode_u8    | X  |  X  | uint8_t (VoltMon_mode_e mode)             | mode  |   1    |   0    |  1   | [0,UINT8_MAX] | [-]  |
 * | Mode_e                |    |  X  | VoltMon_mode_e (static)                   |  -    |   1    |   0    |  1   | -     | [0,2]  |
 * | StatusFlg_u32         |    |  X  | uint32_t (static)                         |  -    |   1    |   0    |  1   | [0,UINT32_MAX] | [-]  |
 *
 * @param mode Requested mode.
 * @return uint8_t
 * @retval 0 Mode accepted.
 * @retval 1 Not initialized.
 * @retval 2 Invalid mode.
 */
uint8_t VoltMon_SetMode_u8(VoltMon_mode_e mode);



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
 * @par Activity diagram
 * @code
 * start
 * :l_CycleCnt_u32++;
 * :l_cfg_pcs = VoltMon_CfgGet_pcfg();
 * :l_errSticky_b = (Mode_e == VoltMon_modeDiag_e);
 * repeat (8 iterations)
 *   if (((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) != 0) && ((Mode_e == VoltMon_modeRun_e) || (Mode_e == VoltMon_modeDiag_e))) then (evaluate)
 *     :l_evalRet_u8 = CheckThresholds_u8(LastVoltage_mV_u16, l_cfg_pcs, &UvActive_b, &OvActive_b);
 *     if (l_evalRet_u8 != 0) then (error)
 *       :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *     endif
 *     :UpdateStatusFlags_v(UvActive_b, OvActive_b, l_errSticky_b);
 *     if ((l_CycleCnt_u32 & 0x1) == 0) then (even)
 *       :no-op;
 *     else (odd)
 *       :no-op;
 *     endif
 *   else (idle / not init)
 *     :no-op;
 *   endif
 * repeat while (iteration < 8)
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Type / Signature              | Param | Factor | Offset | Size | Range | Unit |
 * |---------------------|----|-----|-------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_Process     |    |  X  | void (void)                   |  -    |   1    |   0    |  -   | -     | [-]  |
 * | Mode_e              | X  |     | VoltMon_mode_e (static)       |  -    |   1    |   0    |  1   | -     | 0,2]  |
 * | LastVoltage_mV_u16  | X  |     | uint16_t (static)             |  -    |   1    |   0    |  2   | [0,UINT16_MAX] | [mV] |
 * | StatusFlg_u32       | X  |  X  | uint32_t (static)             |  -    |   1    |   0    |  4   | [0,UINT32_MAX] | [-]  |
 * | l_CycleCnt_u32      | X  |  X  | uint32_t (static local)       |  -    |   1    |   0    |  4   | [0,UINT32_MAX] | [-]  |
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
 * @par Activity diagram
 * @code
 * start
 * if ((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *   :return 0;
 * else (init)
 *   :return LastVoltage_mV_u16;
 * endif
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface                   | In | Out | Type / Signature              | Param | Factor | Offset | Size | Range | Unit |
 * |----------------------------|----|-----|-------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_GetVoltage_mV_u16  |    |  X  | uint16_t (void)               |  -    |   1    |   0    |  2   | [0,UINT16_MAX] | [mV] |
 * | LastVoltage_mV_u16         | X  |     | uint16_t (static)             |  -    |   1    |   0    |  2   | [0,UINT16_MAX] | [mV] |
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
 * @par Activity diagram
 * @code
 * start
 * :return StatusFlg_u32;
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface                | In | Out | Type / Signature        | Param | Factor | Offset | Size | Range | Unit |
 * |-------------------------|----|-----|-------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_GetStatus_u32   |    |  X  | uint32_t (void)         |  -    |   1    |   0    |  4   | [0,UINT32_MAX] | [-]  |
 * | StatusFlg_u32           | X  |     | uint32_t (static)       |  -    |   1    |   0    |  4   | [0,UINT32_MAX] | [-]  |
 *
 * @return uint32_t Status flags bitmask.
 */
uint32_t VoltMon_GetStatus_u32(void);

#ifdef __cplusplus
}
#endif

/** @} */ /* end of defgroup VoltMon */

#endif /* VOLTMON_H */