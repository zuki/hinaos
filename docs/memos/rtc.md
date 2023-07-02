# Goldfish RTCドライバの追加

## RTCドライバの作成

```bash
$ vi messages.idl
$ mkdir -p servers/rtc
$ vi build.mk
$ vi rtc.h
$ vi main.c
$ vi servers/shell/command.c
$ vi Makefile
$ make
...
ld.lld: error: undefined symbol: __udivdi3
>>> referenced by rtc.h:61 (servers/rtc/rtc.h:61)
>>>               build/servers/rtc/main.o:(main)
make: *** [build/servers/rtc.elf] Error 1
```

## __udivdi3関数を追加

https://github.com/raphael/linux-samus/blob/master/build/linux/arch/riscv/lib/udivdi3.S
を見つけて導入。

```bash
$ vi libs/common/riscv32/udivdi3.S
$ vi libs/common/riscv32/build.mk
$ make
...
Kernel Executable: build/hinaos.elf (4024 KiB)
BootFS Image:      build/bootfs.bin (3316 KiB)
HinaFS Image:      build/hinafs.img (131072 KiB)
$ make run
...
[rtc] WARN: UBSan: libs/user/ipc.c:247: pointer 2a48f4d4 is not aligned to 8
[rtc] WARN: UBSan: libs/user/ipc.c:247: pointer 2a48f4d4 is not aligned to 8
[kernel] WARN: UBSan: kernel/interrupt.c:99: pointer 803ed1ec is not aligned to 8   // これが多発
[rtc] rtc_uaddr: 0x2a491000
```

## gettimeofdayの定義を変更

`rpc gettimeofday() -> (time: uint64);` => `rpc gettimeofday() -> (high: uint, low: uint);`

```
$ make run
[kernel] WARN: rtc: vm_map: no page for paddr 00101000
[vm] WARN: vm_map failed: Invalid Physical Address
...
[fs] ready

Welcome to HinaOS!

shell> time
[vm] ERROR: unknown memory address (addr=2a491000, IP=2a000292), killing rtc...
[kernel] destroying a task "rtc" (tid=7)
shell> date
[vm] ERROR: unknown memory address (addr=2a491000, IP=2a0002ec), killing rtc...
[kernel] destroying a task "rtc" (tid=7)
```

## serversではなくsystem callに書き換え

```
shell> time
[kernel] PANIC: page fault in kernel: vaddr=00101000, sepc=8000c562, reason=1
[kernel] WARN:     #0: 8000bb3c riscv32_handle_trap()+0x4d4
[kernel] WARN:     #1: 8000b5ec riscv32_trap_handler()+0xac
[kernel] WARN:     #2: 80007324 handle_syscall()+0x404
[kernel] WARN:     #3: 8000b7bc riscv32_handle_trap()+0x154
[kernel] WARN:     #4: 8000b5ec riscv32_trap_handler()+0xac
[kernel] WARN:     #5: 2a001b48 (invalid address)
[kernel] WARN: kernel halted (CPU #0)
QEMU: Terminated
```

## vm.cでRTCのmmioを恒等マッピング

```
shell> time
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:65: pointer 80ca7cfc is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:65: pointer 80ca7d04 is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:66: pointer 80ca7cfc is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:66: pointer 80ca7d04 is not aligned to 8
[kernel] ORG: time: 393057487, High: 4074693848, Low: 0
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:67: pointer 80ca7cfc is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:67: pointer 80ca7d04 is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:67: pointer 80ca7cfc is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:67: pointer 80ca7cfc is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:68: pointer 80ca7cfc is not aligned to 8
[kernel] WARN: UBSan: kernel/riscv32/rtc.c:68: pointer 80ca7cfc is not aligned to 8
[kernel] SEC: time: 144118978
UTC: 0
shell> shutdown
```
