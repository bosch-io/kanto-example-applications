# Introduction

This application aims to demonstrate how kanto components can be built using gn and ninja. Of course building go applications using this tooling does not bring any benefits on its own. This appraoch can be used to integrate these components in existing builds. 

## Prerequisites

1. Go
1. Python
1. GN
1. Ninja

## Initial preparation

This simple build is preconfigured to build the Eclipse Kanto suite-connector. The BUILD.gn file can be modified and extended to build any other go application.

1. Checkout Eclipse Kanto suite-connector 
    ```bash
    git clone https://github.com/eclipse-kanto/suite-connector.git
    ```
1. Initialize the gn build and configure the target OS and target architecture. This can be done with the following command.
    ```bash
    gn gen out/linux/arm64 --args="target_os=\"linux\" target_cpu=\"arm64\""
    ```
    If you need to build the suite-connector for your host machine these configurations can be omitted
    ```bash
        gn gen out/linux/amd64
    ```

## Execute the build

This build provides two targets. The first one executes the python script BUILD.py which builds the go application and moves it in the output directory. The second one copies the iothub.crt in the output directory, since the suite-connector tries to load this file.

   ```bash
   ninja -C out/linux/arm64 suite-connector iothub-crt
   ```