// MENU - glowne elementy
String menuItems[] = {"Wilgotnosc P", "Wilgotnosc G", "Swiatlo", "Poziom UV", "Rezerwuar", "Temperatura", "Pompa Wodna"};

// Zmienne do obslugi klawiszy
int readKey;
int savedDistance = 0;

// Zmienne kontoroli menu
int menuPage = 0;
int maxMenuPages = round(((sizeof(menuItems) / sizeof(String)) / 2) + 2);
int cursorPosition = 0;

//////////4 Znaki, ktore dodane sa… 8-bitowo do obslugi menu//////////////////
byte downArrow[8] = {
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b10101, // * * *
  0b01110, //  ***
  0b00100  //   *
};
byte upArrow[8] = {
  0b00100, //   *
  0b01110, //  ***
  0b10101, // * * *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100, //   *
  0b00100  //   *
};
byte menuCursor[8] = {
  B01000, //  *
  B00100, //   *
  B00010, //    *
  B00001, //     *
  B00010, //    *
  B00100, //   *
  B01000, //  *
  B00000  //
};
byte Celcius[8] = {
  B01110, // ***
  B10001, //*   *
  B10001, //*   *
  B01110, // ***
  B00000, //
  B00000, //
  B00000, //
  B00000  //
};

////////////// Dodawanie Bibliotek /////////////////
#include <Wire.h>
////////////////////////////////////////////////////

//////////////// Obsluga LCD ///////////////////////
#include <LiquidCrystal.h>
////////////////////////////////////////////////////

////////////// Obsluga DHT22////////////////////////
#include <DHT.h>
#define DHTPIN 53     // ustawianmy pin
#define DHTTYPE DHT22   // DHT 22  (AM2302)
int chkDHT1;
float humDHT22;  // zmienna wilgotnosci
float tempDHT22; // zmienna temperatury
// inicjalizujemy DHT22 dla arduino
DHT dht(DHTPIN, DHTTYPE);
void readDHT22() {
  humDHT22 = dht.readHumidity();
  tempDHT22 = dht.readTemperature();
  // Kontola Bledow, jezli DHT22 zwroci nan wyswietlamy przez port szeregowy informacje o bledzie
  if (isnan(tempDHT22) || isnan(humDHT22))
  {
    Serial.println("DHT22: Blad odczytu temperatury z czujnika!");
  }
  else
  {
    // Wysylamy wyniki przez port szeregowy odczyt co 100ms
    Serial.print("Wilgotnosc: ");
    Serial.print(humDHT22);
    Serial.print(" % ");
    Serial.print("Temperatura: ");
    Serial.print(tempDHT22);
    Serial.println(" *C");
  }
}
///////////////////////////////////////////////////////////////////

///////////////////////// Light Sensora ///////////////////////////
int lightSensor = A11;
///////////////////////////////////////////////////////////////////

////////////////////////// GY-ML8511 //////////////////////////////
int UVOUT = A14; //wyjscie z sensora
int REF_3V3 = A15; //3.3V referencyjne
// Mapowanie floating point
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//funkcja sredniajaca
float averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0;
  for (int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return (runningValue);
}
///////////////////////////////////////////////////////////////////

//////////////_Obsluga czujnika wilgotnosci gleby_/////////////////
float valueSoil = 0;
int soilPin = A13;
int soilPower = 51;
float readSoil()
{

  digitalWrite(soilPower, HIGH);
  delay(10);
  valueSoil = analogRead(soilPin);
  digitalWrite(soilPower, LOW);
  return valueSoil;
}
///////////////////////////////////////////////////////////////////

///////////////////// Ustwienie pinow do LCD //////////////////////
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
///////////////////////////////////////////////////////////////////

//////////// Obsluga czujnika poziomou wody ///////////////////////
int liqidSensor = A12;
float liquid_level;
///////////////////////////////////////////////////////////////////

//////////// Obsluga Pompy ////////////////////////////////
int pumpStatus = 50;
void pumpState1() {
  Serial.println();
  Serial.print("Rezerwuar: UZUPELNIJ");
  Serial.print(" / Wilgotnosc: NISKA");
  Serial.println(" / Pompa: OFF");
  Serial.println("");
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Pompa: OFF                 ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Rezerwuar: Pusty           ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Gleba: Sucha               ");
  digitalWrite(pumpStatus, LOW);
  delay(1000);

}

