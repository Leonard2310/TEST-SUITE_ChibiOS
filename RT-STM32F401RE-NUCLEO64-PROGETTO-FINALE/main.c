// TEAM 23 PROJECT

// PA0 (A0)  buzzer TIM5 PWM
// PC6 PC8 PC9 LED RGB con TIM3 PWM
// PB7 PB6 (D10) encoder TIM4 INTERRUPT
// PB9 (D14) PB8 (D15) oled
// PA1 (A1) PA4 (A2) joystick

// LIBRERIE CHIBIOS:
#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "ssd1306.h"

// LIBRERIE C:
#include "string.h"                                 // Provides strcmp
#include "math.h"                                   // Provides sqrt, atan and pow
#include "stdlib.h"                                 // Provides atoi
#include "ctype.h"                                  // Provides isdigit

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
BaseSequentialStream *chp = (BaseSequentialStream*)&SD2;
BaseSequentialStream *stream = NULL;

// LED LINES
#define LED_RED_LINE    PAL_LINE(GPIOC, 6U)
#define LED_GREEN_LINE    PAL_LINE(GPIOC, 8U)
#define LED_BLUE_LINE    PAL_LINE(GPIOC, 9U)

// JOY LINES
#define JOY_X_LINE LINE_ADC1_IN1
#define JOY_Y_LINE LINE_ADC1_IN4

// BUZZER LINES
#define BUZZER_LINE    PAL_LINE(GPIOA, 0U)

// ADC COSTANTS
#define ADC_GRP_NUM_CHANNELS   2
#define ADC_GRP_BUF_DEPTH      16

static adcsample_t samples[ADC_GRP_NUM_CHANNELS * ADC_GRP_BUF_DEPTH];

#define BUFF_SIZE   20

static int red_led = 0;                             // RED LED Flag
static int green_led = 0;                           // GREEN LED Flag
static int blue_led = 0;                            // BLUE LED Flag

static int joy_flag = 0;                            // JOY Flag
static int cartesian = 0;                           // JOY CARTESIAN Flag
float degrees = 0;                                  // Angle in degrees
float cartesian_x = 0, cartesian_y = 0;             // x and y coordinates for JOY XY
float polar_rho = 0, polar_theta = 0;               // Radius and angle for JOY POLAR

static int oled_flag = 0;                           // OLED Flag
static int oled_flag_led = 0;                       // OLED LED Flag
char buff[BUFF_SIZE];

static int buzzPeriod = 0;                          // Buzzer period
static int ledPeriod = 0;                           // Led period

uint32_t counter = 0;                               // Number of periods
uint32_t last_width = 0;                            // Number of ticks of last pulse
uint32_t RGBCOLOR = 1;                              // RGB Color Flag

static int demo_flag = 0;                           // DEMO Flag
static float red_percentage = 0;                    // RED WHEEL percentage
static float green_percentage = 0;                  // GREEN WHEEL percentage
static float blue_percentage = 0;                   // BLUE WHEEL percentage


// I2C configuration structure
const I2CConfig i2ccfg = {OPMODE_I2C, 400000, FAST_DUTY_CYCLE_2};

// Display configuration structure
static const SSD1306Config ssd1306cfg = {
    &I2CD1,                                         //I2C Driver
    &i2ccfg,                                        //I2C Configuration structure
    SSD1306_SAD_0X78,                               //I2C Device address
    };

// Driver definition
static SSD1306Driver SSD1306D1;

// PWM Driver Configuration.
static PWMConfig pwmcfg = {
    10000,
    200,
    NULL,
    {
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_DISABLED, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_ACTIVE_HIGH, NULL} },
    0,
    0
};

//PWM buzzer configuration
static PWMConfig buzzpwmcfg = {
    10000,
    10000,
    NULL,
    {
      {PWM_OUTPUT_ACTIVE_HIGH, NULL},
      {PWM_OUTPUT_DISABLED, NULL},
      {PWM_OUTPUT_DISABLED, NULL},
      {PWM_OUTPUT_DISABLED, NULL}
    },
    0,
    0
};

/*  ADC conversion group.
    Mode:        Linear buffer, 16 samples of 2 channel, SW triggered
    Channels:    IN1 IN2                                                                  */
