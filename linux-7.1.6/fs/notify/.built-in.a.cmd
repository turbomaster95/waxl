savedcmd_fs/notify/built-in.a := rm -f fs/notify/built-in.a;  printf "fs/notify/%s " dnotify/built-in.a inotify/built-in.a fanotify/built-in.a | xargs ar cDPrST fs/notify/built-in.a
