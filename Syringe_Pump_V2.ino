/***
 * Sebelum memulai alangkah baiknya untuk membaca Bismillah
 * Project Syringe Pump v.1
 * 
 * Code by @neutron_cs
***/

#include <Stepper.h>   // library stepper
#include <Keypad.h>    // library keypad by Alexander Brevig

// -------------- VARIABEL KEYPAD --------------------------
// untuk keypad 4 x 4
const byte ROWS = 4; //jumlah baris
const byte COLS = 4; //jumlah kolom

// mapping karakter yang ada di keypad bisa diganti sesuai keinginan
// terdiri dari baris dan kolom yang didefinisikan sebelumnya
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5}; //pin yang digunakan untuk baris keypad
byte colPins[COLS] = {6, 7, 8, 9}; //pin yang digunakan untuk kolom keypad

// deklarasi variabel keypad 'key'
Keypad keycustom = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String keypress;

// ----------------------- Variabel waktu -----------------------------
unsigned long waktu = 0;
unsigned long waktusekarang;
unsigned long interval = 5000; // interval default 5 detik = 1ml/jam
unsigned long interval_waktu;
unsigned long waktu_volumestop= 0;
unsigned long volume_stop = 3600000;
// ---------------------- Variabel kondisi ----------------------------
boolean start = false;
boolean seting = false;
boolean x = false;
// ---------------------- Variabel Stepper Motor ----------------------
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 10, 11, 12, 13);
int steps = 1 ; // berlawanan arah jarum jam
int kunci = 0;
String arah = "Mundur";
//
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

// Kode utama
void loop() {
  // put your main code here, to run repeatedly:
  char key = keycustom.getKey();
  if (key) Serial.println(key);
  switch (key){
    case 'A':{
    start = true;
    while (start){
      steppermotor();
      }
    break;}
    case 'B':{
    keypress = "";
    seting = true;
    while (seting){
      inputinterval();
      }
    break;}
    case 'C': heavy_rotation();break;
    case 'D': x = true; while (x == true) volume();break;
  }
}

// ------------------ STEPPER MOTOR -------------------------
// Ketika masuk mode stepper motor semua fungsi tombol dimatikan kecuali '#'
// harus ditekan '#' dulu untuk menghentikan fungsi steppermotor
// baru semua fungsi tombol kembali normal

void steppermotor(){
  char key = keycustom.getKey();
  waktusekarang = millis();
  interval_waktu = millis();
  
  if (waktusekarang - waktu >= interval){
    Serial.println(interval); // untuk debug interval
    Serial.println(arah); // untuk debug stepper motor
    myStepper.step(steps);
    waktu = waktusekarang;     
    kunci += 1;
    Serial.println(kunci);
    }

  if (volume_stop == key) { 
    start = false;
    Serial.println("Stop");
  }
  
  switch (key){
    // fungsi untuk enter atau keluar mode stepper motor
    case '#': {
      start = false; 
      Serial.println("Stop");
      break;  
    }
  }
}

// --------------------- Mode mengubah debit ----------------------
// semua fungsi tombol mode dimatikan 
// harus menekan '#' dulu untuk input angka

void inputinterval(){
  // deklarasi variabel keypad
  char key = keycustom.getKey();

  // agar keypad berfungsi bisa diganti dengan if juga
  switch (key){
    case '1': keypress = keypress + key; Serial.println(keypress); break;
    case '2': keypress = keypress + key; Serial.println(keypress); break;
    case '3': keypress = keypress + key; Serial.println(keypress); break;
    case '4': keypress = keypress + key; Serial.println(keypress); break;
    case '5': keypress = keypress + key; Serial.println(keypress); break;
    case '6': keypress = keypress + key; Serial.println(keypress); break;
    case '7': keypress = keypress + key; Serial.println(keypress); break;
    case '8': keypress = keypress + key; Serial.println(keypress); break;
    case '9': keypress = keypress + key; Serial.println(keypress); break;
    case '0': keypress = keypress + key; Serial.println(keypress); break;

    // fungsi untuk enter atau keluar mode input data
    case '#': {
      seting = false;break;
    }
    case 'A':{
      int a = keypress.toInt(); // variabel input debit ml/jam
      if (a <= 0)  a=1; //antisipasi jika user tidak menginput data
      interval = ((3140/628)*1000)/a; // rumus perhitungan interval
      Serial.println(interval);
      keypress ="";
      break;
    }
    case 'B':{
      int b = keypress.toInt();
      if (b<= 0) b=1;
      volume_stop = 720 * b;
      Serial.println(volume_stop);
      keypress = "";
      break;
    }
  }
}

// --------------------- Mode mengubah rotasi stepper --------------------
void heavy_rotation(){
  // step dari motordriver ingin berlawanan atau searah
   steps = steps * -1;
   if (steps == -1){
    arah = "Maju"; // jika nilainya -1 maka akan searah jarumjam
   }
   else{
    arah = "Mundur"; // jika nilainya 1 maka akan berlawanan arah jarum jam
   }
   Serial.println(arah);
}

// -------------------- Mode seting volume -------------------------------
void volume(){
  char key = keycustom.getKey();
  Serial.println(arah);
  myStepper.step(steps);
  delay(50);
  switch(key){
  case 'D': x = false ;break;}
}
