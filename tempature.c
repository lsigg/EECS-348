#include <stdio.h>
#include <ctype.h>

float c_to_f(float celsius) {
    return(9.0 / 5.0) * celsius + 32;
}

float f_to_c(float fahrenheit){
    return(5.0 / 9.0) * (fahrenheit - 32);
}

float c_to_k(float celsius) {
    return celsius + 273.15;
}

float k_to_c(float kelvin) {
    return kelvin - 273.15;
}

float f_to_k(float fahrenheit) {
    return c_to_k(f_to_c(fahrenheit));
}

float k_to_f(float kelvin) {
    return c_to_f(k_to_c(kelvin));
}

void category(float celsius){
    if (celsius < 0.0){
        printf("Category: Freezing\n");
        printf("Wear heavy clothing\n");
    } else if (celsius >= 0.0 && celsius <= 10.0) {
        printf("Category: Cold\n");
        printf("Wear a hoodie\n");
    } else if (celsius >= 10.1 && celsius <= 25.0) {
        printf("Category: Comfortable\n");
        printf("Warm outside\n");
    } else if (celsius >= 25.1 && celsius <= 35) {
        printf("Category: Hot\n");
        printf("Drink water and stay out of sun\n");
    } else if (celsius > 35) {
        printf("Category: Extreme Heat\n");
        printf("Dont go outside, stay indoors\n");
    }
}

int main(void) {
    float value, converted, celsius_num;
    char from_scale, to_scale;

    printf("Enter a Tempature: ");
    if (scanf("%f", &value) != 1) {
        printf("Error: Invalid numeric input\n");
        return 1;
    }

    printf("Enter a scale (C/F/K): \n");
    if (scanf(" %c", &from_scale) != 1) {
        printf("Error: invalid answer\n");
        return 1;
    }

    printf("Enter target scale (C/F/K): \n");
    if (scanf(" %c", &to_scale) != 1) {
        printf("Error: invalid target\n");
        return 1;
    }

    from_scale = (char)toupper((unsigned char)from_scale);
    to_scale = (char)toupper((unsigned char)to_scale);

    if ((from_scale != 'C' && from_scale != 'F' && from_scale != 'K') || (to_scale != 'C' && to_scale != 'F' && to_scale != 'K')) {
        printf("Error: Scale must be C, F, K.\n");
        return 1;
    }

    if (from_scale == to_scale) {
        printf("Error: Invalid conversion choice.\n");
        return 1;
    }

    if (from_scale == 'K' && value < 0.0f) {
        printf("Error: Kelvin cannot negative. \n");
        return 1;
    }
    
        
    if (from_scale == 'C') {
        celsius_num = value;
    } else if (from_scale == 'F') {
        celsius_num = f_to_c(value);
    } else {
        celsius_num = k_to_c(value);
    }

    if (to_scale == 'C') {
        converted = celsius_num;
    } else if (to_scale == 'F') {
        converted = c_to_f(celsius_num);
    } else if (to_scale == 'K') {
        converted = c_to_k(celsius_num);
    }

    if (to_scale == 'K' && converted < 0.0f) {
        printf("Error: Conversion resulted in negative Kelvin\n");
        return 1;
    }

    printf("Converted temparture: %.2f %c\n", converted, to_scale);
    category(celsius_num);

    return 0;
}