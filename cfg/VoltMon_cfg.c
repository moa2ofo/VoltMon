/*==================[VoltMon_cfg.c]==========================================*/
/**
 * @file    VoltMon_cfg.c
 * @brief   Configuration definitions for VoltMon.
 * @author  -
 * @date    2026-01-30
 *
 * @defgroup VoltMonCfgImpl VoltMon Configuration Implementation
 * @{
 */

#include "VoltMon_cfg.h"

/*==================[config instance]========================================*/
/* Project-specific numbers can be changed here without touching VoltMon.c. */
static const VoltMon_cfg_s VoltMon_Cfg_s =
{
  /* rawMax_u16     */ 4095u,
  /* factor_u16     */ 2u,      /* Example: 2 mV per count */
  /* offset_s16     */ 0,
  /* uvTh_mV_u16    */ 9000u,   /* 9.0 V */
  /* ovTh_mV_u16    */ 16000u,  /* 16.0 V */
  /* hyst_mV_u16    */ 200u     /* 0.2 V */
};

const VoltMon_cfg_s * VoltMon_CfgGet_pcfg(void)
{
  /* Single access point; never return NULL to simplify caller logic. */
  const VoltMon_cfg_s * l_cfg_pcs;

  l_cfg_pcs = &VoltMon_Cfg_s;

  /* No side effects: deterministic and safe. */
  return l_cfg_pcs;
}

/** @} */ /* end of defgroup VoltMonCfgImpl */
