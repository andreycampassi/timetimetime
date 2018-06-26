---
---
# Componentes

## ESP8266 NodeMCU 1.0 Microcontroller

![Imagem do Microcontrolador NodeMCU](/assets/images/essentials-esp8266-nodemcu-10-microcontroller.png)

Este microcontrolador é parte central do projeto, ele é o responsável pelo controle de todos os outros componentes e de fazer a comunicação com a página web onde os vídeos serão apresentados ao público. Ele pode ser programado com a linguagem padrão do Arduino e conta com uma antena Wi-Fi, sendo ideal para IoT.

## Protoboard com 830 pontos

![Imagem da Protoboard com 830 pontos](/assets/images/essentials-830-and-430-points-breadboard.png)

Para facilitar a prototipagem do projeto, como fizemos de maneira gradual, soldar as peças a todo momento seria um problema, para isso servem as Protoboards/Breadboards. Com isso ao invés da solda, simplesmente usamos os Jumpers subcitados para fazer a passagem de corrente eletrica. Saiba mais[Protoboard].  


## Cabo Micro USB

![Imagem de um cabo Micro USB](/assets/images/essentials-micro-usb-cable.png)

Usado tanto para alimentar a placa ESP8266 quanto para transferencia de arquivos do computador.

## Jumpers

![Imagem de Jumpers](/assets/images/essentials-rigid-jumper.png)

Além dos jumpers rígidos originais oferecidos para o projeto e dos jumpers "flexíveis" macho-femea, utilizamos jumpers macho-macho para facilitar devido a sobreposição de jumpers rígidos e ausencia do tamanho ideal na hora de conectar certos componentes.

## Resistores

### 10kΩ Resistor

![Imagem de um resistor 10kΩ](/assets/images/essentials-10k-resistor.png)

Utilizado para controlar o fluxo de energia no circuito, essencial para o funcionamento do [PushButton] para controlar os estados _Pressionado_ e _Não Pressionado_.

### 330Ω Resistor

![Imagem de um resistor 330Ω](/assets/images/essentials-330-resistor.png)

Utilizado para previnir danos ao microcontrolador e para alimentar os LEDs de maneira a não queimá-los.

Saiba mais [Resistores]

## PushButton

![Imagem de um botão](/assets/images/push_button.png)

Pequeno botão que utilizamos para fazer o acionamento de um vídeo.

## Sensor Ultrassônico de Presença

![Imagem de um botão](/assets/images/HCSR04.png)

Utlizado para detectar mãos curiosas chegando perto de uninas nucleares, de modo a informar o usuário o lado ruim de tal tecnologia.

## LEDs RGB

![Imagem de um LED RGB](/assets/images/rgb_led.png)

Os LEDs RGB foram utilizados para simular luzes vindas de uma torre de resfriamento de uma usina nuclear.

## LEDs Brancos

![Imagem de um LED Branco](/assets/images/red_led.png)

LEDs brancos foram utilizados para simular a iluminação de uma cidade.

_ Adaptado de [Pincello]/[Essentials] _

[//]: # (Referências
To-do: Adicionar imagens do projeto
Adicionar links para pushbutton, resistores e ultrassom
)

   [Essentials]: <https://efduarte.github.io/pincello/#/essentials>
   [Pincello]: <https://efduarte.github.io/pincello/#/>
   [Protoboard]: <https://learn.sparkfun.com/tutorials/how-to-use-a-breadboard>
   [PushButton]:
   [Resistores]:
   