
#ifndef BUTTERFLYSIMULATION_H
#define BUTTERFLYSIMULATION_H

#include "simulation.h"
#include <string>
#include <sstream>
#include <stdio.h>

using namespace std;

// My constant defines
#define IS_LITERAL  0
#define IS_GPIO     1
#define IS_BUFFER   2
#define IS_SWITCH   3
#define IS_LED      4

#define IS_LITERAL_NEXT_CHAR_SIZE 1
#define IS_GPIO_NEXT_CHAR_SIZE    2
#define IS_BUFFER_NEXT_CHAR_SIZE  1
#define IS_SWITCH_NEXT_CHAR_SIZE  1
#define IS_LED_NEXT_CHAR_SIZE     1

#define BUFFER_ARRAY_SIZE   5
#define LED_ARRAY_SIZE      5
#define NUM_SATURATION_ITR  5
#define MAX_CHAR_ARRAY_SIZE 1024

// GPIO structs
struct gpio_input {
    bool g31;
    bool g32;
};

struct gpio_output {
    bool g06;
    bool g07;
    bool g08;
    bool g09;
    bool g10;
    bool g13;
    bool g14;
    bool g15;
    bool g16;
    bool g17;
    bool g18;
    bool g19;
    bool g20;
    bool g21;
    bool g22;
    bool g23;
    bool g24;
    bool g25;
};

struct gpio_header {
    gpio_input input;
    gpio_output output;
};

// struct that receives the string
struct butterfly_request {
    char my_string[MAX_CHAR_ARRAY_SIZE];
};

// struct that tracks the virtual LED states
struct butterfly_data {
    bool virtual_led[LED_ARRAY_SIZE];

    string serialize() const {
        stringstream stream;
        for(int i = 0; i < LED_ARRAY_SIZE; i++){
            stream << "led" << i << "=" << virtual_led[i];
            if(i < LED_ARRAY_SIZE - 1){
                stream << "&";
            }
        }

        return stream.str();
    }
};

class Breadboard : public Simulation<butterfly_data, butterfly_request> {
    private:
        gpio_header my_header;
        bool buffer[BUFFER_ARRAY_SIZE];
        string my_string;
    public:

        Breadboard() = default;
        virtual void update(double delta) override;
        virtual void initialize() override;

        void print_gpio_header_states();
        void print_buffer_states();
        void print_led_states();
        int read_literal_logic(string s);
        int read_switch_logic(string s);
        bool read_gpio_logic(string s);
        void update_gpio_logic(string s, int o);
        int read_buffer_logic(string s);
        void update_buffer_logic(string s, int o);
        void update_led_logic(string s, int o);
        int read_gate_input(char c);
        int read_gate_output(char c);
        int handle_input(string substring, int &start_index);
        void handle_output(string substring, int &start_index, int my_output);
        int read_logic_gate(string substring);
        bool check_if_same(gpio_header h2, bool buffer_copy[]);
};

#endif