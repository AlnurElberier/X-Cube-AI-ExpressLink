#!/bin/bash
#export BOARD='B-L4S5I-IOT01A'
export BOARD='B-U585I-IOT02A'
#export BOARD='NUCLEO-G0B1RE'
#export BOARD='NUCLEO-G071RB'
#export BOARD='NUCLEO-H503RB'
#export BOARD='NUCLEO-U545RE-Q'
#export BOARD='NUCLEO-U575ZI-Q'
#export BOARD='NUCLEO-WB55RG'
#export BOARD='STM32L562E-DK'

export BIN_LOCATION="../../${BOARD}_FreeRTOS/"
# export BIN_FILE=$BOARD'_FreeRTOS.bin'
# export BIN_FILE='signed_binary.bin'
export BIN_FILE='signed_network_data.bin'

export AWS_CLI_PROFILE='default'
export THING_NAME='b16c3350-8ca9-43d2-a626-c8f69ba77fe6'
export ROLE='OTA_ROLE'
export OTA_SIGNING_PROFILE='OTA_SIGNER'
export S3BUCKET='ota-bucket-redmond-iot'
# CERT_ARN is used only when creating new signing profile <OTA_SIGNING_PROFILE>. Ignored by the script if using existing signing profile
export CERT_ARN='arn:aws:acm:us-west-1:006151905315:certificate/a441625c-2041-454c-8e0a-24dec43eae95'

export QC_PATH=$(pwd)

# AWS keys Start
#Reserved for workshop

# AWS Keys End

#Reserved for workshop
#export AWS_REGION="us-east-1"


clear

python3 $QC_PATH/hota_update.py --profile=$AWS_CLI_PROFILE --thing=$THING_NAME --bin-file=$BIN_FILE --bucket=$S3BUCKET --role=$ROLE --signer=$OTA_SIGNING_PROFILE --path=$BIN_LOCATION --certarn=$CERT_ARN

# Reserved for thing group updates
# python3 $QC_PATH/hota_update.py --profile=$AWS_CLI_PROFILE --thing-group=$BOARD --bin-file=$BIN_FILE --bucket=$S3BUCKET --role=$ROLE --signer=$OTA_SIGNING_PROFILE --path=$BIN_LOCATION --certarn=$CERT_ARN

# Reserved for workshop
# python3 $QC_PATH/hota_update.py                          --thing=$THING_NAME --bin-file=$BIN_FILE --bucket=$S3BUCKET --role=$ROLE --signer=$OTA_SIGNING_PROFILE --path=$BIN_LOCATION --certarn=$CERT_ARN --region=$AWS_REGION --accesskey=$AWS_ACCESS_KEY_ID --secretkey=$AWS_SECRET_ACCESS_KEY --sessiontoken=$AWS_SESSION_TOKEN
