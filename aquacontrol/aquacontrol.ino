/*
________Martin Vert 2015____________________________________________________________________________________________________________________ 
Controle de l'instalation aquaponique suspendue de l'atelier de la petite rockette

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
    ** 1 - pin A2 (pompe primaire)      >> R0
    ** 2 - pin A3 (eclairage aquarium)  >> R1
    ** 3 - pin 4                        >> R2
    ** 4 - pin 7                        >> R3
 * servomoteur de nourissage des poissons
    ** pwm - pin 3
 * capteur de niveau d'eau aquarium
    **  A prevoir niveau materiel (A0 ?)
 * capteur ph eau de l'aquarium  (A1 ?)
    **  A prevoir niveau materiel
==================================le fichier de configuration config.txt sur la carte sd==================================
 Contient, (separé par des virgules?), différentes variables de configuration du cycle:
  -horaires de declenchement de la pompe
  -horaires de declenchement de l'eclairage
  -nombre de minutes entre chaque prise de logs
  -eventuellement ph cible (pour les versions anterieures, car le materiel n'est pas pret..)
  -eventuellement les niveau mini et maxi tolerables dans l'aquarium (pour les prochaines versions, car le materiel n'est pas pret..)

==================================le fichier de logs log.csv sur la carte sd==================================
 Contient, au format csv, les données des logs

________Martin Vert 2015____________________________________________________________________________________________________________________ 
*/
 
 
 // declaration des E/S
     short servo_nourrssage = 3; //commande du servomoteur de nourrissage
     int chipSelect = 8; //CS sur le spi de la carte SD
     int DDS = 2; // DATA sur les dallas 18b20
     int R0 = A2; //commande du relais 0
     int R1 = A3; //commande du relais 1
     int R2 = 4; //commande du relais 2
     int R3 = 7; //commande du relais 3
     
 // declaration de variables d'etat des 4 relais de commande      
     int R0STATE = '0';
     int R1STATE = '0';
     int R2STATE = '0';
     int R3STATE = '0';
    
 // declaration des librairies
    #include <SPI.h>
    #include <SD.h>
    #include <Wire.h>
    #include <Time.h>
    #include <DS1307RTC.h>
    #include <OneWire.h>
    #include <dht.h> 
    #include <Servo.h> 

 // autres declarations
    Servo nourrissage;                // initialisation de la librairie servo  
    tmElements_t tm;                  // prise de l'heure
    int lastminute = tm.Minute ;      // initialise les variables liees à la mesure du temps
    int theminute = tm.Minute;        // initialise les variables liees à la mesure du temps
  
void setup() {                        // code executé une fois au démarage
 // initialise les quatres sorties relais à l'etat 0
    pinMode(R0, OUTPUT);
    digitalWrite(R0, LOW);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);  
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    
    nourrissage.attach(servo_nourrssage);  // liaison du servomoteur de nourrisage à 'servo_nourrisage'
    nourrissage.write(0);                  // servo_nourrissage en position '0'
    delay(150);                            // petite pose
    Serial.begin(115200);                  // initialisation de la liaison serielle (115200 bauds)
    while (!Serial) ;                      // wait for serial
    delay(200);                            // petite pause
 // message initial sur le terminal serie   
    Serial.println(" __________________________ ");
    Serial.println("(Controlleur Mur vegetal MV)");
    Serial.println("(V. 0.0.0 non fonctionnel)"); 
    Serial.print("initialisation de la carte SD...");
 // placer le chipselect du spi en output au cas ou...
    pinMode(chipSelect, OUTPUT);
 // verrifier si la carte sd est bien presente et initialisée.
    if (!SD.begin(chipSelect)) {
      Serial.println("Erreur de carte SD, ou carte SD non presente");
      return;
    }
    Serial.println("Carte SD initialisee.");
// ouvre config.txt. NOTE: un seul fichier peut etre ouvert à la fois il faudra fermer ce fichier avant d'acceder à un autre dans le sketch
    File dataFile = SD.open("config.txt");

// if the file is available, write to it: si le fichier est disponible alors on affiche son contenu sur le terminal serie puis on le charge dans l'EEPROM                      FIXME  FIXME 
    if (dataFile) {
      while (dataFile.available()) {
      Serial.write(dataFile.read());
      }
      dataFile.close();
    }  
// si le fichier ne se charge pas , on affiche un message d'erreur sur le terminal serie
    else {
    Serial.println("erreur à l'ouverture de config.txt");
    } 
    delay(850); // pause
  }
  
void loop() {
    tmElements_t tm;                                           //prise de l'heure
   if (theminute > lastminute){
//code executé tout les X minutes (ecore non codé...) FIXME FIXME
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
      relais('0');
      relais('1');
      relais('2');
      relais('3');
     
      
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
//sequence de nourrissage des poissons à l'aide du servomoteur prévu à cet effet
      Serial.println("Nourrissage des poissons.");
      Serial.println();
nourrissage.write(0);              // tell servo to go to position in variable '0' 
    delay(150);   
nourrissage.write(255);              // tell servo to go to position in variable '255' 
    delay(1500); 
nourrissage.write(0);              // tell servo to go to position in variable '0' 
    delay(1500);    
}


void relais(int numero)
{
  
//allume le relais ou l'eteind selon son etat
  switch (numero) {
    case '0':   
      if(R0STATE == '0')
    {
      digitalWrite(R0, HIGH);
      R0STATE = '1';
      Serial.println("RO ON");
    }
    else
    {
      digitalWrite(R0, LOW);
      R0STATE = '0';
      Serial.println("RO OFF");
    }
      break;
    case '1':    
           if(R1STATE == '0')
    {
      digitalWrite(R1, HIGH);
      R1STATE = '1';
      Serial.println("R1 ON");
    }
    else
    {
      digitalWrite(R1, LOW);
      R0STATE = '0';
      Serial.println("R1 OFF");
    }
      break;
    case '2':    
            if(R2STATE == '0')
    {
      digitalWrite(R2, HIGH);
      R2STATE = '1';
      Serial.println("R2 ON");
    }
    else
    {
      digitalWrite(R2, LOW);
      R2STATE = '0';
      Serial.println("R2 OFF");
    }
      break;
      
      
    case '3':    
            if(R3STATE == '0')
    {
      digitalWrite(R3, HIGH);
      R3STATE = '1';
      Serial.println("R3 ON");
    }
    else
    {
      digitalWrite(R3, LOW);
      R3STATE = '0';
      Serial.println("R3 OFF");
    }
    break;
    default: 
    Serial.println("mauvais appel relais");
     
  }
}


void enregistre()
{
//enregistre les données des differents capteurs sur la carte sd
}
