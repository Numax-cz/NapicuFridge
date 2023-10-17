import {AppComponent} from "./app.component";
import {BluetoothLE, OperationResult} from "@awesome-cordova-plugins/bluetooth-le";
import {Configuration} from "./config/configuration";
import {Observable} from "rxjs";
import {FridgePowerMode} from "./interface/Enums";

//Třída pro správu charakteristiky BLE
export class CharacteristicController {
  //Funkce pro přihlášení se k odběru pro získávání dat z vnitřního teploměru (Pokud se vrátí null, zařízení není připojené)
  public static subscribeInTemp(): Observable<OperationResult> | null {
      //Kontrola, zda je zařízení spárované
      if(AppComponent.connected_device) {
          //Přihlášení se k odběru charakteristiky vnitřní teploty
          return BluetoothLE.subscribe({
              address: AppComponent.connected_device.address,
              service: Configuration.SERVICE_UUID,
              characteristic: Configuration.CHARACTERISTIC_DHT_INSIDE_UUID
          });
      }
      //Vrácení null, pokud není připojené zařízení
      return null;
  }

  //Funkce pro přihlášení se k odběru pro získávání dat z venkovního teploměru (Pokud se vrátí null, zařízení není připojené)
  public static subscribeOutTemp():  Observable<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
        //Přihlášení se k odběru charakteristiky venkovní teploty
        return BluetoothLE.subscribe({
            address: AppComponent.connected_device.address,
            service: Configuration.SERVICE_UUID,
            characteristic: Configuration.CHARACTERISTIC_DHT_OUTSIDE_UUID
        });
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro přihlášení se k odběru pro získávání dat z teploměru na chaldiči (Pokud se vrátí null, zařízení není připojené)
  public static subscribeCoolerTemp(): Observable<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Přihlášení se k odběru charakteristiky teploty chladiče
      return BluetoothLE.subscribe({
        address: AppComponent.connected_device.address,
        service: Configuration.SERVICE_UUID,
        characteristic: Configuration.CHARACTERISTIC_NTC_COOLER_UUID
      });
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro zápis stavu displeje chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static writeDisplayState(value: number): Promise<OperationResult> | null {
      //Kontrola, zda je zařízení spárované
      if(AppComponent.connected_device) {
        //Převedení stringu do bytes
        let bytes: Uint8Array = BluetoothLE.stringToBytes(value.toString());
        //Funkce pro převod pole unit8Array na řetězec v kódování base64 pro zápis znaků nebo deskriptorů
        let encodedUnicodeString: string = BluetoothLE.bytesToEncodedString(bytes);
        //Zapsání charakteristiky
        return BluetoothLE.write({
          address: AppComponent.connected_device.address,
          service: Configuration.SERVICE_UUID,
          characteristic: Configuration.CHARACTERISTIC_DISPLAY_STATE_UUID,
          value: encodedUnicodeString,
        });
      }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro čtení stavu displeje z chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static readDisplayState(): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
        //Získání stavu displeje
        return BluetoothLE.read({address: AppComponent.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_DISPLAY_STATE_UUID})
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro zápis vypnutí, nebo zapnutí displeje (Pokud se vrátí null, zařízení není připojené)
  public static writeDisplayAvailable(value: boolean): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device)  {
      //Převedení stringu do bytes
      let bytes: Uint8Array = BluetoothLE.stringToBytes(value ? "1" : "0");
      //Funkce pro převod pole unit8Array na řetězec v kódování base64 pro zápis znaků nebo deskriptorů
      let encodedUnicodeString: string = BluetoothLE.bytesToEncodedString(bytes);
      //Zapsání charakteristiky
     return BluetoothLE.write({
        address: AppComponent.connected_device.address,
        service: Configuration.SERVICE_UUID,
        characteristic: Configuration.CHARACTERISTIC_DISPLAY_ENABLE_UUID,
        value: encodedUnicodeString,
      });
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro čtení zda je display vypnutý, nebo zapnutý (Pokud se vrátí null, zařízení není připojené)
  public static readIsDisplayAvailable(): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
        //Získání zda je displej povolen
        return BluetoothLE.read({address: AppComponent.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_DISPLAY_ENABLE_UUID});
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }


  //Funkce pro zápis vypnutí, nebo zapnutí vnitřních ventilátoru (Pokud se vrátí null, zařízení není připojené)
  public static writeInFansAvailable(value: boolean): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device)  {
      //Převedení stringu do bytes
      let bytes: Uint8Array = BluetoothLE.stringToBytes(value ? "1" : "0");
      //Funkce pro převod pole unit8Array na řetězec v kódování base64 pro zápis znaků nebo deskriptorů
      let encodedUnicodeString: string = BluetoothLE.bytesToEncodedString(bytes);
      //Zapsání charakteristiky
      return BluetoothLE.write({
        address: AppComponent.connected_device.address,
        service: Configuration.SERVICE_UUID,
        characteristic: Configuration.CHARACTERISTIC_IN_FANS_UUID,
        value: encodedUnicodeString,
      });
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro čtení stavu vnitřních ventilátorů z chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static readInFansAvailable(): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Získání stavu displeje
      return BluetoothLE.read({address: AppComponent.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_IN_FANS_UUID});
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro nastavení výkonu chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static writePowerMode(value: FridgePowerMode): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device)  {
      //Převedení hodnoty na string a následně do bytes
      let bytes: Uint8Array = BluetoothLE.stringToBytes(value.toString());
      //Funkce pro převod pole unit8Array na řetězec v kódování base64 pro zápis znaků nebo deskriptorů
      let encodedUnicodeString: string = BluetoothLE.bytesToEncodedString(bytes);
      //Zapsání charakteristiky
      return BluetoothLE.write({
        address: AppComponent.connected_device.address,
        service: Configuration.SERVICE_UUID,
        characteristic: Configuration.CHARACTERISTIC_POWER_MODE_UUID,
        value: encodedUnicodeString,
      });
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro čtení režimu napájení z chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static readPowerMode(): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Získání režimu napájení
      return BluetoothLE.read({address: AppComponent.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_POWER_MODE_UUID});
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro zápis režimu piezo při chybě z chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static writeBuzzingOnError(value: boolean): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device)  {
      //Převedení stringu do bytes
      let bytes: Uint8Array = BluetoothLE.stringToBytes(value ? "1" : "0");
      //Funkce pro převod pole unit8Array na řetězec v kódování base64 pro zápis znaků nebo deskriptorů
      let encodedUnicodeString: string = BluetoothLE.bytesToEncodedString(bytes);
      //Zapsání charakteristiky
      return BluetoothLE.write({
        address: AppComponent.connected_device.address,
        service: Configuration.SERVICE_UUID,
        characteristic: Configuration.CHARACTERISTIC_BUZZING_ON_ERROR_UUID,
        value: encodedUnicodeString,
      });
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro čtení režimu piezo při chybě z chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static readBuzzingOnError(): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Získání režimu piezo při chybě
      return BluetoothLE.read({address: AppComponent.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_BUZZING_ON_ERROR_UUID});
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }

  //Funkce pro čtení doby od spuštění chytré ledničky (Pokud se vrátí null, zařízení není připojené)
  public static readUptime(): Promise<OperationResult> | null {
    //Kontrola, zda je zařízení spárované
    if(AppComponent.connected_device) {
      //Získání doby spuštění chytré ledničky
      return BluetoothLE.read({address: AppComponent.connected_device.address, service: Configuration.SERVICE_UUID, characteristic: Configuration.CHARACTERISTIC_UPTIME_UUID});
    }
    //Vrácení null, pokud není připojené zařízení
    return null;
  }
}