static const ADCConversionGroup adcgrpcfg = {
    FALSE,                                                            // circular     : Circular buffer mode
    ADC_GRP_NUM_CHANNELS,                                             // num_channels : Number of channels
    NULL,                                                             // end_cb       : End Callback
    NULL,                                                             // error_cb     : Error Callback
    0,                                                                // CR1
    ADC_CR2_SWSTART,                                                  // CR2
    ADC_SMPR1_SMP_AN11(ADC_SAMPLE_3),                                 // SMPR1
    0,                                                                // SMPR2
    0,                                                                // HTR
    0,                                                                // LTR
    0,                                                                // SQR1
    0,                                                                // SQR2
    ADC_SQR3_SQ2_N(ADC_CHANNEL_IN4)| ADC_SQR3_SQ1_N(ADC_CHANNEL_IN1)  // SQR3
};

static void cmd_led(BaseSequentialStream *chp, int argc, char *argv[]) {

  /*TS-REQ01-1: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC]
      If STATIC then [ON|OFF] else DYNAMIC [100÷1000] (ms)    */

  bool test = true;
  RGBCOLOR = 1;


  if (argc == 3) {
    if (!(strcmp(argv[0], "RED")) || !(strcmp(argv[0], "GREEN")) || !(strcmp(argv[0], "BLUE"))) {
      if (!(strcmp(argv[1], "STATIC"))) {   // Static case
        if (!(strcmp(argv[2], "ON"))) {

          palSetLineMode(LED_RED_LINE, PAL_MODE_OUTPUT_PUSHPULL);
          palClearLine(LED_RED_LINE);
          palSetLineMode(LED_GREEN_LINE, PAL_MODE_OUTPUT_PUSHPULL);
          palClearLine(LED_GREEN_LINE);
          palSetLineMode(LED_BLUE_LINE, PAL_MODE_OUTPUT_PUSHPULL);
          palClearLine(LED_BLUE_LINE);

          if (!(strcmp(argv[0], "RED"))) {
            // Clear flag
            ledPeriod = 0;
            // Disable pwm
            pwmDisableChannel(&PWMD3, 0);
            // Set mode static
            palSetLineMode(LED_RED_LINE, PAL_MODE_OUTPUT_PUSHPULL);
            // Led on
            palSetLine(LED_RED_LINE);
          }
          else if (!(strcmp(argv[0], "GREEN"))) {
            // Clear flag
            ledPeriod = 0;
            // Disable pwm
            pwmDisableChannel(&PWMD3, 2);
            // Set mode static
            palSetLineMode(LED_GREEN_LINE, PAL_MODE_OUTPUT_PUSHPULL);
            // Led on
            palSetLine(LED_GREEN_LINE);

          }
          else if (!(strcmp(argv[0], "BLUE"))) {
            // Clear flag
            ledPeriod = 0;
            // Disable pwm
            pwmDisableChannel(&PWMD3, 3);
            // Set mode static
            palSetLineMode(LED_BLUE_LINE, PAL_MODE_OUTPUT_PUSHPULL);
            // Led on
            palSetLine(LED_BLUE_LINE);
          }
        }
        else if (!(strcmp(argv[2], "OFF"))) {
          if (!(strcmp(argv[0], "RED"))) {
            //Clear flag
            ledPeriod = 0;
            //Disable pwm
            pwmDisableChannel(&PWMD3, 0);
            //Set mode static
            palSetLineMode(LED_RED_LINE, PAL_MODE_OUTPUT_PUSHPULL);
            //Led off
            palClearLine(LED_RED_LINE);
          }
          else if (!(strcmp(argv[0], "GREEN"))) {
            //Clear flag
            ledPeriod = 0;
            //Disable pwm
            pwmDisableChannel(&PWMD3, 2);
            //Set mode static
            palSetLineMode(LED_GREEN_LINE, PAL_MODE_OUTPUT_PUSHPULL);
            //Led off
            palClearLine(LED_GREEN_LINE);
          }
          else if (!(strcmp(argv[0], "BLUE"))) {
            //Clear flag
            ledPeriod = 0;
            //Disable pwm
            pwmDisableChannel(&PWMD3, 3);
            //Set mode static
            palSetLineMode(LED_BLUE_LINE, PAL_MODE_OUTPUT_PUSHPULL);
            //Led off
            palClearLine(LED_BLUE_LINE);
          }
        }
        else
          chprintf(chp, "Usage: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC] [ON-OFF|100-1000]\r\n");
      }
      else if (!(strcmp(argv[1], "DYNAMIC"))) {         // Dynamic case

        palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(2));
        palSetPadMode(GPIOC, 8, PAL_MODE_ALTERNATE(2));
        palSetPadMode(GPIOC, 9, PAL_MODE_ALTERNATE(2));
        pwmStart(&PWMD3, &pwmcfg);

        for (int i = 0; (argv[2][i] != '\0'); i++)
          if (!isdigit((int)argv[2][i]))
            test = false;

        if ((test == true) && (atoi(argv[2]) >= 100) && (atoi(argv[2]) <= 1000)) {
          if (!(strcmp(argv[0], "RED"))) {
            // Set mode dynamic
            palSetLineMode(LED_RED_LINE, PAL_MODE_ALTERNATE(2));
            // Set period
            ledPeriod = atoi(argv[2]);
            // Change period
            pwmChangePeriod(&PWMD3, 10 * ledPeriod);
            // Enable pwm
            pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));

            red_led = 1;
            green_led = 0;
            blue_led = 0;
          }
          else if (!(strcmp(argv[0], "GREEN"))) {
            // Set mode dynamic
            palSetLineMode(LED_GREEN_LINE, PAL_MODE_ALTERNATE(2));
            // Set period
            ledPeriod = atoi(argv[2]);
            // Change period
            pwmChangePeriod(&PWMD3, 10 * ledPeriod);
            // Enable pwm
            pwmEnableChannel(&PWMD3, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));

            red_led = 0;
            green_led = 1;
            blue_led = 0;
          }
          else if (!(strcmp(argv[0], "BLUE"))) {
            // Set mode dynamic
            palSetLineMode(LED_BLUE_LINE, PAL_MODE_ALTERNATE(2));
            // Set period
            ledPeriod = atoi(argv[2]);
            // Change period
            pwmChangePeriod(&PWMD3, 10 * ledPeriod);
            // Enable pwm
            pwmEnableChannel(&PWMD3, 3, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));

            red_led = 0;
            green_led = 0;
            blue_led = 1;
          }
        }
        else
          chprintf(chp, "Usage: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC] [ON-OFF|100-1000]\r\n");
      }
      else
        chprintf(chp, "Usage: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC] [ON-OFF|100-1000]\r\n");
    }
    else
      chprintf(chp, "Usage: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC] [ON-OFF|100-1000]\r\n");
  }
  else
    chprintf(chp, "Usage: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC] [ON-OFF|100-1000]\r\n");
}

