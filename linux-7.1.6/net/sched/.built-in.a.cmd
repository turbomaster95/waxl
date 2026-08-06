savedcmd_net/sched/built-in.a := rm -f net/sched/built-in.a;  printf "net/sched/%s " sch_generic.o sch_mq.o | xargs ar cDPrST net/sched/built-in.a
