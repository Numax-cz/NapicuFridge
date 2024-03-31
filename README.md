<h1 align="center">
  <br>
  <img src="./NapicuFridge/src/assets/fridge.png" alt="NapicuFridge image" width="130">
  <br>
    NapicuFridge
  <br>
</h1>

- NapicuFridge je OpenSource školní projekt chytré mini ledničky s mobilní aplikací.
- Tento projekt mi sloužil pro úspěšné ukončení studia na SŠ.
- Celá chytrá lednička je postavena na ESP32-WROOM-32D. Ke komunikaci s ESP32 a mobilním telefonem je využíváno spojení skrze BluetoothLE.
- Chladící systém obstarávají peltierovy články.
- Celou dokumentaci této práce najdete v souboru dokumentace.pdf.
---

## Instalace
### Co potřebuji ?
- [Git](https://git-scm.com/)
- [Node - LTS](https://nodejs.org/en/)
- [Android Studio](https://developer.android.com/studio)
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/)

### Naklonujte repozitář
   ```sh
   git clone https://github.com/Numax-cz/NapicuFridge.git
   ```
### DŮLEŽITÉ UPOZORNĚNÍ
- Aplikace je ve výchozím stavu dostupná pouze pro platformu Android. Pro práci na
platformě IOS je potřeba změnit parametry příkazů v package.json a přizpůsobit kód.

### NAHRÁNÍ KÓDU NA ESP32
1. Nainstalujeme veškeré prostředky. V našem případě Visual Studio Code + PlatformIO
   (Lze použít i jiné, např. Arduino IDE).
2. Načteme projekt ve Visual Studio Code a necháme nainstalovat veškeré balíčky
3. Pomocí zkratky `Ctrl + Alt + U`, nebo zmáčkneme šipku v pravém horním rohu pro
   sestavení kódu a nahrání na ESP32. 

### SPUŠTĚNÍ MOBILNÍ APLIKACE
1. Nainstalujeme veškeré prostředky.
2. Ve složce `NapicuFridge/` nainstalujeme veškeré balíčky pomocí příkazu `npm
   install`. 
3. Pomocí příkazu `npm run start` spustíme webovou aplikace na localhostu na portu 4200. 
   - Ve webovém rozhraní aplikace nebude umožňovat určité funkce, jelikož není dostupná
   cordova a ve vývojářské konzole uvidíme chyby. Cordova je dostupná, jakmile se aplikace
   spustí nativně na mobilním zařízení.

### BUILDNUTÍ APLIKACE PRO MOBILNÍ ZAŘÍZENÍ
1. Pomocí příkazu `npm run sync` zkompilujeme webové prostředky pro platformu
   Android. Následně se vygeneruje složka `NapicuFridge/Android`.
2. Pomocí příkazu `npm run open` otevřeme Android Studio (pokud je nainstalované).
3. Pomocí příkazu `npm run gen-icon` vygenerujeme aplikační ikonu. Ikona pro
   vygenerování se musí nacházet ve složce `NapicuFridge/src/assets` Pod jménem
   `icon`
---

## Použité zdroje
- https://ionicframework.com - Ionic, sada vývojových nástrojů pro hybridní vývoj mobilních
aplikací
- https://cordova.apache.org – Cordova, framework pro vývoj mobilních aplikací
- https://github.com/don/cordova-plugin-ble-central - Balíček pro Cordova Framework, slouží
pro komunikaci skrze Bluetooth Low Energy
- https://github.com/apache/cordova-plugin-screen-orientation - Balíček pro Cordova
Framework, slouží pro správu orientace obrazovky
- https://angular.io – Framework pro vývoj jednostránkových webových aplikací
- https://www.sliderrevolution.com/resources/styling-radio-buttons - Web ze kterého byl
čerpán grafický styl přepínacího tlačítka v aplikaci
- https://github.com/adafruit/DHT-sensor-library - Balíček pro správu teplotního modulu
(DHT11)
- https://github.com/adafruit/Adafruit_Sensor - Balíček pro jednoduchou správu senzorů
- https://github.com/adafruit/Adafruit-GFX-Library - Základní grafická knihovna pro displeje
- https://github.com/adafruit/Adafruit_SSD1306 - Grafická knihovna pro OLED displeje
- https://github.com/YuriiSalimov/NTC_Thermistor - Balíček pro jednoduchou správu NTC
termistorů
- https://github.com/minhaj6/DigiPotX9Cxxx - Balíček pro jednoduchou správu digitálního
potenciometru
- https://arduinojson.org – Balíček pro správu JSON dokumentů
- https://github.com/swimlane/ngx-charts - Balíček pro tvorbu grafů
- https://github.com/Numax-cz/NapicuDateFormatter - Můj vlastní balíček pro jednoduché
formátovaní času
- https://www.npmjs.com/package/@capacitor/clipboard - Balíček pro jednoduchou správu
kopírovací schránky zařízení
- https://github.com/danisss9/ngx-slider - Balíček pro vlastní posuvník v HTML
- https://github.com/scttcper/ngx-color - Balíček pro výběr barev v HTML

## Použité vyvojové prostředí
- https://www.jetbrains.com/idea - Intellij IDEA 2023 Ultimate
- https://code.visualstudio.com – Visual Studio Code
- https://developer.android.com/studio - Android Studio
- https://www.autodesk.cz/products/fusion-360 - Autodesk Fusion 360

## Další zdroje
- https://365.altium.com/files/C4E20C96-C92D-11EB-A2F6-0A0ABF5AFC1B - Schéma digitálního
potenciometru – modul X9C103S
- https://cz.pinterest.com/pin/345158758924878376 - Schéma relé modulu HL-52S
- https://img.radiokot.ru/files/98120/medium/kc1tmkvhv.jpg - Schéma měniče