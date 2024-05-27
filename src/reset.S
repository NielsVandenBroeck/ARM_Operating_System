.section .text.reset
.global reset_system

reset_system:
    // Write the watchdog timer value
    ldr x0, =0x3F100024      // Load the address of the watchdog timer
    ldr x1, =0x5A000001      // Load the watchdog reset value with password
    str w1, [x0]             // Store the value to the watchdog timer register

    // Write the reset control value
    ldr x0, =0x3F10001C      // Load the address of the reset control
    ldr x1, =0x5A000020      // Load the full reset command with password
    str w1, [x0]             // Store the value to the reset control register

    // Infinite loop to wait for the reset to take effect
1:  b 1b                    // Branch to label 1 (infinite loop)

.align 4
    .word 0x3F100024        // Watchdog timer address
    .word 0x5A000001        // Watchdog reset value with password
    .word 0x3F10001C        // Reset control address
    .word 0x5A000020        // Full reset command with password