void pumpState2() {
  Serial.println();
  Serial.print("Rezerwuar: UZUPELNIJ");
  Serial.print(" / Wilgotnosc: OK");
  Serial.println(" / Pompa: OFF");
  Serial.println("");
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Pompa: OFF                 ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Rezerwuar: Pusty           ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Gleba: OK               ");
  digitalWrite(pumpStatus, LOW);
  delay(1000);
}
void pumpState3() {
  Serial.println();
  Serial.print("Rezerwuar: OK");
  Serial.print(" / Wilgotnosc: OK");
  Serial.println(" / Pompa: OFF");
  Serial.println("");
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Pompa: OFF                 ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Rezerwuar: OK           ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Gleba: OK               ");
  digitalWrite(pumpStatus, LOW);
  delay(1000);

}
void pumpState4() {
  Serial.println();
  Serial.print("Rezerwuar: OK");
  Serial.print(" / Wilgotnosc: NISKA");
  Serial.println(" / Pompa: ON");
  Serial.println("");
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Pompa: ON                 ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Rezerwuar: OK           ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Gleba: Sucha               ");
  digitalWrite(pumpStatus, HIGH);
  delay(1000);
}

// Funkcja pracująca w tle
int pumpSetupContinous() {
  int current = 0;
  float readliquid_level = 0;
  float readSoilValue = 0;
  readliquid_level = liquid_level; // 640 * 100 ; // 640 * 100; // (850 - maksymalne wskazanie czujnika)
  liquid_level = analogRead(liqidSensor);
  readSoil();
  readSoilValue = readSoil() / 850 * 100; // (850 - maksymalne wskazanie czujnika)
  // WODA ZLE GLEBA ZLE - POMPA OFF
  if (readliquid_level < 10.00) {
    if (readSoilValue < 40.00) {
      digitalWrite(pumpStatus, LOW);
      delay(1000);
    }
  }
  // WODA ZLE GLEBA OK  - POMPA OFF
  if (readliquid_level < 10.00) {
    if (readSoilValue > 40.00) {
      digitalWrite(pumpStatus, LOW);
      delay(1000);
    }
  }

  // WODA OK GLEBA OK   - POMPA OFF
  if (readliquid_level > 10.00) {
    if (readSoilValue > 40.00) {
      digitalWrite(pumpStatus, LOW);
      delay(1000);
    }
  }

  // WODA OK GLEBA ZLE  - POMPA ON
  if (readliquid_level > 10.00) {
    if (readSoilValue < 40.00) {
      digitalWrite(pumpStatus, HIGH);
      delay(1000);
    }
  }

}
int pumpSetup() {
  //pompa - pinout

  /// TESTOWANIE PINOUTU
 // digitalWrite(pumpStatus, HIGH);
   //delay(1000);
   //digitalWrite(pumpStatus, LOW);

  //DANE Z REZERWUARU

  int current = 0;
  float readliquid_level = 0;
  float readSoilValue = 0;
  readliquid_level = liquid_level; // 640 * 100 ; // 640 * 100; // (850 - maksymalne wskazanie czujnika)
  liquid_level = analogRead(liqidSensor);
  //  Serial.print("readliquid_level: ");
  //  Serial.print(readliquid_level);
  //  Serial.print(" / liquid_level: ");
  //  Serial.println(liquid_level);

  // DANE Z GLEBY
  readSoil();
  readSoilValue = readSoil() / 850 * 100; // (850 - maksymalne wskazanie czujnika)
  //  Serial.print("readSoil(): ");
  //  Serial.print(readSoil());
  //  Serial.print(" / readSoilValue: ");
  //  Serial.println(readSoilValue);

  // WODA ZLE GLEBA ZLE - POMPA OFF
  if (readliquid_level < 10.00) {
    if (readSoilValue < 40.00) {
      pumpState1();
    }
  }

  // WODA ZLE GLEBA OK  - POMPA OFF
  if (readliquid_level < 10.00) {
    if (readSoilValue > 40.00) {
      pumpState2();
    }
  }

  // WODA OK GLEBA OK   - POMPA OFF
  if (readliquid_level > 10.00) {
    if (readSoilValue > 40.00) {
      pumpState3();
    }
  }

  // WODA OK GLEBA ZLE  - POMPA ON
  if (readliquid_level > 10.00) {
    if (readSoilValue < 40.00) {
      pumpState4();
    }
  }

}
///////////////////////////////////////////////////////////////////