// Joy function
static void cmd_joy(BaseSequentialStream *chp, int argc, char *argv[]) {

  /*TS-REQ01-2: JOY [XY|POLAR]
      Once activated, until the escape sequence is activated, the following are shown:
        - if in XY: the Cartesian coordinates relative to the joypad position
        - if in POLAR: the polar coordinates relative to the joypad position              */

  if (argc == 1) {
    if (!(strcmp(argv[0], "XY")) || !(strcmp(argv[0], "POLAR"))) {
      if (!(strcmp(argv[0], "XY")))
        cartesian = 1;
      else
        cartesian = 0;
      stream = chp;
      joy_flag = 1;
    }
    else if (!(strcmp(argv[0], "OFF")))
      joy_flag = 0;
    else
      chprintf(chp, "Usage: JOY [XY|POLAR]\r\n");
  }
  else
    chprintf(chp, "Usage: JOY [XY|POLAR]\r\n");
}

// Calculate polar and Cartesian coordinates
void acquire_joy(void) {

  float avg_sample_x = 0, avg_sample_y = 0;

  // Start synchronous conversion
  adcConvert(&ADCD1, &adcgrpcfg, samples, ADC_GRP_BUF_DEPTH);
  // Average acquired values
  for (int i = 0; i < ADC_GRP_BUF_DEPTH; i++)
    if (i % 2 == 0)
      avg_sample_x += samples[i];
    else
      avg_sample_y += samples[i];
  avg_sample_x /= (ADC_GRP_BUF_DEPTH / 2);
  avg_sample_y /= (ADC_GRP_BUF_DEPTH / 2);

  // Convert average values in cartesian form
  cartesian_x = 2 * (avg_sample_x / 4095) - 1;
  cartesian_y = 2 * ((-avg_sample_y + 4095) / 4095) - 1;

  // Convert cartesian into polar
  polar_rho = sqrt(pow(cartesian_x, 2) + pow(cartesian_y, 2));
  if (cartesian_x != 0 && cartesian_y != 0) {
    polar_theta = 2 * atan(cartesian_y / (polar_rho + cartesian_x));
    if (polar_rho < 0.1)
        degrees= 0;
    else
        degrees = polar_theta * 57.295779513082320876798154814105;
  }
  else {
    polar_theta = 0;
    degrees=0;
  }
}

