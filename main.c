/*
 * File:   main.c
 * Author: vboxuser
 *
 * Created on March 30, 2023, 9:14 PM
 */

// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = OFF     // CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h> // Inclui a biblioteca XC para programação em C para microcontroladores
#include "nxlcd.h" // Inclui a biblioteca de controle de LCD
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define _XTAL_FREQ 20000000 // Define a frequência do cristal para 20MHz

#define COL1 PORTBbits.RB0 // Define o bit 0 do PORTB como COL1
#define COL2 PORTBbits.RB1 // Define o bit 1 do PORTB como COL2
#define COL3 PORTBbits.RB2 // Define o bit 2 do PORTB como COL3
#define COL4 PORTBbits.RB3 // Define o bit 3 do PORTB como COL4
#define ROW1 PORTBbits.RB4 // Define o bit 4 do PORTB como ROW1
#define ROW2 PORTBbits.RB5 // Define o bit 5 do PORTB como ROW2
#define ROW3 PORTBbits.RB6 // Define o bit 6 do PORTB como ROW3
#define ROW4 PORTBbits.RB7 // Define o bit 7 do PORTB como ROW4

#define DEBOUNCE_TIME 300


// Definição da estrutura para armazenar a senha e o nível de acesso
typedef struct {
    char password[5];
    unsigned char accessLevel;
} User;

char read_keypad(void); // Protótipo da função read_keypad que retorna um char
void EEPROM_write( unsigned char data, unsigned char address  );
unsigned char EEPROM_read( unsigned char address );
void EEPROM_read_user(User* user, unsigned char start_address);
void EEPROM_write_user(const User* user, unsigned char start_address);
unsigned char grant_access(const char* input_password);
void update_password(const char* current_password, const char* new_password);
void assign_default_status_relay(void);
int has_status_relay(void);
void assign_default_passwords(void);
int has_passwords(void);
void clear_EEPROM(void);
void config(void);
void show_temp(void);
void start(void);
void change_password(void);
int convert_temp(void);
void clean_lcd(void);
void delay_show_lcd(void);
void get_password(const char* password);
void update_status_relay(void);
void show_status_relay(void);
void toggle_relay(int relay_number);
int option_relay(int relay_number);

#include <xc.h> // Inclui a biblioteca XC para programação em C para microcontroladores
#include "nxlcd.h" // Inclui a biblioteca de controle de LCD
#include <stdio.h>
#include <string.h>
#define _XTAL_FREQ 20000000 // Define a frequência do cristal para 20MHz

#define POT 1

#define COL1 PORTBbits.RB0 // Define o bit 0 do PORTB como COL1
#define COL2 PORTBbits.RB1 // Define o bit 1 do PORTB como COL2
#define COL3 PORTBbits.RB2 // Define o bit 2 do PORTB como COL3
#define COL4 PORTBbits.RB3 // Define o bit 3 do PORTB como COL4
#define ROW1 PORTBbits.RB4 // Define o bit 4 do PORTB como ROW1
#define ROW2 PORTBbits.RB5 // Define o bit 5 do PORTB como ROW2
#define ROW3 PORTBbits.RB6 // Define o bit 6 do PORTB como ROW3
#define ROW4 PORTBbits.RB7 // Define o bit 7 do PORTB como ROW4

int access_level = 0;
int flag_status_relay = 0;
int row = -1;

void __interrupt(high_priority) high_isr(void) {
    if (PIR1bits.ADIF) {
        flag_status_relay = convert_temp() > 70;
        PORTCbits.RC2 = convert_temp() > 50;
        PIR1bits.ADIF = 0;
    }
    if (INTCONbits.RBIF == 1){// testa se o flag da RB está acionado
     if (ROW1 == 0) row = 1;
     if (ROW2 == 0) row = 2;
     if (ROW3 == 0) row = 3;
     if (ROW4 == 0) row = 4;
     INTCONbits.RBIF = 0; // Limpa o flag bit da interrupção externa RB
   }
}

void main(void) {
    config();
   
    assign_default_passwords();
    assign_default_status_relay();
    
    char key;
        
    while (1) {
        __delay_ms(100);
        update_status_relay();
        key = read_keypad();
        clean_lcd();
        show_temp();
        if (key != '\0') {
            if (key == '#') start();
            if (key == '*') change_password();
        }
    }
}

