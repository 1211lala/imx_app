#pragma once

int gpio_export(unsigned short pin, const char *mode);
int gpio_unexport(unsigned short pin);
int gpio_set_level(unsigned short pin, const char *status);

