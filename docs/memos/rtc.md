# Goldfish RTCドライバの追加

## RTCドライバの作成

### 1. RTCレジスタのmmio read, shellからの呼び出しを共にipcで行う

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

#### __udivdi3関数を追加

[gcc](https://github.com/gcc-mirror/gcc/blob/master/libgcc/config/riscv/div.S)のコードを導入。


```bash
$ vi libs/common/riscv32/div.S
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

#### gettimeofdayの定義を変更

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

### 2. RTCレジスタのmmio read, shellからの呼び出しを共にsystem callで行う

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

#### vm.cでRTCのmmioを恒等マッピング

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

### 3. RTCレジスタのmmio readはシステムコール, shellからの呼び出しはipcで実装

```c
# kernel/riscv32/rtc.c

int64_t arch_rtc_epoch(void) {
    uint32_t low = mmio_read32_paddr(RTC_ADDR + RTC_TIME_LOW);
    int32_t high = (int32_t)mmio_read32_paddr(RTC_ADDR + RTC_TIME_HIGH);
    int64_t time = ((int64_t)high << 32) | low;
    INFO("time: 0x%llx, high: 0x%llx, low: 0x%llx", time, high, low);
    time = time / NSEC_PER_SEC;
    INFO("sec: 0x%llx", time);
    return time;
}
```

- 64bitの割り算 `time = time / NSEC_PER_SEC` が正しく処理されない。
- vprintf.c がint64_tを4バイト分しか表示しない。

```
shell> time
[kernel] time: 0x176df98f, high: 0x73f13458, low: 0x0
[kernel] sec: 0x16394e1c
UTC: 262144
```

### gdbで変数を見てみる

```
loc low = 1408376112, high = 393082628, time = 1688277033294110000
────────────────────────────────────────────────────────────────────────────────
>>> p/x time
$1 = 0x176df70453f21d30         // 正しい時刻を取得できている
>>> p/x high
$2 = 0x176df704
>>> p/x low
$3 = 0x53f21d30					[kernel] time: 0x176df704, high: 0x53f21d30, low: 0x0
-----
time/10000000000                // 割り算実行
------
>>> p/x time
$4 = 0xda8382400000003          // 割り算の結果が正しくない
>>> p time
$6 = 984098245850431491
>>> p $1/10000000000L           // gdbでは64bitの割り算も問題ない
$7 = 168827703

=========== kernel/syscall.c#sys_epoch
>>> p/x epoch_low
$9 = 0x3
>>> p/x epoch_high
$10 = 0xda83824
>>> p/x epoch
$11 = 0xda8382400000003
```

### 4. HinaOS側の修正を諦め、QEMUを修正してnano秒でなく秒をint32_tで返すように変更

```diff
diff --git a/hw/rtc/goldfish_rtc.c b/hw/rtc/goldfish_rtc.c
index 19a56402a0..dcb74ae659 100644
--- a/hw/rtc/goldfish_rtc.c
+++ b/hw/rtc/goldfish_rtc.c
@@ -104,7 +104,7 @@ static uint64_t goldfish_rtc_read(void *opaque, hwaddr offset,
      */
     switch (offset) {
     case RTC_TIME_LOW:
-        r = goldfish_rtc_get_count(s);
+        r = goldfish_rtc_get_count(s) / 1000000000ULL;
         s->time_high = r >> 32;
         r &= 0xffffffff;
         break;
```

#### 正常に動いた

```
shell> time
UTC: 1688349911
shell> date
2023-07-03 11:05:13 JST
```

#### 変更および新規作成ファイル

```bash
-$ git status
-On branch rtc
-Changes not staged for commit:
-	modified:   Makefile
-	modified:   kernel/arch.h
-	modified:   kernel/riscv32/build.mk
-	new file:   kernel/riscv32/rtc.c
-	new file:   kernel/riscv32/rtc.h
-	modified:   kernel/syscall.c
-	modified:   libs/user/syscall.c
-	modified:   libs/user/syscall.h
-	modified:   messages.idl
-	new file:   servers/rtc/build.mk
-	new file:   servers/rtc/main.c
-	modified:   servers/shell/command.c
```
