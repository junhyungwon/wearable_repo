/* --COPYRIGHT--,EPL
 *  Copyright (c) 2010 Texas Instruments and others.
 *  All rights reserved. This program and the accompanying materials
 *  are made available under the terms of the Eclipse Public License v1.0
 *  which accompanies this distribution, and is available at
 *  http://www.eclipse.org/legal/epl-v10.html
 * 
 *  Contributors:
 *      Texas Instruments - initial implementation
 * 
 * --/COPYRIGHT--*/

var peripheralsTable = {};

peripheralsTable["MSP430G2x01"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: false, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x01",
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x11"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: false, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x11",
        required: ["comparator"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x21"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: false, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x21",
        required: ["usi"],
        params: {name: "gpio"}
    }, 
];

peripheralsTable["MSP430G2x31"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: false, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x31",
        required: ["usi", "adc10"],
        params: {name: "gpio"}
    }    
];

peripheralsTable["MSP430G2x02"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x02",
        required: ["usi"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x12"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x12",
        required: ["usi", "comparator"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x32"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x32",
        required: ["usi", "adc10"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x52"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x52",
        required: ["usi", "comparator", "adc10"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x03"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "Timer1_A3", baseAddr: 0x180, intNum: 8, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x03",
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x13"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "Timer1_A3", baseAddr: 0x180, intNum: 8, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x13",
        required: ["comparator"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x33"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "Timer1_A3", baseAddr: 0x180, intNum: 8, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x33",
        required: ["adc10"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430G2x53"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, hasRosc: false, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "Timer1_A3", baseAddr: 0x180, intNum: 8, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430G2x53",
        required: ["adc10", "comparator"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F20x1"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F20x1",
        required: ["comparator"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F20x2"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F20x2",
        required: ["usi", "adc10"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F20x3"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: false, hasXT2: false, 
                 hasRosc: false, hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USI",
        required: ["clock"],
        params: {name: "usi"}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A2",
        required: ["clock"],
        params: {name: "Timer_A2", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.SD16_A",
        required: ["clock"],        
        params: {name: "sd16"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F20x3",
        required: ["usi", "sd16"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F21x2"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: false, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "Timer0_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A2",
        required: ["clock"],
        params: {name: "Timer1_A2", baseAddr: 0x180, intNum: 13, 
                 numberOfTimers: 2}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F21x2",
        required: ["adc10", "comparator", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F21x1"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: false, hasRosc: false, 
                 hasVLO: false, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F21x1",
        required: ["comparator"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F22x2"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: false, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B3",
        required: ["clock"],
        params: {name: "Timer_B3", baseAddr: 0x180, intNum: 13, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F22x2",
        required: ["adc10", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F22x4"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: false, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.oa.OA_2",
        params: {name: "oa"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B3",
        required: ["clock"],
        params: {name: "Timer_B3", baseAddr: 0x180, intNum: 13, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC10",
        required: ["clock"],
        params: {name: "adc10"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F22x4",
        required: ["adc10", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F241x"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.svs.SVS",
        params: {name: "svs"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: true, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A1_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B1_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 25,
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B7",
        required: ["clock"],
        params: {name: "Timer_B7", baseAddr: 0x180, intNum: 29,
                 numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC12",
        required: ["clock"],
        params: {name: "adc12"}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F241x",
        required: ["adc12", "comparator", "svs", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F261x"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.svs.SVS",
        params: {name: "svs"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: true, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A1_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B1_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 25,
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B7",
        required: ["clock"],
        params: {name: "Timer_B7", baseAddr: 0x180, intNum: 29,
                 numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.dac.DAC12",
        params: {name: "dac12"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC12",
        required: ["clock"],
        params: {name: "adc12"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F261x",
        required: ["adc12", "comparator", "svs", "dac12", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F23x"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.svs.SVS",
        params: {name: "svs"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: true, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B3",
        required: ["clock"],
        params: {name: "Timer_B3", baseAddr: 0x180, intNum: 13, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC12",
        required: ["clock"],
        params: {name: "adc12"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F23x",
        required: ["adc12", "comparator", "svs", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F24x"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.svs.SVS",
        params: {name: "svs"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: true, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A1_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B1_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 25,
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B7",
        required: ["clock"],
        params: {name: "Timer_B7", baseAddr: 0x180, intNum: 29,
                 numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.adc.ADC12",
        required: ["clock"],
        params: {name: "adc12"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F24x",
        required: ["adc12", "comparator", "svs", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F24x1"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.svs.SVS",
        params: {name: "svs"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: true, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A1_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B1_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B1_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B1_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B7",
        required: ["clock"],
        params: {name: "Timer_B7", baseAddr: 0x180, intNum: 13, 
                 numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F24x1",
        required: ["comparator", "svs", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F23x0"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.SR",
        params: {name: "statusRegister"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE2",
        params: {name: "interruptEnableRegister2"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.BCSplus",
        params: {name: "clock", baseAddr: 0x53, 
                 hasHFXT1: true, hasXT2: false, hasRosc: true, 
                 hasVLO: true, hasAllCal: true, 
                 maxCpuFrequency: 16000000}
    },    
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x120}
    },
    {
        factory: "ti.catalog.msp430.peripherals.memory.Flash_2xx",
        required: ["clock"],
        params: {name: "flash"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_UART_2xx",
        required: ["clock"],
        params: {name: "usci_A0_UART"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_A0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_A0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_SPI_2xx",
        required: ["clock"],
        params: {name: "usci_B0_SPI"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.communication.USCI_B0_I2C_2xx",
        required: ["clock"],
        params: {name: "usci_B0_I2C"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_A3",
        required: ["clock"],
        params: {name: "Timer_A3", baseAddr: 0x160, intNum: 9, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer_B3",
        required: ["clock"],
        params: {name: "Timer_B3", baseAddr: 0x180, intNum: 13, 
                 numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.comparator.Comparator_Aplus",
        params: {name: "comparator"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.gpio.GPIO_MSP430F23x0",
        required: ["comparator", "clock"],
        params: {name: "gpio"}
    }
];

peripheralsTable["MSP430F5131"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.UCS",
        params: {name: "clock", baseAddr: 0x160, maxCpuFrequency: 25000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A3",
        required: ["clock"],
        params: {name: "TA0", baseAddr: 0x3C0, intNum: 52, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x15c}
    }
];

peripheralsTable["MSP430F5438"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        // TODO: this device does not have a BCSplus it has a UCS!!!!
        factory: "ti.catalog.msp430.peripherals.clock.UCS",
        params: {name: "clock", baseAddr: 0x160, maxCpuFrequency: 18000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A5",
        required: ["clock"],
        params: {name: "TA0", baseAddr: 0x340, intNum: 54, numberOfTimers: 5}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "TA1", baseAddr: 0x380, intNum: 49, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_B7",
        required: ["clock"],
        params: {name: "TB0", baseAddr: 0x3C0, intNum: 60, numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x15c}
    }
];

peripheralsTable["MSP430F5529"] = [
    {
        /* this device does not have IE1! We have this here to satify other 
         * peripherals that require this register.
         */
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.UCS",
        /* TODO: maxFrequency is a function of core voltage and may be 
         * less than 25000000!!!!
         */
        params: {name: "clock", baseAddr: 0x160, maxCpuFrequency: 25000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A5",
        required: ["clock"],
        params: {name: "TA0", baseAddr: 0x340, intNum: 53, numberOfTimers: 5}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "TA1", baseAddr: 0x380, intNum: 49, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer2_A3",
        required: ["clock"],
        params: {name: "TA2", baseAddr: 0x400, intNum: 44, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_B7",
        required: ["clock"],
        params: {name: "TB0", baseAddr: 0x3C0, intNum: 59, numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x15c}
    }
];

peripheralsTable["MSP430F5630"] = [
    {
        /* this device does not have IE1! We have this here to satify other 
         * peripherals that require this register.
         */
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.UCS",
        /* TODO: maxFrequency is a function of core voltage and may be 
         * less than 20000000!!!!
         */
        params: {name: "clock", baseAddr: 0x160, maxCpuFrequency: 20000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A5",
        required: ["clock"],
        params: {name: "TA0", baseAddr: 0x340, intNum: 53, numberOfTimers: 5}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "TA1", baseAddr: 0x380, intNum: 49, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer2_A3",
        required: ["clock"],
        params: {name: "TA2", baseAddr: 0x400, intNum: 40, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_B7",
        required: ["clock"],
        params: {name: "TB0", baseAddr: 0x3C0, intNum: 59, numberOfTimers: 7}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x15c}
    }
];

peripheralsTable["CC430F6137"] = [
    {
        factory: "ti.catalog.msp430.peripherals.special_function.IE1",
        params: {name: "interruptEnableRegister1"}
    },
    {
        factory: "ti.catalog.msp430.peripherals.clock.UCS",

        /* maxFrequency is a function of core voltage and may be less than
         * 20000000!!!!
         */
        params: {name: "clock", baseAddr: 0x160, maxCpuFrequency: 20000000}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer0_A5",
        required: ["clock"],
        params: {name: "TA0", baseAddr: 0x340, intNum: 55, numberOfTimers: 5}
    },
    {
        factory: "ti.catalog.msp430.peripherals.timer.Timer1_A3",
        required: ["clock"],
        params: {name: "TA1", baseAddr: 0x380, intNum: 51, numberOfTimers: 3}
    },
    {
        factory: "ti.catalog.msp430.peripherals.watchdog.WDTplus",
        required: ["clock"],
        params: {name: "watchdog", baseAddr: 0x15c}
    }
];

/*
 *  ======== instance$meta$init ========
 */
function instance$meta$init(revision)
{
    this.$private.realDevice = revision;
    var IC = xdc.useModule(
        "ti.catalog.msp430.peripherals.interrupt.Interrupt_Controller"
    );
    var Boot = xdc.useModule('ti.catalog.msp430.init.Boot');
    Boot.disableWatchdog = true;

    var tableEntry;    
    /*
     * Define which actual devices correspond to the above defined
     * descriptor tables. Also, additional configurations important
     * to SYS/BIOS are made here.
     */
    if (this.$private.realDevice.match(/^MSP430G2\d01/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x01";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d11/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x11";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d21/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x21";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d31/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x31";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d02/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x02";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d12/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x12";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d32/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x32";
    }    
    else if (this.$private.realDevice.match(/^MSP430G2\d52/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x52";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d03/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x03";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d13/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x13";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d33/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x33";
    }
    else if (this.$private.realDevice.match(/^MSP430G2\d53/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430G2x53";
    }
    else if (this.$private.realDevice.match(/^MSP430F20\d1/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F20x1";
    }
    else if (this.$private.realDevice.match(/^MSP430F20\d2/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F20x2";
    }
    else if (this.$private.realDevice.match(/^MSP430F20\d3/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F20x3";
    }
    else if (this.$private.realDevice.match(/^MSP430F21\d2/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        tableEntry = "MSP430F21x2";
    }
    else if (this.$private.realDevice.match(/^MSP430F21\d1/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        tableEntry = "MSP430F21x1";
    }   
    else if (this.$private.realDevice.match(/^MSP430F22\d2/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        tableEntry = "MSP430F22x2";
    }
   else if (this.$private.realDevice.match(/^MSP430F22\d4/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        tableEntry = "MSP430F22x4";
    }
    /* trap MSP430F2616, F2617, F2618, F2619 */
    else if (this.$private.realDevice.match(/^MSP430F261[6-9]/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFC0, numInterrupts: 32}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F261x";
    }
    /* trap MSP430F247, F248, F249 */
    else if (this.$private.realDevice.match(/^MSP430F24[7-9]/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFC0, numInterrupts: 32}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F24x";
    }
    /* trap MSP430F2471, F2481, F2491 */
    else if (this.$private.realDevice.match(/^MSP430F24[7-9]1/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFC0, numInterrupts: 32}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F24x1";
    }  
    else if (this.$private.realDevice.match(/^MSP430F24\d1/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F24x1";
    }
    else if (this.$private.realDevice.match(/^MSP430F23\d0/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F23x0";
    } 
    else if (this.$private.realDevice.match(/^MSP430F23\d$/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F23x";
    }  
    /* trap MSP430F2416, F2417, F2418, F2419 */
    else if (this.$private.realDevice.match(/^MSP430F241[6-9]/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFC0, numInterrupts: 32}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F241x";
    }
    else if (this.$private.realDevice.match(/^MSP430F24(\d$|10)/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFFE0, numInterrupts: 16}
        );
        Boot.configureDCO = false;
        Boot.disableWatchdog = false;
        tableEntry = "MSP430F24x";
    }     
    else if (this.$private.realDevice.match(/^MSP430BT51\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5438";
    }
    else if (this.$private.realDevice.match(/^MSP430F51\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5131";
    }
    else if (this.$private.realDevice.match(/^MSP430F52\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5529";
    }
    else if (this.$private.realDevice.match(/^MSP430F53\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5529";
    }
    else if (this.$private.realDevice.match(/^MSP430F54\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5438";
    }
    else if (this.$private.realDevice.match(/^MSP430F55\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5529";
    }
    else if (this.$private.realDevice.match(/^MSP430F56\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5630";
    }
    else if (this.$private.realDevice.match(/^MSP430F66\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "MSP430F5630";
    }
    else if (this.$private.realDevice.match(/^CC430F5\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "CC430F6137";
    }
    else if (this.$private.realDevice.match(/^CC430F6\d+/)) {
        this.peripherals["interruptController"] = IC.create(
            {name: "interruptController", baseAddr: 0xFF80, numInterrupts: 64}
        );
        Boot.configureDCO = true;
        tableEntry = "CC430F6137";
    }
    else {
        this.$module.$logFatal("Device " + revision + " is not supported.",
            this.$module);
    }

    if (peripheralsTable[tableEntry] != null) {
        for (var i = 0; i < peripheralsTable[tableEntry].length; i++) {
            var entry = peripheralsTable[tableEntry][i];
            var modName = entry.factory;
            var mod = xdc.useModule(modName);
            var params = entry.params;

            /* construct the create arguments */
            var args = [];
            if ("required" in entry) {
                for (var j = 0; j < entry.required.length; j++) {
                    var peripheralName = entry.required[j];
                    var peripheral = this.peripherals[peripheralName];
                    if (!peripheral) {
                        this.$module.$logWarning(
                            this.$private.realDevice
                                + " " + peripheralName
                                + " must be initialized before " + params.name,
                            this, "peripherals");
                    }
                    args.push(peripheral);
                }
            }
            args.push(params);

            /* construct the peripheral */
            this.peripherals[params.name] = mod.create.$apply(mod, args);
        }
    }
    
    Boot.watchdogAddress = this.peripherals["watchdog"].$orig.baseAddr;
}

/*
 *  ======== MSP430.getMemoryMap ========
 *  Returns the memory map as seen by a program running on this device.
 */
function getMemoryMap(regs)
{
    return (null);
}
