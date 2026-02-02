/*==================[testVoltMon_Init_u8.c]==================================*/
/**
 * @file    testVoltMon_Init_u8.c
 * @brief   Unit tests for VoltMon_Init_u8()
 */

#include "unity.h"

#include "VoltMon_Init_u8.h"  /* Function under test + Get/Set APIs */
#include "mock_VoltMon_cfg.h" /* Mock: VoltMon_CfgGet_pcfg() */

/*============================================================================*/
/* Test helper functions required by project rules                             */
/*============================================================================*/

void testVariablesAsset(uint32_t expStatusFlg_u32, VoltMon_mode_e expMode_e, uint16_t expLastRawAdc_u16, uint16_t expLastVoltage_mV_u16, bool expUvActive_b, bool expOvActive_b) {
  TEST_ASSERT_EQUAL_UINT32(expStatusFlg_u32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_INT((int)expMode_e, (int)VoltMon_GetMode_e());
  TEST_ASSERT_EQUAL_UINT16(expLastRawAdc_u16, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(expLastVoltage_mV_u16, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL(expUvActive_b, VoltMon_GetUvActive_b());
  TEST_ASSERT_EQUAL(expOvActive_b, VoltMon_GetOvActive_b());
}

void testVariableReset(uint32_t statusFlg_u32, VoltMon_mode_e mode_e, uint16_t lastRawAdc_u16, uint16_t lastVoltage_mV_u16, bool uvActive_b, bool ovActive_b) {
  VoltMon_SetStatusFlg_u32(statusFlg_u32);
  VoltMon_SetMode_e(mode_e);
  VoltMon_SetLastRawAdc_u16(lastRawAdc_u16);
  VoltMon_SetLastVoltage_mV_u16(lastVoltage_mV_u16);
  VoltMon_SetUvActive_b(uvActive_b);
  VoltMon_SetOvActive_b(ovActive_b);
}

/*============================================================================*/
/* Unity hooks                                                                 */
/*============================================================================*/

void setUp(void) {
  /* Reset all module state to guarantee test isolation */
  testVariableReset(0u, VoltMon_modeIdle_e, 0u, 0u, false, false);
}

void tearDown(void) { /* nothing */ }

/*============================================================================*/
/* Tests                                                                       */
/*============================================================================*/

void test_VoltMon_Init_u8_should_return_1_and_set_ERR_when_cfg_is_NULL(void) {
  /* Arrange */
  VoltMon_CfgGet_pcfg_ExpectAndReturn(NULL);

  /* Act */
  uint8_t ret_u8 = VoltMon_Init_u8();

  /* Assert */
  TEST_ASSERT_EQUAL_UINT8(1u, ret_u8);
  TEST_ASSERT_TRUE((VoltMon_GetStatusFlg_u32() & VOLTMON_STATUS_ERR_U32) != 0u);

  /* The function must still bring module into a deterministic safe base state */
  TEST_ASSERT_EQUAL_INT((int)VoltMon_modeIdle_e, (int)VoltMon_GetMode_e());
  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
  TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_VoltMon_Init_u8_should_return_1_and_set_ERR_when_rawMax_is_0(void) {
  /* Arrange */
  static const VoltMon_cfg_s cfg_s = {
      .rawMax_u16 = 0u,
      .factor_u16 = 1u,
      .offset_s16 = 0,
      .uvTh_mV_u16 = 1000u,
      .ovTh_mV_u16 = 5000u,
      .hyst_mV_u16 = 100u,
  };

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  /* Act */
  uint8_t ret_u8 = VoltMon_Init_u8();

  /* Assert */
  TEST_ASSERT_EQUAL_UINT8(1u, ret_u8);
  TEST_ASSERT_TRUE((VoltMon_GetStatusFlg_u32() & VOLTMON_STATUS_ERR_U32) != 0u);

  /* Deterministic safe base state */
  TEST_ASSERT_EQUAL_INT((int)VoltMon_modeIdle_e, (int)VoltMon_GetMode_e());
  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
  TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_VoltMon_Init_u8_should_return_0_set_INIT_and_clear_flags_when_cfg_is_valid(void) {
  /* Arrange */
  static const VoltMon_cfg_s cfg_s = {
      .rawMax_u16 = 4095u,
      .factor_u16 = 1u,
      .offset_s16 = 0,
      .uvTh_mV_u16 = 1000u,
      .ovTh_mV_u16 = 5000u,
      .hyst_mV_u16 = 100u,
  };

  /* Pre-load garbage state to verify the function really resets everything */
  testVariableReset(0xFFFFFFFFu, VoltMon_modeDiag_e, 1234u, 4321u, true, true);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  /* Act */
  uint8_t ret_u8 = VoltMon_Init_u8();

  /* Assert */
  TEST_ASSERT_EQUAL_UINT8(0u, ret_u8);

  /* Expected clean initialized state */
  testVariablesAsset(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 0u, 0u, false, false);
}