void start(){
    while(1){
        ADCON0bits.GO_DONE = 1;
        clean_lcd();
        WriteCmdXLCD(0x81);
        putsXLCD ("DIGITE A SENHA: ");  // Escreve a string "TECLA" no LCD
        char current_password[5];
        __delay_ms(100);
        get_password(current_password);
        __delay_ms(500);
        access_level = (grant_access(current_password) - '0');
        if (access_level > 0) { // Se a senha digitada é igual à senha correta
            clean_lcd();
            putsXLCD ("ACESSO"); // Escreve a string "ACESSO" no LCD
            WriteCmdXLCD(0xC0); // Seleciona a posição Coluna 1 e Linha 2
            putsXLCD ("CONCEDIDO"); // Escreve a string "CONCEDIDO" no LCD
            delay_show_lcd();
            show_status_relay();
            break;
        } else { // Se a senha digitada é diferente da senha correta
            clean_lcd();
            putsXLCD ("SENHA"); // Escreve a string "ACESSO" no LCD
            WriteCmdXLCD(0xC0); // Seleciona a posição Coluna 1 e Linha 2
            putsXLCD ("NAO ENCONTRADA"); // Escreve a string "NEGADO" no LCD
            delay_show_lcd();
        }
    }
}

void show_status_relay(){
    char key = '0';
    do{
        ADCON0bits.GO_DONE = 1;
        if (isdigit(key) || flag_status_relay){
            if (option_relay((key - '0')) == 0 && key != '0' && !flag_status_relay){
                clean_lcd();
                putsXLCD("SEM ACESSO");
                delay_show_lcd();
            }
            update_status_relay();
            clean_lcd();
            char* rl1 = (EEPROM_read(200) == '1') ? "ON" : "OFF";
            putsXLCD("RL1 - ");
            putsXLCD(rl1);
            WriteCmdXLCD(0xC0);
            char* rl2 = (EEPROM_read(201) == '1') ? "ON" : "OFF";
            putsXLCD("RL2 - ");
            putsXLCD(rl2);
            WriteCmdXLCD(0xCC);
            putsXLCD("(#)\x7E");
            flag_status_relay = 0;
            __delay_ms(500);
        }
        key = read_keypad();
    } while(key != '#');
}

int option_relay(int relay_number) {
    // Verificar se o nível de acesso permite ativar o relé correspondente
    switch (relay_number) {
        case 1:
            if (access_level > 0) {
                // Ativar o primeiro relé
                toggle_relay(relay_number - 1);
                return 1;
            }
            break;
        case 2:
            if (access_level > 1) {
                // Ativar o segundo relé
                toggle_relay(relay_number - 1);
                return 1;
            }
            break;
    }
    // Nível de acesso insuficiente ou relé inválido
    return 0;
}

void toggle_relay(int relay_number){
    int status = !(EEPROM_read(200 + relay_number) - '0');
    EEPROM_write((status + '0'), 200 + relay_number);
}

void clean_lcd(){
    WriteCmdXLCD(0x01); // Limpa o LCD com retorno do cursor
    __delay_ms(10);  // Atraso de 10ms para aguardar a execução do comando
    WriteCmdXLCD(0x80); // Seleciona a posição Coluna 1 e Linha 1
}

void delay_show_lcd(){
    __delay_ms(1000);
}

void change_password(){
    while(1) {
    ADCON0bits.GO_DONE = 1;
    clean_lcd();
    WriteCmdXLCD(0x82);
    putsXLCD ("SENHA ATUAL: ");  // Escreve a string "TECLA" no LCD
    char current_password[5];
    get_password(current_password);
    __delay_ms(500);
    if ((grant_access(current_password) - '0') > 0) { // Se a senha digitada é igual à senha correta
        clean_lcd();
        WriteCmdXLCD(0x83);
        putsXLCD ("NOVA SENHA: ");  // Escreve a string "TECLA" no LCD
        char new_password[5];
        get_password(new_password);
        __delay_ms(500);
        update_password(current_password, new_password);
        clean_lcd();
        putsXLCD ("SENHA"); // Escreve a string "ACESSO" no LCD
        WriteCmdXLCD(0xC0); // Seleciona a posição Coluna 1 e Linha 2
        putsXLCD ("ALTERADA"); // Escreve a string "CONCEDIDO" no LCD
        delay_show_lcd();
        return;
    } else { // Se a senha digitada é diferente da senha correta
        clean_lcd();
        putsXLCD ("SENHA"); // Escreve a string "ACESSO" no LCD
        WriteCmdXLCD(0xC0); // Seleciona a posição Coluna 1 e Linha 2
        putsXLCD ("NAO ENCONTRADA"); // Escreve a string "NEGADO" no LCD
        delay_show_lcd();
    }
    }
}