////////////////////// GLOWNE USTAWIENIA //////////////////////////
void setup() {
  Serial.begin(9600); // wlaczamy interfejs szeregowy z predkoscia 9600
  Serial.println(analogRead(A0)); // debugujemy analogowy pin A0
  lcd.begin(16, 2); // pierwsza inicjalizacja ekranu
  lcd.clear(); // czyszczenie ekranu

  ///////////////////////// Obsluga DHT22 ///////////////////////////
  dht.begin(); // inicjalizacja modulu dht22
  ///////////////////////////////////////////////////////////////////

  ////////////// Obsluga czujnika wilgotnosci gleby /////////////////
  pinMode(soilPower, OUTPUT); // ustawienie pina zadeklarowanego w soilPower jako wyjscie
  digitalWrite(soilPower, LOW); // ustawiamy stan niski (0)
  pinMode(soilPin, INPUT); // ustawienie pina zadeklarowanego w soilPin jako wyjscie
  ///////////////////////////////////////////////////////////////////

  ////////////Tworzymy znaki z deklaracji byte nazwaznaku[8]/////////
  lcd.createChar(0, menuCursor); // kursor menu
  lcd.createChar(1, upArrow); // strzalka w gore
  lcd.createChar(2, downArrow); // strzalka w dol
  lcd.createChar(3, Celcius); // znak Stopnia
  ///////////////////////////////////////////////////////////////////

  /////////////////////////// GY-ML8511 /////////////////////////////
  pinMode(UVOUT, INPUT); // ustawienie pina zadeklarowanego w UVOUT jako wejscie
  pinMode(REF_3V3, INPUT); // ustawienie pina zadeklarowanego w REF_3V3 jako wejscie
  ///////////////////////////////////////////////////////////////////

  //////////// Obsluga czujnika poziomou wody////////////////////////
  pinMode(liqidSensor, INPUT); //ustawienie pina zadeklarowanego w liqidSensor jako wejscie
  ///////////////////////////////////////////////////////////////////


  pinMode(lightSensor, INPUT); //ustawienie pina zadeklarowanego w lightSensor jako wejscie

  //////////// Obsługa POMPKY ////////////////////////////////
  pinMode(pumpStatus, OUTPUT);
  ///////////////////////////////////////////////////////////////////
  // Powitanie
  WelcomeMessage();
}

void loop() {
  mainMenuDraw();
  drawCursor();
  operateMainMenu();
  //pumpSetupContinous();
}

//////////////// Wiadomosc Powitalna / Splash /////////////////////
void WelcomeMessage() {
  Serial.println("Powitanie - Zainicjalizowane");
  lcd.setCursor(0, 0);
  lcd.print("     Witaj!     ");
  lcd.setCursor(0, 1);
  lcd.print("Bartosz Bartosik");
  delay(3000);
  Serial.println("Powitanie - Koniec");
}
///////////////////////////////////////////////////////////////////

////////////////////// Rysowanie Menusow //////////////////////////
void mainMenuDraw() {
  int menuNumber = menuPage + 1; // iterator menu
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(menuItems[menuPage]);
  lcd.setCursor(1, 1);
  lcd.print(menuItems[menuPage + 1]);
  Serial.print("Obecnie zaznaczono Menu Numer: ");
  Serial.println(menuNumber);
  if (menuPage == 0) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
  } else if (menuPage > 0 and menuPage < maxMenuPages) {
    lcd.setCursor(15, 1);
    lcd.write(byte(2));
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  } else if (menuPage == maxMenuPages) {
    lcd.setCursor(15, 0);
    lcd.write(byte(1));
  }
}
///////////////////////////////////////////////////////////////////

