execve("./shell", ["./shell"], 0x7ffd407670b0 /* 78 vars */) = 0
brk(NULL)                               = 0x563edfaa9000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=125369, ...}) = 0
mmap(NULL, 125369, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f0c873fd000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260\34\2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=2030544, ...}) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0c873fb000
mmap(NULL, 4131552, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f0c86e04000
mprotect(0x7f0c86feb000, 2097152, PROT_NONE) = 0
mmap(0x7f0c871eb000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e7000) = 0x7f0c871eb000
mmap(0x7f0c871f1000, 15072, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f0c871f1000
close(3)                                = 0
arch_prctl(ARCH_SET_FS, 0x7f0c873fc540) = 0
mprotect(0x7f0c871eb000, 16384, PROT_READ) = 0
mprotect(0x563eded3c000, 4096, PROT_READ) = 0
mprotect(0x7f0c8741c000, 4096, PROT_READ) = 0
munmap(0x7f0c873fd000, 125369)          = 0
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=SIG_DFL, sa_mask=[], sa_flags=0}, 8) = 0
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 3
read(3, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 1), ...}) = 0
brk(NULL)                               = 0x563edfaa9000
brk(0x563edfaca000)                     = 0x563edfaca000
fstat(0, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 1), ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, 0x563edfaa9670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, 8) = 0
rt_sigreturn({mask=[]})                 = 0
read(0, "\n", 1024)                     = 1
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 4
read(4, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, "sleep 120\n", 1024)            = 10
dup(1)                                  = 5
dup(0)                                  = 6
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f0c873fc810) = 19119
wait4(-1, NULL, 0, NULL)                = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_KILLED, si_pid=19119, si_uid=1000, si_status=SIGINT, si_utime=0, si_stime=0} ---
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, 8) = 0
kill(19119, SIGINT)                     = 0
rt_sigreturn({mask=[]})                 = 61
wait4(-1, NULL, 0, NULL)                = 19119
dup2(5, 1)                              = 1
dup2(6, 0)                              = 0
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 7
read(7, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, 0x563edfaa9670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, 8) = 0
kill(19119, SIGINT)                     = -1 ESRCH (No such process)
rt_sigreturn({mask=[]})                 = 0
read(0, "\n", 1024)                     = 1
getcwd("/home/ajainuary/OS/shell", 4096) = 25
write(1, "Error: No such process\n", 23) = 23
read(0, 0x563edfaa9670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, 8) = 0
kill(19119, SIGINT)                     = -1 ESRCH (No such process)
rt_sigreturn({mask=[]})                 = 0
read(0, "\n", 1024)                     = 1
getcwd("/home/ajainuary/OS/shell", 4096) = 25
write(1, "Error: No such process\n", 23) = 23
read(0, 0x563edfaa9670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, 8) = 0
kill(19119, SIGINT)                     = -1 ESRCH (No such process)
rt_sigreturn({mask=[]})                 = 0
read(0, "\n", 1024)                     = 1
getcwd("/home/ajainuary/OS/shell", 4096) = 25
write(1, "Error: No such process\n", 23) = 23
read(0, "\n", 1024)                     = 1
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 8
read(8, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, "\33[Ads\n", 1024)              = 6
dup(1)                                  = 9
dup(0)                                  = 10
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f0c873fc810) = 23219
wait4(-1, NULL, 0, NULL)                = 23219
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=23219, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
dup2(9, 1)                              = 1
dup2(10, 0)                             = 0
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 11
read(11, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, "sd\n", 1024)                   = 3
dup(1)                                  = 12
dup(0)                                  = 13
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f0c873fc810) = 23754
wait4(-1, NULL, 0, NULL)                = 23754
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=23754, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
dup2(12, 1)                             = 1
dup2(13, 0)                             = 0
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 14
read(14, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, "sd\n", 1024)                   = 3
dup(1)                                  = 15
dup(0)                                  = 16
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f0c873fc810) = 24226
wait4(-1, NULL, 0, NULL)                = 24226
--- SIGCHLD {si_signo=SIGCHLD, si_code=CLD_EXITED, si_pid=24226, si_uid=1000, si_status=0, si_utime=0, si_stime=0} ---
dup2(15, 1)                             = 1
dup2(16, 0)                             = 0
getcwd("/home/ajainuary/OS/shell", 4096) = 25
getuid()                                = 1000
openat(AT_FDCWD, "/etc/passwd", O_RDONLY) = 17
read(17, "root:x:0:0:root:/root:/bin/bash\n"..., 1000000) = 2580
uname({sysname="Linux", nodename="ajainuary", ...}) = 0
write(1, "<ajainuary@ajainuary:~> ", 24) = 24
read(0, 0x563edfaa9670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGINT {si_signo=SIGINT, si_code=SI_KERNEL} ---
rt_sigaction(SIGINT, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, {sa_handler=0x563edeb39f80, sa_mask=[INT], sa_flags=SA_RESTORER|SA_RESTART, sa_restorer=0x7f0c86e42f20}, 8) = 0
kill(24226, SIGINT)                     = -1 ESRCH (No such process)
rt_sigreturn({mask=[]})                 = 0
read(0, "\n", 1024)                     = 1
getcwd("/home/ajainuary/OS/shell", 4096) = 25
write(1, "Error: No such process\n", 23) = 23
read(0, 0x563edfaa9670, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGHUP {si_signo=SIGHUP, si_code=SI_KERNEL} ---
+++ killed by SIGHUP +++