// JOYSTICK THREAD
static THD_WORKING_AREA(waJoy, 128);
static THD_FUNCTION(ThJoy, arg) {

  (void)arg;
  chRegSetThreadName("joy");

  while (true) {
    // Thread continues as far as a button is pushed
    while (joy_flag == 1) {
      if (cartesian)
        chprintf(stream, "Cartesian coordinates:\r\n");
      else
        chprintf(stream, "Polar coordinates:\r\n");
      while (joy_flag == 1) {
        // Acquire values from ADC
        acquire_joy();
        // Print values
        if (cartesian) {
          // Print cartesian
          chprintf(stream, "Value X = %.2f\r\n", cartesian_x);
          chprintf(stream, "Value Y = %.2f\r\n", cartesian_y);
        }
        else {
          // Print polar
          chprintf(stream, "Value Rho = %.2f\r\n", polar_rho);
          chprintf(stream, "Value Theta = %.2f°\r\n", degrees);
        }
        chThdSleepMilliseconds(2000);
      }
    }
    chThdSleepMilliseconds(1000);
  }
}

// Oled function
static void cmd_oled(BaseSequentialStream *chp, int argc, char *argv[]) {

  // Using the SSD1306 OLED Display
  /* • TS-REQ01-3: OLED [LED|JOY]
   *       Once activated, the status of the LED or the JOYPAD is printed on the oled-display */

  if (argc == 1) {   // Right number of parameters
    if (!(strcmp(argv[0], "LED")) || !(strcmp(argv[0], "JOY"))) {
      if (!(strcmp(argv[0], "LED"))) {
        oled_flag_led = 1;
        demo_flag = 0;
      }
      else
        oled_flag_led = 0;
      oled_flag = 1;
      demo_flag = 0;
    }
    else
      chprintf(chp, "Usage: OLED [LED|JOY]\r\n");
  }
  else
    chprintf(chp, "Usage: OLED [LED|JOY]\r\n");
}

