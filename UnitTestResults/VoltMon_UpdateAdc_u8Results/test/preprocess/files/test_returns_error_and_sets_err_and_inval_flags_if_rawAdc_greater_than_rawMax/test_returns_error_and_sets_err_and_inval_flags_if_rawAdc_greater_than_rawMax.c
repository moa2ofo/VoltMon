// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

static void reset_all(void)
{
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  VoltMon_SetLastRawAdc_u16(0u);
  VoltMon_SetLastVoltage_mV_u16(0u);
  VoltMon_SetUvActive_b(
                       0
                            );
  VoltMon_SetOvActive_b(
                       0
                            );
}

void setUp(void)
{
  reset_all();
}

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_and_inval_flags_if_rawAdc_greater_than_rawMax(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 500u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(27, &cfg);
  uint16_t rawAdc = 501u;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(30), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  do { if (((0u) != ((status & (0x00000002u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((32))); } } while (0);
  do { if (((0u) != ((status & (0x00000010u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((33))); } } while (0);
}