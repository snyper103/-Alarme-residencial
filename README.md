# Alarme residencial
Este é o algoritmo de um  alarme residencial para o microcontrolador da placa Blue Pill (STM32F103C8T6) no VScode + Platformio.

## Requsitos
É necessário ter o Visula Studio Code instalado, juntamente com o PlatformIO, que é uma extensão do VSCode, também será necessário possuir uma Blue Pill e um speaker para conectar nela.

### Execução
Basta compilar o código e depois fazer o upload dele na Blue Pill.

### Funcionamento
O alarme ”A” de uma casa é ativado por um sensor de movimento ”M” e por um sensor de abertura de janelas ”J”:

1) O sensor ”M” ativa o alarme quando detecta a presença de pessoas;
2) O sensor ”J” ativa o alarme quando a janela é aberta;
3) Há ainda um chave ”B” para ligar ou desligar o alarme;
4) O alarme ”A” deve ser acionado por 10s e depois desligar automaticamente.
