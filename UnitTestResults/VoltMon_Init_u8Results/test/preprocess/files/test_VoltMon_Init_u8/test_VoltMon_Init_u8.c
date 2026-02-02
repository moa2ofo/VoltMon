// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_Init_u8.h"
#include "mock_VoltMon_cfg.h"

void testVariablesAsset(uint32_t expStatusFlg_u32, VoltMon_mode_e expMode_e, uint16_t expLastRawAdc_u16, uint16_t expLastVoltage_mV_u16,
                                                                                                                                        _Bool
                                                                                                                                             expUvActive_b,
                                                                                                                                                            _Bool
                                                                                                                                                                 expOvActive_b)
{
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expStatusFlg_u32)), (UNITY_INT)(UNITY_UINT32)((VoltMon_GetStatusFlg_u32())), (
 ((void *)0)
 ), (UNITY_UINT)(17), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(((int)expMode_e)), (UNITY_INT)(((int)VoltMon_GetMode_e())), (
 ((void *)0)
 ), (UNITY_UINT)(18), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((expLastRawAdc_u16)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(19), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((expLastVoltage_mV_u16)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(20), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)((expUvActive_b)), (UNITY_INT)((VoltMon_GetUvActive_b())), (
 ((void *)0)
 ), (UNITY_UINT)(21), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)((expOvActive_b)), (UNITY_INT)((VoltMon_GetOvActive_b())), (
 ((void *)0)
 ), (UNITY_UINT)(22), UNITY_DISPLAY_STYLE_INT);
}

void testVariableReset(uint32_t statusFlg_u32, VoltMon_mode_e mode_e, uint16_t lastRawAdc_u16, uint16_t lastVoltage_mV_u16,
                                                                                                                           _Bool
                                                                                                                                uvActive_b,
                                                                                                                                            _Bool
                                                                                                                                                 ovActive_b)
{
  VoltMon_SetStatusFlg_u32(statusFlg_u32);
  VoltMon_SetMode_e(mode_e);
  VoltMon_SetLastRawAdc_u16(lastRawAdc_u16);
  VoltMon_SetLastVoltage_mV_u16(lastVoltage_mV_u16);
  VoltMon_SetUvActive_b(uvActive_b);
  VoltMon_SetOvActive_b(ovActive_b);
}

void setUp(void)
{
  testVariableReset(0u, VoltMon_modeIdle_e, 0u, 0u,
                                                   0
                                                        ,
                                                          0
                                                               );
}

void tearDown(void)
{ }

void test_VoltMon_Init_u8_should_return_1_and_set_ERR_when_cfg_is_NULL(void)
{
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(51,
 ((void *)0)
 );

  uint8_t ret_u8 = VoltMon_Init_u8();

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((1u)), (UNITY_INT)(UNITY_UINT8 )((ret_u8)), (
 ((void *)0)
 ), (UNITY_UINT)(57), UNITY_DISPLAY_STYLE_UINT8);
  do { if (((VoltMon_GetStatusFlg_u32() & (0x00000002u)) != 0u))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((58))); } } while (0);

  UnityAssertEqualNumber((UNITY_INT)(((int)VoltMon_modeIdle_e)), (UNITY_INT)(((int)VoltMon_GetMode_e())), (
 ((void *)0)
 ), (UNITY_UINT)(61), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0u)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(62), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0u)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(63), UNITY_DISPLAY_STYLE_UINT16);
  do { if (!(VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((64))); } } while (0);
  do { if (!(VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((65))); } } while (0);
}

void test_VoltMon_Init_u8_should_return_1_and_set_ERR_when_rawMax_is_0(void)
{
  static const VoltMon_cfg_s cfg_s = {
      .rawMax_u16 = 0u,
      .factor_u16 = 1u,
      .offset_s16 = 0,
      .uvTh_mV_u16 = 1000u,
      .ovTh_mV_u16 = 5000u,
      .hyst_mV_u16 = 100u,
  };

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(79, &cfg_s);

  uint8_t ret_u8 = VoltMon_Init_u8();

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((1u)), (UNITY_INT)(UNITY_UINT8 )((ret_u8)), (
 ((void *)0)
 ), (UNITY_UINT)(85), UNITY_DISPLAY_STYLE_UINT8);
  do { if (((VoltMon_GetStatusFlg_u32() & (0x00000002u)) != 0u))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((86))); } } while (0);

  UnityAssertEqualNumber((UNITY_INT)(((int)VoltMon_modeIdle_e)), (UNITY_INT)(((int)VoltMon_GetMode_e())), (
 ((void *)0)
 ), (UNITY_UINT)(89), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0u)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(90), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0u)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(91), UNITY_DISPLAY_STYLE_UINT16);
  do { if (!(VoltMon_GetUvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((92))); } } while (0);
  do { if (!(VoltMon_GetOvActive_b()))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((93))); } } while (0);
}

void test_VoltMon_Init_u8_should_return_0_set_INIT_and_clear_flags_when_cfg_is_valid(void)
{
  static const VoltMon_cfg_s cfg_s = {
      .rawMax_u16 = 4095u,
      .factor_u16 = 1u,
      .offset_s16 = 0,
      .uvTh_mV_u16 = 1000u,
      .ovTh_mV_u16 = 5000u,
      .hyst_mV_u16 = 100u,
  };

  testVariableReset(0xFFFFFFFFu, VoltMon_modeDiag_e, 1234u, 4321u,
                                                                  1
                                                                      ,
                                                                        1
                                                                            );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(110, &cfg_s);

  uint8_t ret_u8 = VoltMon_Init_u8();

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((ret_u8)), (
 ((void *)0)
 ), (UNITY_UINT)(116), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAsset((0x00000001u), VoltMon_modeIdle_e, 0u, 0u,
                                                                         0
                                                                              ,
                                                                                0
                                                                                     );
}