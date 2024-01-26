const int MPU_addr = 0x68;  // I2C address of the MPU-6050

mpu6050 offset_mpu6050;
mpu6050 vals_mpu6050[MPU6050_VALS_LEN];

mpu6050 init_mpu6050(double AcX, double AcY, double AcZ, double Tmp, double GyX, double GyY, double GyZ) {
  mpu6050 ret;

  ret.AcX = AcX;
  ret.AcY = AcY;
  ret.AcZ = AcZ;
  ret.Tmp = Tmp;
  ret.GyX = GyX;
  ret.GyY = GyY;
  ret.GyZ = GyZ;

  return ret;
};

mpu6050 average_mpu6050(mpu6050 *val, int n) {
  double _AcX = 0, _AcY = 0, _AcZ = 0, _Tmp = 0, _GyX = 0, _GyY = 0, _GyZ = 0;

  for (int i = 0; i < n; i++) {
    _AcX += val[i].AcX;
    _AcY += val[i].AcY;
    _AcZ += val[i].AcZ;
    _Tmp += val[i].Tmp;
    _GyX += val[i].GyX;
    _GyY += val[i].GyY;
    _GyZ += val[i].GyZ;
  }

  // print_mpu6050(init_mpu6050(_AcX, _AcY, _AcZ, _Tmp, _GyX, _GyY, _GyZ));

  return init_mpu6050(
    _AcX / n,
    _AcY / n,
    _AcZ / n,
    _Tmp / n,
    _GyX / n,
    _GyY / n,
    _GyZ / n);
}

void set_offset_mpu6050() {
  gyro_z = 0;
  mpu6050 _vals[INIT_OFFSET_COUNT];

  Serial.println("==========");
  Serial.println("Start mpu6050 offset setup");

  for (int i = 0; i < INIT_OFFSET_COUNT; i++) {
    _vals[i] = get_mpu6050();
    // print_mpu6050(_vals[i]);
    delay(DELTA_T);
  }
  // Serial.println("---");
  offset_mpu6050 = average_mpu6050(_vals, INIT_OFFSET_COUNT);
  // print_mpu6050(offset_mpu6050);

  Serial.println("End mpu6050 offset setup");
  Serial.println("==========");
}

void setup_mpu6050() {
  Serial.println("==========");
  Serial.println("Start mpu6050 setup");
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(1000);
  set_offset_mpu6050();
  delay(500);
  Serial.println("End mpu6050 setup");
  Serial.println("==========");
}

mpu6050 get_mpu6050() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);  // request a total of 14 registers

  double _AcX = Wire.read() << 8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  double _AcY = Wire.read() << 8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  double _AcZ = Wire.read() << 8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  double _Tmp = Wire.read() << 8 | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  double _GyX = Wire.read() << 8 | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  double _GyY = Wire.read() << 8 | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  double _GyZ = Wire.read() << 8 | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  return init_mpu6050(_AcX, _AcY, _AcZ, _Tmp, _GyX, _GyY, _GyZ);
}

mpu6050 get_in_offset_mpu6050() {
  mpu6050 _val = get_mpu6050();

  return init_mpu6050(
    _val.AcX - offset_mpu6050.AcX,
    _val.AcY - offset_mpu6050.AcY,
    _val.AcZ - offset_mpu6050.AcZ,
    _val.Tmp,
    _val.GyX - offset_mpu6050.GyX,
    _val.GyY - offset_mpu6050.GyY,
    _val.GyZ - offset_mpu6050.GyZ);
}

void print_mpu6050(mpu6050 val) {
  Serial.print("AcX = ");
  Serial.print(val.AcX);
  Serial.print(" \t| AcY = ");
  Serial.print(val.AcY);
  Serial.print(" \t| AcZ = ");
  Serial.print(val.AcZ);
  Serial.print(" \t| Tmp = ");
  Serial.print(val.Tmp / 340.00 + 36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" \t| GyX = ");
  Serial.print(val.GyX);
  Serial.print(" \t| GyY = ");
  Serial.print(val.GyY);
  Serial.print(" \t| GyZ = ");
  Serial.println(val.GyZ);
}

mpu6050 calcAccelData(mpu6050 *_val) {
  return init_mpu6050(
    (int((*_val).AcX / 16384.0 * ACCEL_G * 10)) / 10.0,
    (int((*_val).AcY / 16384.0 * ACCEL_G * 10)) / 10.0,
    (int((*_val).AcZ / 16384.0 * ACCEL_G * 10)) / 10.0,
    (int((*_val).Tmp / 16384.0 * ACCEL_G * 10)) / 10.0,
    (int((*_val).GyX / 16384.0 * ACCEL_G * 10)) / 10.0,
    (int((*_val).GyY / 16384.0 * ACCEL_G * 10)) / 10.0,
    (int((*_val).GyZ / 16384.0 * ACCEL_G * 10)) / 10.0);
}

double get_gyro() {
  return gyro_z;
}

mpu6050 get_calc_mpu6050() {
  return val;
}

long start = 0, end = 0;

void set_mpu6050_timer_start() {
  start = millis();
}

void set_mpu6050_timer_end() {
  start = millis();
}

void calc_mpu6050() {
  val = get_in_offset_mpu6050();
  // mpu6050 val = get_mpu6050();
  // print_mpu6050(val);
  for (int i = MPU6050_VALS_LEN - 1; i > 0; i--) {
    vals_mpu6050[i] = vals_mpu6050[i - 1];
  }
  vals_mpu6050[0] = val;
  val = average_mpu6050(vals_mpu6050, MPU6050_VALS_LEN);
  val = calcAccelData(&val);
  print_mpu6050(val);

  long time = end - start;

  gyro_z += val.GyZ * (time / 100.0);
}
