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

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_flag_if_not_initialized(void)
{
  const VoltMon_cfg_s *dummyCfg = (const VoltMon_cfg_s *)0x1;
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(27, dummyCfg);
  uint8_t ret = VoltMon_UpdateAdc_u8(100u);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((1u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(29), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  do { if (((0u) != ((status & (0x00000002u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((31))); } } while (0);
}