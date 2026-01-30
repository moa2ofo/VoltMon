/*==================[VoltMon_priv.h]=========================================*/
/**
 * @file    VoltMon_priv.h
 * @brief   Private declarations for VoltMon (static functions in VoltMon.c).
 * @author  -
 * @date    2026-01-30
 *
 * @defgroup VoltMonPriv VoltMon Private Interface
 * @{
 *
 * @details
 * This header declares the internal (cfile-static) functions implemented in
 * VoltMon.c. It is included only by VoltMon.c.
 */

#ifndef VOLTMON_PRIV_H
#define VOLTMON_PRIV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "VoltMon_cfg.h"

/**
 * @brief Compute voltage in mV from ADC raw sample.
 *
 * @details
 * **Goal of the function**
 *
 * Convert the raw ADC value into millivolts using configuration parameters:
 * - mV = raw * factor + offset
 * - clamp to [0..65535] to avoid wrap
 *
 * @par Interface summary
 *
 * | Interface             | In | Out | Type / Signature                    | Param | Factor | Offset | Size | Range | Unit |
 * |----------------------|----|-----|-------------------------------------|-------|--------|--------|------|-------|------|
 * | ComputeVoltage_u16   | X  |  X  | uint16_t (uint16_t raw, cfg*)       | raw   |   1    |   0    |  2   | -     | [-]  |
 * |                      | X  |     | const VoltMon_cfg_s * cfg           | cfg   |   1    |   0    |  4   | -     | [-]  |
 *
 * @param rawAdc_u16 Raw ADC counts.
 * @param cfg_pcs Pointer to configuration.
 * @return uint16_t Voltage [mV] clamped to uint16_t.
 */
static uint16_t ComputeVoltage_u16(uint16_t rawAdc_u16, const VoltMon_cfg_s * cfg_pcs);

/**
 * @brief Check thresholds and compute new UV/OV state.
 *
 * @details
 * **Goal of the function**
 *
 * Evaluate voltage with hysteresis:
 * - UV set if V <= uvTh
 * - UV cleared if V >= uvTh + hyst
 * - OV set if V >= ovTh
 * - OV cleared if V <= ovTh - hyst
 *
 * @par Interface summary
 *
 * | Interface              | In | Out | Type / Signature                              | Param | Factor | Offset | Size | Range | Unit |
 * |-----------------------|----|-----|-----------------------------------------------|-------|--------|--------|------|-------|------|
 * | CheckThresholds_u8    | X  |  X  | uint8_t (uint16_t v, cfg*, bool*, bool*)      | v     |   1    |   0    |  2   | -     | [mV] |
 *
 * @param voltage_mV_u16 Voltage [mV].
 * @param cfg_pcs Pointer to configuration.
 * @param uvActive_pb Pointer to UV state (in/out).
 * @param ovActive_pb Pointer to OV state (in/out).
 * @return uint8_t 0 on success, 1 if cfg invalid.
 */
static uint8_t CheckThresholds_u8(uint16_t voltage_mV_u16,
                                  const VoltMon_cfg_s * cfg_pcs,
                                  bool * uvActive_pb,
                                  bool * ovActive_pb);

/**
 * @brief Update status flags coherently from internal states.
 *
 * @details
 * **Goal of the function**
 *
 * Apply UV/OV bits into the status bitmask:
 * - Set or clear VOLTMON_STATUS_UV_U32
 * - Set or clear VOLTMON_STATUS_OV_U32
 * - Keep INIT as-is
 * - Keep ERR sticky if requested
 *
 * @par Interface summary
 *
 * | Interface              | In | Out | Type / Signature                | Param | Factor | Offset | Size | Range | Unit |
 * |-----------------------|----|-----|---------------------------------|-------|--------|--------|------|-------|------|
 * | UpdateStatusFlags_v   | X  |  X  | void (bool uv, bool ov, bool)   |  -    |   1    |   0    |  -   | -     | [-]  |
 *
 * @param uvActive_b Current undervoltage state.
 * @param ovActive_b Current overvoltage state.
 * @param errSticky_b If true, keep ERR flag sticky once set.
 * @return void
 */
static void UpdateStatusFlags_v(bool uvActive_b, bool ovActive_b, bool errSticky_b);

#ifdef __cplusplus
}
#endif

/** @} */ /* end of defgroup VoltMonPriv */

#endif /* VOLTMON_PRIV_H */
