#!/usr/bin/env bash
grep -L "NoDisplay=true" /usr/share/applications/*.desktop | sed 's/\/usr\/share\/applications\///g' | sed 's/\.desktop//g'
