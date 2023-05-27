# ESP32WebServerTemplate
ESP32WebServerTemplate is a template project for developing web server applications using ESP32 microcontroller and Arduino framework. It provides a simple and easy-to-use framework for building web applications with TypeScript.
* Without TypeScript: https://github.com/Numax-cz/ESP32WebServerTemplate/tree/non-ts

---

## Instalation

### What do you need? 
- [Git](https://git-scm.com/)
- [Node - LTS](https://nodejs.org/en/)
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

---

1. Use this template

2. Clone the repository

3. Install the packages
   ```sh
   npm install
   ```
4. Set up PlatformIO config if you need

5. Configure WiFi in `src/main.cpp`
   ```c++
   const char *SSID = "";
   const char *PASSWORD = "";
   ```
---
## Upload files to board
1. Run build script 
   ```sh
   npm run build
   ```
   After you run the build script, your data directory has been created.
   
   ![Build](/doc/run_build.png)

2. Build Filesystem Image
   
   ![Build](/doc/build.png)

3. Upload Filesystem Image
   
   ![Build](/doc/upload.png)