////////////////////// Rysowanie Kursora //////////////////////////
void drawCursor() {
  for (int x = 0; x < 2; x++) {
    lcd.setCursor(0, x);
    lcd.print(" "); //pusty w przypadku gdy zaznaczona jest inna pozycja - czyszczenie
  }
  if (menuPage % 2 == 0) {
    if (cursorPosition % 2 == 0) {
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
  }
  if (menuPage % 2 != 0) {
    if (cursorPosition % 2 == 0) {
      lcd.setCursor(0, 1);
      lcd.write(byte(0));
    }
    if (cursorPosition % 2 != 0) {
      lcd.setCursor(0, 0);
      lcd.write(byte(0));
    }
  }
}
///////////////////////////////////////////////////////////////////

////////////////////Glowna Funkcja Menu////////////////////////////

void operateMainMenu() {
  int activeButton = 0;
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 0:
        // Kiedy przycisk jest zwrocony jako 0 nie podejmujemy zadnej akcji. Brak wcisnietego przycisku
        break;
      case 1:  // wstecz
        button = 0;
        switch (cursorPosition) { // wybor menu po cursorPosition
          case 0:
            Serial.println("Menu Wilgotnosci Powietrza");
            menuItem1();
            break;
          case 1:
            Serial.println("Menu Wilgotnosci Gleby");
            menuItem2();
            break;
          case 2:
            Serial.println("Menu Swiatla");
            menuItem3();
            break;
          case 3:
            Serial.println("Menu Poziomu UV");
            menuItem4();
            break;
          case 4:
            Serial.println("Rezerwuaru");
            menuItem5();
            break;
          case 5:
            Serial.println("Menu Temperatury");
            menuItem6();
            break;
          case 6:
            Serial.println("Menu Pompy");
            menuItem7();
            break;
        }
        activeButton = 1;
        mainMenuDraw();
        drawCursor();
        break;
      case 2:
        button = 0;
        if (menuPage == 0) {
          cursorPosition = cursorPosition - 1;
          cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        }
        if (menuPage % 2 == 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage - 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition - 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));

        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
      case 3:
        button = 0;
        if (menuPage % 2 == 0 and cursorPosition % 2 != 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        if (menuPage % 2 != 0 and cursorPosition % 2 == 0) {
          menuPage = menuPage + 1;
          menuPage = constrain(menuPage, 0, maxMenuPages);
        }

        cursorPosition = cursorPosition + 1;
        cursorPosition = constrain(cursorPosition, 0, ((sizeof(menuItems) / sizeof(String)) - 1));
        mainMenuDraw();
        drawCursor();
        activeButton = 1;
        break;
    }
  }
}
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
/*Funkcja wywolywana za kazdym razem gdy wcisniety jest klawisz pod ekranem
  Shield obserwuje spadki napiecia na A0, kazdy przycisk zwraca inna wartosc napiecia.
  Prawo <50
  Gora <195
  Dol <380
  Lewo <790
*/
///////////////////////////////////////////////////////////////////
int evaluateButton(int x) {
  int result = -1;
  if (x < 50) {
    result = 1; // right
  } else if (x < 195) {
    result = 2; // up
  } else if (x < 380) {
    result = 3; // down
  } else if (x < 790) {
    result = 4; // left
  }// else if (0 < x < 1  )
  // {result = 1; // SELECT}
  return result;
}
///////////////////////////////////////////////////////////////////
/////////////////////////// Menu Od 1-6 ///////////////////////////
// 0 = Wilgotnosc Powietrza
// 1 = Wilgotnosc Gleby
// 2 = Swiatlo
// 3 = Poziom UV
// 4 = Rezerwuar
// 5 = Temperatura
// 6 = Pompa Wodna
///////////////////////////////////////////////////////////////////
void menuItem1() {
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Wilgotnosc Pow");
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(15, 0);
  lcd.print(">");
  lcd.setCursor(0, 1);
  lcd.print("Wynosi:");
  lcd.setCursor(15, 1);
  lcd.print("%");

  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    readDHT22(); //odczyt
    lcd.setCursor(9, 1);
    lcd.print(humDHT22);
    delay(50);  // odczyt
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Wstecz
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem2() {
  int activeButton = 0;
  float readSoilValue = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wilgotnosc Gleby");
  lcd.setCursor(0, 1);
  lcd.print("Wynosi:");

  while (activeButton == 0) {
    readSoil();
    readSoilValue = readSoil() / 850 * 100; // (850 - maksymalne wskazanie czujnika)
    lcd.setCursor(9, 1);
    lcd.print(readSoilValue); //wyliczamy procentowa wartosc wilgotnosci
    lcd.setCursor(15, 1);
    lcd.print("%");
    Serial.print("readSoil(): ");
    Serial.print(readSoil());
    Serial.print(" / readSoilValue: ");
    Serial.println(readSoilValue);
    delay(200);
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Wstecz
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem3() {
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Swiatlo    ");
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(15, 0);
  lcd.print(">");
  while (activeButton == 0) {
    int lightSensorValue = digitalRead(lightSensor);
    if (lightSensorValue == 1) {
      lcd.setCursor(0, 1);
      lcd.print("     NOC      ");

    }
    else {
      lcd.setCursor(0, 1);
      lcd.print("     Dzien");
    }
    Serial.println(analogRead(lightSensor));
    Serial.println(digitalRead(lightSensor));
    Serial.println(lightSensorValue);
    delay(50);
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Wstecz
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem4() {
  int activeButton = 0;
  float uvIntensity = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Poziom UV");
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(15, 0);
  lcd.print(">");
  while (activeButton == 0) {
    int uvLevel = averageAnalogRead(UVOUT);
    int refLevel = averageAnalogRead(REF_3V3);
    float outputVoltage = 3.3 / refLevel * uvLevel;
    float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Konwersja napiecia na poziom UV

    float correct = 0;
    if (uvIntensity < 0) // petla korygujaca odczyt ujemnych wartosci (
    {
      correct = correct + abs(uvIntensity);
    }

    Serial.print("OUT A13: ");   Serial.print(refLevel);
    Serial.print("ML8511 OUT: ");   Serial.print(uvLevel);
    Serial.print(" / ML8511 Napiecie: ");   Serial.print(outputVoltage);
    Serial.print(" / UV (mW/cm^2): ");   Serial.print(uvIntensity);
    Serial.print(" / Realne UV (mW/cm^2): ");   Serial.print(uvIntensity + correct);
    Serial.print(" / Korekta: ");   Serial.print(correct);
    Serial.println();

    lcd.setCursor(3, 1);
    lcd.print(abs(uvIntensity + correct));
    lcd.setCursor(7, 1);
    lcd.print("mW/cm^2");
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem5() {
  int activeButton = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Rezerwuar");
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(15, 0);
  lcd.print(">");
  lcd.setCursor(0, 1);
  lcd.print("Wynosi:");
  while (activeButton == 0) {
    float readliquid_level = 0;
    readliquid_level = liquid_level; // 640 * 100 ; // 640 * 100; // (850 - maksymalne wskazanie czujnika)
    liquid_level = analogRead(liqidSensor);
    lcd.setCursor(9, 1);
    lcd.print(readliquid_level);
    lcd.setCursor(14, 1);
    lcd.print(" %");
    Serial.println("readliquid_level: ");
    Serial.println(readliquid_level);
    Serial.println("liquid_level: ");
    Serial.println(liquid_level);
    delay(500);
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Wstecz
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem6() {
  int activeButton = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Temperatura");
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(15, 0);
  lcd.print(">");
  lcd.setCursor(0, 1);
  lcd.print("Wynosi:");
  lcd.setCursor(14, 1);
  lcd.write(byte(3));
  lcd.setCursor(15, 1);
  lcd.print("C");
  while (activeButton == 0) {
    int button;
    readKey = analogRead(0);
    readDHT22(); //odczyt
    lcd.setCursor(9, 1);
    lcd.print(tempDHT22);
    delay(100);  // odczyt
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Wstecz
        button = 0;
        activeButton = 1;
        break;
    }
  }
}

void menuItem7() {
  int activeButton = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Pompa");
  lcd.setCursor(0, 0);
  lcd.print("<");
  lcd.setCursor(15, 0);
  lcd.print(">");
  while (activeButton == 0) {
    pumpSetup();
    int button;
    readKey = analogRead(0);
    if (readKey < 790) {
      delay(100);
      readKey = analogRead(0);
    }
    button = evaluateButton(readKey);
    switch (button) {
      case 4:  // Wstecz
        button = 0;
        activeButton = 1;
        break;
    }
  }
}


