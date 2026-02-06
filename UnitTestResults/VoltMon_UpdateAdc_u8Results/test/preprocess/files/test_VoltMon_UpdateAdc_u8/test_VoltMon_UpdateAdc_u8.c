// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

void test_VoltMon_UpdateAdc_u8_returns_invalid_param_for_rawAdc_above_max(void)
{
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  uint16_t rawAdc = 0xFFFF;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retInvalidParam_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(15), UNITY_DISPLAY_STYLE_UINT8);
}

void test_VoltMon_UpdateAdc_u8_returns_not_init_when_mode_is_idle(void)
{
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  uint16_t rawAdc = 100;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retNotInit_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(22), UNITY_DISPLAY_STYLE_UINT8);
}

void test_VoltMon_UpdateAdc_u8_updates_voltage_and_status_under_uv_threshold(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 10, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);
  VoltMon_SetUvActive_b(
                       0
                            );
  VoltMon_SetOvActive_b(
                       0
                            );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(33, &cfg);

  uint16_t computedVoltage = 900;
  ComputeVoltage_u16_CMockExpectAndReturn(36, 1234, &cfg, computedVoltage);

 _Bool
      uvActive, ovActive;

  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  1

    *ovActive_pb =
                  0

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(44,
 1
 ,
 0
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(1234);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((1234)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(47), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((computedVoltage)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(48), UNITY_DISPLAY_STYLE_UINT16);
  do { if ((VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((49))); } } while (0);
  do { if (!(VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((50))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_updates_voltage_and_status_under_ov_threshold(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 10, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetUvActive_b(
                       0
                            );
  VoltMon_SetOvActive_b(
                       0
                            );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(58, &cfg);
  uint16_t computedVoltage = 3100;
  ComputeVoltage_u16_CMockExpectAndReturn(60, 2000, &cfg, computedVoltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  0

    *ovActive_pb =
                  1

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(66,
 0
 ,
 1
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(2000);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(68), UNITY_DISPLAY_STYLE_UINT8);
  do { if (!(VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((69))); } } while (0);
  do { if ((VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((70))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_updates_voltage_and_status_with_no_thresholds_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 10, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetUvActive_b(
                       1
                           );
  VoltMon_SetOvActive_b(
                       1
                           );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(78, &cfg);
  uint16_t computedVoltage = 2000;
  ComputeVoltage_u16_CMockExpectAndReturn(80, 1500, &cfg, computedVoltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  0

    *ovActive_pb =
                  0

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(86,
 0
 ,
 0
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(1500);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(88), UNITY_DISPLAY_STYLE_UINT8);
  do { if (!(VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((89))); } } while (0);
  do { if (!(VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((90))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_returns_busy_when_mode_diag(void)
{
  VoltMon_SetMode_e(VoltMon_modeDiag_e);
  uint16_t rawAdc = 100;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retBusyOrRejected_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(97), UNITY_DISPLAY_STYLE_UINT8);
}

void test_VoltMon_UpdateAdc_u8_voltage_exactly_at_uv_threshold_sets_uv_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 1, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(103, &cfg);
  uint16_t voltage = 1000;
  ComputeVoltage_u16_CMockExpectAndReturn(105, 500, &cfg, voltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  1

    *ovActive_pb =
                  0

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(111,
 1
 ,
 0
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(500);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(113), UNITY_DISPLAY_STYLE_UINT8);
  do { if ((VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((114))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_voltage_just_below_uv_threshold_clears_uv_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 1, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetUvActive_b(
                       1
                           );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(121, &cfg);
  uint16_t voltage = 999;
  ComputeVoltage_u16_CMockExpectAndReturn(123, 499, &cfg, voltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  0

    *ovActive_pb =
                  0

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(129,
 0
 ,
 0
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(499);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(131), UNITY_DISPLAY_STYLE_UINT8);
  do { if (!(VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((132))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_voltage_just_above_uv_threshold_sets_uv_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 1, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetUvActive_b(
                       0
                            );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(139, &cfg);
  uint16_t voltage = 1001;
  ComputeVoltage_u16_CMockExpectAndReturn(141, 501, &cfg, voltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  1

    *ovActive_pb =
                  0

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(147,
 1
 ,
 0
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(501);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(149), UNITY_DISPLAY_STYLE_UINT8);
  do { if ((VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((150))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_voltage_exactly_at_ov_threshold_sets_ov_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 1, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetOvActive_b(
                       0
                            );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(157, &cfg);
  uint16_t voltage = 3000;
  ComputeVoltage_u16_CMockExpectAndReturn(159, 1500, &cfg, voltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  0

    *ovActive_pb =
                  1

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(165,
 0
 ,
 1
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(1500);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(167), UNITY_DISPLAY_STYLE_UINT8);
  do { if ((VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((168))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_voltage_just_below_ov_threshold_clears_ov_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 1, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetOvActive_b(
                       1
                           );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(175, &cfg);
  uint16_t voltage = 2999;
  ComputeVoltage_u16_CMockExpectAndReturn(177, 1499, &cfg, voltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  0

    *ovActive_pb =
                  0

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(183,
 0
 ,
 0
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(1499);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(185), UNITY_DISPLAY_STYLE_UINT8);
  do { if (!(VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((186))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_voltage_just_above_ov_threshold_sets_ov_active(void)
{
  VoltMon_cfg_s cfg = {.rawMax_u16 = 4095, .factor_u16 = 1, .offset_s16 = 0, .uvTh_mV_u16 = 1000, .ovTh_mV_u16 = 3000, .hyst_mV_u16 = 100};
  VoltMon_SetMode_e(VoltMon_modeRun_e);
  VoltMon_SetOvActive_b(
                       0
                            );
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(193, &cfg);
  uint16_t voltage = 3001;
  ComputeVoltage_u16_CMockExpectAndReturn(195, 1501, &cfg, voltage);
  CheckThresholds_u8_Stub([](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs,
                                                                                               _Bool
                                                                                                    *uvActive_pb,
                                                                                                                  _Bool
                                                                                                                       *ovActive_pb) -> uint8_t {
    *uvActive_pb =
                  0

    *ovActive_pb =
                  1

    return 0;
  });
  UpdateStatusFlags_v_CMockExpect(201,
 0
 ,
 1
 ,
 0
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(1501);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((VoltMon_retOk_e)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(203), UNITY_DISPLAY_STYLE_UINT8);
  do { if ((VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((204))); } } while (0);
}