// Button Swap taiHEN plugin
// Copyright (c) 2016 Scorp

#include <psp2/kernel/modulemgr.h>
#include <psp2/power.h>
#include <taihen.h>
static SceUID g_hooks[4];
static tai_hook_ref_t power_hook1;
static int power_patched1(int freq) {
    return TAI_CONTINUE(int, power_hook1, 444);
}

static tai_hook_ref_t power_hook2;
static int power_patched2(int freq) {
    return TAI_CONTINUE(int, power_hook2, 222);
}

static tai_hook_ref_t power_hook3;
static int power_patched3(int freq) {
    return TAI_CONTINUE(int, power_hook3, 222);
}

static tai_hook_ref_t power_hook4;
static int power_patched4(int freq) {
    return TAI_CONTINUE(int, power_hook4, 166);
}

void _start() __attribute__ ((weak, alias ("module_start")));

int module_start(SceSize argc, const void *args) {

 scePowerSetArmClockFrequency(444);
 scePowerSetBusClockFrequency(222);
 scePowerSetGpuClockFrequency(222);
 scePowerSetGpuXbarClockFrequency(166);

  g_hooks[0] = taiHookFunctionImport(&power_hook1, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0x74DB5AE5, // scePowerGetArmClockFrequency
                                      power_patched1);

  g_hooks[1] = taiHookFunctionImport(&power_hook2, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0xB8D7B3FB, // scePowerSetBusClockFrequency
                                      power_patched2);

  g_hooks[2] = taiHookFunctionImport(&power_hook3, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0x717DB06C, // scePowerSetGpuClockFrequency
                                      power_patched3);

  g_hooks[3] = taiHookFunctionImport(&power_hook4, 
                                      TAI_MAIN_MODULE,
                                      TAI_ANY_LIBRARY,
                                      0xA7739DBE, // scePowerSetGpuXbarClockFrequency
                                      power_patched4);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {

  // free hooks that didn't fail
  if (g_hooks[0] >= 0) taiHookRelease(g_hooks[0], power_hook1);
  if (g_hooks[1] >= 0) taiHookRelease(g_hooks[1], power_hook2);
  if (g_hooks[2] >= 0) taiHookRelease(g_hooks[2], power_hook3);
  if (g_hooks[3] >= 0) taiHookRelease(g_hooks[3], power_hook4);
  return SCE_KERNEL_STOP_SUCCESS;
}
