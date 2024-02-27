# AWS STM32 ML at Edge Accelerator

## Introduction

This Github repository presents an MLOps infrastructure for implementing a Human Activity Recognition model, using the [STM32 Model Zoo](https://github.com/STMicroelectronics/stm32ai-modelzoo). Our infrastructure is designed to streamline the development process, making it faster and more efficient for you to build and deploy your model.
We use AWS SageMaker pipeline to train and generate the Human Activity Recognition model that will run on edge devices (STM32U5 series), with OTA updates using FreeRTOS. The devices are connected to IoT Core, and data is collected via MQTT. Additionally, we are implementing an IaC infrastructure using AWS CDK(Cloud Development Kit).

## Solution Architecture

To deploy our solution, we have designed the following target architecture. Our solution benefits from the integration with [STM32Cube.AI Developer Cloud](https://stm32ai-cs.st.com/home). It provides us with a comprehensive set of tools for benchmarking our model. This integration allows us to measure the memory footprint and inference time of our model, as well as generate the necessary files for the application code.
Our architecture is comprised of three interconnected stacks: ML, IoT, and Pipeline.

1. ML Stack: Responsible for managing the MLOps process.
2. IoT Stack : Responsible for edge deployment, OTA updates, and AWS IoT Core connectivity.
3. Pipeline Stack: Responsible for orchestrating the CI/CD workflow, which automates the deployment of the ML and IoT stacks.

![architecture](./doc/images/architecture.png)

## Deployment

We have built the project using CDK IaC, which allows for easy deployment to your AWS account with a single deploy command. This means that you can quickly and easily set up the infrastructure needed to implement the Human Activity Recognition model on your STM32U5 series target.

### Pre-requisites

- Ensure your AWS credentials are in place for your Pipeline account (The account that will host the pipeline stack if different)
- Ensure you have [Node.js](https://nodejs.org) and [Docker](https://www.docker.com/products/docker-desktop/) installed
- Create an account in [STM32Cube.AI Developer Cloud](https://stm32ai-cs.st.com/home)
- In your AWS Account that will host the ML Stack (if different). Go to secrets manager console page:
  1. Create a new secret for your ST username
      1. Choose type other
      1. Choose Plaintext tab
      1. Overwrite content with you username
      1. Give this secret the following name `STDEVCLOUD_USERNAME_SECRET` (if you require a different name, then ensure to update this new name in `cdk.json`)
  1. Create a new secret for your ST password
      1. Repeat the previous step but with the following secret name `STDEVCLOUD_PASSWORD_SECRET`

- Bootstrap your account (or accounts, if you have more than one) using the following command, replacing the placeholders with your account and region information:

```sh
npx cdk bootstrap aws://<ACCOUNT-NUMBER>/<REGION> --toolkit-stack-name CDKToolkit-StMicro --qualifier stmicrohar
```

### deploy the CICD Pipeline

1. Fork this repo.
1. Clone your fork locally.
1. Go to [cdk.json](./cdk.json) and put your aws account details for each stack in the `env` field, you could deploy the stacks in the same account or in different accounts
1. You need to set up a connection between the pipeline and your forked Repo
   1. Follow the steps to create & verify the connection [here](https://docs.aws.amazon.com/dtconsole/latest/userguide/connections-create-github.html)
   1. Go to [cdk.json](./cdk.json) and paste your forked Repo name, branch & connection Arn in the config section.
1. In the root folder, run the following commands

   ```sh
   npm install
   npm run cdk synth
   ```

1. After editing cdk.json, a cdk.context.json file will be created. Remember to commit both files and push the commit to your forked repo.
1. Once your repo has been updated with the edited cdk.json and the newly created cdk.context.json, run the following command to deploy the changes:

   ```sh
   npm run deploy
   ```

1. For future changes, simply push new commits to your repo and the pipeline will redeploy the updated code.

### Setup the Device

To set up the device, follow these steps assuming you have a [B-U585I-IOT02A](https://www.st.com/en/evaluation-tools/b-u585i-iot02a.html) device.

#### Setup Pre-requisites

- Ensure your AWS credentials are in place for your IoT account (The account that will host the IoT stack if different)
- Download and Install [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) to be able to flash device with initial firmware

  **Note**
  If installing on a Mac make sure you run the following command before installing to give it exec permission `sudo xattr -cr ~/SetupSTM32CubeProgrammer-macos.app`

#### Provision Your Board

To provision your board follow below steps:

1. Navigate to the stm32 folder

   ```sh
   cd stm32
   ```

1. Setup your environment

   ```sh
   sh tools/env_setup.sh
   ```

1. Source your environment

   ```sh
   source .venv/bin/activate
   ```

1. Plug in the device you want to provision

1. To run the ProvisionScript for your device, follow these steps:
   1. Go to the AWS Console and navigate to the CloudFormation page.
   2. Click on IoTStack and view the Output tab.
   3. Copy the ProvisionScript and replace the < > with a unique name for your device and AWS profile. You can keep all default values, but make sure to fill in the WIFI SSID and password details. The script should look like the following:

   ```sh
   python tools/provision.py --interactive --aws-region <Region> --aws-profile <Profile> --thing-name <Thing-Name>
   ```

After successfully provisioning your device, follow these steps:

 1. Go back to the Output tab of the IoTStack in the CloudFormation page.
 2. Look for the public key value and copy it, including the public key labels before and after the code.

**Warning**
Note that the public key displayed in the Output tab may not be properly formatted. Before continuing, please ensure that you fix the format by adding the correct newlines using any text editor. Make sure that you don't miss the line break that splits the key itself into two lines.

```text
-----BEGIN PUBLIC KEY-----
MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAECSEgXkzVQkq2fQwPjIhKIkvJBVCl
VaRDhiyj9BGkHbuAnGzM0gC+z6oYpxbkgE3qa6fHJoE99QTwrRh8XWwyCg==
-----END PUBLIC KEY-----
```

To complete the process, follow these steps:

1. Connect to device using `screen /dev/tty.usb* 115200`,
1. Alternatively, you can connect to the device using screen <Serial_Port_Filename> 115200. You will need to find and replace the correct serial port file name (e.g. /dev/tty.usbmodem1203). The approach will be different for each platform, but you can try running ls /dev/tty.usb*.
1. Run this command `pki import key ota_signer_pub`
1. Paste the public key you copied earlier. You should see a confirmation that the key has been registered. If not, please make sure that you followed the correct formatting for the line breaks as mentioned earlier.

## Flashing the device

Before we can use over the air (OTA) firmware updates, we need to flash the device with the firmware manually at least once. Then future changes will be handled by Freertos OTA updates.

Before we can use over the air (OTA) firmware updates, we need to flash the device with the firmware manually at least once. The binary file will have the follwing name `b_u585i_iot02a_ntz.bin`, please download this file.
If you have already installed [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html), follow these steps to flash the device with the firware.

**Note**
if installing on a Mac make sure you run the following command before installing to give it exec permission `sudo xattr -cr ~/SetupSTM32CubeProgrammer-macos.app`

1. Connect your device to your computer
1. Open STM32CubeProgrammer
1. Click Connect button on top right to connect to device
1. Go to Menu and choose `Erase & programming`
1. Click Browse and choose our binary file we just downloaded from s3
1. Click `Start Programming` to flash binary to device
1. When finished click `Disconnect` on top right corner

**Note**
If you face any issue with device, try clicking on `Full chip erase` and re-program

Once flashed if you are still connected to the device using the screen command i.e. `screen /dev/tty.usbmodem103 115200`, you should see the device starting to log sound class detected from sensors.

## Iot

Your device should now be publishing MQTT messages. To verify this, go to the AWS IoT console page and open the MQTT test client. In the filter, type `<Device_Name>/har_inference_result` and subscribe. You will be able to see the messages coming in from your device.

## Continous Deployment

Whenever changes are made in the repository, the pipeline will be triggered. The pipeline will go through all the stages that have been set up, rebuild the ML model if necessary, and ultimately deploy the updated firmware to the device over the air.

### Over-The-Air updates (OTA)

Please note that any changes made to the firmware will only be accepted by the device if the semantic version is higher than its current version. Therefore, when creating a new release, you must update the semantic versioning in the following file [ota_firmware_version.c](stm32/Projects/b_u585i_iot02a_ntz/Src/ota_pal/ota_firmware_version.c). Once the semantic version has been updated, the changes will be deployed over the air.

So for example lets say we want to remove a class name `Stationary` from the Model.

1. We go to [user_config.yaml](mlops/pipelines/stm/stm32ai-modelzoo/human_activity_recognition/scripts/training/user_config.yaml) in training script section and remove the class in class_names array.
1. We also go to [user_config.yaml](mlops/pipelines/stm/stm32ai-modelzoo/human_activity_recognition/scripts/evaluate/user_config.yaml) in evaluate script section and remove the class in class_names array.
1. Go to [ota_firmware_version.c](stm32/Projects/b_u585i_iot02a_ntz/Src/ota_pal/ota_firmware_version.c) and increment `APP_VERSION_MINOR`
1. Commit and push your changes.

Watch changes propagating through the pipeline till it lands on the devices connected.

## Clean up

To remove all resources created by this stack run the following

```sh
npm run destroy
```

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This library is licensed under the MIT-0 License. See the LICENSE file.
