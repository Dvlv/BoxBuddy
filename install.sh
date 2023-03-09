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

# copy icons
mkdir -p ${DATAHOME}/boxbuddy/
cp -r data/imgs ${DATAHOME}/boxbuddy/
cp -r data/imgs ${DATAHOME}/boxbuddy/

# copy binary and scripts
mkdir -p ${BIN_DIR}
cp boxbuddy ${BIN_DIR}/
cp src/*.sh ${BIN_DIR}/

# copy icons
mkdir -p ~/.icons/

cp -r data/icons/* ~/.icons/

# desktop
cp boxbuddy.desktop ${DATAHOME}/applications/
