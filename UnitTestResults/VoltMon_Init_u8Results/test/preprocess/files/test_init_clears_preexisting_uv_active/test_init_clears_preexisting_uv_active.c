// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon_Init_u8.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg_ReturnNull(int call_count)
{
  return
        ((void *)0)
}

static VoltMon_cfg_s valid_cfg;

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg_ReturnValid(int call_count)
{
  valid_cfg.rawMax_u16 = 4095u;
  valid_cfg.factor_u16 = 10u;
  valid_cfg.offset_s16 = 0;
  valid_cfg.uvTh_mV_u16 = 9000u;
  valid_cfg.ovTh_mV_u16 = 16000u;
  valid_cfg.hyst_mV_u16 = 500u;
  return &valid_cfg;
}

void setUp(void)
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

void tearDown(void)
{
}

void test_init_clears_preexisting_uv_active(void)
{
  VoltMon_SetUvActive_b(
                       1
                           );

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  do { if (!(VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((48))); } } while (0);
}