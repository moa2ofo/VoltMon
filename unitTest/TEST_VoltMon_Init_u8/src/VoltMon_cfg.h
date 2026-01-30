/*==================[VoltMon_cfg.h]==========================================*/
/**
 * @file    VoltMon_cfg.h
 * @brief   Configuration interface for the VoltMon module.
 * @author  -
 * @date    2026-01-30
 *
 * @defgroup VoltMonCfg VoltMon Configuration
 * @{
 *
 * @details
 * This file contains the configuration types and the configuration getter.
 * The actual configuration instance is defined in VoltMon_cfg.c.
 */

#ifndef VOLTMON_CFG_H
#define VOLTMON_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*==================[types]==================================================*/

/**
 * @brief VoltMon configuration structure.
 *
 * @details
 * - rawMax_u16: maximum allowed ADC raw count (input validation)
 * - factor_u16: scaling factor for mV conversion (mV = raw * factor + offset)
 * - offset_s16: offset for mV conversion (signed)
 * - uvTh_mV_u16: undervoltage threshold [mV]
 * - ovTh_mV_u16: overvoltage threshold [mV]
 * - hyst_mV_u16: hysteresis [mV] applied to clear UV/OV flags
 */
typedef struct
{
  uint16_t rawMax_u16;
  uint16_t factor_u16;
  int16_t  offset_s16;
  uint16_t uvTh_mV_u16;
  uint16_t ovTh_mV_u16;
  uint16_t hyst_mV_u16;
} VoltMon_cfg_s;

/*==================[configuration access]===================================*/

/**
 * @brief Get the active VoltMon configuration pointer.
 *
 * @details
 * **Goal of the function**
 *
 * Provide read-only access to the configuration stored in VoltMon_cfg.c.
 * This indirection allows:
 * - project-specific configuration without modifying platform code
 * - a single consistent access point for VoltMon.c
 *
 * @par Interface summary
 *
 * | Interface             | In | Out | Type / Signature                      | Param | Factor | Offset | Size | Range | Unit |
 * |-----------------------|----|-----|---------------------------------------|-------|--------|--------|------|-------|------|
 * | VoltMon_CfgGet_pcfg   |    |  X  | const VoltMon_cfg_s * (void)          |  -    |   1    |   0    |  4   | -     | [-]  |
 *
 * @return const VoltMon_cfg_s* Pointer to the configuration (never NULL).
 */
const VoltMon_cfg_s * VoltMon_CfgGet_pcfg(void);

#ifdef __cplusplus
}
#endif

/** @} */ /* end of defgroup VoltMonCfg */

#endif /* VOLTMON_CFG_H */
