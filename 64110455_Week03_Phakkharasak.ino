int x = 0;
float y = 0.0;
float z = 0.0;
float w = 0.0; // เส้นสีขาว
float frequencyY = 1.5; // ความถี่ของเส้น sine wave y
float frequencyZ = 1.5; // ความถี่ของเส้น cosine wave z
float amplitude = 1.5; // ความสูงของเส้น sine wave y, cosine wave z

void setup() {
  Serial.begin(9600);
}

void loop() {
  x = x + 1;
  y = amplitude * sin(2 * PI * frequencyY * x / 100); // สร้าง sine wave y
  z = amplitude * cos(2 * PI * frequencyZ * x / 100); // สร้าง cosine wave z
  w = 0.0; // เส้นสีขาวคือค่าคงที่เป็น 0
  
  Serial.printf("%f %f %f\n", y, z, w); // ส่งทั้งสามค่าไปยัง Serial Monitor
  delay(50); // ความหน่วงระหว่างข้อมูล
}