void update_status_relay(){
    if(flag_status_relay) {
        EEPROM_write('0', 200);
        EEPROM_write('0', 201);
    }
    PORTCbits.RC6=(EEPROM_read(200) - '0');
    PORTCbits.RC7=(EEPROM_read(201) - '0');
    flag_status_relay = 0;
}

void get_password(const char* password){
    char key; // Declaração da variável que armazenará a tecla pressionada
    char entered_password[5]; // Senha digitada pelo usuário
    int i = 0; // Índice para armazenar a senha digitada
        
    while(i < 4) {
        ADCON0bits.GO_DONE = 1;
	    key = read_keypad(); // Chama a função read_keypad() e armazena o resultado em key
        if (key != '\0') { // Se uma tecla foi pressionada
            WriteCmdXLCD(0xC6 + i); // Seleciona a posição Coluna 1 + i e Linha 2
            WriteDataXLCD(key);// Escreve o caractere da tecla pressionada no LCD
            __delay_ms(200);  // Atraso de 10ms para aguardar a execução do comando
            WriteCmdXLCD(0xC6 + i); // Seleciona a posição Coluna 1 + i e Linha 2
            WriteDataXLCD('*');// Escreve o caractere da tecla pressionada no LCD
            entered_password[i] = key; // Armazena o caractere da tecla pressionada na senha digitada
            i++; // Incrementa o índice da senha digitada
            
            if (i == 4) strcpy(password, entered_password);
        }
    }
    return ;
}

void show_temp() {
    int adc = convert_temp();
    char int_str[20];
    clean_lcd();
    WriteCmdXLCD(0x83);
    sprintf(int_str, "TEMP: %d\xDF\x43", adc);
    putsXLCD(int_str);
    WriteCmdXLCD(0xC0);
    putsXLCD("\x7F(*)");
    WriteCmdXLCD(0xCC);
    putsXLCD("(#)\x7E");
}

int convert_temp(){
    ADCON0bits.GO_DONE = 1;
    while(ADCON0bits.GO_DONE);
    if(POT){
        return (ADRESH * 256 + ADRESL)/10;
    }
    double resultado = (ADRESH * 256 + ADRESL) * 0.48875;
    return (int)round(resultado);
}

void config(){
    TRISCbits.TRISC6 = 0; // RC6 configurada como saída digital
    TRISCbits.TRISC7 = 0; // RC7 configurada como saída digital
    TRISCbits.TRISC2 = 0; // RC2 configurada como saída digital
    
    TRISB = 0xF0;
    TRISAbits.TRISA0 = 1;
    
    RBPU = 0;
    
    ADCON1 = 0b00001101;
    ADCON0 = 0b00000001;
    ADCON2 = 0b10010101;
    
    OpenXLCD(FOUR_BIT & LINES_5X7);
    
    RCONbits.IPEN = 1; // Enable priority levels
    INTCONbits.GIEH = 1; // Enable high priority interrupts

    PIE1bits.ADIE = 1; // Enable A/D interrupt
    IPR1bits.ADIP = 1; // Set A/D interrupt as high priority
    
    INTCONbits.RBIE = 1; // Ativa a interrupção externa RB
    INTCONbits.RBIF = 0; // Limpa o flag bit da interrupção externa RB
}

// Função para conceder acesso com base na senha
unsigned char grant_access(const char* input_password) {
    User user;
    unsigned char address = 0;
    
    // Loop para verificar as senhas de todos os usuários
    while (address < 200) {
        EEPROM_read_user(&user, address);
        
        if (strcmp(input_password, user.password) == 0) {
            return user.accessLevel; // Retorna o nível de acesso correspondente
        }
        
        address += 6; // Avança para o próximo usuário na EEPROM
    }
    
    return '0'; // Retorna 0 se a senha não for encontrada
}

// Função para gravação do usuário na EEPROM
void EEPROM_write_user(const User* user, unsigned char start_address) {
    unsigned char address = start_address;
    
    for (unsigned char i = 0; i < 5; i++) {
        EEPROM_write(user->password[i], address);
        address++;
    }
    EEPROM_write(user->accessLevel, address);
}

