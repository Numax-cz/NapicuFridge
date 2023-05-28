import { CapacitorConfig } from '@capacitor/cli';

const config: CapacitorConfig = {
  appId: 'com.napicu.app',
  appName: 'NapicuFridge',
  webDir: 'dist/napicu-fridge-app',
  server: {
    androidScheme: 'https'
  },
};

export default config;
