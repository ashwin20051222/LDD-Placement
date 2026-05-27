# Hello Kernel Module

A simple Linux kernel module that prints messages when loaded and unloaded.

## Files

- `hello_kernel.c` - Kernel module source code
- `Makefile` - Build configuration

## Prerequisites

Install required packages (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```

## Compilation

```bash
make
```

After compilation, `hello_kernel.ko` will be generated.

## Loading the Module

```bash
sudo insmod hello_kernel.ko
```

## Verification

### Check if module is loaded:
```bash
lsmod | grep hello_kernel
```

### Check kernel messages:
```bash
dmesg | tail
```

Expected output:
```
Hello: Kernel Module Loaded
```

## Removing the Module

```bash
sudo rmmod hello_kernel
```

## Verify Removal

```bash
dmesg | tail
```

Expected output:
```
Hello: Kernel Module Removed
```

## Clean Build

```bash
make clean
```

## Notes

- Module messages are printed to kernel log buffer (viewable via `dmesg`)
- `printk()` is used instead of `printf()` for kernel-space logging
- Log levels: `KERN_INFO`, `KERN_ERR`, `KERN_ALERT`, etc.
