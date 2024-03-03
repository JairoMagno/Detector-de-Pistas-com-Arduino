# Projeto de Detecção de Terrenos com Arduino Uno, MPU6050 e Python

Este projeto visa a utilização de um Arduino Uno em conjunto com um módulo de acelerômetro MPU6050 para a detecção de terrenos por meio de análise de dados de aceleração. O projeto consiste em quatro scripts, cada um com funcionalidades específicas:

## 1. Leitura de Dados de Aceleração (Ler-Dados-Acel)
Este script em Arduino (ino) é responsável por ler amostras do módulo de acelerometria e enviar esses dados lidos através da porta serial.

## 2. Amostragem de Dados (Amostra-Dados)
Utilizando a linguagem Python, este script permite ao usuário escolher o tipo de terreno em que o veículo está passando. A coleta de dados é realizada via serial a partir do script Ler-Dados-Acel, sendo os dados adicionados a um arquivo txt. Foram conduzidos testes em quatro tipos de terrenos distintos, com visualizações gráficas utilizando o Matplotlib para verificação visual.

## 3. Análise Estatística (Verificar-Estatisticas)
Este script realiza o cálculo das médias dos dados coletados para cada tipo de terreno, consolidando as estatísticas em um único script para cada tipo de pista. Esses dados preparados são essenciais para a fase final do projeto.

## 4. Detector de Pistas (Detector-de-Pistas)
O script principal do projeto, em Arduino (ino), utiliza os dados coletados e as médias calculadas para determinar o tipo de pista em que o veículo está se deslocando. O acelerômetro é novamente empregado para coletar dados na pista desejada pelo usuário, e a correspondência com as médias prévias define o tipo de terreno percorrido pelo carro.