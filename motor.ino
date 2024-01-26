#define DIFF 0.1

const uint8_t motor_pin[6][2] = {
  { 33, 32 },  // motor 0
  { 13, 12 },  // motor 1
  { 27, 14 },  // motor 2
  { 26, 25 },  // motor 3
  { 17, 5 },   // motor 4
  { 4, 16 },   // motor 5
};

void setup_motor() {
  for (int i = 0; i < 6; i++) {
    pinMode(motor_pin[i][0], OUTPUT);
    pinMode(motor_pin[i][1], OUTPUT);
    digitalWrite(motor_pin[i][0], LOW);
    digitalWrite(motor_pin[i][1], LOW);
  }
}

void motor(int motor_name, int state) {
  digitalWrite(motor_pin[motor_name][0], state == F || state == S);
  digitalWrite(motor_pin[motor_name][1], state == B || state == S);
}

void move_foward() {
  // if (get_calc_mpu6050().AcY > DIFF) {
  //   move_left();
  // } else if (get_calc_mpu6050().AcY < -DIFF) {
  //   move_right();
  // } else {
    motor(0, F);
    motor(1, B);
    motor(2, B);
    motor(3, F);
  // }
}

void move_back() {
  motor(0, B);
  motor(1, F);
  motor(2, F);
  motor(3, B);
}

void move_right() {
  motor(0, B);
  motor(1, B);
  motor(2, F);
  motor(3, F);
}

void move_left() {
  motor(0, F);
  motor(1, F);
  motor(2, B);
  motor(3, B);
}

void move_foward_right() {
  motor(0, N);
  motor(1, B);
  motor(2, N);
  motor(3, F);
}

void move_foward_left() {
  motor(0, F);
  motor(1, N);
  motor(2, B);
  motor(3, N);
}

void move_back_left() {

  motor(0, N);
  motor(1, F);
  motor(2, N);
  motor(3, B);
}

void move_back_right() {
  motor(0, B);
  motor(1, N);
  motor(2, F);
  motor(3, N);
}

void spin_left() {
  for (int i = 0; i < 4; i++) {
    motor(i, B);
  }
}

void spin_right() {
  for (int i = 0; i < 4; i++) {
    motor(i, F);
  }
}
