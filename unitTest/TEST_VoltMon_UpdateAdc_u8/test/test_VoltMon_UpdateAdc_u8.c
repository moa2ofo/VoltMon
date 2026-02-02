/*==================[test_VoltMon_UpdateAdc_u8.c]============================*
 * Unit tests for VoltMon_UpdateAdc_u8 (Ceedling + Unity + CMock)
 *
 * Assumption (per your update):
 * - ComputeVoltage_u16() is NOT static anymore, so it can be mocked via CMock
 *   by including mock_VoltMon_priv.h.
 *==========================================================================*/

#include "unity.h"
#include <stdbool.h>
#include <stdint.h>

#include "VoltMon.h"
#include "VoltMon_UpdateAdc_u8.h"
#include "VoltMon_cfg.h"
#include "VoltMon_priv.h"

/* Mocks */
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

/*==================[Helpers required by rules]==============================*/

static void testVariablesAssert(uint32_t expStatus_u32, VoltMon_mode_e expMode_e, uint16_t expLastRaw_u16, uint16_t expLastVoltage_u16, bool expUvActive_b, bool expOvActive_b) {
  TEST_ASSERT_EQUAL_UINT32(expStatus_u32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_INT((int)expMode_e, (int)VoltMon_GetMode_e());
  TEST_ASSERT_EQUAL_UINT16(expLastRaw_u16, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(expLastVoltage_u16, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT8((uint8_t)expUvActive_b, (uint8_t)VoltMon_GetUvActive_b());
  TEST_ASSERT_EQUAL_UINT8((uint8_t)expOvActive_b, (uint8_t)VoltMon_GetOvActive_b());
}

static void testVariablesReset(uint32_t status_u32, VoltMon_mode_e mode_e, uint16_t lastRaw_u16, uint16_t lastVoltage_u16, bool uvActive_b, bool ovActive_b) {
  VoltMon_SetStatusFlg_u32(status_u32);
  VoltMon_SetMode_e(mode_e);
  VoltMon_SetLastRawAdc_u16(lastRaw_u16);
  VoltMon_SetLastVoltage_mV_u16(lastVoltage_u16);
  VoltMon_SetUvActive_b(uvActive_b);
  VoltMon_SetOvActive_b(ovActive_b);
}

/*==================[Ceedling fixtures]=====================================*/

void setUp(void) { testVariablesReset(0u, VoltMon_modeIdle_e, 0u, 0u, false, false); }

void tearDown(void) {}

/*==================[Tests]==================================================*/

void test_VoltMon_UpdateAdc_u8_should_return_1_and_set_ERR_when_not_initialized(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 1000u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(0u, VoltMon_modeIdle_e, 11u, 22u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  TEST_ASSERT_EQUAL_UINT8(1u, VoltMon_UpdateAdc_u8(10u));

  testVariablesAssert(VOLTMON_STATUS_ERR_U32, VoltMon_modeIdle_e, 11u, 22u, false, false);
}

void test_VoltMon_UpdateAdc_u8_should_return_2_and_set_ERR_INVAL_when_cfg_is_NULL(void) {
  testVariablesReset(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 7u, 8u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn((const VoltMon_cfg_s *)0);

  TEST_ASSERT_EQUAL_UINT8(2u, VoltMon_UpdateAdc_u8(0u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 7u, 8u, false, false);
}

/* ---- Range / boundary tests from implementation rule: valid if raw <= rawMax ---- */

void test_VoltMon_UpdateAdc_u8_should_accept_raw_min_0_when_rawMax_allows_it(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 100u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 9u, 99u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  ComputeVoltage_u16_ExpectAndReturn(0u, (const VoltMon_cfg_s *)0, 111u);
  ComputeVoltage_u16_IgnoreArg_cfg_pcs();

  TEST_ASSERT_EQUAL_UINT8(0u, VoltMon_UpdateAdc_u8(0u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 0u, 111u, false, false);
}

void test_VoltMon_UpdateAdc_u8_should_accept_raw_equal_to_rawMax_as_valid_boundary(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 100u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 9u, 99u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  ComputeVoltage_u16_ExpectAndReturn(100u, (const VoltMon_cfg_s *)0, 4321u);
  ComputeVoltage_u16_IgnoreArg_cfg_pcs();

  TEST_ASSERT_EQUAL_UINT8(0u, VoltMon_UpdateAdc_u8(100u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 100u, 4321u, false, false);
}

void test_VoltMon_UpdateAdc_u8_should_return_2_and_set_ERR_INVAL_when_raw_is_rawMax_plus_1(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 100u;
  cfg_s.factor_u16 = 2u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 55u, 66u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  TEST_ASSERT_EQUAL_UINT8(2u, VoltMon_UpdateAdc_u8((uint16_t)(cfg_s.rawMax_u16 + 1u)));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 55u, 66u, false, false);
}

void test_VoltMon_UpdateAdc_u8_should_handle_corner_case_rawMax_0_only_raw_0_valid(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 0u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  /* raw=0 valid */
  testVariablesReset(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 77u, 88u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);
  ComputeVoltage_u16_ExpectAndReturn(0u, (const VoltMon_cfg_s *)0, 5u);
  ComputeVoltage_u16_IgnoreArg_cfg_pcs();

  TEST_ASSERT_EQUAL_UINT8(0u, VoltMon_UpdateAdc_u8(0u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 0u, 5u, false, false);

  /* raw=1 invalid */
  testVariablesReset(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 10u, 20u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  TEST_ASSERT_EQUAL_UINT8(2u, VoltMon_UpdateAdc_u8(1u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 10u, 20u, false, false);
}

void test_VoltMon_UpdateAdc_u8_should_accept_raw_uint16_max_when_rawMax_uint16_max(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 65535u;
  cfg_s.factor_u16 = 1u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 1u, 2u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  ComputeVoltage_u16_ExpectAndReturn(65535u, (const VoltMon_cfg_s *)0, 60000u);
  ComputeVoltage_u16_IgnoreArg_cfg_pcs();

  TEST_ASSERT_EQUAL_UINT8(0u, VoltMon_UpdateAdc_u8(65535u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32, VoltMon_modeIdle_e, 65535u, 60000u, false, false);
}

void test_VoltMon_UpdateAdc_u8_should_update_last_values_clear_INVAL_and_keep_ERR_if_it_was_already_set(void) {
  VoltMon_cfg_s cfg_s;
  cfg_s.rawMax_u16 = 500u;
  cfg_s.factor_u16 = 3u;
  cfg_s.offset_s16 = 0;
  cfg_s.uvTh_mV_u16 = 0u;
  cfg_s.ovTh_mV_u16 = 0u;
  cfg_s.hyst_mV_u16 = 0u;

  testVariablesReset(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_modeIdle_e, 1u, 2u, false, false);

  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg_s);

  ComputeVoltage_u16_ExpectAndReturn(50u, (const VoltMon_cfg_s *)0, 1234u);
  ComputeVoltage_u16_IgnoreArg_cfg_pcs();

  TEST_ASSERT_EQUAL_UINT8(0u, VoltMon_UpdateAdc_u8(50u));

  testVariablesAssert(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32, VoltMon_modeIdle_e, 50u, 1234u, false, false);
}
