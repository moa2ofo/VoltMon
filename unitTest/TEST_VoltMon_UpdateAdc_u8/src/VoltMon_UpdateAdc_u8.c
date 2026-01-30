
#include "VoltMon_UpdateAdc_u8.h"


/* Static module variables */
static uint32_t        StatusFlg_u32        = 0u;
static VoltMon_mode_e  Mode_e               = VoltMon_modeIdle_e;

static uint16_t        LastRawAdc_u16       = 0u;
static uint16_t        LastVoltage_mV_u16   = 0u;

static bool            UvActive_b           = false;
static bool            OvActive_b           = false;

/* Get/Set Implementations */

uint32_t VoltMon_GetStatusFlg_u32(void)
{
    return StatusFlg_u32;
}

void VoltMon_SetStatusFlg_u32(uint32_t value)
{
    StatusFlg_u32 = value;
}

VoltMon_mode_e VoltMon_GetMode_e(void)
{
    return Mode_e;
}

void VoltMon_SetMode_e(VoltMon_mode_e mode)
{
    Mode_e = mode;
}

uint16_t VoltMon_GetLastRawAdc_u16(void)
{
    return LastRawAdc_u16;
}

void VoltMon_SetLastRawAdc_u16(uint16_t value)
{
    LastRawAdc_u16 = value;
}

uint16_t VoltMon_GetLastVoltage_mV_u16(void)
{
    return LastVoltage_mV_u16;
}

void VoltMon_SetLastVoltage_mV_u16(uint16_t value)
{
    LastVoltage_mV_u16 = value;
}

bool VoltMon_GetUvActive_b(void)
{
    return UvActive_b;
}

void VoltMon_SetUvActive_b(bool value)
{
    UvActive_b = value;
}

bool VoltMon_GetOvActive_b(void)
{
    return OvActive_b;
}

void VoltMon_SetOvActive_b(bool value)
{
    OvActive_b = value;
}



/* FUNCTION TO TEST */


uint8_t VoltMon_UpdateAdc_u8(uint16_t rawAdc_u16) {
  uint8_t l_ret_u8;
  const VoltMon_cfg_s * l_cfg_pcs;

  l_ret_u8 = 0u;
  l_cfg_pcs = VoltMon_CfgGet_pcfg();

  if((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0u) {
    StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    l_ret_u8 = 1u;
  } else if((l_cfg_pcs == (const VoltMon_cfg_s *)0) || (rawAdc_u16 > l_cfg_pcs->rawMax_u16)) {
    StatusFlg_u32 |= (VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32);
    l_ret_u8 = 2u;
  } else {
    LastRawAdc_u16 = rawAdc_u16;
    LastVoltage_mV_u16 = ComputeVoltage_u16(rawAdc_u16, l_cfg_pcs);

    /* Valid sample: clear INVAL but keep ERR depending on mode/previous state. */
    StatusFlg_u32 &= ~VOLTMON_STATUS_INVAL_U32;
    l_ret_u8 = 0u;
  }

  return l_ret_u8;
}
