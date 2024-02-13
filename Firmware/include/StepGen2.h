
#ifndef STEPGEN
#define STEPGEN
#include <HardwareTimer.h>

class StepGen2
{
public:
    volatile double_t actualPosition;
    volatile int32_t nSteps;
    volatile uint32_t timerPulseSteps;
    volatile uint32_t timerFrequency;

public:
    volatile float Tstartf;                   // Starting delay in secs
    volatile uint32_t Tstartu;                // Starting delay in usecs
    volatile float Tpulses;                   // Time it takes to do pulses. Debug
    const float maxAllowedFrequency = 100000; // 100 kHz for now
    HardwareTimer *pulseTimer;
    uint32_t pulseTimerChan;
    HardwareTimer *startTimer; // 10,11,13,14
    uint8_t dirPin;
    PinName stepPin;
    const uint32_t Tjitter = 500; // Time unit is microseconds
    uint64_t dbg;

public:
    volatile double_t commandedPosition;    // End position when this cycle is completed
    volatile int32_t commandedStepPosition; // End step position when this cycle is completed
    volatile double_t initialPosition;      // From previous cycle
    volatile int32_t initialStepPosition;   // From previous cycle
    int16_t stepsPerMM;                     // This many steps per mm
    volatile uint8_t enabled;               // Enabled step generator
    volatile float frequency;

    static uint32_t sync0CycleTime; // Nominal EtherCAT cycle time
    volatile float lcncCycleTime;   // Linuxcnc nominal cycle time in sec (1 ms often)

    StepGen2(TIM_TypeDef *Timer, uint32_t _timerChannel, PinName _stepPin, uint8_t _dirPin, void irq(void), TIM_TypeDef *Timer2, void irq2(void));

    uint32_t handleStepper(uint64_t irqTime /* time for irq nanosecs */);
    void startTimerCB();
    void pulseTimerCB();
    uint32_t updatePos(uint32_t i);
};

class extend32to64
{
public:
    int64_t previousTimeValue = 0;
    const uint64_t ONE_PERIOD = 4294967296; // almost UINT32_MAX;
    const uint64_t HALF_PERIOD = 2147483648;
    int64_t extendTime(uint32_t in);
};

#endif