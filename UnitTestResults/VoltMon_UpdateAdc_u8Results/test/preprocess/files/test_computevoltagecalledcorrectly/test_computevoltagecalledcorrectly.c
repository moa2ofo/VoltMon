// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

static const VoltMoncfgs *callbackcfgptr =
                                          ((void *)0)

static uint16t callbackcomputed_voltage = 0;

static const VoltMoncfgs *VoltMonCfgGetpcfgCallback(int callcount)
{
  (void)call_count;
  return callbackcfgptr;
}

static uint16t ComputeVoltageu16Callback(uint16t rawAdcu16, const VoltMoncfgs *cfgpcs, int call_count)
{
  (void)call_count;
  (void)rawAdc_u16;
  (void)cfg_pcs;
  return callbackcomputedvoltage;
}

void setUp(void)
{
  VoltMonSetStatusFlgu32(0);
  VoltMonSetModee(VoltMonmodeIdlee);
  VoltMonSetLastRawAdcu16(0);
  VoltMonSetLastVoltagemV_u16(0);
  VoltMonSetUvActiveb(
                     0
                          );
  VoltMonSetOvActiveb(
                     0
                          );

  callbackcfgptr =
                  ((void *)0)

  callbackcomputedvoltage = 0;
}

void tearDown(void)
{
}

void testcomputevoltagecalledcorrectly(void)
{
  uint8_t result;
  VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};

  VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
  callbackcfgptr = &cfg;
  callbackcomputedvoltage = 3050;

  VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
  ComputeVoltageu16ExpectAndReturn(300, &cfg, 3050);

  result = VoltMonUpdateAdcu8(300);

  TESTASSERTEQUAL_UINT8(0, result);
  TESTASSERTEQUALUINT16(300, VoltMonGetLastRawAdc_u16());
  TESTASSERTEQUALUINT16(3050, VoltMonGetLastVoltagemVu16());
  TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}