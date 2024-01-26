#include <Ps3Controller.h>

bool ps3_up = false, ps3_l2 = false;

bool get_ps3(int button_id) {
  switch (button_id) {
    case UP:
      return ps3_up;
    case L2:
      return ps3_l2;
  }
}

void setup_ps3() {
  // コールバック関数の登録(オプション)
  Ps3.attach(notify);
  // コールバック関数の登録(オプション)
  Ps3.attachOnConnect(onConnect);
  // コールバック関数の登録(オプション)
  Ps3.attachOnDisconnect(onDisconnect);
  // 接続するデバイスのアドレスに書き換える
  Ps3.begin("08:D1:F9:EB:35:5A");
  Serial.println("Ready.");
}

bool ps3_isConnected() {
  return Ps3.isConnected();
}

void notify() {
  if (Ps3.event.button_down.up) {
    Serial.println("Started pressing the up button");
    ps3_up = true;
  }
  if (Ps3.event.button_up.up) {
    Serial.println("Released the up button");
    ps3_up = false;
  }

  if (Ps3.event.button_down.l1) {
    is_dribbler = !is_dribbler;
    delay(5);
  }

  if (Ps3.event.button_down.r1) {
    is_kicker = true;
  }

  if (Ps3.event.button_down.l2) {
    ps3_l2 = true;
  }
  if (Ps3.event.button_up.l2) {
    ps3_l2 = false;
  }

  if (Ps3.event.button_down.start) {
    while (dist_right > 3) {
      move_right();
    }
    delay(1000);
    for (int i = 0; i < 4; i++) {
      motor(i, N);
    }
    delay(1000);
    set_offset_mpu6050();
    delay(100);
    while (dist_right < dist_left) {
      move_left();
    }
    for (int i = 0; i < 4; i++) {
      motor(i, S);
    }
  }

  if (Ps3.event.button_down.select) {
    if (dist_right < dist_left) {
      while (dist_right < dist_left) {
        move_left();
      }
    } else {
      while (dist_right > dist_left) {
        move_right();
      }
    }
    for (int i = 0; i < 4; i++) {
      motor(i, S);
    }
    delay(500);
    for (int i = 0; i < 4; i++) {
      motor(i, N);
    }
  }
}

void set_ps3_joystick() {
  if (Ps3.event.analog_changed.stick.lx) {
    joystick_left_x = Ps3.data.analog.stick.lx;
  }
  if (Ps3.event.analog_changed.stick.ly) {
    joystick_left_y = Ps3.data.analog.stick.ly;
  }
  if (Ps3.event.analog_changed.stick.rx) {
    joystick_right_x = Ps3.data.analog.stick.rx;
  }
}

int calc_ps3_joystick_direction(int x, int y) {
  int dist = sqrt(x * x + y * y);
  if (dist < 50) {
    return -1;
  }

  int angle = atan2(x, y) * 180.0 / 3.14159;
  angle = (angle < 0 ? 360 + angle : angle);
  // Serial.print(dist);
  // Serial.print(",\t");
  // Serial.println(angle);

  if (158 <= angle && angle < 202)
    return DIRECTION_0;
  else if (113 <= angle && angle < 158)
    return DIRECTION_45;
  else if (67 <= angle && angle < 113)
    return DIRECTION_90;
  else if (22 <= angle && angle < 67)
    return DIRECTION_135;
  else if (338 <= angle || angle < 22)
    return DIRECTION_180;
  else if (292 <= angle && angle < 338)
    return DIRECTION_225;
  else if (258 <= angle && angle < 292)
    return DIRECTION_270;
  else if (202 <= angle && angle < 258)
    return DIRECTION_315;

  return -1;
}

void onConnect() {
  // 接続されたときに呼び出されるコールバック関数
  Serial.println("Connected!.");
}

void onDisconnect() {
  // 切断されたときに呼び出されるコールバック関数
  Serial.println("DisConnected!.");
}