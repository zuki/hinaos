# QEMU/RISC-Vの実行メモ

- macで実行
- 必要なパッケージはすべてインストール済みだった

```
$ make
    UPDATE  build/consts.mk
        CC  kernel/main.c
        CC  kernel/printk.c
        CC  kernel/memory.c
        CC  kernel/task.c
        CC  kernel/interrupt.c
        CC  kernel/ipc.c
        CC  kernel/syscall.c
        CC  servers/vm/main.c
        CC  servers/vm/task.c
        CC  servers/vm/bootfs.c
        CC  servers/vm/pm.c
        CC  servers/vm/page_fault.c
        CC  servers/crack/main.c
        CC  libs/common/list.c
        CC  libs/common/vprintf.c
        CC  libs/common/backtrace.c
        CC  libs/common/symbol_table.S
        CC  libs/common/string.c
        CC  libs/common/ubsan.c
        CC  libs/common/error.c
        CC  libs/common/message.c
        CC  libs/common/riscv32/backtrace.c
        LD  build/libs/common.o
        CC  libs/user/printf.c
        CC  libs/user/syscall.c
        CC  libs/user/malloc.c
        CC  libs/user/init.c
        CC  libs/user/ipc.c
        CC  libs/user/task.c
        CC  libs/user/driver.c
        CC  libs/user/dmabuf.c
        CC  libs/user/riscv32/start.S
        CC  libs/user/virtio/virtio_mmio.c
        LD  build/libs/user.o
        CC  build/program_names/crack.c
       GEN  build/user_ld_params.h
       CPP  build/servers/crack/user.ld
        LD  build/servers/crack.elf
        NM  build/servers/crack.elf
   SYMBOLS  build/servers/crack.symbols
       GEN  build/servers/crack.elf.gdb
     STRIP  build/servers/crack.elf
        CC  servers/fs/main.c
        CC  servers/fs/block.c
        CC  servers/fs/fs.c
        CC  build/program_names/fs.c
       CPP  build/servers/fs/user.ld
        LD  build/servers/fs.elf
        NM  build/servers/fs.elf
   SYMBOLS  build/servers/fs.symbols
       GEN  build/servers/fs.elf.gdb
     STRIP  build/servers/fs.elf
        CC  servers/hello/main.c
        CC  build/program_names/hello.c
       CPP  build/servers/hello/user.ld
        LD  build/servers/hello.elf
        NM  build/servers/hello.elf
   SYMBOLS  build/servers/hello.symbols
       GEN  build/servers/hello.elf.gdb
     STRIP  build/servers/hello.elf
        CC  servers/hello_hinavm/main.c
        CC  build/program_names/hello_hinavm.c
       CPP  build/servers/hello_hinavm/user.ld
        LD  build/servers/hello_hinavm.elf
        NM  build/servers/hello_hinavm.elf
   SYMBOLS  build/servers/hello_hinavm.symbols
       GEN  build/servers/hello_hinavm.elf.gdb
     STRIP  build/servers/hello_hinavm.elf
        CC  servers/pong/main.c
        CC  build/program_names/pong.c
       CPP  build/servers/pong/user.ld
        LD  build/servers/pong.elf
        NM  build/servers/pong.elf
   SYMBOLS  build/servers/pong.symbols
       GEN  build/servers/pong.elf.gdb
     STRIP  build/servers/pong.elf
        CC  servers/shell/main.c
        CC  servers/shell/command.c
        CC  servers/shell/fs.c
        CC  servers/shell/http.c
        CC  build/program_names/shell.c
       CPP  build/servers/shell/user.ld
        LD  build/servers/shell.elf
        NM  build/servers/shell.elf
   SYMBOLS  build/servers/shell.symbols
       GEN  build/servers/shell.elf.gdb
     STRIP  build/servers/shell.elf
        CC  servers/tcpip/main.c
        CC  servers/tcpip/mbuf.c
        CC  servers/tcpip/device.c
        CC  servers/tcpip/ethernet.c
        CC  servers/tcpip/arp.c
        CC  servers/tcpip/ipv4.c
        CC  servers/tcpip/tcp.c
        CC  servers/tcpip/udp.c
        CC  servers/tcpip/dhcp.c
        CC  servers/tcpip/dns.c
        CC  build/program_names/tcpip.c
       CPP  build/servers/tcpip/user.ld
        LD  build/servers/tcpip.elf
        NM  build/servers/tcpip.elf
   SYMBOLS  build/servers/tcpip.symbols
       GEN  build/servers/tcpip.elf.gdb
     STRIP  build/servers/tcpip.elf
        CC  servers/virtio_blk/main.c
        CC  build/program_names/virtio_blk.c
       CPP  build/servers/virtio_blk/user.ld
        LD  build/servers/virtio_blk.elf
        NM  build/servers/virtio_blk.elf
   SYMBOLS  build/servers/virtio_blk.symbols
       GEN  build/servers/virtio_blk.elf.gdb
     STRIP  build/servers/virtio_blk.elf
        CC  servers/virtio_net/main.c
        CC  build/program_names/virtio_net.c
       CPP  build/servers/virtio_net/user.ld
        LD  build/servers/virtio_net.elf
        NM  build/servers/virtio_net.elf
   SYMBOLS  build/servers/virtio_net.symbols
       GEN  build/servers/virtio_net.elf.gdb
     STRIP  build/servers/virtio_net.elf
  MKBOOTFS  build/bootfs.bin
        CC  servers/vm/bootfs_image.S
        CC  build/program_names/vm.c
       CPP  build/servers/vm/user.ld
        LD  build/servers/vm.elf
        NM  build/servers/vm.elf
   SYMBOLS  build/servers/vm.symbols
       GEN  build/servers/vm.elf.gdb
     STRIP  build/servers/vm.elf
        CC  kernel/bootelf.S
        CC  kernel/hinavm.c
        CC  kernel/riscv32/boot.S
        CC  kernel/riscv32/setup.c
        CC  kernel/riscv32/task.c
        CC  kernel/riscv32/vm.c
        CC  kernel/riscv32/mp.c
        CC  kernel/riscv32/switch.S
        CC  kernel/riscv32/handler.S
        CC  kernel/riscv32/trap.c
        CC  kernel/riscv32/usercopy.S
        CC  kernel/riscv32/debug.c
        CC  kernel/riscv32/uart.c
        CC  kernel/riscv32/plic.c
        CC  build/program_names/kernel.c
       CPP  build/kernel/kernel.ld
        LD  build/hinaos.elf
        NM  build/hinaos.elf
   SYMBOLS  build/hinaos.symbols
       GEN  build/hinaos.elf.gdb
     STRIP  build/hinaos.elf
      MKFS  build/hinafs.img
       GEN  build/compile_commands.json

Kernel Executable: build/hinaos.elf (3733 KiB)
BootFS Image:      build/bootfs.bin (3016 KiB)
HinaFS Image:      build/hinafs.img (131072 KiB)
dspace@mini:~/hobby_os/microkernel-book$ make run
    UPDATE  build/consts.mk
       GEN  build/compile_commands.json

Kernel Executable: build/hinaos.elf (3733 KiB)
BootFS Image:      build/bootfs.bin (3016 KiB)
HinaFS Image:      build/hinafs.img (131072 KiB)
      QEMU  build/hinaos.elf
Booting HinaOS...
[kernel] free memory: 803bd000 - 88000000 (124MiB)
[kernel] MMIO memory: 10001000 - 10002000 (4KiB)
[kernel] MMIO memory: 10002000 - 10003000 (4KiB)
[kernel] created a task "vm" (tid=1)
[kernel] bootelf: 32000000 - 3200c9b0 r-x (50 KiB)
[kernel] bootelf: 3200d000 - 3234144a r-- (3281 KiB)
[kernel] bootelf: 32342000 - 32789118 rw- (4380 KiB)
[kernel] CPU #0 is ready
[vm] bootfs: found following 9 files
[vm] bootfs: "pong" (323 KiB)
[vm] bootfs: "virtio_blk" (323 KiB)
[vm] bootfs: "hello" (323 KiB)
[vm] bootfs: "hello_hinavm" (323 KiB)
[vm] bootfs: "tcpip" (379 KiB)
[vm] bootfs: "crack" (323 KiB)
[vm] bootfs: "virtio_net" (323 KiB)
[vm] bootfs: "shell" (342 KiB)
[vm] bootfs: "fs" (342 KiB)
[vm] launching pong...
[kernel] created a task "pong" (tid=2)
[vm] launching virtio_blk...
[kernel] created a task "virtio_blk" (tid=3)
[vm] launching tcpip...
[kernel] created a task "tcpip" (tid=4)
[vm] launching virtio_net...
[kernel] created a task "virtio_net" (tid=5)
[vm] launching shell...
[kernel] created a task "shell" (tid=6)
[vm] launching fs...
[kernel] created a task "fs" (tid=7)
[vm] ready
[vm] service "pong" is up
[pong] ready
[tcpip] starting...
[vm] fs: waiting for service "blk_device"
[vm] service "blk_device" is up
[vm] tcpip: waiting for service "net_device"
[virtio_blk] ready
[fs] block 0 is not in cache, reading from disk
[virtio_net] MAC address = 52:54:00:12:34:56
[vm] service "net_device" is up
[virtio_net] ready
[fs] block 1 is not in cache, reading from disk
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[vm] service "tcpip" is up
[tcpip] ready
[fs] block 2 is not in cache, reading from disk
[fs] block 3 is not in cache, reading from disk
[fs] block 4 is not in cache, reading from disk
[fs] block 5 is not in cache, reading from disk
[fs] successfully loaded the file system
[vm] service "fs" is up
[fs] ready

Welcome to HinaOS!

shell> ls
[shell] Contents of /:
[fs] block 7 is not in cache, reading from disk
[shell] [FILE] "hello.txt"
shell> cat hello.txt
[fs] block 6 is not in cache, reading from disk
[shell] Hello World from HinaFS!

shell> help
[shell]
[shell] Available commands:
[shell]
[shell]   help: Show this help
[shell]   echo: Print arguments
[shell]   http: Fetch a URL
[shell]   cat: Show file contents
[shell]   write: Write text to a file
[shell]   ls: List files in a directory
[shell]   mkdir: Create a directory
[shell]   delete: Delete a file or directory
[shell]   start: Launch a task from bootfs
[shell]   sleep: Pause for a while
[shell]   ping: Send a ping to pong server
[shell]   uptime: Show seconds since boot
[shell]   shutdown: Shut down the system
[shell]
shell> ls .
[shell] Contents of .:
[fs] PANIC: detected an undefined behavior: pointer overflow (ip=22001e2e)
[fs] WARN:     #0: 220053ea __ubsan_handle_pointer_overflow()+0x40
[fs] WARN:     #1: 22001e2e lookup()+0x35c
[fs] WARN:     #2: 22002a48 fs_readdir()+0x22
[fs] WARN:     #3: 22000546 main()+0x546
[fs] WARN:     #4: 22008462 start()+0x12
[kernel] exiting a task "fs" (tid=7)
[kernel] destroying a task "fs" (tid=7)
[vm] fs exited gracefully
QEMU 6.2.0 monitor - type 'help' for more information
(qemu) q
dspace@mini:~/hobby_os/microkernel-book$ make run
    UPDATE  build/consts.mk
       GEN  build/compile_commands.json

Kernel Executable: build/hinaos.elf (3733 KiB)
BootFS Image:      build/bootfs.bin (3016 KiB)
HinaFS Image:      build/hinafs.img (131072 KiB)
      QEMU  build/hinaos.elf
Booting HinaOS...
[kernel] free memory: 803bd000 - 88000000 (124MiB)
[kernel] MMIO memory: 10001000 - 10002000 (4KiB)
[kernel] MMIO memory: 10002000 - 10003000 (4KiB)
[kernel] created a task "vm" (tid=1)
[kernel] bootelf: 32000000 - 3200c9b0 r-x (50 KiB)
[kernel] bootelf: 3200d000 - 3234144a r-- (3281 KiB)
[kernel] bootelf: 32342000 - 32789118 rw- (4380 KiB)
[kernel] CPU #0 is ready
[vm] bootfs: found following 9 files
[vm] bootfs: "pong" (323 KiB)
[vm] bootfs: "virtio_blk" (323 KiB)
[vm] bootfs: "hello" (323 KiB)
[vm] bootfs: "hello_hinavm" (323 KiB)
[vm] bootfs: "tcpip" (379 KiB)
[vm] bootfs: "crack" (323 KiB)
[vm] bootfs: "virtio_net" (323 KiB)
[vm] bootfs: "shell" (342 KiB)
[vm] bootfs: "fs" (342 KiB)
[vm] launching pong...
[kernel] created a task "pong" (tid=2)
[vm] launching virtio_blk...
[kernel] created a task "virtio_blk" (tid=3)
[vm] launching tcpip...
[kernel] created a task "tcpip" (tid=4)
[vm] launching virtio_net...
[kernel] created a task "virtio_net" (tid=5)
[vm] launching shell...
[kernel] created a task "shell" (tid=6)
[vm] launching fs...
[kernel] created a task "fs" (tid=7)
[vm] ready
[vm] service "pong" is up
[pong] ready
[tcpip] starting...
[vm] fs: waiting for service "blk_device"
[vm] service "blk_device" is up
[vm] tcpip: waiting for service "net_device"
[virtio_blk] ready
[fs] block 0 is not in cache, reading from disk
[virtio_net] MAC address = 52:54:00:12:34:56
[vm] service "net_device" is up
[virtio_net] ready
[fs] block 1 is not in cache, reading from disk
[tcpip] dhcp: leased ip=10.0.2.15, netmask=255.255.255.0, gateway=10.0.2.2
[vm] service "tcpip" is up
[tcpip] ready
[fs] block 2 is not in cache, reading from disk
[fs] block 3 is not in cache, reading from disk
[fs] block 4 is not in cache, reading from disk
[fs] block 5 is not in cache, reading from disk
[fs] successfully loaded the file system
[vm] service "fs" is up
[fs] ready

Welcome to HinaOS!

shell> ls /
[shell] Contents of /:
[fs] block 7 is not in cache, reading from disk
[shell] [FILE] "hello.txt"
shell> help
[shell]
[shell] Available commands:
[shell]
[shell]   help: Show this help
[shell]   echo: Print arguments
[shell]   http: Fetch a URL
[shell]   cat: Show file contents
[shell]   write: Write text to a file
[shell]   ls: List files in a directory
[shell]   mkdir: Create a directory
[shell]   delete: Delete a file or directory
[shell]   start: Launch a task from bootfs
[shell]   sleep: Pause for a while
[shell]   ping: Send a ping to pong server
[shell]   uptime: Show seconds since boot
[shell]   shutdown: Shut down the system
[shell]
shell> exit
[shell] WARN: unknown command: exit
shell> quit
[shell] WARN: unknown command: quit
shell> QEMU: Terminated
```