// Função para leitura do usuário da EEPROM
void EEPROM_read_user(User* user, unsigned char start_address) {
    unsigned char address = start_address;
    
    for (unsigned char i = 0; i < 5; i++) {
        user->password[i] = EEPROM_read(address);
        address++;
    }
    user->accessLevel = EEPROM_read(address);
}

void clear_EEPROM()
{
    // Percorre todos os endereços de memória da EEPROM
    for (unsigned char address = 0; address < 255; address++)
    {
        EEPROM_write(0xFF, address);  // Grava o valor vazio (0x00) em cada endereço
        __delay_ms(10);  // Aguarda um pequeno intervalo entre as gravações para garantir a estabilidade da EEPROM
    }
}

void EEPROM_write( unsigned char data, unsigned char address  )
{
    
    EEADR = address;            //Endereço de gravação
    EEDATA = data;              //Dado a ser gravado na EEPROM
    EECON1bits.EEPGD = 0;       //Seleção da memória EEPROM
    EECON1bits.CFGS  = 0;       //Acessar a memória EEPROM
    EECON1bits.WREN  = 1;       //Liberar ciclo de escrita
    INTCONbits.GIE   = 0;       //Desativa momentaneamente a int. de perifericos
    
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;          //Iniciar gravação
    // required sequence end
    while (!PIR2bits.EEIF); // aguarda completar a escrita 
    INTCONbits.GIE = 1; // reativa as interrupções 
    PIR2bits.EEIF = 0; // zera o flag de escrita 
    EECON1bits.WREN=0;
                                                        
}

unsigned char EEPROM_read( unsigned char address )
{

    EEADR = address;            //Carrega endereço
    EECON1bits.EEPGD = 0;       //Seleção da memória EEPROM
    EECON1bits.CFGS  = 0;       //Acessar a memória EEPROM
    EECON1bits.RD    = 1;       //Solicita leitura da memória
    return( EEDATA );           //Retorna dado lido
}

// Função para atualizar a senha de um usuário
void update_password(const char* current_password, const char* new_password) {
    User user;
    unsigned char address = 0;
    
    // Loop para verificar as senhas de todos os usuários
    while (address < 200) {
        EEPROM_read_user(&user, address);
        
        if (strcmp(current_password, user.password) == 0) {
            strcpy(user.password, new_password);
            EEPROM_write_user(&user, address); // Atualiza a senha na EEPROM
            return;
        }
        
        address += 6; // Avança para o próximo usuário na EEPROM
    }
}



// Função para verificar se há status do rele
int has_status_relay() {
    unsigned char address = 200;
    
    while (address < 255) {
        if (EEPROM_read(address) != 0xFF) {
            return 1; // Senha encontrada
        }
        address++;
    }
    
    return 0; // Nenhuma senha encontrada
}

// Função para verificar se há senhas atribuídas
int has_passwords() {
    unsigned char address = 0;
    
    while (address < 200) {
        if (EEPROM_read(address) != 0xFF) {
            return 1; // Senha encontrada
        }
        address++;
    }
    
    return 0; // Nenhuma senha encontrada
}

// Função para atribuir senhas padrões para 3 usuários
void assign_default_passwords() {
    User user1 = { "1234", '1' }; // Senha padrão e nível de acesso para usuário 1
    User user2 = { "5678", '2' }; // Senha padrão e nível de acesso para usuário 2
    User user3 = { "9876", '3' }; // Senha padrão e nível de acesso para usuário 3
    
    // Verifica se já existem senhas atribuídas
    if (!has_passwords()) {
        EEPROM_write_user(&user1, 0); // Grava usuário 1 na posição 0 da EEPROM
        EEPROM_write_user(&user2, 6); // Grava usuário 2 na posição 6 da EEPROM
        EEPROM_write_user(&user3, 12); // Grava usuário 3 na posição 12 da EEPROM
    }
}

void assign_default_status_relay(){
    // Verifica se já existem senhas atribuídas
    if (!has_status_relay()) {
        EEPROM_write('0', 200); // Grava usuário 1 na posição 0 da EEPROM
        EEPROM_write('0', 201); // Grava usuário 2 na posição 6 da EEPROM
    }
}

