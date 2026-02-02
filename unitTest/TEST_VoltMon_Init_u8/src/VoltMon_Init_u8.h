
#include "VoltMon.h"
#include "VoltMon_cfg.h"
#include "VoltMon_priv.h"

/* Get/Set API for VoltMon module */

uint32_t VoltMon_GetStatusFlg_u32(void);
void VoltMon_SetStatusFlg_u32(uint32_t value);

VoltMon_mode_e VoltMon_GetMode_e(void);
void VoltMon_SetMode_e(VoltMon_mode_e mode);

uint16_t VoltMon_GetLastRawAdc_u16(void);
void VoltMon_SetLastRawAdc_u16(uint16_t value);

uint16_t VoltMon_GetLastVoltage_mV_u16(void);
void VoltMon_SetLastVoltage_mV_u16(uint16_t value);

bool VoltMon_GetUvActive_b(void);
void VoltMon_SetUvActive_b(bool value);

bool VoltMon_GetOvActive_b(void);
void VoltMon_SetOvActive_b(bool value);

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