// Oled print function
void oled(void) {
  if (oled_flag_led == 1) {
    ssd1306GotoXy(&SSD1306D1, 0, 1);
    chsnprintf(buff, BUFF_SIZE, "LED STATUS ");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);
    if (ledPeriod == 0) {
      if (palReadLine(LED_RED_LINE) == PAL_HIGH) {
        ssd1306GotoXy(&SSD1306D1, 0, 20);
        chsnprintf(buff, BUFF_SIZE, "LED RED ON     ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
      }
      else {
        ssd1306GotoXy(&SSD1306D1, 0, 20);
        chsnprintf(buff, BUFF_SIZE, "LED RED OFF      ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
      }
    if (palReadLine(LED_GREEN_LINE) == PAL_HIGH) {
        ssd1306GotoXy(&SSD1306D1, 0, 35);
        chsnprintf(buff, BUFF_SIZE, "LED GREEN ON       ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
      }
    else {
      ssd1306GotoXy(&SSD1306D1, 0, 35);
      chsnprintf(buff, BUFF_SIZE, "LED GREEN OFF        ");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);
    }
    if (palReadLine(LED_BLUE_LINE) == PAL_HIGH) {
      ssd1306GotoXy(&SSD1306D1, 0, 50);
      chsnprintf(buff, BUFF_SIZE, "LED BLUE ON        ");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);
    }
    else {
      ssd1306GotoXy(&SSD1306D1, 0, 50);
      chsnprintf(buff, BUFF_SIZE, "LED BLUE OFF     ");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);
    }
    }
    else {
      if ( red_led == 1) {
        ssd1306GotoXy(&SSD1306D1, 0, 20);
        chsnprintf(buff, BUFF_SIZE, "LED RED P: %i      ", ledPeriod);
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
        ssd1306GotoXy(&SSD1306D1, 0, 35);
        chsnprintf(buff, BUFF_SIZE, "LED GREEN OFF       ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
        ssd1306GotoXy(&SSD1306D1, 0, 50);
        chsnprintf(buff, BUFF_SIZE, "LED BLUE OFF     ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
      }
      else if (green_led == 1) {
        ssd1306GotoXy(&SSD1306D1, 0, 20);
        chsnprintf(buff, BUFF_SIZE, "LED RED OFF      ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
        ssd1306GotoXy(&SSD1306D1, 0, 35);
        chsnprintf(buff, BUFF_SIZE, "LED GREEN P: %i        ", ledPeriod);
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
        ssd1306GotoXy(&SSD1306D1, 0, 50);
        chsnprintf(buff, BUFF_SIZE, "LED BLUE OFF     ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
      }
      else if (blue_led == 1) {
        ssd1306GotoXy(&SSD1306D1, 0, 20);
        chsnprintf(buff, BUFF_SIZE, "LED RED OFF      ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
        ssd1306GotoXy(&SSD1306D1, 0, 35);
        chsnprintf(buff, BUFF_SIZE, "LED GREEN OFF       ");
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
        ssd1306GotoXy(&SSD1306D1, 0, 50);
        chsnprintf(buff, BUFF_SIZE, "LED BLUE P: %i     ", ledPeriod);
        ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
        ssd1306UpdateScreen(&SSD1306D1);
      }
    }
  }
  else {
    ssd1306GotoXy(&SSD1306D1, 0, 1);
    chsnprintf(buff, BUFF_SIZE, "COORDINATES ");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);

    acquire_joy();

    ssd1306GotoXy(&SSD1306D1, 0, 20);
    chsnprintf(buff, BUFF_SIZE, "X: %.2f  ", cartesian_x);
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
    chsnprintf(buff, BUFF_SIZE, "Y: %.2f  ", cartesian_y);
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);

    ssd1306GotoXy(&SSD1306D1, 0, 35);
    chsnprintf(buff, BUFF_SIZE, "RADIUS: %.2f   ", polar_rho);
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);

    ssd1306GotoXy(&SSD1306D1, 0, 50);
    chsnprintf(buff, BUFF_SIZE, "ANGLE: %.2f deg      ", degrees);
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);
  }

  ssd1306UpdateScreen(&SSD1306D1);
  chThdSleepMilliseconds(20);
}

// OLED THREAD
static THD_WORKING_AREA(waOledDisplay, 512);
static THD_FUNCTION(OledDisplay, arg) {
  (void)arg;

  chRegSetThreadName("OledDisplay");

  while (true) {
    while (oled_flag == 1) {
      ssd1306FillScreen(&SSD1306D1, 0x00);
      oled_flag = 2;
      while (oled_flag == 2)
        oled();
    }
    chThdSleepMilliseconds(20);
  }
}

// Oled initialization
void oled_init(void) {
  chThdCreateStatic(waOledDisplay, sizeof(waOledDisplay), NORMALPRIO, OledDisplay, NULL);

  // Initialize SSD1306 Display Driver Object.
  ssd1306ObjectInit(&SSD1306D1);

  // Start the SSD1306 Display Driver Object with configuration.
  ssd1306Start(&SSD1306D1, &ssd1306cfg);

  ssd1306FillScreen(&SSD1306D1, 0x00);

  ssd1306GotoXy(&SSD1306D1, 15, 0);
  chsnprintf(buff, BUFF_SIZE, "NISC 2021");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);

  ssd1306GotoXy(&SSD1306D1, 12, 20);
  chsnprintf(buff, BUFF_SIZE, "Project Team 23");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);

  ssd1306GotoXy(&SSD1306D1, 15, 40);
  chsnprintf(buff, BUFF_SIZE, "INSERT COMMAND");
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);

}

