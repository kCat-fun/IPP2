void set_sss() {
  Wire.beginTransmission(8);//ID8のSlaveとの通信確立
  Wire.write(0);//送信用データの選択
  Wire.endTransmission();//データの送信と送信終了

  Wire.requestFrom(8, 1);  //IDが8番のSlaveから1byteのデータを要求

  while (Wire.available()) {  //受信データがある場合はwhile文を抜けない
    byte data = Wire.read();  //byte型の変数dataに受信データを代入
    dist_right = data;
  }

  Wire.beginTransmission(8);//ID8のSlaveとの通信確立
  Wire.write(1);//送信用データの選択
  Wire.endTransmission();//データの送信と送信終了

  Wire.requestFrom(8, 1);  //IDが8番のSlaveから1byteのデータを要求

  while (Wire.available()) {  //受信データがある場合はwhile文を抜けない
    byte data = Wire.read();  //byte型の変数dataに受信データを代入
    dist_left = data;
  }
}