#include <iostream>
#include <stdio.h>
#include <string.h>
#include "butterfly.h"

using namespace std;

void ButterflySimulation::initialize(){
    // this->targetDevice->initializeSimulation(SIM_OUTPUT_GPIO_NUM, SIM_INPUT_GPIO_NUM);
    // this->targetDevice.setGpio(0, true); //first output gpio
    // this->targetDevice.getGpio(0);  // first input gpio, which is different

    for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
        this->output_gpio_tracker[i] = false;
    }

    for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
        this->input_gpio_tracker[i] = 0;
    }


    // My buffer
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        this->buffer[i] = false;
    }

    for(int i = 0; i < LED_ARRAY_SIZE; i++){
        mState.virtual_led[i] = false;
    }

    setReportWhenMarked(true);
}

void ButterflySimulation::print_gpio_header_states(){
    
    cout << "----- Input -----" << endl;
    for(int i = 0; i < SIM_INPUT_GPIO_NUM; i++){
        cout << "g" << i << ": " << this->input_gpio_tracker[i] << endl;
    }

    cout << "----- Output -----" << endl;
    for(int i = 0; i < SIM_OUTPUT_GPIO_NUM; i++){
        cout << "g" << i << ": " << this->output_gpio_tracker[i] << endl;
    }

}

void ButterflySimulation::print_buffer_states(){
    cout << "buffer: ";
    for(int i = 0; i < BUFFER_ARRAY_SIZE; i++){
        cout << this->buffer[i] << " ";
    }
    cout << endl;
}

void ButterflySimulation::print_led_states(){
    cout << "virtual led: ";
    for(int i = 0; i < LED_ARRAY_SIZE; i++){
        cout << mState.virtual_led[i] << " ";
    }
    cout << endl;
}

int ButterflySimulation::read_literal_logic(string s){
    return s[0] == 'T';
}

int ButterflySimulation::read_switch_logic(string s){
    return s[0] == 'T';
}

bool ButterflySimulation::read_gpio_logic(string s){
    
    char char_array[IS_GPIO_NEXT_CHAR_SIZE + 1];
    strcpy(char_array, s.c_str());
    int index = atoi(char_array);
    return this->input_gpio_tracker[index];
    
}

void ButterflySimulation::update_gpio_logic(string s, int o){
    
    char char_array[IS_GPIO_NEXT_CHAR_SIZE + 1];
    strcpy(char_array, s.c_str());
    int index = atoi(char_array);
    this->output_gpio_tracker[index] = (bool)o;

}

int ButterflySimulation::read_buffer_logic(string s){
    int index = stoi(s);
    return this->buffer[index];
}

void ButterflySimulation::update_buffer_logic(string s, int o){
    int index = stoi(s);
    this->buffer[index] = (bool)o;
}

void ButterflySimulation::update_led_logic(string s, int o){
    int index = stoi(s);
    mState.virtual_led[index] = (bool)o;
    // requestReportState();
}

int ButterflySimulation::read_gate_input(char c){
    int in;
    switch(c){
        case 'L':
            in = IS_LITERAL;
            break;
        case 'g':
            in = IS_GPIO;
            break;
        case 'b':
            in = IS_BUFFER;
            break;
        case 'S':
            in = IS_SWITCH;
            break;
        default:
            break;
    }
    return in;
}

int ButterflySimulation::read_gate_output(char c){
    int out;
    switch(c){
        case 'g':
            out = IS_GPIO;
            break;
        case 'b':
            out = IS_BUFFER;
            break;
        case 'd':
            out = IS_LED;
            break;
        default:
            break;
    }
    return out;
}