// Buzz function
static void cmd_buzz(BaseSequentialStream *chp, int argc, char *argv[]) {

  /*TS-REQ01-4: BUZZ [ON|OFF] [PERIOD]
      A buzzer tone is reproduced, with the period expressed in whole multiples of the second       */

  pwmStart(&PWMD5, &buzzpwmcfg);
  bool test = true;

  if (argc == 1) {
    if (!(strcmp(argv[0], "OFF")))
      pwmDisableChannel(&PWMD5, 0);
    else
      chprintf(chp, "Usage: BUZZ [ON|OFF] [PERIOD]\r\n");
  }
  else if (argc == 2) {
    if (!(strcmp(argv[0], "ON"))) {
      for (int i = 0; (argv[1][i] != '\0'); i++)
        if (!isdigit((int)argv[1][i]))
          test = false;

      if (test == true) {
        palSetLineMode(BUZZER_LINE, PAL_MODE_ALTERNATE(2));

        buzzPeriod = atoi(argv[1]);
        pwmChangePeriod(&PWMD5, 10000 * buzzPeriod);

        // This enables 0 channel with a new duty cycle
        pwmEnableChannel(&PWMD5, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, 5000));
      }
      else {
        chprintf(chp, "Error: PERIOD is not a number\r\n");
        chprintf(chp, "Usage: BUZZ [ON|OFF] [PERIOD]\r\n");
        test = true;
      }
    }
    else
      chprintf(chp, "Usage: BUZZ [ON|OFF] [PERIOD]\r\n");
  }
  else
    chprintf(chp, "Usage: BUZZ [ON|OFF] [PERIOD]\r\n");
}

// ENCODER THREAD
static THD_WORKING_AREA(waEncoder, 128);
static THD_FUNCTION(thEncoder, arg) {
  (void)arg;
  chRegSetThreadName("Encoder");

  stm32_tim_t *enc = STM32_TIM4;

  palSetPadMode(GPIOB, 6, PAL_MODE_ALTERNATE(2) | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUPDR_PULLUP); // Setting encoders pin

  palSetPadMode(GPIOB, 7, PAL_MODE_ALTERNATE(2) | PAL_STM32_OTYPE_PUSHPULL | PAL_STM32_PUPDR_PULLUP); // Setting encoders pin

  rccEnableTIM4(TRUE);

  enc->SMCR = 3;        // Encoder mode 3
  enc->CCER = 0;        // Rising edge polarity
  enc->ARR = 0xffff;    // Count from 0-ARR or ARR-0
  enc->CCMR1 = 0xc1c1;  // f_DTS/16, N=8 IC1->TI1, IC2->TI2
  enc->CNT = 0;         // Initialize counter
  enc->EGR = 1;         // Generate an update event
  enc->CR1 = 1;         // Enable the counter

  while (TRUE) {
    if (counter != (enc->CNT >> 2)) {
      counter = enc->CNT >> 2; // read encoder counter state
      if (counter >= 37)
      {
        enc->CNT = 0;  // Restrict counter between 0 and 36
        counter = enc->CNT >> 2; //update counter value to ensure correct reading
      }
    }
    chThdSleepMilliseconds(100);
  }
}

// DIMMER THREAD
static THD_WORKING_AREA(wadimmer, 128);
static THD_FUNCTION(dimmer, arg) {
  (void)arg;
  chRegSetThreadName("dimmer");

  while (true ) {

    if (RGBCOLOR != 1)   // Check for active dimmer flag
    {
      pwmDisableChannel(&PWMD3, 0);   // Restore PWM pin states
      pwmDisableChannel(&PWMD3, 2);
      pwmDisableChannel(&PWMD3, 3);
      pwmEnableChannel(&PWMD3, RGBCOLOR, PWM_FRACTION_TO_WIDTH(&PWMD3, 36000, counter * 1000));   // Set PWM state of RGB led
      chThdSleepMilliseconds(100);
    }
    else chThdSleepMilliseconds(100);
  }
}

