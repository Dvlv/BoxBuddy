#!/usr/bin/env bash
grep -L "NoDisplay=true" /usr/share/applications/*.desktop | while read line
do
    NAME=$(grep -m 1 "^Name=" $line | sed 's/Name=//' | tr -d '\n')
    EXEC=$(grep -m 1 "^Exec=" $line | sed 's/Exec=//' | tr -d '\n')
    ICON=$(grep -m 1 "^Icon=" $line | sed 's/Icon=//' | tr -d '\n')
    LINESTRIPPED=$(echo $line | sed 's/\/usr\/share\/applications\///g' | sed 's/\.desktop//g');

    echo "${LINESTRIPPED};${ICON};${NAME};${EXEC}"
done
