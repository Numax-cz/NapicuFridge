const fs = require("fs");
const { exec } = require('child_process');

async function main() {
    await cleanDistFolder();

    await buildTypescriptFiles();

    await copyAssets();

    console.log();
    console.log("/data has been successfully created!")
}

function cleanDistFolder() {
    console.log("Removing /data...");
    return new Promise((resolve, reject) => {
        console.log();
        let cmd = exec("npm run clean");

        cmd.on("error", (er) => {
            console.log(er);
        });

        cmd.stdout.on('data', (data) => {
            console.log("/data has been successfully deleted");
        });

        cmd.on('close', (code) => {
            if (code === 0) {
                resolve(code);
            } else {
                reject(code);
            }
        });
    });
}

function buildTypescriptFiles() {
    console.log("Building typescript files...");
    return new Promise((resolve, reject) => {
        console.log();
        let cmd = exec("npm run build-tsc");

        cmd.on("error", (er) => {
            console.log(er);
        });

        cmd.stdout.on('data', (data) => {
            console.log(data.toString());
        });

        cmd.stderr.on('data', (data) => {
            console.log(data)
        });

        cmd.on('close', (code) => {
            if (code === 0) {
                resolve(code);
            } else {
                reject(code);
            }
        });
    });
}

function copyAssets() {
    console.log("Copying assets...");
    return new Promise((resolve, reject) => {
        console.log();
        let cmd = exec("npm run copy");

        cmd.on("error", (er) => {
            console.log(er);
        });

        cmd.stdout.on('data', (data) => {
            console.log(data.toString());
        });

        cmd.stderr.on('data', (data) => {
            console.log(data)
        });

        cmd.on('close', (code) => {
            if (code === 0) {
                resolve(code);
            } else {
                reject(code);
            }
        });
    });
}

main();