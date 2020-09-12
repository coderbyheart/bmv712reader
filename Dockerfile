FROM coderbyheart/fw-nrfconnect-nrf-docker:latest
RUN rm -rf /workdir/ncs
COPY . /workdir/ncs/zephyr-bmv712reader
RUN \
    # Zephyr requirements of zephyr-bmv712reader
    cd /workdir/ncs/zephyr-bmv712reader; west init -l && \
    cd /workdir/ncs; west update && \
    pip3 install -r zephyr/scripts/requirements.txt && \
    pip3 install -r nrf/scripts/requirements.txt && \
    pip3 install -r bootloader/mcuboot/scripts/requirements.txt && \
    rm -rf /workdir/ncs/zephyr-bmv712reader
# Pull in https://github.com/nrfconnect/sdk-nrfxlib/pull/250
RUN \
    cd /workdir/ncs/nrfxlib && \
        git remote add evenl https://github.com/evenl/nrfxlib.git && \
        git fetch evenl && \
        git checkout -t evenl/bsdlib_release_0.7.9
        