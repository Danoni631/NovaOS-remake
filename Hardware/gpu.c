/*
    CODED BY Danoni631

    This code get GPU capactity and model
*/

#include "../Include/stdint.h"
#include "../Font/text.h"

#include "pci.h"
#include "gpu.h"

const char* GetGPUVendor(uint16_t vendor)
{
    switch (vendor)
    {
        case 0x10DE: return "NVIDIA";
        case 0x1002: return "AMD";
        case 0x8086: return "INTEL";
        default: return "Unknown";
    }
}

const char* GetGPUName(uint16_t vendor, uint16_t device_id)
{
    if (vendor == 0x10DE)
    {
        return "NVIDIA Video cardboard";
    }

    if (vendor == 0x1002)
    {
        return "AMD Radeon";
    }

    if (vendor == 0x8086)
    {
        return "Intel UHD Graphics";
    }

    return "Unknown GPU";
}

void ShowGPUName()
{
    for (uint8_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++) 
        {
            uint32_t data = PCIConfigReadWord(bus, slot, 0, 0x00);
            uint16_t vendor_id = data & 0xFFFF;
            uint16_t device_id = (data >> 16) & 0xFFFF;
            
            if (vendor_id != 0xFFFF)
            {
                const CHAR* vendor = GetGPUVendor(vendor_id);
                const CHAR* name = GetGPUName(vendor_id, device_id);

                Print(name, 0xFFFFFFFF);
                return;
            }
        }
    }
}

void ShowGPUInfo()
{
    for (uint8_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++)
        {
            uint32_t data = PCIConfigReadWord(bus, slot, 0, 0x00);
            uint16_t vendor_id = data & 0xFFFF;
            uint16_t device_id = (data >> 16) & 0xFFFF;

            if (vendor_id != 0xFFFF)
            {
                Print("Vendor ID: ", 0xFFFFFFFF);
                PrintHex(vendor_id, 0xFFFFFFFF);
                Print(" (", 0xFFFFFFFF);
                Print(GetGPUVendor(vendor_id), 0xFFFFFFFF);
                Print(")\n", 0xFFFFFFFF);

                Print("Device ID: ", 0xFFFFFFFF);
                PrintHex(device_id, 0xFFFFFFFF);
                Print(" (", 0xFFFFFFFF);
                Print(GetGPUName(vendor_id, device_id), 0xFFFFFFFF);
                Print(")\n", 0xFFFFFFFF);
            }
        }
    }
}
