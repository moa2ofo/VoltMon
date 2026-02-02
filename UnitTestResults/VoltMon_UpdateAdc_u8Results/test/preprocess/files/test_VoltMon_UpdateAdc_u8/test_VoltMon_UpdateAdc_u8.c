// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

static void testVariablesAssert(uint32_t expStatus_u32, VoltMon_mode_e expMode_e, uint16_t expLastRaw_u16, uint16_t expLastVoltage_u16,
                                                                                                                                       _Bool
                                                                                                                                            expUvActive_b,
                                                                                                                                                           _Bool
                                                                                                                                                                expOvActive_b)
{
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expStatus_u32)), (UNITY_INT)(UNITY_UINT32)((VoltMon_GetStatusFlg_u32())), (
 ((void *)0)
 ), (UNITY_UINT)(25), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(((int)expMode_e)), (UNITY_INT)(((int)VoltMon_GetMode_e())), (
 ((void *)0)
 ), (UNITY_UINT)(26), UNITY_DISPLAY_STYLE_INT);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((expLastRaw_u16)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((expLastVoltage_u16)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(28), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )(((uint8_t)expUvActive_b)), (UNITY_INT)(UNITY_UINT8 )(((uint8_t)VoltMon_GetUvActive_b())), (
 ((void *)0)
 ), (UNITY_UINT)(29), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )(((uint8_t)expOvActive_b)), (UNITY_INT)(UNITY_UINT8 )(((uint8_t)VoltMon_GetOvActive_b())), (
 ((void *)0)
 ), (UNITY_UINT)(30), UNITY_DISPLAY_STYLE_UINT8);
}

static void testVariablesReset(uint32_t status_u32, VoltMon_mode_e mode_e, uint16_t lastRaw_u16, uint16_t lastVoltage_u16,
                                                                                                                          _Bool
                                                                                                                               uvActive_b,
                                                                                                                                           _Bool
                                                                                                                                                ovActive_b)
{
  VoltMon_SetStatusFlg_u32(status_u32);
  VoltMon_SetMode_e(mode_e);
  VoltMon_SetLastRawAdc_u16(lastRaw_u16);
  VoltMon_SetLastVoltage_mV_u16(lastVoltage_u16);
  VoltMon_SetUvActive_b(uvActive_b);
  VoltMon_SetOvActive_b(ovActive_b);
}

void setUp(void)
{ testVariablesReset(0u, VoltMon_modeIdle_e, 0u, 0u,
                                                                     0
                                                                          ,
                                                                            0
                                                                                 ); }

void tearDown(void)
{}

void test_VoltMon_UpdateAdc_u8_should_return_1_and_set_ERR_when_not_initialized(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 1000u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(0u, VoltMon_modeIdle_e, 11u, 22u,
                                                      0
                                                           ,
                                                             0
                                                                  );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(61, &cfg_s);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((1u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(10u))), (
 ((void *)0)
 ), (UNITY_UINT)(63), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000002u), VoltMon_modeIdle_e, 11u, 22u,
                                                                           0
                                                                                ,
                                                                                  0
                                                                                       );
}

void test_VoltMon_UpdateAdc_u8_should_return_2_and_set_ERR_INVAL_when_cfg_is_NULL(void)
{
  testVariablesReset((0x00000001u), VoltMon_modeIdle_e, 7u, 8u,
                                                                         0
                                                                              ,
                                                                                0
                                                                                     );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(71, (const VoltMon_cfg_s *)0);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(0u))), (
 ((void *)0)
 ), (UNITY_UINT)(73), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u) | (0x00000002u) | (0x00000010u), VoltMon_modeIdle_e, 7u, 8u,
                                                                                                                              0
                                                                                                                                   ,
                                                                                                                                     0
                                                                                                                                          );
}

void test_VoltMon_UpdateAdc_u8_should_accept_raw_min_0_when_rawMax_allows_it(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 100u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset((0x00000001u) | (0x00000010u), VoltMon_modeIdle_e, 9u, 99u,
                                                                                                     0
                                                                                                          ,
                                                                                                            0
                                                                                                                 );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(91, &cfg_s);

  ComputeVoltage_u16_CMockExpectAndReturn(93, 0u, (const VoltMon_cfg_s *)0, 111u);
  ComputeVoltage_u16_CMockIgnoreArg_cfg_pcs(94);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(0u))), (
 ((void *)0)
 ), (UNITY_UINT)(96), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u), VoltMon_modeIdle_e, 0u, 111u,
                                                                            0
                                                                                 ,
                                                                                   0
                                                                                        );
}

