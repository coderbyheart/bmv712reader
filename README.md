# Publish BMV-712 readings using nRF9160 DK

## Building

### Prepare the Docker image with all build dependencies

    docker build -t zephyr-bmv712reader .

### Build the firmware

    docker run --rm -v ${PWD}:/workdir/ncs/zephyr-bmv712reader zephyr-bmv712reader \
        /bin/bash -c 'cd /workdir/ncs/zephyr-bmv712reader && west build -p always -b nrf9160dk_nrf9160ns'

### Flash the firmware

    nrfjprog -f nrf91 --program build/zephyr/merged.hex --sectoranduicrerase -r --log
