#/usr/bin/env bash
if [ -z "${XDG_DATA_HOME}" ]; then 
    DATAHOME=~/.local/share
else 
    DATAHOME=${XDG_DATA_HOME}
fi

if [ -z "${INSTALL_DIR}" ]; then 
    BIN_DIR=~/.local/bin
else 
    BIN_DIR=${INSTALL_DIR}/bin
fi

# copy images
mkdir -p ${DATAHOME}/boxbuddy/
cp -r data/imgs ${DATAHOME}/boxbuddy/
cp -r data/imgs ${DATAHOME}/boxbuddy/

# copy binary and scripts
mkdir -p ${BIN_DIR}
cp boxbuddy ${BIN_DIR}
cp src/*.sh ${BIN_DIR}

# copy icons
mkdir -p ${DATAHOME}/icons/boxbuddy/
cp -r data/imgs/boxbuddy.svg ${DATAHOME}/icons/boxbuddy/

# desktop
cp boxbuddy.desktop ${DATAHOME}/applications/
sed -i "s|Icon=.*|Icon=${DATAHOME}/icons/boxbuddy/boxbuddy.svg|" ${DATAHOME}/applications/boxbuddy.desktop
