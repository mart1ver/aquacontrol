/*
controle instalation aquaponique suspendue

 * SD connctée au bus SPI comme ceci:
    ** MOSI - pin 11
    ** MISO - pin 12
    ** CLK - pin 13
    ** CS - pin 8
 * module rtc et  eeprom connecté au bus I2C comme ceci:
   ** sda - pin A4
   ** scl - pin A5
 * capteurs de temperature ds 18B20 connecté sur bus dedié en mode parasite comme ceci:
   ** data - pin 2
 * 3 capteurs dht11 conectes sur les broches pwm suivantes:
   ** 1 - pin 9
   ** 2 - pin 6
   ** 3 - pin 5
 * 4 relais de commande pompes et eclairages connectes sur les broches suivante:
   ** 1 - pin A2 (pompe primaire)
   ** 2 - pin A3 (eclairage aquarium)
   ** 3 - pin 4
   ** 4 - pin 7
 * servomoteur de nourissage des poissons
   ** pwm - pin 3
 * capteur de niveau d'eau aquarium
   **  
 * capteur ph eau de l'aquarium
   **


________Martin Vert 2015____________________________________________________________________________________________________________________ 
 */
 // declaration des E/S
    short chipSelect = 8; //CS sur le spi de la carte SD
    short DDS = 2; // DATA sur les dallas 18b20
    short R0 = A2; //commande du relais 0
    short R1 = A3; //commande du relais 1
    short R2 = 4; //commande du relais 2
    short R3 = 7; //commande du relais 3
    short servo_nourrssage = 3; //commande du servomoteur de nourrissage

// declaration des librairies
    #include <SPI.h>
    #include <SD.h>
    #include <Wire.h>
    #include <Time.h>
    #include <DS1307RTC.h>
    #include <OneWire.h>
    #include <dht.h> 
    #include <Servo.h> 

//autres declarations
    Servo nourrissage; //initialisation de la lib servo  
    tmElements_t tm; // prise de l'heure
    int lastminute = tm.Minute ;//initialise les variables liees à la mesure du temps
    int theminute = tm.Minute;//initialise les variables liees à la mesure du temps
  
void setup() {
    nourrissage.attach(servo_nourrssage);  // attaches the servo on 'servo_nourrisage' to the servo object 
    nourrissage.write(0);              // servo_nourrissage en position '0'
    delay(150);   
    Serial.begin(115200);
    while (!Serial) ; // wait for serial
    delay(200);
    Serial.println(" __________________________ ");
    Serial.println("(Controlleur Mur vegetal MV)");
    Serial.println("(V. 0.0.0 non fonctionnel)"); 
    Serial.print("initialisation de la carte SD...");
// make sure that the default chip select pin is set to
// output, even if you don't use it:
    pinMode(10, OUTPUT);
// see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Erreur de carte SD, ou carte SD non presente");
// don't do anything more:
      return;
    }
    Serial.println("Carte SD initialisee.");
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
    File dataFile = SD.open("config.txt");
// if the file is available, write to it:
    if (dataFile) {
      while (dataFile.available()) {
      Serial.write(dataFile.read());
      }
      dataFile.close();
    }  
// if the file isn't open, pop up an error:
    else {
    Serial.println("error opening config.txt");
    } 
    delay(850);
  }

void loop() {
    tmElements_t tm;
   if (theminute > lastminute){
//code executé tout les X minutes (ecore non codé...)
//fin du code executé toutes les X minutes
} 
   if (theminute > lastminute){
// code executé toute les minutes
      Serial.println();
      Serial.print("Ok, Heure = ");
      print2digits(tm.Hour);
      Serial.write(':');
      print2digits(tm.Minute);
      Serial.write(':');
      print2digits(tm.Second);
      Serial.print(", Date = ");
      Serial.print(tm.Day);
      Serial.write('/');
      Serial.print(tm.Month);
      Serial.write('/');
      Serial.print(tmYearToCalendar(tm.Year));
      Serial.println();
      nourir();
      lastminute = tm.Minute;
//fin du code executé toutes les minutes
   }
   if (RTC.read(tm)) {
     //code executée toutes les secondes
     Serial.print(".");
     
     //fin du code executée toutes les secondes
     } else {
     if (RTC.chipPresent()) {
       Serial.println("Le DS1307 est arrete.  Reglez l'heure SVP.");
       Serial.println();
       } else {
       Serial.println("DS1307 en defaut!  Verifiez le cablage SVP.");
       Serial.println();
         }
     delay(9000);
      }
    delay(500);
    theminute = tm.Minute;
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}
void nourir() {
  //sequence de nourrissage des poissons
   Serial.println("Nourrissage des poissons.");
      Serial.println();
  nourrissage.write(0);              // tell servo to go to position in variable '0' 
    delay(150);   
nourrissage.write(255);              // tell servo to go to position in variable '255' 
    delay(1500); 
nourrissage.write(0);              // tell servo to go to position in variable '0' 
    delay(1500);    
}


void relais()
{
//allume le relais ou l'eteind selon son etat
}


void enregistre(){

}
