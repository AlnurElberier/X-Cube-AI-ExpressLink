# Developer's Guide: Cloud-Enabled X-CUBE-AI Demo with I-CUBE-ExpressLink

## Overview
This repository extends the [I-CUBE-ExpressLink](https://github.com/stm32-hotspot/I-CUBE-ExpressLink) package to implement a cloud-enabled AI demonstration using X-CUBE-AI. The project runs on the **B-U585I-IOT02A Discovery Kit**, integrating the **Express Link Secure Boot** with **X-CUBE-AI** and **Over-the-Air (OTA)** updates for model weights.

## Features
- **Secure Boot Integration**: Ensures a trusted execution environment.
- **X-CUBE-AI Integration**: Enables AI model execution on the STM32 platform.
- **Cloud Connectivity**: Uses ExpressLink for seamless communication with AWS.
- **OTA Updates for Model Weights**: Allows AI model weights to be updated remotely.
- **Quick Connect Feature**: Simplifies AWS IoT Core registration and provides a serverless dashboard for real-time sensor data and shadow control.
- **Host Over-The-Air (HOTA) Updates**: Enables secure firmware updates across multiple devices.
- **Reserved Memory Regions**: Secure Boot, main application, and model weights.

## Memory Layout
| Section         | Start Address  |
|----------------|---------------|
| Secure Boot    | `0x08000000`   |
| Main App       | `0x08008000`   |
| Model Weights  | `0x080FC000`   |

## Usage Instructions
### Initial Setup
1. **[Register the ExpressLink module with AWS](https://github.com/stm32-hotspot/I-CUBE-ExpressLink?tab=readme-ov-file#3--get-started)** using the Quick Connect feature for streamlined setup.
2. **Clone Repository** 
    `git clone https://github.com/AlnurElberier/X-Cube-AI-ExpressLink`
3. **Sign Application** 
    ```
    cd ./scripts/sign/Local/
    chmod +x sign.sh
    ./sign.sh
    ```
4. **Sign Weights***
    ```
    # cd ./scripts/sign/Local/
    chmod +x sign_weights.sh
    ./sign_weights.sh
    ```
4. **Flash the firmware and weights** 
    ```
    cd ./scripts/flash/
    chmod +x flash_binary.sh
    ./flash_binary.sh
    ```

5. **Run the application** and verify cloud connectivity in aws.

### OTA Update Process
1. **Update the revision file `./B-U585I-IOT02A_FreeRTOS/Inc/application_version.h`** to reflect the new model version.
2. **Import new weights file into STM32CubeMX**
3. **Sign the new weights file** 
    ```
    # cd ./scripts/sign/Local/
    chmod +x sign_weights.sh
    ./sign_weights.sh
    ```
4. **Configure OTA update** using the provided `/scripts/host-ota/hota_update.sh` script. 
5. **Push the OTA update**
    ```
    # cd ./scripts/host-ota/
    chmod +x hota_update.sh
    ./hota_update.sh
    ```

### Secure Boot Considerations
- Before implementing the Secure Boot project, ensure a successful Host Over-The-Air (HOTA) update.
- Refer to the [SecureBoot README](https://github.com/stm32-hotspot/I-CUBE-ExpressLink/blob/main/Projects/SecureBoot/README.md) for detailed steps.

## Contributing
Contributions are welcome! Please submit issues and pull requests to improve this project.

## License
This project follows the **BSD-3-Clause License**. See `LICENSE` for details.

