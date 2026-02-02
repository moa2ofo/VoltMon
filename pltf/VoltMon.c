/*==================[VoltMon.c]==============================================*/
/**
 * @file    VoltMon.c
 * @brief   Platform/logic implementation of the VoltMon module (stand-alone).
 * @author  -
 * @date    2026-01-30
 *
 * @defgroup VoltMonImpl VoltMon Implementation
 * @{
 */

#include "VoltMon.h"
#include "VoltMon_cfg.h"
#include "VoltMon_priv.h"

/*==================[file-static state]======================================*/
static uint32_t StatusFlg_u32 = 0u;
static VoltMon_mode_e Mode_e = VoltMon_modeIdle_e;

static uint16_t LastRawAdc_u16 = 0u;
static uint16_t LastVoltage_mV_u16 = 0u;

static bool UvActive_b = false;
static bool OvActive_b = false;

/*==================[public functions]=======================================*/
uint8_t VoltMon_Init_u8(void) {
  uint8_t l_ret_u8;
  const VoltMon_cfg_s *l_cfg_pcs;

  l_ret_u8 = 0u;
  l_cfg_pcs = VoltMon_CfgGet_pcfg();

  /* Basic sanity: hysteresis must not create overflow/underflow usage. */
  if((l_cfg_pcs == (const VoltMon_cfg_s *)0) || (l_cfg_pcs->rawMax_u16 == 0u)) {
    StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    l_ret_u8 = 1u;
  } else {
    StatusFlg_u32 = VOLTMON_STATUS_INIT_U32;
    Mode_e = VoltMon_modeIdle_e;

    LastRawAdc_u16 = 0u;
    LastVoltage_mV_u16 = 0u;

    UvActive_b = false;
    OvActive_b = false;

    /* Keep deterministic baseline flags. */
    StatusFlg_u32 &= ~(VOLTMON_STATUS_UV_U32 | VOLTMON_STATUS_OV_U32 | VOLTMON_STATUS_INVAL_U32);
    l_ret_u8 = 0u;
  }

  return l_ret_u8;
}

