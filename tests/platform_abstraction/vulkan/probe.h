/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef PROBE_H
#define PROBE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <volk.h>

bool has_name(const VkExtensionProperties *list, uint32_t count,
    const char *name);
void print_gpus(VkInstance instance);
void print_lacks(VkPhysicalDevice gpu, const VkExtensionProperties *list,
    uint32_t count);

int canary_2024(int argc, char **argv, VkInstance instance);

#endif
