# Publish BMV-712 readings using nRF9160 DK

## Connect BMV-712

- Pin 2 to Pin 14 on DK
- Pin 3 to Pin 15 on DK

Do not connect GND and VDD.

## Readings

The BMV-712 returns these values:

| Property | Value Example | Units    | Description                              |
| -------- | ------------: | -------- | ---------------------------------------- |
| PID      |        0xA381 |          | Product ID                               |
| V        |         12991 | mV       | Main or channel 1 (battery) voltage      |
| T        |            22 | °C       | Battery temperature                      |
| I        |         -4696 | mA       | Main or channel 1 battery current        |
| P        |           -61 | W        | Instantaneous power                      |
| CE       |         -2160 | mAH      | Consumed Amp Hours                       |
| SOC      |           993 | ‰        | State-of-charge                          |
| TTG      |          1750 | Minutes  | Time-to-go                               |
| Alarm    |           OFF |          | Alarm condition active                   |
| Relay    |           OFF |          | Relay state                              |
| AR       |             0 |          | Alarm reason                             |
| BMV      |     712 Smart |          | Model description (deprecated)           |
| FW       |          0403 |          | Firmware version                         |
| H1       |        -28064 | mAh      | Depth of the deepest discharge           |
| H2       |         -2160 | mAh      | Depth of the last discharge              |
| H3       |             0 | mAh      | Depth of the average discharge           |
| H4       |             0 |          | Number of charge cycles                  |
| H5       |             0 |          | Number of full discharges                |
| H6       |        -49512 | mAh      | Cumulative Amp Hours drawn               |
| H7       |          -314 | mV       | Minimum main (battery) voltage           |
| H8       |         14686 | mV       | Maximum main (battery) voltage           |
| H9       |          4747 | Seconds  | Number of seconds since last full charge |
| H10      |             0 |          | Number of automatic synchronizations     |
| H11      |             0 |          | Number of low main voltage alarms        |
| H12      |             0 |          | Number of high main voltage alarms       |
| H15      |             0 | mV       | Minimum auxiliary (battery) voltage      |
| H16      |             0 | mV       | Maximum auxiliary (battery) voltage      |
| H17      |            63 | 0.01 kWh | Amount of discharged energy              |
| H18      |            96 | 0.01 kWh | Amount of charged energy                 |

## Building

### Prepare the Docker image with all build dependencies

    docker build -t zephyr-bmv712reader .

### Build the firmware

    docker run --rm -v ${PWD}:/workdir/ncs/zephyr-bmv712reader zephyr-bmv712reader \
        /bin/bash -c 'cd /workdir/ncs/zephyr-bmv712reader && west build -p always -b nrf9160dk_nrf9160ns'

### Flash the firmware

    nrfjprog -f nrf91 --program build/zephyr/merged.hex --sectoranduicrerase -r --log
