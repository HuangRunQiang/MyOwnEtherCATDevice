#include <Arduino.h>

#include <stdio.h>
extern "C"
{
#include "ecat_slv.h"
#include "utypes.h"
};


HardwareSerial Serial1(PA10, PA9);
_Objects Obj;

#include "MyEncoder.h"
#define INDEX_PIN PA2
void indexPulseEncoderCB1(void);
MyEncoder Encoder1(INDEX_PIN, indexPulseEncoderCB1);
void indexPulseEncoderCB1(void)
{
   Encoder1.indexPulse();
}

#include "StepGen.h"
void timerCallbackStep1(void);
StepGen Step1(TIM1, 4, PA11, PA12, timerCallbackStep1);
void timerCallbackStep1(void)
{
   Step1.timerCB();
}

void cb_set_outputs(void) // Master outputs gets here, slave inputs, first operation
{
   if (Obj.IndexLatchEnable && !Encoder1.OldLatchCEnable) // Should only happen first time IndexCEnable is set
   {
      Encoder1.pleaseZeroTheCounter = 1;
   }
   Encoder1.OldLatchCEnable = Obj.IndexLatchEnable;

   if (Encoder1.CurPosScale != Obj.EncPosScale && Obj.EncPosScale != 0)
   {
      Encoder1.CurPosScale = Obj.EncPosScale;
      Encoder1.PosScaleRes = 1.0 / double(Encoder1.CurPosScale);
   }
   Step1.cmdPos(Obj.StepGenIn1.CommandedPosition);
}

void cb_get_inputs(void) // Set Master inputs, slave outputs, last operation
{
   Obj.IndexStatus = Encoder1.indexHappened();
   Obj.EncPos = Encoder1.currentPos();
   Obj.EncFrequency = Encoder1.frequency(ESCvar.Time);
   Obj.IndexByte = Encoder1.getIndexState();
    if (Obj.IndexByte)
      Serial1.printf("IS 1\n");
   Obj.StepGenOut1.ActualPosition = Step1.actPos();
   Obj.DiffT = 10000 * Step1.reqPos();
}

void ESC_interrupt_enable(uint32_t mask);
void ESC_interrupt_disable(uint32_t mask);
uint16_t dc_checker(void);
void sync0Handler(void);
void handleStepper(void);
void makePulses(uint64_t cycleTime /* in usecs */, int32_t pulsesAtEnd /* nr of pulses to do*/);

static esc_cfg_t config =
    {
        .user_arg = NULL,
        .use_interrupt = 1,
        .watchdog_cnt = 150,
        .set_defaults_hook = NULL,
        .pre_state_change_hook = NULL,
        .post_state_change_hook = NULL,
        .application_hook = handleStepper,
        .safeoutput_override = NULL,
        .pre_object_download_hook = NULL,
        .post_object_download_hook = NULL,
        .rxpdo_override = NULL,
        .txpdo_override = NULL,
        .esc_hw_interrupt_enable = ESC_interrupt_enable,
        .esc_hw_interrupt_disable = ESC_interrupt_disable,
        .esc_hw_eep_handler = NULL,
        .esc_check_dc_handler = dc_checker,
};

volatile byte serveIRQ = 0;

void setup(void)
{
   Serial1.begin(115200);
   rcc_config();

   Step1.setScale(500);
   Encoder1.init(Tim2);


   ecat_slv_init(&config);
}

void loop(void)
{
   ESCvar.PrevTime = ESCvar.Time;
   if (serveIRQ)
   {
      DIG_process(DIG_PROCESS_WD_FLAG | DIG_PROCESS_OUTPUTS_FLAG |
                  DIG_PROCESS_APP_HOOK_FLAG | DIG_PROCESS_INPUTS_FLAG);
      serveIRQ = 0;
   }
   ecat_slv_poll();
}

void sync0Handler(void)
{
   serveIRQ = 1;
}

void handleStepper(void)
{
   Step1.handleStepper();
}

void ESC_interrupt_enable(uint32_t mask)
{
   // Enable interrupt for SYNC0 or SM2 or SM3
   uint32_t user_int_mask = ESCREG_ALEVENT_DC_SYNC0 |
                            ESCREG_ALEVENT_SM2 |
                            ESCREG_ALEVENT_SM3;
   if (mask & user_int_mask)
   {
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() | (mask & user_int_mask));
      attachInterrupt(digitalPinToInterrupt(PC3), sync0Handler, RISING);

      // Set LAN9252 interrupt pin driver as push-pull active high
      uint32_t bits = 0x00000111;
      ESC_write(0x54, &bits, 4);

      // Enable LAN9252 interrupt
      bits = 0x00000001;
      ESC_write(0x5c, &bits, 4);
   }
}

void ESC_interrupt_disable(uint32_t mask)
{
   // Enable interrupt for SYNC0 or SM2 or SM3
   uint32_t user_int_mask = ESCREG_ALEVENT_DC_SYNC0 |
                            ESCREG_ALEVENT_SM2 |
                            ESCREG_ALEVENT_SM3;

   if (mask & user_int_mask)
   {
      // Disable interrupt from SYNC0
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() & ~(mask & user_int_mask));
      detachInterrupt(digitalPinToInterrupt(PC3));
      // Disable LAN9252 interrupt
      uint32_t bits = 0x00000000;
      ESC_write(0x5c, &bits, 4);
   }
}

extern "C" uint32_t ESC_SYNC0cycletime(void);
// Setup of DC
uint16_t dc_checker(void)
{
   // Indicate we run DC
   ESCvar.dcsync = 0;
   Step1.setCycleTime(ESC_SYNC0cycletime() / 1000); // nsec to usec
   return 0;
}