void test_VoltMon_UpdateAdc_u8_should_accept_raw_equal_to_rawMax_as_valid_boundary(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 100u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset((0x00000001u) | (0x00000010u), VoltMon_modeIdle_e, 9u, 99u,
                                                                                                     0
                                                                                                          ,
                                                                                                            0
                                                                                                                 );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(112, &cfg_s);

  ComputeVoltage_u16_CMockExpectAndReturn(114, 100u, (const VoltMon_cfg_s *)0, 4321u);
  ComputeVoltage_u16_CMockIgnoreArg_cfg_pcs(115);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(100u))), (
 ((void *)0)
 ), (UNITY_UINT)(117), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u), VoltMon_modeIdle_e, 100u, 4321u,
                                                                               0
                                                                                    ,
                                                                                      0
                                                                                           );
}

void test_VoltMon_UpdateAdc_u8_should_return_2_and_set_ERR_INVAL_when_raw_is_rawMax_plus_1(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 100u;
  cfg_s.factor_u16 = 2u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset((0x00000001u), VoltMon_modeIdle_e, 55u, 66u,
                                                                           0
                                                                                ,
                                                                                  0
                                                                                       );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(133, &cfg_s);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8((uint16_t)(cfg_s.rawMax_u16 + 1u)))), (
 ((void *)0)
 ), (UNITY_UINT)(135), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u) | (0x00000002u) | (0x00000010u), VoltMon_modeIdle_e, 55u, 66u,
                                                                                                                                0
                                                                                                                                     ,
                                                                                                                                       0
                                                                                                                                            );
}

void test_VoltMon_UpdateAdc_u8_should_handle_corner_case_rawMax_0_only_raw_0_valid(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 0u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset((0x00000001u) | (0x00000010u), VoltMon_modeIdle_e, 77u, 88u,
                                                                                                      0
                                                                                                           ,
                                                                                                             0
                                                                                                                  );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(152, &cfg_s);
  ComputeVoltage_u16_CMockExpectAndReturn(153, 0u, (const VoltMon_cfg_s *)0, 5u);
  ComputeVoltage_u16_CMockIgnoreArg_cfg_pcs(154);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(0u))), (
 ((void *)0)
 ), (UNITY_UINT)(156), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u), VoltMon_modeIdle_e, 0u, 5u,
                                                                          0
                                                                               ,
                                                                                 0
                                                                                      );

  testVariablesReset((0x00000001u), VoltMon_modeIdle_e, 10u, 20u,
                                                                           0
                                                                                ,
                                                                                  0
                                                                                       );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(163, &cfg_s);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(1u))), (
 ((void *)0)
 ), (UNITY_UINT)(165), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u) | (0x00000002u) | (0x00000010u), VoltMon_modeIdle_e, 10u, 20u,
                                                                                                                                0
                                                                                                                                     ,
                                                                                                                                       0
                                                                                                                                            );
}

void test_VoltMon_UpdateAdc_u8_should_accept_raw_uint16_max_when_rawMax_uint16_max(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 65535u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset((0x00000001u) | (0x00000010u), VoltMon_modeIdle_e, 1u, 2u,
                                                                                                    0
                                                                                                         ,
                                                                                                           0
                                                                                                                );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(181, &cfg_s);

  ComputeVoltage_u16_CMockExpectAndReturn(183, 65535u, (const VoltMon_cfg_s *)0, 60000u);
  ComputeVoltage_u16_CMockIgnoreArg_cfg_pcs(184);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(65535u))), (
 ((void *)0)
 ), (UNITY_UINT)(186), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u), VoltMon_modeIdle_e, 65535u, 60000u,
                                                                                  0
                                                                                       ,
                                                                                         0
                                                                                              );
}

void test_VoltMon_UpdateAdc_u8_should_update_last_values_clear_INVAL_and_keep_ERR_if_it_was_already_set(void)
{
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 500u;
  cfg_s.factor_u16 = 3u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset((0x00000001u) | (0x00000002u) | (0x00000010u), VoltMon_modeIdle_e, 1u, 2u,
                                                                                                                             0
                                                                                                                                  ,
                                                                                                                                    0
                                                                                                                                         );

  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(202, &cfg_s);

  ComputeVoltage_u16_CMockExpectAndReturn(204, 50u, (const VoltMon_cfg_s *)0, 1234u);
  ComputeVoltage_u16_CMockIgnoreArg_cfg_pcs(205);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((VoltMon_UpdateAdc_u8(50u))), (
 ((void *)0)
 ), (UNITY_UINT)(207), UNITY_DISPLAY_STYLE_UINT8);

  testVariablesAssert((0x00000001u) | (0x00000002u), VoltMon_modeIdle_e, 50u, 1234u,
                                                                                                       0
                                                                                                            ,
                                                                                                              0
                                                                                                                   );
}