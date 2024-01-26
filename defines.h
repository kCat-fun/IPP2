#define DELTA_T 5
#define INIT_OFFSET_COUNT 10
#define MPU6050_VALS_LEN 5
#define ACCEL_G 9.81

struct mpu6050 {
  double AcX;
  double AcY;
  double AcZ;
  double Tmp;
  double GyX;
  double GyY;
  double GyZ;
};

mpu6050 val;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define L2 5

#define DIRECTION_0 1 // 正面角度
#define DIRECTION_45 2 // 右前斜め45度
#define DIRECTION_90 3 // 右角度
#define DIRECTION_135 4 // 右後斜め45度
#define DIRECTION_180 5 // 後ろ角度
#define DIRECTION_225 6 // 左後斜め45度
#define DIRECTION_270 7 // 左角度
#define DIRECTION_315 8 // 左前斜め45度

#define N 0
#define F 1
#define B 2
#define S 3

int dist_left = 0, dist_right = 0;
int joystick_left_x = 0, joystick_left_y = 0;
int joystick_right_x = 0;

bool is_dribbler = false, is_kicker = false, is_kicker_stopper = false;


double gyro_z = 0;