int ButterflySimulation::handle_input(string substring, int &start_index){
    int p;
    p = read_gate_input((char)substring[start_index++]);
    int my_input = 0;
    int temp = start_index;
    switch(p){
        case IS_LITERAL:
            my_input = read_literal_logic(substring.substr(temp, IS_LITERAL_NEXT_CHAR_SIZE));
            start_index += IS_LITERAL_NEXT_CHAR_SIZE;
            break;
        case IS_SWITCH:
            my_input = read_switch_logic(substring.substr(temp, IS_SWITCH_NEXT_CHAR_SIZE));
            start_index += IS_SWITCH_NEXT_CHAR_SIZE;
            break;
        case IS_GPIO:
            my_input = read_gpio_logic(substring.substr(temp, IS_GPIO_NEXT_CHAR_SIZE));
            start_index += IS_GPIO_NEXT_CHAR_SIZE;
            break;
        case IS_BUFFER:
            my_input = read_buffer_logic(substring.substr(temp, IS_BUFFER_NEXT_CHAR_SIZE));
            start_index += IS_BUFFER_NEXT_CHAR_SIZE;
            break;
        default:
            break;
    }
    return my_input;
}

void ButterflySimulation::handle_output(string substring, int &start_index, int my_output){
    int p;
    p = read_gate_output((char)substring[start_index++]);
    int temp = start_index;
    switch(p){
        case IS_GPIO:
            update_gpio_logic(substring.substr(temp, IS_GPIO_NEXT_CHAR_SIZE), my_output);
            start_index += IS_GPIO_NEXT_CHAR_SIZE;
            break;
        case IS_BUFFER:
            update_buffer_logic(substring.substr(temp, IS_BUFFER_NEXT_CHAR_SIZE), my_output);
            start_index += IS_BUFFER_NEXT_CHAR_SIZE;
            break;
        case IS_LED:
            update_led_logic(substring.substr(temp, IS_LED_NEXT_CHAR_SIZE), my_output);
            start_index += IS_LED_NEXT_CHAR_SIZE;
            break;
        default:
            break;
    }
}

int ButterflySimulation::read_logic_gate(string substring){
    int start_index = 0;
    int my_input_1 = -1;
    int my_input_2 = -1;
    int my_output = -1;
    if(substring[start_index] == 'n'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = !my_input_1;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'a'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);
        my_input_2 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1 & my_input_2;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'o'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);
        my_input_2 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1 | my_input_2;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'x'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);
        my_input_2 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1 ^ my_input_2;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }
    else if(substring[start_index] == 'y'){
        // Handle input portion
        start_index++;
        my_input_1 = handle_input(substring, start_index);

        // Compute boolean logic
        my_output = my_input_1;

        // Handle output portion
        handle_output(substring, start_index, my_output);
        while(substring[start_index] == ','){
            start_index++;
            handle_output(substring, start_index, my_output);
        }
    }

    return start_index;
}

void ButterflySimulation::update(double delta){

    ButterflyRequest request;
    bool requestWasRead = readRequest(request);
    if(requestWasRead) {
        this->my_string = string(request.my_string);
    }
    string my_string = this->my_string;

    int index = 0;
    int my_string_length = my_string.length();
    int while_loop_counter = 1;

    if(my_string_length < 2){
        return;
    }

    cout << endl << "===== GPIO STATES =====" << endl;
        print_gpio_header_states();
        cout << endl << "===== BUFFER STATES =====" << endl;
        print_buffer_states();
        cout << endl << "===== LED STATES =====" << endl;
        print_led_states();
        cout << mState.serialize() << endl;

        cout << "===== STRING PROTOCOL =====" << endl;
        while(index < my_string_length){
            cout << "My string: " << my_string << endl;
            cout << "Iter " << while_loop_counter << " : " << my_string.substr(index, my_string_length - index);
            while_loop_counter++;
            index += read_logic_gate(my_string.substr(index, my_string_length - index));
            if(my_string[index] == ';' || my_string[index] == '\n'){
                index = index + 1;
            }
        }

        index = 0;
        while_loop_counter = 1;

    cout << endl << "===== GPIO STATES =====" << endl;
    print_gpio_header_states();
    cout << endl << "===== BUFFER STATES =====" << endl;
    print_buffer_states();
    cout << endl << "===== LED STATES =====" << endl;
    print_led_states();
    cout << mState.serialize() << endl;

    requestReportState();
    // reportUpdate();
}
