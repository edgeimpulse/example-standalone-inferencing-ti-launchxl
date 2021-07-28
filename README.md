# example-standalone-inferencing-ti-launchxl
Builds and runs an exported impulse locally (Texas Instruments LaunchXL)


## Texas Instruments Simplelink SDK

- Uses the TI Simplelink SDK `simplelink_cc13x2_26x2_sdk_5.20.00.52`
- SDK directory should be next to project directory
- If this is not desired change `SIMPLELINK_CC13X2_26X2_SDK_INSTALL_DIR`

## Building the application

1. Build the application by calling `make` as follows:

    ```
    $ cd gcc
    $ make
    ```
1. Connect the board to your computer using USB.
1. Flash the board

    **Windows**

    ```
    $ dslite.bat -c tools\user_files\configs\cc1352p1f3.ccxml -l tools\user_files\settings\generated.ufsettings -e -f -v gcc\build\edge-impulse-standalone.out
    ```

    **Linux, macOS**

    ```
    $ dslite.sh -c tools/user_files/configs/cc1352p1f3.ccxml -l tools/user_files/settings/generated.ufsettings -e -f -v gcc/build/edge-impulse-standalone.out
    ```

    *Note*: the above expects [UniFlash](https://www.ti.com/tool/UNIFLASH#downloads) is installed and `PATH` is set up properly to find `dslite.sh`.

### Or build with Docker

1. Build the Docker image:
    ```
    $ docker build -t ti-build .
    ```
1. Build the application by running the container as follows:

    **Windows**

    ```
    $ docker run --rm -it -v "%cd%":/app ti-build /bin/bash -c "cd gcc && make"
    ```

    **Linux, macOS**

    ```
    $ docker run --rm -it -v $PWD:/app:delegated ti-build /bin/bash -c "cd gcc && make"
    ```

1. Connect the board to your computer using USB.
1. Flash the board:

    ```
    $ dslite.sh -c tools/user_files/configs/cc1352p1f3.ccxml -l tools/user_files/settings/generated.ufsettings -e -f -v gcc/build/edge-impulse-standalone.out
    ```

## Troubleshooting

**fatal: IcePick_C: Error initializing emulator: (Error -260 @ 0x0)**

If during flashing you encounter the above titled issue. Then..

Ensure:

1. your device is properly connected and/or your cable is not damaged. 
2. you have the proper permissions to access the USB device.

If on Linux you may want to try copying `tools/71-ti-permissions.rules` to `/etc/udev/rules.d/`. Then re-attach the USB cable and try again.

