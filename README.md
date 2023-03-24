# Raspberry Pico Serial Demo for Zephyr RTOS

Example project for a simple USB Serial using the Raspberry Pi Pico, using Zephyr RTOS.
There are a number of USB Serial examples for different Zephyr supported devices but few that are up-to-date with the latest version of Zephyr (3.3.99 at the time of writing).

This is a simple demo for how get a Serial Console from the RPi Pico over USB CDC ACM.

## Explanation

The following files are required to enable a USB Serial Console on the RPi Pico.

### prj.conf

The `*.conf` file is used by Zephyr as a `Kconfig` file, which specifies which kernel configuration options should be used for this application.
These settings are used along with the board/device specific settings to generate a specific Zephyr kernel configuration.

In our application we have the following configs:

```bash
CONFIG_USB_DEVICE_STACK=y # Enables the USB Device Stack (required for USB Serial)
CONFIG_USB_DEVICE_PRODUCT="RPi Pico USB Serial Console" # Gives the USB Device a human-readable name (optional)
CONFIG_USB_DEVICE_VID=0001 # Gives the USB Device a vendor ID (optional)

CONFIG_SERIAL=y # Enable options for serial drivers
CONFIG_CONSOLE=y # Enables a console for use with USB CDC applications
CONFIG_UART_CONSOLE=y # Sets the default for printk and printf to output to the UART serial console
CONFIG_UART_LINE_CTRL=y # This enables the API for apps to control the serial line, such as baud rate, CTS and RTS.
```

If you're unsure of what a specific option does, the Zephyr Docs helpfully provide a [nice tool](https://docs.zephyrproject.org/latest/kconfig.html) for searching for each mainline config does.

### app.overlay

Device Tree Overlays can be used by a Zephyr application to specify new hardware or re-assign existing hardware.
In our application, we'll use the `app.overlay` to inform Zephyr that we want to have a USB CDC ACM device.

Breaking this down even further and quoting the Linux documentation:

> The `chosen` node does not represent a real device, but serves as a place for passing data between firmware and the operating system, like boot arguments.
Data in the `chosen` node does not represent the hardware.
Typically the `chosen` node is left empty in .dts source files and populated at boot time.

```dts
/ {
    chosen {
        zephyr,console = &cdc_acm_uart0;
    };
};
```

In this instance, we're specifying that the zephyr

Again quoting the Linux Kernel docs:

> A Devicetree's overlay purpose is to modify the kernel’s live tree, and have the modification affecting the state of the kernel in a way that is reflecting the changes.
Since the kernel mainly deals with devices, any new device node that result in an active device should have it created while if the device node is either disabled or removed all together, the affected device should be deregistered.

```dts
&zephyr_udc0 {
    cdc_acm_uart0: cdc_acm_uart0 {
        compatible = "zephyr,cdc-acm-uart";
        label = "CDC_ACM_0";
    };
};
```

In our case, we know that the RPi Pico board has a `zephyr_udc0` (USB device controller) node and we update the node to specify that we want this to be a CDC ACM type device, specifically compatible with `"zephyr,cdc-acm-uart"`.

### .vscode/c_cpp_properties.json

This file, while not specific to Zephyr, is helpful when developing Zephyr Applications using VSCode as it allows the C/CPlusPlus [IntelliSense](https://code.visualstudio.com/docs/editor/intellisense) tools to resolve the missing header/includes for your Zephyr libraries.
Without this `.vscode/c_cpp_properties.json` file in your project (and then ), you won't get any of the helpful tooltips that instruct you about specific functions or data structures.

## Glossary

- CDC -  Communications Device Class (USB)
- ACM -  Abstract Control Model (USB)
- UDC -  USB Device Controller
- UART - Universal Asynchronous Receiver-Transmitter