// Dimmer function
static void cmd_dimmer(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)chp;

  if (argc == 2) {
    // Enable Hardware PWM for PINs C6-C8-C9
    palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOC, 8, PAL_MODE_ALTERNATE(2));
    palSetPadMode(GPIOC, 9, PAL_MODE_ALTERNATE(2));

    // Configure PWM
    pwmStart(&PWMD3, &pwmcfg);

    if (!(strcmp(argv[0], "ON"))  || !(strcmp(argv[0], "OFF"))) {
          if (!(strcmp(argv[0], "ON"))) {
              if (strcmp(argv[1], "RED") == 0)
                RGBCOLOR = 0; // Enable channel 0 of TIM3 connected with RED led
              else
              if (strcmp(argv[1], "BLUE") == 0)
                RGBCOLOR = 3; // Enable channel 3 of TIM3 connected with BLUE led
              else
              if (strcmp(argv[1], "GREEN") == 0)
                RGBCOLOR = 2; // Enable channel 2 of TIM3 connected with GREEN led
              else
                    chprintf(chp, "Usage: DIMMER [ON|OFF] [RED|GREEN|BLUE]\r\n");
          }
          else
          if (!(strcmp(argv[0], "OFF"))) {
              if (strcmp(argv[1], "RED") == 0)
                RGBCOLOR = 1;       // Set inactive dimmer flag
              else if (strcmp(argv[1], "BLUE") == 0)
                RGBCOLOR = 1;       // Set inactive dimmer flag
              else if (strcmp(argv[1], "GREEN") == 0)
                RGBCOLOR = 1;       // Set inactive dimmer flag
              else
                  chprintf(chp, "Usage: DIMMER [ON|OFF] [RED|GREEN|BLUE]\r\n");
          }
        }
         else
             chprintf(chp, "Usage: DIMMER [ON|OFF] [RED|GREEN|BLUE]\r\n");
  }
  else
       chprintf(chp, "Usage: DIMMER [ON|OFF] [RED|GREEN|BLUE]\r\n");
}


// Demo function
static void cmd_demo(BaseSequentialStream *chp, int argc, char *argv[]) {

  /*TS-REQ02-1: DEMO
      Implement a color wheel with OLED-display and buzzer.
      The buzzer emits a tone when one of the three primary colors is reached and the percentage
      of the single dominant colors (simultaneously) is shown on the display.                           */

  (void)chp;
  (void)argv;

  if (argc == 0) {
      demo_flag = 1;

      // Disable other threads
      joy_flag = 0;
      oled_flag = 0;
  }
  else
      chprintf(chp, "Usage: DEMO\r\n");
}

