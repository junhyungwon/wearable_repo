#!/bin/sh
user="$1" realm="$2" pass="$3" \
&&  printf "%s:%s:%s\n" "$user" "$realm" "$(printf "%s" "$user:$realm:$pass" | md5sum  | awk '{print $1}')" > $4 \
&&  printf "%s:%s:%s\n" "$user" "$realm" "$(printf "%s" "$user:$realm:$pass" | sha256sum  | awk '{print $1}')" >> $4
