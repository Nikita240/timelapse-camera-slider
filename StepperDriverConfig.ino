#include "StepperDriverDef.h"

void stepperDriverConfig() {

  SPI.begin();                        

  stepperDriver.SPIPortConnect(&SPI);

  //The BUSY pin on the AutoDriver actually has two possible functions: it can indicate when the board is BUSY (usually indicating that a motion command is underway and has not yet completed) or it can be used to output a sync signal for counting full motor steps with an external device.
  //
  //There are constants defined for the two parameters: the first can be either BUSY_PIN or SYNC_PIN. If SYNC_PIN is passed, the second parameter should be one of the following:
  //
  //SYNC_FS_2 - two pulses on sync pin per full step of motor
  //SYNC_FS - one pulse per full step
  //SYNC_XFS - where X can be 2, 4, 8, 16, 32, or 64, and X indicates the number of full steps between pulses on the sync pin
  //If BUSY_PIN is passed, the second paramater should be zero.
  stepperDriver.configSyncPin(BUSY_PIN, 0);
  
  //The AutoDriver is capable of microstepping, wherein the output signal is PWMed to create a pseudo-sine wave output which makes the transition from one step to the next less jerky. There are 8 possible microstep options, and defines have been provided for selecting between them:
  //
  //STEP_FS - Full-step mode; microstepping disabled
  //STEP_FS_X - Enable microstepping with X microsteps per full step. X can be 2, 4, 8, 16, 32, 64, or 128.
  //Note that enabling microstepping has no effect on motion commands or sync pulse outputs; it is not possible to move less than one full step. Microstepping simply makes the transition between steps smoother.
  stepperDriver.configStepMode(STEP_FS_128); 

  //Provide an upper limit to the speed the driver will attempt to reach. Attempts to exceed this speed will result in motion being completed at this speed. The value established by this command will also be the value used for motion commands such as goTo() where no speed parameter is provided.
  stepperDriver.setMaxSpeed(SLIDER_MAX_SPEED);

  //The minimum speed is slowest speed the motor will run. If low speed optimization is enabled (see below), minimum speed is automatically zero, and the special low-speed waveform optimization will be used until minimum speed is reached. Defaults to zero.
  stepperDriver.setMinSpeed(0);

  //If microstepping is enabled, this parameter sets the speed above which microstepping is disabled and the driver engages full step mode.
  stepperDriver.setFullSpeed(SLIDER_MAX_SPEED);

  //Set the acceleration/deceleration curves to be used. The maximum value for this is 29802; above that, the AutoDriver will not use any curve at all.
  stepperDriver.setAcc(170);
  stepperDriver.setDec(170);

  //Sets the level at which an overcurrent event occurs. There are 16 different options; all take the format OC_XmA, where X is the limit and can be any of these values: 375, 750, 1125, 1500, 1875, 2250, 2625, 3000, 3375, 3750, 4125, 4500, 4875, 5250, 5625, or 6000.
  stepperDriver.setOCThreshold(OC_6000mA);
  
  //There’s a separate internal clock for the PWM frequency used by the chip when microstepping or when KVAL settings (more on these later) call for a reduction in current. This frequency is 31.3kHz (nominal, when using the internal 16MHz clock), and is adjusted by the divisor and multiplier sent to this function. Again, we’ve created a set of defines for the possible values:
  //
  //For divisor, define syntax is PWM_DIV_X, where X can be any value 1-7.
  //For multiplier, define syntax is PWM_MUL_X, where X can be 0_625 (for 0.625), 0_75 (for 0.75), 0_875, 1, 1_25, 1_5, 1_75, or 2.
  //It’s a good idea to keep the frequency above 20kHz or so, to avoid annoying those in close proximity to the device, as lower frequencies can cause an audible ring or buzz.
  stepperDriver.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq

  //The slew rate is the slope of the voltage change coming out of the driver. There are three options here: 180V/us, 290V/us, and 530V/us. Higher slew rates increase the torque at higher speeds, at the risk of increased electromagnetic emissions, which may or may not matter to you. The defines for this are SR_180V_us, SR_290V_us, SR_530V_us.
  stepperDriver.setSlewRate(SR_180V_us);   // Upping the edge speed increases torque.

  //By default, the drive transistors in the L6470 chip will shutdown on an overcurrent event to prevent damage to motor and driver. This can be disabled by passing the define OC_SD_DISABLE to this function, and re-enabled by passing OC_SD_ENABLE.
  stepperDriver.setOCShutdown(OC_SD_DISABLE);

  //This is one of the more important of the basic parameters. By default, the chip will run at 16MHz on its internal oscillator, and that suffices for most applications. However, in a situation where more than one AutoDriver is being used in a circuit, it’s best to drive all of the boards from a common clock, so the motors will remain synchronized. That clock source can be either an external clock fed to the first chip and then passed along to subsequent chips, or it can be the internal clock source of the first chip, passed along to later devices. There are rather a lot of possible options here; we’ve created a verbose set of constants to help you select the right one:
  //
  //INT_16MHZ - Use the internal 16MHz oscillator, with no output on the OSCOUT line.
  //INT_16MHZ_OSCOUT_2MHZ - Internal 16MHz, 2MHz on OSCOUT. Default.
  //INT_16MHZ_OSCOUT_4MHZ - Internal 16MHz, 4MHz on OSCOUT.
  //INT_16MHZ_OSCOUT_8MHZ - Internal 16MHz, 8MHz on OSCOUT.
  //INT_16MHZ_OSCOUT_16MHZ - Internal 16MHz, 16MHz on OSCOUT. Recommended for the first AutoDriver in a system with more than one AutoDriver.
  //EXT_8MHZ_XTAL_DRIVE - External 8MHz crystal. Not recommended.
  //EXT_16MHZ_XTAL_DRIVE - External 16MHz crystal. Not recommended.
  //EXT_24MHZ_XTAL_DRIVE - External 24MHz crystal. Not recommended.
  //EXT_32MHZ_XTAL_DRIVE - External 32MHz crystal. Not recommended.
  //EXT_8MHZ_OSCOUT_INVERT - 8MHz clock to OSCIN. Inverted OSCIN on OSCOUT.
  //EXT_16MHZ_OSCOUT_INVERT - 16MHz clock to OSCIN. Inverted OSCIN on OSCOUT. Recommended for subsequent boards in a multi-board system.
  //EXT_24MHZ_OSCOUT_INVERT - 24MHz clock to OSCIN. Inverted OSCIN on OSCOUT.
  //EXT_32MHZ_OSCOUT_INVERT - 32MHz clock to OSCIN. Inverted OSCIN on OSCOUT.
  //Two things of note regarding the osciallator settings: first, if you select an invalid setting (for example, an external crystal in a system with no crystal), the AutoDriver board will stop responding. Because the settings are stored in RAM, however, a reset or power cycle of the chip will restore it to operation, allowing you to change your program to a supported clock mode.
  //
  //Second, the frequency specified in this is used by the library to convert user-friendly units to units the chip understands. Using any frequency besides 16MHz will result in scale errors when setting speeds in steps per second, acceleration in steps per second per second, etc.
  stepperDriver.setOscMode(INT_16MHZ);

  //Voltage compensation attempts to keep the motor’s behavior consistent across varying supply voltage. This is not as straightforward as it sounds, and users wanting to employ this functionality are urged to consider page 34 of the L6470 datasheet.
  //
  //The defines to enable or disable this are VS_COMP_ENABLE and VS_COMP_DISABLE.
  stepperDriver.setVoltageComp(VS_COMP_DISABLE);

  //The switch input on the AutoDriver mode can be made to do one of two things: hard-stop the motor (for limit switch functionality), or perform user-based functions by exposing the switch mode to the user through an internal register. The constants to select between the modes are SW_HARD_STOP and SW_USER.  
  stepperDriver.setSwitchMode(SW_USER); //Disable hard stop

  //The KVAL settings allow you to impose a global scaling on the current used for the four conditions listed above. The input ranges from 0-255, or 0% to 100% in steps of approximately .4%. This can be a good way to reduce the power consumption of your system if the full torque provided by 100% current operation is not required.
  stepperDriver.setAccKVAL(50);           // We'll tinker with these later, if needed.
  stepperDriver.setDecKVAL(50);
  stepperDriver.setRunKVAL(50);
  stepperDriver.setHoldKVAL(30);           // This controls the holding current; keep it low.

  stepperDriver.setLoSpdOpt(false);

//  stepperDriver.move(REV, 232039);
}