void delay_ms(int ms) {
    for(int i = 0; i < ms; i++) {
        __delay_ms(1);
    }
}

// Função que lê a tecla pressionada no teclado
/*char read_keypad(void) {
    char key = '\0';
    // Matriz de caracteres que representa as teclas do teclado
    const char keys[4][4] = {
        {'A', 'B', 'C', 'D'},
        {'3', '6', '9', '#'},
        {'2', '5', '8', '0'},
        {'1', '4', '7', '*'}
    };
   int col = 0; // Variável que armazena a coluna atual do teclado
    do{
        switch(col) // Seleciona a coluna atual
        {
            case 0:
                COL1 = 0; // Desativa a primeira coluna
                COL2 = 1;
                COL3 = 1;
                COL4 = 1;
                break;
            case 1:
                COL1 = 1;
                COL2 = 0; // Desativa a segunda coluna
                COL3 = 1;
                COL4 = 1;
                break;
            case 2:
                COL1 = 1;
                COL2 = 1;
                COL3 = 0; // Desativa a terceira coluna
                COL4 = 1;
                break;
            case 3:
                COL1 = 1;
                COL2 = 1;
                COL3 = 1;
                COL4 = 0; // Desativa a quarta coluna
                break;
        }
        // Verifica se alguma das linhas está com nível lógico baixo (tecla pressionada)
        if(row == 1) {
            row = -1;
            key = keys[col][0]; // Retorna o valor da tecla correspondente se tecla da linha 1 foi pressionada
        }
        if(row == 2) {
            row = -1;
            key = keys[col][1]; // Retorna o valor da tecla correspondente se tecla da linha 2 foi pressionada
        }
        if(row == 3) {
            row = -1;
            key = keys[col][2]; // Retorna o valor da tecla correspondente se tecla da linha 3 foi pressionada
        }
        if(row == 4) {
            row = -1;
            key = keys[col][3]; // Retorna o valor da tecla correspondente se tecla da linha 4 foi pressionada
        }
        col++; // Incrementa para a próxima coluna
    } while(col < 4); // Loop até percorrer todas as colunas
    delay_ms(DEBOUNCE_TIME);
    return key; // Retorna um valor nulo se nenhuma tecla foi pressionada
}*/

char read_keypad(void) {
    // Matriz de caracteres que representa as teclas do teclado
    const char keys[4][4] = {
        {'A', 'B', 'C', 'D'},
        {'3', '6', '9', '#'},
        {'2', '5', '8', '0'},
        {'1', '4', '7', '*'}
    };
   int col = 0; // Variável que armazena a coluna atual do teclado
    do{
        switch(col) // Seleciona a coluna atual
        {
            case 0:
                COL1 = 0; // Desativa a primeira coluna
                COL2 = 1;
                COL3 = 1;
                COL4 = 1;
                break;
            case 1:
                COL1 = 1;
                COL2 = 0; // Desativa a segunda coluna
                COL3 = 1;
                COL4 = 1;
                break;
            case 2:
                COL1 = 1;
                COL2 = 1;
                COL3 = 0; // Desativa a terceira coluna
                COL4 = 1;
                break;
            case 3:
                COL1 = 1;
                COL2 = 1;
                COL3 = 1;
                COL4 = 0; // Desativa a quarta coluna
                break;
        }
        // Verifica se alguma das linhas está com nível lógico baixo (tecla pressionada)
               if(ROW1 == 0) {
            delay_ms(DEBOUNCE_TIME);
            return keys[col][0]; // Retorna o valor da tecla correspondente se tecla da linha 1 foi pressionada
        }
        if(ROW2 == 0) {
            delay_ms(DEBOUNCE_TIME);
            return keys[col][1]; // Retorna o valor da tecla correspondente se tecla da linha 2 foi pressionada
        }
        if(ROW3 == 0) {
            delay_ms(DEBOUNCE_TIME);
            return keys[col][2]; // Retorna o valor da tecla correspondente se tecla da linha 3 foi pressionada
        }
        if(ROW4 == 0) {
            delay_ms(DEBOUNCE_TIME);
            return keys[col][3]; // Retorna o valor da tecla correspondente se tecla da linha 4 foi pressionada
        }
        col++; // Incrementa para a próxima coluna
    } while(col < 4); // Loop até percorrer todas as colunas
    return '\0'; // Retorna um valor nulo se nenhuma tecla foi pressionada
}