// DEMO THREAD
static THD_WORKING_AREA(waDemo, 128);
static THD_FUNCTION(ThDemo, arg) {

  int count= 0;

  (void)arg;
  chRegSetThreadName("demo");

  while(true){                                                      // Thread continues as far as a button is pushed
    while (demo_flag == 1) {

      if (count == 0) {                                             // Oled cleanup
        ssd1306FillScreen(&SSD1306D1, 0x00);
        count++;
       }

      // Acquire coordinates
      acquire_joy();

      // Compute percentage of different colours
      if (polar_rho <= 0.1) {
         green_percentage = 0;
         red_percentage = 0;
         blue_percentage = 0;
      }
      else if((polar_theta >= -0.79) && (polar_theta <= 1.57)){     // Red-Blue sector
        red_percentage = 100*((polar_theta+0.79)/2.36);
        blue_percentage = 100 - red_percentage;
        green_percentage = 0;
      }
      else if((polar_theta <= -0.79) && (polar_theta >= -2.36)){    // Green-Blue sector
        blue_percentage = 100*((polar_theta+2.36)/1.57);
        green_percentage = 100 - blue_percentage;
        red_percentage = 0;
      }
      else if(polar_theta >= 1.57){                                 // Green-Red sector
        green_percentage = 67*((polar_theta-1.57)/1.57);
        red_percentage = 100 - green_percentage;
        blue_percentage = 0;
      }
      else if(polar_theta <= -2.36){                                // Green-Red sector
        green_percentage = 67 + 33*((polar_theta+3.14)/0.78);
        red_percentage = 100 - green_percentage;
        blue_percentage = 0;
      }

      // Print coordinates on OLED
      ssd1306GotoXy(&SSD1306D1, 0, 1);
      chsnprintf(buff, BUFF_SIZE, "COLOR WHEEL");
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);

      ssd1306GotoXy(&SSD1306D1, 0, 20);
      chsnprintf(buff, BUFF_SIZE, "RED %%: %.2f %%      ", red_percentage);
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);

      ssd1306GotoXy(&SSD1306D1, 0, 35);
      chsnprintf(buff, BUFF_SIZE, "GREEN %%: %.2f %%    ", green_percentage);
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);

      ssd1306GotoXy(&SSD1306D1, 0, 50);
      chsnprintf(buff, BUFF_SIZE, "BLUE %%: %.2f %%     ", blue_percentage);
      ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
      ssd1306UpdateScreen(&SSD1306D1);

      // Check if one of the primary color is reached
      if((red_percentage >= 97) || (green_percentage >= 97) || (blue_percentage >= 97)) {
        // Set buzz mode static
        palSetLineMode(BUZZER_LINE, PAL_MODE_OUTPUT_PUSHPULL);
        // Buzz on
        palSetLine(BUZZER_LINE);
      }
      else{
        // Set buzz mode static
        palSetLineMode(BUZZER_LINE, PAL_MODE_OUTPUT_PUSHPULL);
        // Buzz off
        palClearLine(BUZZER_LINE);
      }
      chThdSleepMilliseconds(20);
    }
    chThdSleepMilliseconds(1000);
  }
}

// SHELL COMMANDS
static const ShellCommand commands[] = {
    {"LED", cmd_led},
    {"JOY", cmd_joy},
    {"OLED", cmd_oled},
    {"BUZZ", cmd_buzz},
    {"DIMMER", cmd_dimmer},
    {"DEMO", cmd_demo},
    {NULL, NULL}
};

// Shell configuration
static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream*)&SD2,
    commands
};

//Application entry point.
int main(void) {
  // SYSTEM INITIALIZATIONS
  halInit();            // HAL initialization
  chSysInit();          // KERNEL initialization

  // OLED PINs initialization
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUPDR_PULLUP);

  shellInit();          // Shell manager initialization.

  oled_init();          // Oled initialization

  sdStart(&SD2, NULL);  // Serial ports initialization.

  // BUZZER PIN initialization
  palSetLineMode(BUZZER_LINE, PAL_MODE_ALTERNATE(2));

  // JOY Analog Input
  palSetLineMode(JOY_X_LINE, PAL_MODE_INPUT_ANALOG);
  palSetLineMode(JOY_Y_LINE, PAL_MODE_INPUT_ANALOG);

  // LED PINs initialization
  palSetLineMode(LED_RED_LINE, PAL_MODE_OUTPUT_PUSHPULL);
  palClearLine(LED_RED_LINE);
  palSetLineMode(LED_GREEN_LINE, PAL_MODE_OUTPUT_PUSHPULL);
  palClearLine(LED_GREEN_LINE);
  palSetLineMode(LED_BLUE_LINE, PAL_MODE_OUTPUT_PUSHPULL);
  palClearLine(LED_BLUE_LINE);

  // Start ADC
  adcStart(&ADCD1, NULL);

  // Create JOY thread
  chThdCreateStatic(waJoy, sizeof(waJoy), NORMALPRIO + 1, ThJoy, NULL);

  // Create DEMO thread
  chThdCreateStatic(waDemo, sizeof(waDemo), NORMALPRIO + 1, ThDemo, NULL);

  // Create Encoder thread
  chThdCreateStatic(waEncoder, sizeof(waEncoder), NORMALPRIO + 2, thEncoder, NULL);

  // Create dimmer thread
  chThdCreateStatic(wadimmer, sizeof(wadimmer), NORMALPRIO + 1, dimmer, NULL);

  // Normal main() thread activity, spawning shells
  while (true) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1, shellThread, (void*)&shell_cfg1);

    chThdWait(shelltp); // Waiting termination
  }
}
