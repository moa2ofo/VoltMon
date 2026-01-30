
#include "VoltMon.h"
#include "VoltMon_priv.h"
#include "VoltMon_cfg.h"


/* Get/Set API for VoltMon module */

uint32_t VoltMon_GetStatusFlg_u32(void);
void     VoltMon_SetStatusFlg_u32(uint32_t value);

VoltMon_mode_e VoltMon_GetMode_e(void);
void           VoltMon_SetMode_e(VoltMon_mode_e mode);

uint16_t VoltMon_GetLastRawAdc_u16(void);
void     VoltMon_SetLastRawAdc_u16(uint16_t value);

uint16_t VoltMon_GetLastVoltage_mV_u16(void);
void     VoltMon_SetLastVoltage_mV_u16(uint16_t value);

bool VoltMon_GetUvActive_b(void);
void VoltMon_SetUvActive_b(bool value);

bool VoltMon_GetOvActive_b(void);
void VoltMon_SetOvActive_b(bool value);





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
 * @par Activity diagram
 * @code
 * start
 * :l_cfg_pcs = VoltMon_CfgGet_pcfg();
 * if ((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
 *   :return 1;
 * elseif ((l_cfg_pcs == 0) || (rawAdc_u16 > l_cfg_pcs->rawMax_u16)) then (invalid input)
 *   :StatusFlg_u32 |= (VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32);
 *   :return 2;
 * else (valid)
 *   :LastRawAdc_u16 = rawAdc_u16;
 *   :LastVoltage_mV_u16 = ComputeVoltage_u16(rawAdc_u16, l_cfg_pcs);
 *   :StatusFlg_u32 &= ~VOLTMON_STATUS_INVAL_U32;
 *   :return 0;
 * endif
 * stop
 * @endcode
 *
 * @par Interface summary
 *
 * | Interface                 | In | Out | Type / Signature   | Param  | Factor | Offset | Size | Range | Unit |
 * |--------------------------|----|-----|---------------------|--------|--------|--------|------|-------|------|
 * | rawAdc                   | X  |  X  | uint16_t            | rawAdc |   1    |   0    |  2   | [0,UINT16_MAX] | [-]  |
 * | LastRawAdc_u16           |    |  X  | uint16_t (static)   |   -    |   1    |   0    |  2   | [0,UINT16_MAX] | [-]  |
 * | LastVoltage_mV_u16       |    |  X  | uint16_t (static)   |   -    |   1    |   0    |  2   | [0,UINT16_MAX] | [mV] |
 * | returned value           | X  |  X  | uint16_t            | - |   1    |   0    |  2   | [0,UINT16_MAX] | [-]  |
 *
 * @param rawAdc_u16 ADC raw counts.
 * @return uint8_t
 * @retval 0 Updated.
 * @retval 1 Not initialized.
 * @retval 2 Input invalid/out of configured range.
 */
uint8_t VoltMon_UpdateAdc_u8(uint16_t rawAdc_u16);