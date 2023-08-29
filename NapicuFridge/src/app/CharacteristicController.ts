import {AppComponent} from "./app.component";
import {BluetoothLE, OperationResult} from "@awesome-cordova-plugins/bluetooth-le";
import {Configuration} from "./config/configuration";
import {Observable} from "rxjs";

//Třída pro správu charakteristiky BLE
export class CharacteristicController {
  //Funkce pro přihlášení se k odběru pro získávání dat z vnitřního teploměru
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

  //Funkce pro přihlášení se k odběru pro získávání dat z venkovního teploměru
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
}