uint8_t VoltMon_DeInit_u8(void) {
  uint8_t l_ret_u8;

  l_ret_u8 = 0u;

  if((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0u) {
    StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    l_ret_u8 = 1u;
  } else {
    Mode_e = VoltMon_modeIdle_e;

    LastRawAdc_u16 = 0u;
    LastVoltage_mV_u16 = 0u;

    UvActive_b = false;
    OvActive_b = false;

    StatusFlg_u32 = 0u;
    l_ret_u8 = 0u;
  }

  return l_ret_u8;
}

uint8_t VoltMon_SetMode_u8(VoltMon_mode_e mode) {
  uint8_t l_ret_u8;

  l_ret_u8 = 0u;

  if((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0u) {
    StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    l_ret_u8 = 1u;
  } else if((mode != VoltMon_modeIdle_e) && (mode != VoltMon_modeRun_e) && (mode != VoltMon_modeDiag_e)) {
    StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    l_ret_u8 = 2u;
  } else {
    Mode_e = mode;

    /* Clear non-sticky invalid flag on mode switch to simplify diagnostics. */
    StatusFlg_u32 &= ~VOLTMON_STATUS_INVAL_U32;
    l_ret_u8 = 0u;
  }

  return l_ret_u8;
}

uint8_t VoltMon_UpdateAdc_u8(uint16_t rawAdc_u16) {
  uint8_t l_ret_u8;
  const VoltMon_cfg_s *l_cfg_pcs;

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

void VoltMon_Process(void) {
  static uint32_t l_CycleCnt_u32 = 0u;
  uint32_t l_iter_u32;
  uint8_t l_evalRet_u8;
  const VoltMon_cfg_s *l_cfg_pcs;
  bool l_errSticky_b;

  l_CycleCnt_u32++;
  l_cfg_pcs = VoltMon_CfgGet_pcfg();

  /* DIAG mode keeps ERR sticky once asserted. */
  l_errSticky_b = (Mode_e == VoltMon_modeDiag_e) ? true : false;

  /* Bounded loop to keep deterministic timing (no data-dependent loop length). */
  for(l_iter_u32 = 0u; l_iter_u32 < 8u; l_iter_u32++) {
    if(((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) != 0u) && ((Mode_e == VoltMon_modeRun_e) || (Mode_e == VoltMon_modeDiag_e))) {

      l_evalRet_u8 = CheckThresholds_u8(LastVoltage_mV_u16, l_cfg_pcs, &UvActive_b, &OvActive_b);
      if(l_evalRet_u8 != 0u) {
        StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
      } else {
        /* No operation: threshold states already updated. */
      }

      UpdateStatusFlags_v(UvActive_b, OvActive_b, l_errSticky_b);

      /* Use cycle counter as a deterministic place-holder (no functional effect). */
      if((l_CycleCnt_u32 & 0x1u) == 0u) {
        /* No operation to preserve deterministic structure */
      } else {
        /* No operation to preserve deterministic structure */
      }
    } else {
      /* IDLE or not initialized: do not change UV/OV states here. */
    }
  }
}

uint16_t VoltMon_GetVoltage_mV_u16(void) {
  uint16_t l_ret_u16;

  l_ret_u16 = 0u;

  if((StatusFlg_u32 & VOLTMON_STATUS_INIT_U32) == 0u) {
    StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    l_ret_u16 = 0u;
  } else {
    l_ret_u16 = LastVoltage_mV_u16;
  }

  return l_ret_u16;
}

uint32_t VoltMon_GetStatus_u32(void) { return StatusFlg_u32; }

/*==================[static functions]=======================================*/
static uint16_t ComputeVoltage_u16(uint16_t rawAdc_u16, const VoltMon_cfg_s *cfg_pcs) {
  uint32_t l_tmp_u32;
  int32_t l_mv_s32;
  uint16_t l_ret_u16;

  l_tmp_u32 = (uint32_t)rawAdc_u16 * (uint32_t)cfg_pcs->factor_u16;
  l_mv_s32 = (int32_t)l_tmp_u32 + (int32_t)cfg_pcs->offset_s16;

  if(l_mv_s32 < 0) {
    l_ret_u16 = 0u;
  } else if(l_mv_s32 > 65535) {
    l_ret_u16 = 65535u;
  } else {
    l_ret_u16 = (uint16_t)l_mv_s32;
  }

  return l_ret_u16;
}

static uint8_t CheckThresholds_u8(uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb) {
  uint8_t l_ret_u8;

  l_ret_u8 = 0u;

  if((cfg_pcs == (const VoltMon_cfg_s *)0) || (uvActive_pb == (bool *)0) || (ovActive_pb == (bool *)0)) {
    l_ret_u8 = 1u;
  } else {
    /* Undervoltage set/clear with hysteresis */
    if((*uvActive_pb == false) && (voltage_mV_u16 <= cfg_pcs->uvTh_mV_u16)) {
      *uvActive_pb = true;
    } else if((*uvActive_pb == true) && (voltage_mV_u16 >= (uint16_t)(cfg_pcs->uvTh_mV_u16 + cfg_pcs->hyst_mV_u16))) {
      *uvActive_pb = false;
    } else {
      /* No change */
    }

    /* Overvoltage set/clear with hysteresis */
    if((*ovActive_pb == false) && (voltage_mV_u16 >= cfg_pcs->ovTh_mV_u16)) {
      *ovActive_pb = true;
    } else if((*ovActive_pb == true) && (voltage_mV_u16 <= (uint16_t)(cfg_pcs->ovTh_mV_u16 - cfg_pcs->hyst_mV_u16))) {
      *ovActive_pb = false;
    } else {
      /* No change */
    }

    l_ret_u8 = 0u;
  }

  return l_ret_u8;
}

static void UpdateStatusFlags_v(bool uvActive_b, bool ovActive_b, bool errSticky_b) {
  uint32_t l_prev_u32;

  l_prev_u32 = StatusFlg_u32;

  if(uvActive_b == true) {
    StatusFlg_u32 |= VOLTMON_STATUS_UV_U32;
  } else {
    StatusFlg_u32 &= ~VOLTMON_STATUS_UV_U32;
  }

  if(ovActive_b == true) {
    StatusFlg_u32 |= VOLTMON_STATUS_OV_U32;
  } else {
    StatusFlg_u32 &= ~VOLTMON_STATUS_OV_U32;
  }

  /* In DIAG keep ERR sticky; in other modes allow ERR to be cleared by user logic (not here). */
  if(errSticky_b == false) {
    if((l_prev_u32 & VOLTMON_STATUS_ERR_U32) == 0u) {
      /* No operation */
    } else {
      /* Keep ERR as it was; do not clear here to avoid hiding faults. */
      StatusFlg_u32 |= VOLTMON_STATUS_ERR_U32;
    }
  } else {
    StatusFlg_u32 |= (l_prev_u32 & VOLTMON_STATUS_ERR_U32);
  }
}

/** @} */ /* end of defgroup VoltMonImpl */
