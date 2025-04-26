#!/bin/sh

SZ=`"$2" "$1" | tail -n 1 | sed 's,\([0-9]*\).*,\1,'`
[ "$SZ" ] && [ "$SZ" = 54 ] && exit 0
exit 1
