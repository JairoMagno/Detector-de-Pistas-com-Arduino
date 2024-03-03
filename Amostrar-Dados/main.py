import serial
import matplotlib.pyplot as plt
import numpy as np

com = 7
qtd_amostras = 1000
baude_rate = 38400
qtd_terrains = 4
ser = serial.Serial(f'COM{com}', baude_rate)
dic_terrains = {'1':"Asfalto", '2':"Calcamento", '3':"Terra", '4':"Concreto"}

while True:

    opc = 0
    cnt = 1
    buffer_x = []
    buffer_y = []
    buffer_z = []
    num_terrains = []
    new_lines = []

    #------------Ler número ultimo arquivo aberto------------#
    while(opc<1 or opc>qtd_terrains):
        opc = int(input("1-Asfalto\n2-Calçamento\n3-Terra\n4-Concreto\n9-Sair\nR: "))
        if opc == 9:
            break

    if opc == 9:
        print("Fechando...")
        break

    with open("data.txt", 'r') as arq:
        lines = arq.readlines()

    for line in lines:
        num_terrains.append(int(line.strip()))
        if cnt == opc:
            new_lines.append(str(int(line.strip())+1)+'\n')
        else:
            new_lines.append(line)
        cnt += 1

    #-------------Salvar Amostras--------------#
    try:
        current_file = dic_terrains[str(opc)]+str(num_terrains[opc-1]+1)+".txt"
        with open(current_file, 'w') as arq:
            ser.flushInput()
            for i in range(qtd_amostras):
                linha = ser.readline().decode().strip()
                valores = linha.split(',')
                x, y, z = map(float, valores)
                buffer_x.append(x)
                buffer_y.append(y)
                buffer_z.append(z-9.8)
                arq.write(f"{x}, {y}, {z}\n")

        with open("data.txt", 'w') as arq:
            arq.writelines(new_lines)

        print("\nSucesso na leitura!\n")
    
    except:
        print("\nErro na leitura.\n")
        continue

    sample_time = np.linspace(0, qtd_amostras, qtd_amostras)

    # Crie uma figura e os três subplots empilhados verticalmente
    plt.figure(figsize=(8, 6))  # Tamanho da figura
    
    plt.subplot(311)  # O primeiro subplot (3 linhas, 1 coluna, primeiro plot)
    plt.plot(sample_time, buffer_x)
    plt.title("Eixo x")
    plt.ylim(-20, 20)

    plt.subplot(312)  # O segundo subplot (3 linhas, 1 coluna, segundo plot)
    plt.plot(sample_time, buffer_y)
    plt.title("Eixo y")
    plt.ylim(-20, 20)

    plt.subplot(313)  # O terceiro subplot (3 linhas, 1 coluna, terceiro plot)
    plt.plot(sample_time, buffer_z)
    plt.title("Eixo z")
    plt.ylim(-20, 20)

    # Ajuste o layout para evitar sobreposição de rótulos
    plt.subplots_adjust(hspace=0.5)
    plt.tight_layout()

    # Exibir os gráficos
    plt.show()
