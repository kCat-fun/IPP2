#include <Wire.h>
#include "defines.h"

void setup() {
  Serial.begin(115200);
  // while (!Serial)
  //   ;  // Leonardo: wait for serial monitor
  Serial.println("Success setup serial");

  Serial.println("\n========== Run program ==========");

  Wire.begin();
  // setup_mpu6050();

  setup_ps3();
  setup_motor();

  pinMode(34, INPUT);
}

void loop() {
  set_mpu6050_timer_start();

  if (!ps3_isConnected())
    return;

  set_sss();
  Serial.print("right:");
  Serial.print(dist_right);
  Serial.print("\t left:");
  Serial.println(dist_left);

  set_ps3_joystick();
  int direction = calc_ps3_joystick_direction(joystick_left_x, joystick_left_y);
  Serial.println(direction);

  if (is_kicker) {
    motor(4, F);
    while (digitalRead(34) != 1)
      delay(5);
    is_kicker = false;
    is_kicker_stopper = false;
  } else if (digitalRead(34) == 1) {
    Serial.println("reload");
    motor(4, F);
  } else {
    if (!is_kicker_stopper) {
      motor(4, S);
      is_kicker_stopper = true;
    } else {
      motor(4, N);
    }
  }

  if (is_dribbler) {
    motor(5, B);
  } else {
    motor(5, N);
  }

  bool move_flag = true;
  switch (direction) {
    case DIRECTION_0:
      move_foward();
      break;
    case DIRECTION_45:
      move_foward_right();
      break;
    case DIRECTION_90:
      move_right();
      break;
    case DIRECTION_135:
      move_back_right();
      break;
    case DIRECTION_180:
      move_back();
      break;
    case DIRECTION_225:
      move_back_left();
      break;
    case DIRECTION_270:
      move_left();
      break;
    case DIRECTION_315:
      move_foward_left();
      break;
    default:
      move_flag = false;
      // スティック入力が無いときはニュートラル（空転）
      for (int i = 0; i < 4; i++)
        motor(i, N);
  }

  if (joystick_right_x < -50) {
    move_flag = true;
    spin_left();
  } else if (joystick_right_x > 50) {
    move_flag = true;
    spin_right();
  }

  if (get_ps3(L2)) {
    delay(DELTA_T * 2);
    for (int i = 0; i < 4; i++) {
      motor(i, N);
    }
  }

  delay(DELTA_T);

  set_mpu6050_timer_end();
  calc_mpu6050();
}