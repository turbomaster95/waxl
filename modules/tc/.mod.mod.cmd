savedcmd_mod.mod := printf '%s\n'   mod.o | awk '!x[$$0]++ { print("./"$$0) }' > mod.mod