```
shell> http http://localhost/index.html
[tcpip] tcp: TX: lport=49152, seq=00000000, ack=00000000, len=0 [ SYN ]
[tcpip] tcp: RX: lport=49152, seq=0000fa01, ack=00000001, len=0 [ SYN ACK ]
[tcpip] tcp: TX: lport=49152, seq=00000001, ack=0000fa02, len=48 [ ACK PSH ]
[tcpip] tcp: RX: lport=49152, seq=0000fa02, ack=00000031, len=868 [ ACK PSH ]
[tcpip] tcp: RX: lport=49152, seq=0000fd66, ack=00000031, len=0 [ FIN ACK ]
[tcpip] tcp: TX: lport=49152, seq=00000031, ack=0000fd67, len=0 [ ACK ]
[shell] HTTP/1.1 200 OK
Date: Mon, 15 May 2023 00:21:46 GMT
Server: Apache/2.4.53 (Unix) PHP/8.1.4
Last-Modified: Tue, 26 Jan 2016 14:39:38 GMT
ETag: "264-52a3da7ac1e80"
Accept-Ranges: bytes
Content-Length: 612
Connection: close
Content-Type: text/html

<!DOCTYPE html>
<html>
<head>
<title>Welcome to nginx!</title>
<style>
    body {
        width: 35em;
        margin: 0 auto;
        font-family: Tahoma, Verdana, Arial, sans-serif;
    }
</style>
</head>
<body>
<h1>Welcome to nginx!</h1>
<p>If you see this page, the nginx web server is successfully installed and
working. Further configuration is required.</p>

<p>For online documentation and support please refer to
<a href="http://nginx.org/">nginx.org</a>.<br/>
Commercial support is available at
<a href="http://nginx.com/">nginx.com</a>.</p>

<p><em>Thank you for using nginx.</em></p>
</body>
</html>

[shell]
shell> http http://192.168.10.103/index.html
[tcpip] tcp: TX: lport=49153, seq=00000000, ack=00000000, len=0 [ SYN ]
QEMU: Terminated
```
