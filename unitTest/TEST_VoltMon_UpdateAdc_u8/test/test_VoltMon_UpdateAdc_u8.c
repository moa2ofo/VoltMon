#include "unity.h"
#include <stdbool.h>
#include <stdint.h>
#include "VoltMon.h"
#include "VoltMon_UpdateAdc_u8.h"
#include "VoltMon_cfg.h"
#include "VoltMon_priv.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"




void test_VoltMon_UpdateAdc_u8_returns_invalid_param_for_rawAdc_above_max(void)
{
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    uint16_t rawAdc = 0xFFFF; // Large value assumed above rawMax_u16 for test
    uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retInvalidParam_e, ret);
}

void test_VoltMon_UpdateAdc_u8_returns_not_init_when_mode_is_idle(void)
{
    VoltMon_SetMode_e(VoltMon_modeIdle_e);
    uint16_t rawAdc = 100;
    uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retNotInit_e, ret);
}

void test_VoltMon_UpdateAdc_u8_updates_voltage_and_status_under_uv_threshold(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=10, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100};
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetLastRawAdc_u16(0);
    VoltMon_SetLastVoltage_mV_u16(0);
    VoltMon_SetUvActive_b(false);
    VoltMon_SetOvActive_b(false);
    // Setup config getter mock
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    // ComputeVoltage expects rawAdc_u16 and cfg pointer
    uint16_t computedVoltage = 900; // below uvTh_mV_u16 of 1000
    ComputeVoltage_u16_ExpectAndReturn(1234, &cfg, computedVoltage);
    bool uvActive, ovActive;
    // CheckThresholds expects voltage, cfg, &uvActive, &ovActive pointers
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = true;
            *ovActive_pb = false;
            return 0;
        });
    UpdateStatusFlags_v_Expect(true, false, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(1234);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_EQUAL_UINT16(1234, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(computedVoltage, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_TRUE(VoltMon_GetUvActive_b());
    TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_VoltMon_UpdateAdc_u8_updates_voltage_and_status_under_ov_threshold(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=10, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100};
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetUvActive_b(false);
    VoltMon_SetOvActive_b(false);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t computedVoltage = 3100; // above ovTh_mV_u16 3000
    ComputeVoltage_u16_ExpectAndReturn(2000, &cfg, computedVoltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = false;
            *ovActive_pb = true;
            return 0;
        });
    UpdateStatusFlags_v_Expect(false, true, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(2000);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
    TEST_ASSERT_TRUE(VoltMon_GetOvActive_b());
}

void test_VoltMon_UpdateAdc_u8_updates_voltage_and_status_with_no_thresholds_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=10, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100};
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetUvActive_b(true);
    VoltMon_SetOvActive_b(true);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t computedVoltage = 2000; // between thresholds
    ComputeVoltage_u16_ExpectAndReturn(1500, &cfg, computedVoltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = false;
            *ovActive_pb = false;
            return 0;
        });
    UpdateStatusFlags_v_Expect(false, false, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(1500);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
    TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_VoltMon_UpdateAdc_u8_returns_busy_when_mode_diag(void)
{
    VoltMon_SetMode_e(VoltMon_modeDiag_e);
    uint16_t rawAdc = 100;
    uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retBusyOrRejected_e, ret);
}

void test_VoltMon_UpdateAdc_u8_voltage_exactly_at_uv_threshold_sets_uv_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=1, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100 };
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t voltage = 1000; // exactly at UV threshold
    ComputeVoltage_u16_ExpectAndReturn(500, &cfg, voltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = true;
            *ovActive_pb = false;
            return 0;
        });
    UpdateStatusFlags_v_Expect(true, false, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(500);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_TRUE(VoltMon_GetUvActive_b());
}

void test_VoltMon_UpdateAdc_u8_voltage_just_below_uv_threshold_clears_uv_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=1, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100 };
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetUvActive_b(true);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t voltage = 999; // just below UV threshold
    ComputeVoltage_u16_ExpectAndReturn(499, &cfg, voltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = false;
            *ovActive_pb = false;
            return 0;
        });
    UpdateStatusFlags_v_Expect(false, false, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(499);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
}

void test_VoltMon_UpdateAdc_u8_voltage_just_above_uv_threshold_sets_uv_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=1, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100 };
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetUvActive_b(false);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t voltage = 1001; // just above UV threshold
    ComputeVoltage_u16_ExpectAndReturn(501, &cfg, voltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = true;
            *ovActive_pb = false;
            return 0;
        });
    UpdateStatusFlags_v_Expect(true, false, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(501);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_TRUE(VoltMon_GetUvActive_b());
}

void test_VoltMon_UpdateAdc_u8_voltage_exactly_at_ov_threshold_sets_ov_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=1, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100 };
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetOvActive_b(false);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t voltage = 3000; // exactly at OV threshold
    ComputeVoltage_u16_ExpectAndReturn(1500, &cfg, voltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = false;
            *ovActive_pb = true;
            return 0;
        });
    UpdateStatusFlags_v_Expect(false, true, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(1500);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_TRUE(VoltMon_GetOvActive_b());
}

void test_VoltMon_UpdateAdc_u8_voltage_just_below_ov_threshold_clears_ov_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=1, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100 };
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetOvActive_b(true);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t voltage = 2999; // just below OV threshold
    ComputeVoltage_u16_ExpectAndReturn(1499, &cfg, voltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = false;
            *ovActive_pb = false;
            return 0;
        });
    UpdateStatusFlags_v_Expect(false, false, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(1499);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_VoltMon_UpdateAdc_u8_voltage_just_above_ov_threshold_sets_ov_active(void)
{
    VoltMon_cfg_s cfg = { .rawMax_u16=4095, .factor_u16=1, .offset_s16=0, .uvTh_mV_u16=1000, .ovTh_mV_u16=3000, .hyst_mV_u16=100 };
    VoltMon_SetMode_e(VoltMon_modeRun_e);
    VoltMon_SetOvActive_b(false);
    VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
    uint16_t voltage = 3001; // just above OV threshold
    ComputeVoltage_u16_ExpectAndReturn(1501, &cfg, voltage);
    CheckThresholds_u8_StubWithCallback(
        [](uint16_t voltage_mV_u16, const VoltMon_cfg_s *cfg_pcs, bool *uvActive_pb, bool *ovActive_pb)->uint8_t {
            *uvActive_pb = false;
            *ovActive_pb = true;
            return 0;
        });
    UpdateStatusFlags_v_Expect(false, true, false);
    uint8_t ret = VoltMon_UpdateAdc_u8(1501);
    TEST_ASSERT_EQUAL_UINT8(VoltMon_retOk_e, ret);
    TEST_ASSERT_TRUE(VoltMon_GetOvActive_b());
}