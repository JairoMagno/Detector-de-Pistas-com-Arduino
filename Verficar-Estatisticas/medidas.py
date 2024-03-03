#calculo da média, variância
import matplotlib.pyplot as plt
from statistics import mean, median, variance, mode
import numpy as np
import sys

def pseudo_frequencia(vet, points):

    soma = 0
    for i in range(len(vet)):
        soma += abs(vet[i] - vet[(i+1)%points])

    return soma/points

def calculo_medias(filename, limit, points):
    
    try:
        with open(filename, 'r') as f:
            axes = f.readlines()
    except FileNotFoundError:
        print(f'Arquivo {filename} não encontrado! Verifique o nome ou seu diretório.')
        sys.exit(1)
    else:

        x_axes = []
        y_axes = []
        z_axes = []

        for row in axes:
            #Tratando a lista para valores válidos.
            values = row.strip().split(',')
            values = [value.lstrip() for value in values]
            #Adicionado os valores de cada eixo a uma lista separada.
            x_axes.append(float(values[0]))
            y_axes.append(float(values[1]))
            z_axes.append(float(values[2]))   
        
    #Cálculos das médias
    x_mean = mean(x_axes)
    x_mode = mode(x_axes)
    x_median = median(x_axes)
    x_variance = variance(x_axes)

    y_mean = mean(y_axes)
    y_mode = mode(y_axes)
    y_median = median(y_axes)
    y_variance = variance(y_axes)

    z_axes = [value - 9.81 for value in z_axes]
    z_mean = mean(z_axes)
    z_mode = mode(z_axes)
    z_median = median(z_axes)
    z_variance = variance(z_axes)

    #Cálculo da pseudo frequência:
    pseudo_freq_X = pseudo_frequencia(x_axes, points)
    pseudo_freq_Y = pseudo_frequencia(y_axes, points)
    pseudo_freq_Z = pseudo_frequencia(z_axes, points)
    
    with open("data_terra.txt", "a+") as arq:
        arq.write(f"{x_variance:.6f},{y_variance:.6f},{z_variance:.6f},,{pseudo_freq_X:.6f},{pseudo_freq_Y:.6f},{pseudo_freq_Z:.6f}\n")

    #String a ser colocada no gráfico
    media_x = f'Média: {x_mean:.5f}\nModa: {x_mode:.5f}\nMediana: {x_median:.5f}\nVariância: {x_variance:.5f}\nMáximo: {max(x_axes):.5f}\nMínino: {min(x_axes):.5f}\nPseudo Freqência: {pseudo_freq_X:.5f}'
    media_y = f'Média: {y_mean:.5f}\nModa: {y_mode:.5f}\nMediana: {y_median:.5f}\nVariância: {y_variance:.5f}\nMáximo: {max(y_axes):.5f}\nMínino: {min(y_axes):.5f}\nPseudo Freqência: {pseudo_freq_Y:.5f}'
    media_z = f'Média: {z_mean:.5f}\nModa: {z_mode:.5f}\nMediana: {z_median:.5f}\nVariância: {z_variance:.5f}\nMáximo: {max(z_axes):.5f}\nMínino: {min(z_axes):.5f}\nPseudo Freqência: {pseudo_freq_Z:.5f}'

    #sample time
    sample_time = np.linspace(start=0, stop=10, num=1000, endpoint=True)

    # Crie uma figura e os três subplots empilhados verticalmente
    fig, ax = plt.subplots(3, 1, figsize=(8, 6))  # Tamanho da figura
    
    #plt.subplot(311)  # O primeiro subplot (3 linhas, 1 coluna, primeiro plot)
    ax[0].plot(sample_time, x_axes)
    ax[0].set_title("Eixo x")
    ax[0].set_ylim(-limit, limit)

    #plt.subplot(312)  # O segundo subplot (3 linhas, 1 coluna, segundo plot)
    ax[1].plot(sample_time, y_axes)
    ax[1].set_title("Eixo y")
    ax[1].set_ylim(-limit, limit)

    #plt.subplot(313)  # O terceiro subplot (3 linhas, 1 coluna, terceiro plot)
    ax[2].plot(sample_time, z_axes)
    ax[2].set_title("Eixo z")
    ax[2].set_ylim(-20, 20)

    #Criando as BBOX para labels externas ao gráfico:
    props = dict(boxstyle='round', facecolor='grey', alpha=0.15)  # bbox features
    ax[0].text(1.02, 0.98, media_x, transform=ax[0].transAxes, fontsize=10, verticalalignment='top', bbox=props)
    ax[1].text(1.02, 0.98, media_y, transform=ax[1].transAxes, fontsize=10, verticalalignment='top', bbox=props)
    ax[2].text(1.02, 0.98, media_z, transform=ax[2].transAxes, fontsize=10, verticalalignment='top', bbox=props)

    # Ajuste o layout para evitar sobreposição de rótulos
    plt.subplots_adjust(hspace=0.5)
    plt.tight_layout()

    # Exibir os gráficos
    plt.show()

    return x_axes, y_axes, z_axes

filename1 = 'Asfalto16.txt'
filename2 = 'Calcamento8.txt'
filename3 = 'Concreto12.txt'
filename4 = 'Terra10.txt'

x_axes, y_axes, z_axes = calculo_medias(filename4, 20, 1000)
